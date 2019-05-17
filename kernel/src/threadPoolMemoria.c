//
// Created by miguelchauca on 28/04/19.
//

#include "threadPoolMemoria.h"

t_list *poolMemoria;
extern config *configuracion;
extern t_log *file_log;
pthread_mutex_t mutex;

void show(){
    st_data_memoria * memoria;
    int i;
    for (i = 0; i < poolMemoria->elements_count ; ++i) {
        memoria = list_get(poolMemoria,i);
        printf("%s\n",memoria->ip);
        printf("%s\n",memoria->puerto);
    }
}

void *loadPoolMemori() {
    //conectarse con la memoria y pelirle la lista
    int socketClient;
    int control = 0;
    message *bufferMensaje;
    header request;
    header response;
    void *buffer = NULL;
    bool error;
    poolMemoria = NULL;
    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return NULL;
    }
    while (1) {
        error = true;
        control = 0;
        socketClient = establecerConexion(configuracion->IP_MEMORIA, configuracion->PUERTO_MEMORIA, file_log, &control);
        if (socketClient != -1) {
            request.letra = 'K';
            request.codigo = 5;
            request.sizeData = 1;
            bufferMensaje = createMessage(&request, " ");
            enviar_message(socketClient, bufferMensaje, file_log, &control);
            free(bufferMensaje);
            if (control == 0) {
                buffer = getMessage(socketClient, &response, &control);
                if (buffer) {
                    error = false;
                    pthread_mutex_lock(&mutex);
                    if (poolMemoria != NULL) {
                        list_destroy(poolMemoria);
                    }
                    poolMemoria = deserealizarListaDataMemoria(buffer, response.sizeData);
                    pthread_mutex_unlock(&mutex);
                    close(socketClient);
                    free(buffer);
                }
            }
        }
        if (error) {
            list_create(poolMemoria);
        }
        sleep(5);
    }
}