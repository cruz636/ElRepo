/*
 * hiloMensajes.c
 *
 *  Created on: 18 abr. 2019
 *      Author: utnso
 */

#include "hiloClientes.h"

#include <pthread.h>
#include <funcionesCompartidas/funcionesNET.h>
#include <semaphore.h>
#include <commons/collections/dictionary.h>


extern t_log* alog;
extern t_dictionary * clientes;

void tratarCliente(int socketC){

	int status=0;
	bool flag = true;

	while(flag){
		mensaje * recibido = malloc(sizeof(mensaje));

		recibido->buffer = getMessage(socketC, &(recibido->head), &status);

		if(recibido->buffer == NULL) flag=false;

		switch(recibido->head.codigo)
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
				flag = false;
				log_info(alog, "Mensaje incorrecto de socket cliente %d", socketC);

		}

		free(recibido->buffer);
		free(recibido);
	}

	log_info(alog, "se desconecto el socket client %d",socketC);
	close(socketC);
	free(dictionary_remove(clientes, string_itoa(socketC)));

	pthread_exit(NULL);
}

