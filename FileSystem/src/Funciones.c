/*
 * Funciones.c
 *
 *  Created on: 12 abr. 2019
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <funcionesCompartidas/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>
#include <commons/bitarray.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

struct stat mystat;

extern char* puerto;
extern char* montaje;
extern int retardo;
extern int tam_value;
extern int tiempo_dump;

extern char* magic_number;
extern int tBloques;
extern int cantBloques;

extern t_dictionary * clientes;
extern t_log* alog;
extern t_bitarray* bitmap;
extern char* posicion;
extern int bitm;

void inicializar(){
	montaje = strdup("");
	magic_number = strdup("");
	puerto = strdup("");
	clientes = dictionary_create();
	alog = crear_archivo_log("File System", true, "/home/utnso/Escritorio");
}

void archivoDeConfiguracion(char* argv)
{
	t_config *configuracion;
	printf("ruta archivo de configuacion: %s \n", argv);
	configuracion = config_create(argv);
	string_append(&puerto, config_get_string_value(configuracion, "PUERTO_ESCUCHA​"));
	string_append(&montaje, config_get_string_value(configuracion, "PUNTO_MONTAJE"));
	retardo = config_get_int_value(configuracion, "RETARDO");
	tam_value = config_get_int_value(configuracion, "TAMAÑO_VALUE");
	tiempo_dump = config_get_int_value(configuracion, "TIEMPO_DUMP");

	log_info(alog, "Lee el archivo de configuracion");


	config_destroy(configuracion);
}

int leer_metadata()
{
	char *ruta = strdup("");
	t_config *configuracion;

	string_append(&ruta,montaje);
	string_append(&ruta,"/Metadata/Metadata.bin");

	configuracion = config_create(ruta);

	tBloques = config_get_int_value(configuracion, "TAMANIO_BLOQUES");
	cantBloques = config_get_int_value(configuracion, "CANTIDAD_BLOQUES");
	string_append(&magic_number, config_get_string_value(configuracion, "MAGIC_NUMBER"));

	if(strcmp(magic_number, "LISSANDRA"))
	{
		config_destroy(configuracion);
		log_info(alog, "No es LISSANDRA");
		free(ruta);
		return -1;
	}

	//escribir_log_con_numero("Tamanio de Bloques: ", tBloques);
	//escribir_log_con_numero("Cantidad de bloques: ", cantBloques);
	//string_append(&magic_number, "\n");
	//escribir_log_compuesto("Magic Number: ",magic_number);

	free(ruta);
	config_destroy(configuracion);
	return 0;

}

int abrir_bitmap()
{
	char *ruta = strdup("");

	string_append(&ruta,montaje);
	string_append(&ruta,"/Metadata/Bitmap.bin");

	int fdbitmap = open(ruta,O_RDWR);
	free(ruta);
	if(fdbitmap==0){
		log_info(alog,"no abrio el bitmap\n");
		close(fdbitmap);
		return -1;
	}

	fstat(fdbitmap,&mystat);

	posicion = mmap(0,mystat.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fdbitmap,0);
	if(posicion == MAP_FAILED){
		log_info(alog,"error en mmap\n");
		fprintf(stderr, "mmap failed: %s\n", strerror(errno));
		close(fdbitmap);
		return -1;
	}
	bitmap = bitarray_create_with_mode(posicion,mystat.st_size,LSB_FIRST);


	return 0;

}

void finalizar(){

	free(montaje);
	free(magic_number);
	free(puerto);
	if(bitm != -1){
		memcpy(posicion,bitmap,mystat.st_size);
		msync(posicion,mystat.st_size,MS_SYNC);
		munmap(posicion,mystat.st_size);
		bitarray_destroy(bitmap);
	}
	free(posicion);
	dictionary_clean(clientes);
	dictionary_destroy(clientes);
	liberar_log(alog);
}



