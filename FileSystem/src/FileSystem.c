/*
 ============================================================================
 Name        : File.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "Funciones.h"
#include <funcionesCompartidas/log.h>
#include <funcionesCompartidas/funcionesNET.h>
#include <pthread.h>
#include "hiloSelect.h"
#include "hiloConsola.h"
#include <commons/collections/dictionary.h>
#include <commons/bitarray.h>


char* magic_number;
int tBloques;
int cantBloques;

t_log* alog;
t_bitarray* bitmap;
int socketfs;
int controlador;
char* posicion;
int bitm;

structConfig * config;
pthread_t hiloConsola, hiloSelect;
t_dictionary * clientes, *memtable;


int main(int argc, char *argv[]) {

	int metadata;

	inicializar();
	archivoDeConfiguracion(argv[1]);

	metadata = leer_metadata();
	bitm = abrir_bitmap();


	if(!(metadata ==-1 || bitm ==-1)){
		//pthread_create(&hiloSelect, NULL, (void*)hiloselect,NULL);
		pthread_create(&hiloConsola, NULL, (void*)hiloconsola,NULL);


		//pthread_join(hiloSelect,NULL);
		pthread_join(hiloConsola, NULL);
	}


	finalizar();

	return EXIT_SUCCESS;
}


