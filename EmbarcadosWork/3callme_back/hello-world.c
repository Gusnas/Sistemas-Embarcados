#include "contiki.h"
#include "lib/random.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
/**************************************************************************************************
 * PRIVATE TYPES
 *************************************************************************************************/
static process_event_t send_hex;
static process_event_t call_sensor;
static process_event_t state_sensor;
static process_event_t state_led;
static process_event_t call_led;
/**************************************************************************************************
 * PRIVATE DATA
 *************************************************************************************************/
static int qtd_callback = 0;
static int sensor_on = 0;
static int sensor_off = 1;
static int led_on = 2;
static int led_off = 3;
static int array_sensors[4] = {0};
static int qtd_led_off = 0;
static int qtd_sensor_off = 0;

/**************************************************************************************************
 * PRIVATE PROTOTYPES
 *************************************************************************************************/
static void event_callback(void *ptr){
    printf("\n--------------------------\n");
    printf("| %d Sensores Ligados     |\n| %d Sensores Desligados. |\n| %d Led Ligadaa.         |\n| %d Led Desligadas.      |",
        array_sensors[sensor_on],array_sensors[sensor_off],array_sensors[led_on],array_sensors[led_off]);
    printf("\n--------------------------\n");
    printf("\n");

    for (int i = 0; i < 4; i++){
        array_sensors[i] = 0;
    }
    ctimer_reset((struct ctimer*) ptr);
}

static void sensor_callback(void *ptr){
    if (qtd_led_off >= 2){
        printf("Permission Denied");
    }
    ctimer_reset((struct ctimer*) ptr);
}

static void led_callback(void *ptr){
    if (qtd_sensor_off >= 2){
        printf("Permission Denied");
    }
    ctimer_reset((struct ctimer*) ptr);
}

/**************************************/

PROCESS(serial_simulator, "Serial Simulator");
PROCESS(main_process, "Main Process");
PROCESS(sensors_process, "Sensor Process");
PROCESS(led_process, "Led Process");

AUTOSTART_PROCESSES(&serial_simulator, &main_process, &sensors_process,&led_process);

PROCESS_THREAD(serial_simulator,ev,data){
  static uint8_t rand; 
  static struct etimer timer;
  int length = 2;
  int i;
  char hex[] = "0123456789ABCDEF";
  static char buffer[2];

  send_hex = process_alloc_event();
  PROCESS_BEGIN();
  etimer_set(&timer, CLOCK_SECOND * 1);

  while(1){
    for(i=0; i<length; i++){
      rand = (random_rand() % 16);
      buffer[i] = hex[rand];
      
    } 
    printf("Sending 0x%s to main process...\n", buffer);
    process_post(&main_process,send_hex,&buffer);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    etimer_reset(&timer);
  }
  PROCESS_END();
}

PROCESS_THREAD(main_process,ev,data){
  int stats;
  
  call_sensor = process_alloc_event();
  call_led = process_alloc_event();

  PROCESS_BEGIN();

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev = send_hex);
    char *buffer = data;
    printf("Received something from serial port...\n");
    if(isdigit(buffer[0])){
      stats=1;
      process_post(&sensors_process,call_sensor,buffer);
    }
    else if(!isdigit(buffer[0])){
      stats=2;
      process_post(&led_process,call_led,buffer);
    }
    else {
      continue;
    }
    
    if(stats==1) {
      PROCESS_WAIT_EVENT_UNTIL(ev = state_sensor);
      uint8_t pscode = ((intptr_t)data);
      if(pscode==1){
        array_sensors[sensor_on]++;
        printf("Sensor Ligado.\n");
        printf("\n");
      }
      if(pscode==0) {
        array_sensors[sensor_off]++;
        if (qtd_callback == 99){
            memset(array_sensors, -1, sizeof(array_sensors));
            qtd_callback = 0;
        }
        
        printf("Sensor Desligado.\n");
        printf("\n");
      }
    }
    else if(stats==2) {
      PROCESS_WAIT_EVENT_UNTIL(ev = state_led);
      uint8_t plcode = ((intptr_t)data);
      if(plcode==1){
        printf("Led Ligado.\n");
        printf("\n");
      }
      if(plcode==0){
        printf("Led Desligado.\n");
        printf("\n");
      }
    }
    else {
      continue;
    }
    
  }
  PROCESS_END();
}

PROCESS_THREAD(sensors_process,ev,data){
  static struct ctimer ct;
  static struct ctimer ct_t;
  static bool status_sensor = false; 
  static uint8_t code;
  state_sensor = process_alloc_event();
  PROCESS_BEGIN();

  ctimer_set(&ct, 10 * CLOCK_SECOND, event_callback, &ct);
  ctimer_set(&ct_t, 1 * CLOCK_SECOND, led_callback, &ct_t);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev = call_sensor);
    char *buffer = data;
    printf("Sensor Lido: %s\n",buffer);
    printf("\n");
    if(status_sensor){
      status_sensor = false;
      code = 0;
      array_sensors[sensor_off]++;
      qtd_sensor_off++;
      process_post(&main_process, state_sensor, &code);

    }else {
      status_sensor = true;
      code = 1;
      array_sensors[sensor_on]++;
      qtd_sensor_off = 0;
      process_post(&main_process, state_sensor, &code);
    }
  }
  PROCESS_END();
}

PROCESS_THREAD(led_process, ev,data){
  static struct ctimer ct_d;
  static bool status_led = false; 
  static uint8_t pvcode;
  state_led = process_alloc_event();

  ctimer_set(&ct_d, 1 * CLOCK_SECOND, sensor_callback, &ct_d);

  PROCESS_BEGIN();

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev = call_led);
    char *buffer = data;
    printf("Led Lido: %s\n", buffer);
    printf("\n");
    if(status_led){
      status_led = false;
      pvcode = 0;
      array_sensors[led_off]++;
      qtd_led_off++;
      process_post(&main_process, state_led, &pvcode);

    }else {
      status_led = true;
      pvcode = 1;
      array_sensors[led_on]++;
      qtd_led_off = 0;
      process_post(&main_process,state_led,&pvcode);
    }
  }
  PROCESS_END();
}