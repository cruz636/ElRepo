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
	char * consistency;
	int partitions;
	int compaction_time;

}structMetadata;

typedef struct {
	int Size;
	char ** bloques;
}structParticion;

int realizarInsert(structInsert * insert);
int realizarSelect(structSelect * select, char ** value);
int realizarCreate(structCreate * create);
int realizarDrop(structDrop * drop);
bool validarArchivos(char * archivo, int* respuesta);



#endif /* SRC_MANEJOARCHIVOS_H_ */
