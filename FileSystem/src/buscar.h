/*
 * buscar.h
 *
 *  Created on: 15 may. 2019
 *      Author: utnso
 */

#ifndef SRC_BUSCAR_H_
#define SRC_BUSCAR_H_


char * buscarKey(char * name, int key, int particion);
structRegistro * buscarEnLista(t_list *list, int key);
structRegistro * buscarEnArchivo(char * path);



#endif /* SRC_BUSCAR_H_ */
