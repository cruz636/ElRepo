//
// Created by miguelchauca on 21/04/19.
//
#include "consola.h"

extern t_log *file_log;
extern config *configuracion;

void crearListInstrucciones(void *instruccion, enum OPERACION type) {
    stinstruccion *newInstruccion = malloc(sizeof(stinstruccion));
    newInstruccion->operacion = type;
    newInstruccion->instruccion = instruccion;
    t_list *listInstrucciones = list_create();
    list_add(listInstrucciones, newInstruccion);
    cargarNuevoScript(listInstrucciones);
}

void armarComando(char *comando) {
    bool flagErrorSintaxis = false;
    int typeComando = getEnumFromString(comando);
    switch (typeComando) {
        case INSERT: {
            structInsert *insert;
            flagErrorSintaxis = true;
            if ((insert = cargarInsert(comando, true))) {
                crearListInstrucciones(insert, INSERT);
                log_info(file_log, "EJECUTANDO COMANDO INSERT");
                flagErrorSintaxis = false;
            }
            break;
        }
        case SELECT: {
            structSelect *_select;
            flagErrorSintaxis = true;
            if ((_select = cargarSelect(comando))) {
                crearListInstrucciones(_select, SELECT);
                flagErrorSintaxis = false;
                log_info(file_log, "EJECUTANDO COMANDO INSERT");
            }
            break;
        }
        case DROP: {
            structDrop *_drop;
            flagErrorSintaxis = true;
            if ((_drop = cargarDrop(comando))) {
                crearListInstrucciones(_drop, DROP);
                flagErrorSintaxis = false;
                log_info(file_log, "EJECUTANDO COMANDO SELECT");
            }
            break;
        }
        case CREATE: {
            structCreate *_create;
            flagErrorSintaxis = true;
            if ((_create = cargarCreate(comando))) {
                crearListInstrucciones(_create, CREATE);
                flagErrorSintaxis = false;
                log_info(file_log, "EJECUTANDO COMANDO SELECT");
            }
            break;
        }
        default: {
            log_error(file_log, "Comando no reconocido");
        }
    }

    if (flagErrorSintaxis) {
        log_error(file_log, "Verificar el comando ingresado");
    }
}

void consola() {
    char *comando;
    comando = readline(">");
    printf("Ingrese comando LQL\n");
    while (strcmp(comando, "exit") != 0) {
        armarComando(comando);
        free(comando);
        comando = readline(">");
    }
}