#include "contiki.h"
#include <stdio.h>
#include <stdint.h>

#include "blinkled.h"
#include "sensors.h"

/*
Monitoramento de Sala - Vazamento de Gás e Temperatura Local em Tempo Real.

O firmware possui 2 sensores, 2 Leds e um Buzzer.
Possui processo principal e 2 secundarios.
É gerado numeros aleatorios que são salvo nos sensores,
o firmware faz a leitura e dai toma certas decisões.

Se o valor de gás for maior do que 20, indica que há
vazamento de gás então o Led vermelho é ativado é 
indicando alerta e o Buzzer é ativado gerando um alarme sonoro.

Se o valor da temepratura for maior do que 60 C, indica
uma alta na temperatura ambiente, que é um mal sinal, então
o Led Vermelho da temperatura é ativado sinalizando  alerta

Se os Leds de cada sensor estiverem verdes significa
que o sistema de monitoramento está em condições ok.
*/

PROCESS(helloworld, "Cicle");

AUTOSTART_PROCESSES(&helloworld);

PROCESS_THREAD(helloworld, ev, data){
	
	static struct etimer timer;
	 
	static uint8_t i = 0;

	PROCESS_BEGIN(); 
	
	process_start(&blinkled, NULL);
	process_start(&sensors, NULL);

	//etimer_set(&timer, 10);

	while(1){
		
		etimer_set(&timer, CLOCK_CONF_SECOND * 1);
		printf("\x1B[37m------------------------------\n");
		printf("\x1B[37m|Monitoramento Embarcado| %d |\n", i);
		printf("\x1B[37m------------------------------\n");
		
		printf("Gas = %.2f\r\n", get_read_gas());
		printf("Temperatura = %.2f\r", get_read_temperature());

		if(get_read_gas()>20.0){
			blinkledgas(true);
			buzzer();
		}
		if(get_read_gas()<=20.0){
			blinkledgas(false);
		}
		

		if(get_read_temperature()>60.0){
			blinkledtemp(true);
		}
		if(get_read_temperature()<=60.0){
			blinkledtemp(false);
		}
		
		printf("\n");
	
		//PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
		PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
		etimer_reset(&timer);
		i++;
	}

	PROCESS_END();

}

