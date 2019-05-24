/*
 * archivos.h
 *
 *  Created on: 18 may. 2019
 *      Author: utnso
 */

#ifndef SRC_ARCHIVOS_H_
#define SRC_ARCHIVOS_H_

typedef struct {
	char * nameTable;
	int particion;
	int bit;
	int size;
}structActualizar;


int Insert(st_insert * insert);
int escribir(int particion, structParticion * part, st_insert * insert);


#endif /* SRC_ARCHIVOS_H_ */
