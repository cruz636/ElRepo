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

extern t_log* alog;

void* hiloconsola(){
	int flag = 0;

	char *ingreso;
	char *identi;



	while(flag){

		scanf("%ms", &ingreso);

		identi = strtok(ingreso, " ");


		switch(getEnumFromString(identi))
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
				flag = 1;
				log_info(alog, "Mensaje incorrecto");

			}
	}


	pthread_exit(NULL);
}

int getEnumFromString ( const char *string ) {
    static struct {
        const char *s;
        enum OPERACION e;
    } map[] = {
        { "INSERT", INSERT },
        { "SELECT", SELECT },
        { "CREATE", CREATE },
		{ "DROP", DROP }
    };

    for ( int i = 0 ; i < sizeof(map)/sizeof(map[0]); i++ ) {
        if ( strcmp(string ,map[i].s) == 0 ) {
            return map[i].e;
        }
    }
    return -1;
}
