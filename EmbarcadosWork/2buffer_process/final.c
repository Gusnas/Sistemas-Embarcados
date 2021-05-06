#include "contiki.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static process_event_t buffer_to_main;
static process_event_t buffer_to_p1;
static process_event_t buffer_to_p2;

PROCESS(main_process, "Main Process");
PROCESS(process_1, "Process 1");
PROCESS(process_2, "Process 2");

AUTOSTART_PROCESSES(&main_process, &process_1, &process_2);

/* Implementation of the main process */

PROCESS_THREAD(main_process, ev, data){

    int i = 0;
    char buffer[10];
    static char static_buffer[10];

	PROCESS_BEGIN();

    buffer_to_p1 = process_alloc_event();
    buffer_to_p2 = process_alloc_event();

	while(1){
		printf("--------------------------\n");
		printf("Digite qualquer palavra:\n");
	if(scanf("%s", buffer)){
        	printf("Palavra recebida: %s\n",buffer);
	}

        for(i=0;i<10;i++){
            static_buffer[i] = buffer[i];
        }
        if(static_buffer[0] == 'p'){
		process_post(&process_1, buffer_to_p1, &static_buffer);
		printf("Buffer enviado para o processo 1\n"); //se [0] for p, vai para o processo 1 e retorna a string com P
	}
        else{
            process_post(&process_2, buffer_to_p2, &static_buffer); //se [0] != p, vai para o processo 2 e retorna a string ao invertida
            printf("Buffer enviado para o processo 2\n");
        }

	PROCESS_WAIT_EVENT_UNTIL(ev == buffer_to_main);
	printf("\n");
        printf("Buffer recebido de processo secundário\n");
        char *ponteiro = data;
        fputs(ponteiro, stdout);
        printf("\n");
	printf("\n");
    }
	PROCESS_END();
}




/* Implementation of the second process */
    PROCESS_THREAD(process_1, ev, data)
    {
    static char static_second_buffer[10];
    
    PROCESS_BEGIN();

    buffer_to_main = process_alloc_event();
    
    while (1)
    {
        PROCESS_WAIT_EVENT_UNTIL(ev == buffer_to_p1);
	printf("\n");
        printf("Recebendo buffer no processo 1\n");
        char *ponteiro = data;
        printf("Valor recebido no processo 1:");
        fputs(data,stdout);
        printf("\n");

	for(int i=0;i<10;i++){
		static_second_buffer[i] = ponteiro[i];
	}

        static_second_buffer[0] = 0x50;
        printf("Buffer do processo 1:");
        fputs(static_second_buffer,stdout);
        printf("\n");
        process_post(&main_process, buffer_to_main, &static_second_buffer); 
    }
    PROCESS_END();
}


/* Implementation of the third process */
    PROCESS_THREAD(process_2, ev, data)
{
    static char static_second_buffer[10];
    
    PROCESS_BEGIN();

    buffer_to_main = process_alloc_event();
    
    while (1)
    {
        PROCESS_WAIT_EVENT_UNTIL(ev == buffer_to_p2);

	printf("\n");
        printf("Recebendo buffer no processo 2\n");
        char *ponteiro = data;
        printf("Valor recebido no processo 2:");
        fputs(data,stdout);
        printf("\n");

        for(int i=0; i<10;i++){
            static_second_buffer[i] = ponteiro [i];
        }

        int k = 0;
        int j = strlen(static_second_buffer) - 1;
	char temp;

        while (k < j) {
            temp = static_second_buffer[k];
            static_second_buffer[k] = static_second_buffer[j];
            static_second_buffer[j] = temp;
            k++;
            j--;
        }

        printf("Buffer do processo 2:");
        fputs(static_second_buffer,stdout);
        printf("\n");
        process_post(&main_process, buffer_to_main, &static_second_buffer); 
    }
    PROCESS_END();
}

