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

typedef struct {
	long int time;
	int key;
	char * value;
}structRegistro;

int realizarInsert(st_insert * insert);
int realizarSelect(st_select * select, char ** value);
int realizarCreate(st_create * create);
int realizarDrop(st_drop * drop);
bool validarArchivos(char * archivo, int* respuesta);



#endif /* SRC_MANEJOARCHIVOS_H_ */
