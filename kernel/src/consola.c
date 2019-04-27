//
// Created by miguelchauca on 21/04/19.
//
#include <funcionesCompartidas/API.h>
#include <funcionesCompartidas/funcionesNET.h>
#include "consola.h"
#include "load_config.h"

extern t_log* file_log;
extern config * configuracion;
int client;

void enviarMjs(void * buffer){
    int control= 0;
    if (enviar_message(client, buffer, file_log, &control) < 0) {
        log_info(file_log, "Error al enviar el bloque");
    }
}

void armarComando(char * comando){
    bool flagErro = true;
    bool flagErrorSintaxis = false;
    if(string_contains(comando,"INSERT")){
        structInsert * insert;
        flagErro = false;
        flagErrorSintaxis = true;
        if((insert = cargarInsert(comando, true))){
            size_t lenght;
            void * insertSerealizado = serealizarInsert(insert,&lenght);
            header request;
            request.letra = 'K';
            request.codigo = 1;
            request.sizeData = lenght;
            void * buffer = createMessage(&request,insertSerealizado);
            enviarMjs(buffer);
            flagErrorSintaxis = false;
            log_info(file_log, "EJECUTANDO COMANDO INSERT");
        }
    }
    if(string_contains(comando, "SELECT")){
        structSelect * _select;
        flagErro = false;
        flagErrorSintaxis = true;
        if((_select = cargarSelect(comando))){
            flagErrorSintaxis = false;
            log_info(file_log, "EJECUTANDO COMANDO SELECT");
        }
    }
    if(string_contains(comando, "CREATE")){
        structCreate * _create;
        flagErro = false;
        flagErrorSintaxis = true;
        if((_create = cargarCreate(comando))){
            flagErrorSintaxis = false;
            log_info(file_log, "EJECUTANDO COMANDO SELECT");
        }
    }

    if(string_contains(comando, "DROP")){
        structDrop * _drop;
        flagErro = false;
        flagErrorSintaxis = true;
        if((_drop = cargarDrop(comando))){
            flagErrorSintaxis = false;
            log_info(file_log, "EJECUTANDO COMANDO SELECT");
        }
    }

    if(flagErro){
        log_error(file_log, "Comando no reconocido");
    }

    if(flagErrorSintaxis){
        log_error(file_log, "Verificar el comando ingresado");
    }
}

void consola(){
    int control = 0;
    char * comando;
    client = establecerConexion(configuracion->IP_MEMORIA,configuracion->PUERTO_MEMORIA,file_log,&control);
    if (control != 0) {
        log_error(file_log, "Error al intentar establecer conneccion");
        return;
    }
    comando = readline(">");
    printf("Ingrese comando LQL\n");
    while (strcmp(comando,"exit") != 0){
        armarComando(comando);
        comando = readline(">");
    }
}