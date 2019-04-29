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
#include "manejoArchivos.h"
#include <commons/string.h>

extern t_log* alog;

void* hiloconsola(){
	int flag = 1;

	char *request;
	char* ingreso;
	size_t tamBuffer = 100;
	structRespuesta * respuesta;

	log_info(alog, "Se creo el hilo consola");

	while(flag){

		ingreso = malloc(sizeof(char) * tamBuffer);

		getline(&ingreso, &tamBuffer, stdin);

		request = strtok(ingreso, "\n");

		switch(getEnumFromString(request))
		{
			case 1:
				log_info(alog, "Recibi un Insert");
				structInsert * insert;

				insert = cargarInsert(request, true);

				respuesta = realizarInsert(insert);

				mostrarRespuesta(respuesta);

				free(insert);
				free(respuesta);
				break;

			case 2:
				log_info(alog, "Recibi un Select");
				structSelect * select;

				select = cargarSelect(request);

				respuesta = realizarSelect(select);


				free(select);
				free(respuesta);
				break;

			case 3:
				log_info(alog, "Recibi un Create");
				structCreate * create;

				create = cargarCreate(request);

				respuesta = realizarCreate(create);


				free(create);
				free(respuesta);
				break;

			case 4:
				log_info(alog, "Recibi un Drop");
				structDrop * drop;

				drop = cargarDrop(request);

				respuesta = realizarDrop(drop);


				free(drop);
				free(respuesta);
				break;

			case 5: //describe
				log_info(alog, "Recibi un Describe");
				//structDescribe * describe;

				//describe = cargarDescribe(request);

				//respuesta = realizarDescribe(describe);


				//free(describe);
				//free(respuesta);
				break;

			default:
				flag = 0;
				log_info(alog, "Mensaje incorrecto");

			}
		//free(request);
		free(ingreso);

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

        if (string_starts_with(string,map[i].s)) {
            return map[i].e;
        }
    }
    return -1;
}


void mostrarRespuesta(structRespuesta* respuesta){

}
