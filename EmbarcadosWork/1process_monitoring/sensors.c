#include "contiki.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


/**************************************************************************************************
 * PRIVATE TYPES
 *************************************************************************************************/
 
 
/**************************************************************************************************
 * PRIVATE PROTOTYPES
 *************************************************************************************************/
static void read_temperature(void);
static void read_gas(void);
void buzzer(void);


/**************************************************************************************************
 * PRIVATE DATA
 *************************************************************************************************/
static float temperature = 0;
static float gas = 0;


/**************************************************************************************************
 * FUNCTIONS DEFINITIONS
 *************************************************************************************************/

void buzzer() {
  printf("\a");
}

static float randf(float min, float max){
    srand(time(NULL));
    float scale = rand()/(float) RAND_MAX;
    return min + scale * (max - min);
}

static void read_temperature(void){
    temperature = randf(15, 80);
}
static void read_gas(void){
    gas = randf(15, 35);
}


float get_read_temperature(void){
    return temperature;
}
float get_read_gas(void){
    return gas;
}


/**************************************/
PROCESS(sensors, "Meus sensors");

PROCESS_THREAD(sensors, ev, data)
{
    static struct etimer timer;	
	
    PROCESS_BEGIN(); 
    while (1)
    {
        // we set the timer from here every time
        etimer_set(&timer, CLOCK_CONF_SECOND * 5);
        
        // and wait until the vent we receive is the one we're waiting for
      
	    read_temperature();
        read_gas();
        get_read_temperature();
        get_read_gas();

	//	printf("captando sensores \n");
     PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

    }
    PROCESS_END();
}
