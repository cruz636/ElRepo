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
#include <Funciones.h>
#include <funcionesCompartidas/log.h>
#include <funcionesCompartidas/funcionesNET.h>
#include <pthread.h>

char* puerto;
char* montaje;
int retardo;
int tam_value;
int tiempo_dump;

char* magic_number;
int tBloques;
int cantBloques;

t_log* log;
t_bitarray* bitmap;
int socketfs;
int controlador;
char* posicion;
int bitm;


int main(int argc, char *argv[]) {

	int metadata;
	int flag=0;
	fd_set readset, tempset;
	int fdmax, selectResult;
	int status;

	inicializar();
	archivoDeConfiguracion(argv[1]);

	metadata = leer_metadata();
	if(metadata == -1) goto finalizar;

	bitm = abrir_bitmap();
	if(bitm == -1) goto finalizar;

	//sockets
	controlador=0;
	socketfs = makeListenSock(puerto, log, &controlador);
	if(socketfs < 0) goto finalizar;

	FD_ZERO(&readset);
	FD_ZERO(tempset);

	FD_SET(socketfs,&readset);
	fdmax = socketfs;

	while(1)
	{
		memcpy(&tempset, &readset, sizeof(tempset));

		selectResult = select(fdmax + 1, &readset, NULL, NULL, NULL);
		log_info(log, "Registro de actividad en Select");

		if (selectResult < 0) log_info(log, "Error en el administrador de conexiones");
		else {
		 //Recorro los descriptores para ver quien llamo
			 //int i;
			 for (int i = 0; i <= fdmax; i++)
			 {
				 if (FD_ISSET(i, &tempset))
				 {
					 //Se detecta un cliente nuevo
					 if (i == socketfs)
					 {
						 int nuevo_socket = aceptar_conexion(socketfs, log, &controlador);
						 if (!(nuevo_socket < 0))
						 {
							 FD_SET(nuevo_socket, &readset);
							 fdmax = (fdmax < nuevo_socket)?nuevo_socket:fdmax;
						 }
					 }else {
						 //Un cliente envia un mensaje
						 mensaje mensRecibido = malloc(sizeof(mensaje));
						 mensRecibido.buffer = getMessage(i,&mensRecibido.head,&status);

						 if(mensRecibido.buffer == NULL){
							 //Se desconecto un cliente
							 log_info(log, "se desconecto el socket client %d",i);
							 close(i);
							 FD_CLR(i, &readset);
							 free(mensRecibido);
						 }else {
							 //Se recibio un mensaje
							 pthread_t hilo;
							 pthread_create(&hilo, NULL, (void*)tratarMensaje,&mensRecibido);
						 }
					 }
				 }
			 }
		}
	}


	if(metadata ==-1 || bitm ==-1){
		finalizar: log_info(log, "Error de lectura de archivos iniciales");
	}
	finalizar();
	return EXIT_SUCCESS;
}


