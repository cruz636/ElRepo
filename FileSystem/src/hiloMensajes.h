/*
 * hiloMensajes.h
 *
 *  Created on: 18 abr. 2019
 *      Author: utnso
 */

#ifndef FILE_SYSTEM_SRC_HILOMENSAJES_H_
#define FILE_SYSTEM_SRC_HILOMENSAJES_H_

#include <funcionesCompartidas/funcionesNET.h>

typedef struct {
	header head;
	char* buffer;
}mensaje;

typedef struct {
	int socket;
	pthread_t hilo;
}cliente_t;

void tratarMensajes(int socket);


#endif /* FILE_SYSTEM_SRC_HILOMENSAJES_H_ */
