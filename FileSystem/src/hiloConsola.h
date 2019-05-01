/*
 * hiloConsola.h
 *
 *  Created on: 20 abr. 2019
 *      Author: utnso
 */

#ifndef FILESYSTEM_SRC_HILOCONSOLA_H_
#define FILESYSTEM_SRC_HILOCONSOLA_H_

#include "manejoArchivos.h"

void* hiloconsola();
int getEnumFromString ( char *string );
void mostrarRespuesta(int respuesta);
structInsert * cargarInserts(char * comando, bool requireTimeStamp);


#endif /* FILESYSTEM_SRC_HILOCONSOLA_H_ */
