//
// Created by miguelchauca on 21/04/19.
//

#include "consola.h"

extern t_log* file_log;

void armarComando(char * comando){
    bool flagErro = true;
    bool flagErrorSintaxis = false;
    if(string_contains(comando,"INSERT")){
        structInsert * insert;
        flagErro = false;
        flagErrorSintaxis = true;
        if((insert = cargarInsert(comando, true))){
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
    char * comando;
    comando = readline(">");
    printf("Ingrese comando LQL\n");
    while (strcmp(comando,"exit") != 0){
        armarComando(comando);
        comando = readline(">");
    }
}