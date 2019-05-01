/*
 * operaciones.c
 *
 *  Created on: 1 may. 2019
 *      Author: utnso
 */

#include "manejoArchivos.h"
#include <stdio.h>
#include "operaciones.h"
#include <commons/bitarray.h>
#include <commons/config.h>
#include "Funciones.h"

extern structConfig * config;
extern t_bitarray* bitmap;
extern int cantBloques;


structMetadata * leerMetadata(char * archivo){
	char *path;
	structMetadata * metadata = malloc(sizeof(structMetadata));
	t_config *configuracion;

	path = armar_path(archivo);
	string_append(&path,"/Metadata");

	configuracion = config_create(path);

	string_append(&metadata->consistency, config_get_string_value(configuracion, "CONSISTENCY"));
	metadata->partitions = config_get_int_value(configuracion, "PARTITIONS");
	metadata->compaction_time = config_get_int_value(configuracion, "COMPACTION_TIME");

	config_destroy(configuracion);
	free(path);

	return metadata;
}

char * buscarKey(int key, int particion){
	char * value;


	return value;
}

int verificar_bloque()
{
	int bit = -1;
	bool comprobar;
	while(bit < cantBloques && comprobar == true)
	{
		bit++;
		comprobar = bitarray_test_bit(bitmap,bit);

	}
	if(bit == cantBloques && comprobar == false){
		return -1;
	}
	else {
		return bit;
	}

}

char * armar_path(char * archivo){
	char * path = string_new();
	string_append(&path,config->montaje);
	string_append(&path,"/Tables/");
	string_to_upper(archivo);
	string_append(&path,archivo);

	return path;
}

bool crearMetadata(structCreate * c, char * path){
	char * contenido;
	FILE * archivo;
	char * meta = string_new();

	contenido = string_from_format("CONSISTENCY=%s\nPARTITIONS=%d\nCOMPACTION_TIME=%d", c->tipoConsistencia,c->numeroParticiones,c->compactionTime);

	string_append(&meta,path);
	string_append(&meta,"/Metadata");
	archivo = fopen(meta,"a+");

	free(meta);

	if(archivo == NULL){
		free(contenido);
		return false;
	}else{
		fputs(contenido, archivo);
		free(contenido);
		fclose(archivo);
		return true;
	}
}

int crearParticiones(structCreate * c, char * path){
	FILE * archivo;
	char * contenido;
	char * completo;
	int bit;
	int flag=0;

	for (int i = 1; i <= c->numeroParticiones; i++) {
		if(flag == 0){

			bit = verificar_bloque();
			if(bit == -1)
			{
				flag = i;

			}else{
				bitarray_set_bit(bitmap,bit);
				contenido = string_from_format("SIZE=0\nBLOQUES=[%d]", bit);

				completo = string_from_format("%s/%d.bin", path, i);
				archivo = fopen(completo, "a+");

				fputs(contenido,archivo);

				fclose(archivo);

				free(contenido);
				free(completo);
			}
		}
	}
	return flag;
}

void eliminarDirectorio(char * path){
	char * pathrm= string_from_format("sudo rm -rf %s",path);
	system(pathrm);
	free(pathrm);
}

void eliminarParticion(char *path, int particion){
	structParticion * contenido;
	char * part = string_from_format("%s/%d.bin", path, particion);
	int i = 0;

	contenido = leerParticion(part);

	while(contenido->bloques[i] != NULL)
	{
		bitarray_clean_bit(bitmap,atoi(contenido->bloques[i]));
		i++;
	}

	remove(part);

	free(part);
	string_iterate_lines(contenido->bloques, (void*)free);
	free(contenido->bloques);
	free(contenido);
}

structParticion * leerParticion(char * path){
	structParticion * contenido = malloc(sizeof(structParticion));
	t_config *config=config_create(path);

	contenido->Size = config_get_int_value(config,"SIZE");
	contenido->bloques = config_get_array_value(config,"BLOQUES");

	config_destroy(config);

	return contenido;
}
