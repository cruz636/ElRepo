/*
 * hiloMensajes.c
 *
 *  Created on: 18 abr. 2019
 *      Author: utnso
 */

#include "hiloMensajes.h"

void tratarMensaje(mensaje mensRecibido){




	free(mensRecibido);
	pthread_exit();
}

