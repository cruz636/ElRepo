/*
 * operaciones.h
 *
 *  Created on: 1 may. 2019
 *      Author: utnso
 */

#ifndef SRC_OPERACIONES_H_
#define SRC_OPERACIONES_H_


structMetadata * leerMetadata(char * archivo);
char * buscarKey(int key, int particion);
int verificar_bloque();
char * armar_path(char * archivo);
bool crearMetadata(structCreate * c, char * path);
int crearParticiones(structCreate * c, char * path);
void eliminarDirectorio(char *path);
void eliminarParticion(char *path, int particion);
structParticion * leerParticion(char * path);


#endif /* SRC_OPERACIONES_H_ */
