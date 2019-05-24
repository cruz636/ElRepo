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
#include <sys/stat.h>
#include <sys/mman.h>

extern structConfig * config;
extern t_bitarray* bitmap;
extern int cantBloques;

extern char* posicion;
extern struct stat mystat;

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

void actualizar_bitmap(){

	memcpy(posicion,bitmap,mystat.st_size);
	msync(posicion,mystat.st_size,MS_SYNC);

}

char * armar_path(char * archivo){
	char * path = string_new();
	string_append(&path,config->montaje);
	string_append(&path,"/Tables/");
	string_to_upper(archivo);
	string_append(&path,archivo);

	return path;
}

char * armar_PathBloque(char * bloque){
	char * path = string_new();
	string_append(&path,config->montaje);
	string_append(&path,"/Bloques/");
	string_append(&path,bloque);
	string_append(&path,".bin");

	return path;
}

/*char * path_bin(char * name, int particion){
	char * path = armar_path(name);

}*/

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
	int flag=-1;

	for (int i = 0; i < c->numeroParticiones; i++) {
		if(flag == -1){

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

void actualizar_Particion(structActualizar * a){
	char * path = armar_path(a->nameTable);
	char * pathPart = string_from_format("%s/%d.bin",path , a->particion);
	t_config *configuracion;
	int size;
	char ** bloques;
	int i;

	configuracion = config_create(path);

	size = config_get_int_value(configuracion, "SIZE");
	config_set_value(configuracion, "SIZE", string_itoa(size+(a->size)));

	bloques = config_get_array_value(configuracion, "BLOQUES");
	while(bloques[i] != NULL) i++;
	bloques[i] = string_itoa(a->bit);
	bloques[i++] = NULL;

	config_get_string_value(configuracion, "BLOQUES");

	config_save(configuracion);

	config_destroy(configuracion);

	free(path);
	free(pathPart);
	string_iterate_lines(bloques, (void*)free);
	free(bloques);
}
