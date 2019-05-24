/*
 * buscar.c
 *
 *  Created on: 15 may. 2019
 *      Author: utnso
 */

#include "manejoArchivos.h"
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include "operaciones.h"
#include "buscar.h"

extern t_dictionary * memtable;

char * buscarKey(char * name, int key, int particion){
	char * value;
	long int time = 0;

	t_list * lista;
	structRegistro * reg;

	char * path = armar_path(name);
	char * completo;

	if(dictionary_has_key(memtable,name)){
		lista = dictionary_get(memtable, name);

		reg = buscarEnLista(lista, key);

		if(reg != NULL){
			time = reg->time;
			value = strdup(reg->value);
			free(reg);
		}
	}

	completo = string_from_format("%s/%d.bin", path, particion);
	reg = buscarEnArchivo(completo);

	if(reg != NULL){
		if(time < reg->time){
			time = reg->time;
			value = strdup(reg->value);
		}

		free(reg);
	}
	free(completo);

	completo = string_from_format("%s/%d.tmp", path, particion);
	reg = buscarEnArchivo(completo);

	if(reg != NULL){
		if(time < reg->time){
			time = reg->time;
			value = strdup(reg->value);
		}

		free(reg);
	}
	free(completo);

	free(path);
	return value;
}

structRegistro * buscarEnLista(t_list *list, int key){
	structRegistro * reg;


	return reg;
}

structRegistro * buscarEnArchivo(char * path){
	structRegistro * reg;


	return reg;
}


