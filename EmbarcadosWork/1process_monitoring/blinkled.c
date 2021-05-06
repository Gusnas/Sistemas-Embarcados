#include "contiki.h"
#include <stdio.h>
#include <stdint.h>

 
/**************************************************************************************************
 * PRIVATE TYPES
 *************************************************************************************************/
 

/**************************************************************************************************
 * PRIVATE PROTOTYPES
 *************************************************************************************************/
void vermelho(void);
void verde(void);
void blinkledtemp(bool);
void blinkledgas(bool);


/**************************************************************************************************
 * PRIVATE DATA
 *************************************************************************************************/
//bool valor = false;

/**************************************************************************************************
 * FUNCTIONS DEFINITIONS
 *************************************************************************************************/

void vermelho() {
  printf("\033[1;31m");
}

void verde() {
  printf("\033[1;32m");
}


void blinkledgas(bool trigg){
	if((trigg) == true){
    vermelho();
		printf("\nLED Gás ON\r");
	}else {
    verde();
		printf("\nLED Gás OFF\r");
	}
}

void blinkledtemp(bool trigger){
	if((trigger) == true){
    vermelho();
		printf("\nLED Temperatura ON\r\n");
	}else {
    verde();
		printf("\nLED Temperatura OFF\r\n");
	}
}


/**************************************/
PROCESS(blinkled, "UART Led Process");

PROCESS_THREAD(blinkled, ev, data){
   //static struct etimer timer;	
	
    PROCESS_BEGIN(); 
    while (1){
        // we set the timer from here every time
        // etimer_set(&timer, CLOCK_CONF_SECOND * 4);
        
        // and wait until the vent we receive is the one we're waiting for
      PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
		//blinkledfunction(valor);
        //printf("valor do led = %d\r\n", valor);

    }
    PROCESS_END();
}
