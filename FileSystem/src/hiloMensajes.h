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

void tratarMensaje(mensaje mensRecibido);


#endif /* FILE_SYSTEM_SRC_HILOMENSAJES_H_ */
