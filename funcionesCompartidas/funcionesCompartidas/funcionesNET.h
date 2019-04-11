#ifndef FUNCIONES_NET
#define FUNCIONES_NET
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <commons/log.h>
#include <commons/string.h>
#include "log.h"
typedef struct {
    void * buffer;
    size_t sizeBuffer;
}message;

typedef struct  {
    char letra;
    int codigo;
    size_t sizeData;
} __attribute__((packed)) header;

int establecerConexion(char *ip_dest, char *port_dest, t_log *log, int *control);
int makeListenSock(char *port_listen, t_log *log, int *control);
int aceptar_conexion(int socket_in, t_log *log, int *control);
message *createMessage(header *head, void *data);
int enviar_message(int socket, message *message, t_log *log, int *control);
char *getMessage(int socket, header *head, int *status);
#endif