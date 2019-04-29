/*
 * manejoArchivos.h
 *
 *  Created on: 28 abr. 2019
 *      Author: utnso
 */

#ifndef SRC_MANEJOARCHIVOS_H_
#define SRC_MANEJOARCHIVOS_H_

#include <funcionesCompartidas/API.h>

typedef struct {

}structRespuesta;

structRespuesta * realizarInsert(structInsert * insert);
structRespuesta * realizarSelect(structSelect * select);
structRespuesta * realizarCreate(structCreate * create);
structRespuesta * realizarDrop(structDrop * drop);


#endif /* SRC_MANEJOARCHIVOS_H_ */
