//
// Created by miguelchauca on 21/04/19.
//

#include "API.h"

structInsert * cargarInsert(char * comando, bool requireTimeStamp){
    char * hayString;
    structInsert * insert = malloc(sizeof(structInsert));
    insert->operacion = INSERT;
    char ** listSplit = string_split(comando, "\"");
    if(listSplit[1] == NULL){
        free(insert);
        return  NULL;
    }
    insert->value = strdup(listSplit[1]);
    if(requireTimeStamp){
        if(listSplit[2]){
            insert->timestamp = strtol(listSplit[2],&hayString,10);
            if(!string_is_empty(hayString)){
                free(insert);
                return NULL;
            }
        }else{
            free(insert);
            return  NULL;
        }
    }
    listSplit = string_split(listSplit[0]," ");
    if(listSplit[1] == NULL){
        free(insert);
        return NULL;
    }
    insert->nameTable = strdup(listSplit[1]);
    if(listSplit[2] == NULL){
        free(insert);
        return NULL;
    }
    insert->key = strtol(listSplit[2],&hayString,10);
    if(!string_is_empty(hayString)){
        free(insert);
        return NULL;
    }
    return insert;
}

structSelect * cargarSelect(char * comando){
    char * hayString;
    structSelect * _select = malloc(sizeof(structSelect));
    _select->operacion = SELECT;
    char ** listSplit = string_split(comando, " ");
    if(listSplit[1] == NULL){
        free(_select);
        return  NULL;
    }
    _select->nameTable = strdup(listSplit[1]);
    if(listSplit[2] == NULL){
        free(_select);
        return  NULL;
    }
    _select->key = strtol(listSplit[2],&hayString,10);
    if(!string_is_empty(hayString)){
        free(_select);
        return NULL;
    }
    return _select;
}

structCreate * cargarCreate(char * comando){
    char * hayString;
    structCreate * _create = malloc(sizeof(structCreate));
    _create->operacion = CREATE;
    char ** listSplit = string_split(comando, " ");
    if(listSplit[1] == NULL){
        free(_create);
        return NULL;
    }
    _create->nameTable = strdup(listSplit[1]);
    if(listSplit[2] == NULL){
        free(_create);
        return  NULL;
    }
    _create->tipoConsistencia = strdup(listSplit[2]);
    if(listSplit[3] == NULL){
        free(_create);
        return  NULL;
    }
    _create->numeroParticiones = strtol(listSplit[3],&hayString,10);
    if(!string_is_empty(hayString)){
        free(_create);
        return NULL;
    }
    if(listSplit[4] == NULL){
        free(_create);
        return  NULL;
    }
    _create->compactionTime = strtol(listSplit[4],&hayString,10);
    if(!string_is_empty(hayString)){
        free(_create);
        return NULL;
    }
    return _create;

}

structDrop * cargarDrop(char * comando){
    structDrop * _drop = malloc(sizeof(structCreate));
    _drop->operacion = DROP;
    char ** listSplit = string_split(comando, " ");
    if(listSplit[1] == NULL){
        free(_drop);
        return NULL;
    }
    _drop->nameTable = strdup(listSplit[1]);
    return _drop;
}