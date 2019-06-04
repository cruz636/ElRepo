/*
 * manejoArchivos.c
 *
 *  Created on: 28 abr. 2019
 *      Author: utnso
 */

#include "manejoArchivos.h"
#include <commons/collections/dictionary.h>
#include <commons/bitarray.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <stdio.h>
#include "Funciones.h"
#include "operaciones.h"
#include "buscar.h"

extern t_dictionary * memtable;


int realizarInsert(st_insert * insert){

	int respuesta;
	t_list * lista;
	structMetadata * metadata;

	structRegistro * registro;

	if(validarArchivos(insert->nameTable, &respuesta)){

		/*value = string_new();

		string_append(&value,string_itoa(insert->timestamp));
		string_append(&value,";");
		string_append(&value,string_itoa(insert->key));
		string_append(&value,";");
		string_append(&value,insert->value);*/

		registro = malloc(sizeof(structRegistro *));
		registro->time = insert->timestamp;
		registro->key = insert->key;
		registro->value = insert->value;

		metadata = leerMetadata(insert->nameTable);
		if(dictionary_has_key(memtable, insert->nameTable)){
			lista = dictionary_get(memtable, insert->nameTable);

			list_add(lista, registro);

		}else{
			lista = list_create();
			list_add(lista, registro);

			dictionary_put(memtable,insert->nameTable, lista);
		}

		respuesta = 5;

		free(metadata->consistency);
		free(metadata);
	}

	return respuesta;
}

int realizarSelect(st_select * select, char ** value){

	int respuesta;
	structMetadata * metadata;
	int particion;

	if(validarArchivos(select->nameTable, &respuesta)){
		metadata = leerMetadata(select->nameTable);

		particion = select->key % metadata->partitions;

		*value = buscarKey(select->nameTable,select->key, particion);

		if(*value ==NULL){
			respuesta = 6;
		}else{
			respuesta = 10;
		}

		free(metadata->consistency);
		free(metadata);
	}

	return respuesta;
}

int realizarCreate(st_create * create){
	int respuesta;
	char * path;
	int part;

	if(!validarArchivos(create->nameTable, &respuesta)){
			path = armar_path(create->nameTable);
			char * pathmkdir= string_from_format("sudo mkdir -p %s",path);
			system(pathmkdir);
			free(pathmkdir);

			if(!crearMetadata(create,path)){
				eliminarDirectorio(path);
				free(path);
				return 10;
			}

			part = crearParticiones(create, path);
			if(part != 0){
				for (int i = 1; i < part; ++i) {
					eliminarParticion(path,i);
				}
				eliminarDirectorio(path);
				free(path);
				return 10;
			}

			free(path);

			respuesta = 8;

	}else{
		respuesta = 7;
	}


	return respuesta;
}

int realizarDrop(st_drop * drop){
	int respuesta;
	structMetadata * metadata;
	char * path;

	if(validarArchivos(drop->nameTable, &respuesta)){
		metadata = leerMetadata(drop->nameTable);

		path = armar_path(drop->nameTable);

		for (int i = 1; i <= metadata->partitions; i++) {
			eliminarParticion(path, i);
		}

		eliminarDirectorio(path);

		respuesta = 9;

		free(path);
		free(metadata->consistency);
		free(metadata);
	}


	return respuesta;
}

bool validarArchivos(char * archivo, int * respuesta){
	FILE * file;
	char *path;

	path = armar_path(archivo);

	file = fopen(path,"r+");

	if(file == NULL){
		*respuesta = 4;
		free(path);
		return false;
	}

	fclose(file);

	free(path);

	return true;
}


