/*
 * hiloConsola.c
 *
 *  Created on: 20 abr. 2019
 *      Author: utnso
 */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <readline/readline.h>
#include <commons/log.h>
#include <funcionesCompartidas/API.h>
#include "hiloConsola.h"
#include <string.h>

extern t_log* alog;

void* hiloconsola(){
	int flag = 1;

	char *request;
	size_t tamBuffer = 100;

	log_info(alog, "Se creo el hilo consola");

	while(flag){

		request = malloc(sizeof(char) * tamBuffer);

		getline(&request, &tamBuffer, stdin);

		switch(getEnumFromString(request))
		{
			case 1:
				log_info(alog, "Recibi un Insert");
				break;

			case 2:
				log_info(alog, "Recibi un Select");
				break;

			case 3:
				log_info(alog, "Recibi un Create");
				break;

			case 4:
				log_info(alog, "Recibi un Drop");
				break;

			case 5: //describe
				log_info(alog, "Recibi un Describe");
				break;

			default:
				flag = 0;
				log_info(alog, "Mensaje incorrecto");

			}
		free(request);
		//free(identi);

	}

	log_info(alog, "Sale del hilo consola");
	pthread_exit(NULL);
}

int getEnumFromString ( char *string ) {
    static struct {
        char *s;
        enum OPERACION e;
    } map[] = {
        { "INSERT", INSERT },
        { "SELECT", SELECT },
        { "CREATE", CREATE },
		{ "DROP", DROP }
    };

    for ( int i = 0 ; i < sizeof(map)/sizeof(map[0]); i++ ) {

        if (string_starts_with(string,map[i].s) /*strcmp(string ,map[i].s) == 0 */) {
            return map[i].e;
        }
    }
    return -1;
}
