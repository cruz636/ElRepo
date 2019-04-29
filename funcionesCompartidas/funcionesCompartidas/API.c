//
// Created by miguelchauca on 21/04/19.
//

#include "API.h"

structInsert * cargarInsert(char * comando, bool requireTimeStamp){
    char * hayString;
    structInsert * insert = malloc(sizeof(structInsert));
    insert->operacion = INSERT;
    char ** listSplit = string_split(comando, "\"");
    char ** listSplit2;
    if(listSplit[1] == NULL){
    	string_iterate_lines(listSplit, (void*)free);
    	free(listSplit);

        free(insert);
        return  NULL;
    }
    insert->value = strdup(listSplit[1]);
    if(requireTimeStamp){
        if(listSplit[2]){
            insert->timestamp = strtol(listSplit[2],&hayString,10);
            if(!string_is_empty(hayString)){
            	string_iterate_lines(listSplit, (void*)free);
            	free(listSplit);

                free(insert);
                return NULL;
            }
        }else{
        	string_iterate_lines(listSplit, (void*)free);
        	free(listSplit);

            free(insert);
            return  NULL;
        }
    }

    listSplit2 = string_split(listSplit[0]," ");
    if(listSplit2[1] == NULL){
    	string_iterate_lines(listSplit, (void*)free);
    	free(listSplit);

        free(insert);
        return NULL;
    }

    insert->nameTable = strdup(listSplit2[1]);
    if(listSplit2[2] == NULL){
    	string_iterate_lines(listSplit, (void*)free);
    	free(listSplit);

    	string_iterate_lines(listSplit2, (void*)free);
    	free(listSplit2);

        free(insert);
        return NULL;
    }
    insert->key = strtol(listSplit2[2],&hayString,10);
    if(!string_is_empty(hayString)){
    	string_iterate_lines(listSplit, (void*)free);
    	free(listSplit);

    	string_iterate_lines(listSplit2, (void*)free);
    	free(listSplit2);
        free(insert);
        return NULL;
    }

    string_iterate_lines(listSplit, (void*)free);
    free(listSplit);

    string_iterate_lines(listSplit2, (void*)free);
    free(listSplit2);
    return insert;
}

structSelect * cargarSelect(char * comando){
    char * hayString;
    structSelect * _select = malloc(sizeof(structSelect));
    _select->operacion = SELECT;
    char ** listSplit = string_split(comando, " ");
    if(listSplit[1] == NULL){
    	string_iterate_lines(listSplit, (void*)free);
    	free(listSplit);

        free(_select);
        return  NULL;
    }
    _select->nameTable = strdup(listSplit[1]);
    if(listSplit[2] == NULL){
    	string_iterate_lines(listSplit, (void*)free);
    	free(listSplit);

        free(_select);
        return  NULL;
    }
    _select->key = strtol(listSplit[2],&hayString,10);
    if(!string_is_empty(hayString)){
    	string_iterate_lines(listSplit, (void*)free);
    	free(listSplit);

        free(_select);
        return NULL;
    }

    string_iterate_lines(listSplit, (void*)free);
    free(listSplit);
    return _select;
}

structCreate * cargarCreate(char * comando){
    char * hayString;
    structCreate * _create = malloc(sizeof(structCreate));
    _create->operacion = CREATE;
    char ** listSplit = string_split(comando, " ");
    if(listSplit[1] == NULL){
    	string_iterate_lines(listSplit, (void*)free);
    	free(listSplit);

        free(_create);
        return NULL;
    }
    _create->nameTable = strdup(listSplit[1]);
    if(listSplit[2] == NULL){
    	string_iterate_lines(listSplit, (void*)free);
    	free(listSplit);

        free(_create);
        return  NULL;
    }
    _create->tipoConsistencia = strdup(listSplit[2]);
    if(listSplit[3] == NULL){
    	string_iterate_lines(listSplit, (void*)free);
    	free(listSplit);

        free(_create);
        return  NULL;
    }
    _create->numeroParticiones = strtol(listSplit[3],&hayString,10);
    if(!string_is_empty(hayString)){
    	string_iterate_lines(listSplit, (void*)free);
    	free(listSplit);

        free(_create);
        return NULL;
    }
    if(listSplit[4] == NULL){
    	string_iterate_lines(listSplit, (void*)free);
    	free(listSplit);

        free(_create);
        return  NULL;
    }
    _create->compactionTime = strtol(listSplit[4],&hayString,10);
    if(!string_is_empty(hayString)){
    	string_iterate_lines(listSplit, (void*)free);
    	free(listSplit);

        free(_create);
        return NULL;
    }

    string_iterate_lines(listSplit, (void*)free);
    free(listSplit);
    return _create;

}

structDrop * cargarDrop(char * comando){
    structDrop * _drop = malloc(sizeof(structCreate));
    _drop->operacion = DROP;
    char ** listSplit = string_split(comando, " ");
    if(listSplit[1] == NULL){
    	string_iterate_lines(listSplit, (void*)free);
    	free(listSplit);

        free(_drop);
        return NULL;
    }
    _drop->nameTable = strdup(listSplit[1]);

    string_iterate_lines(listSplit, (void*)free);
    free(listSplit);
    return _drop;
}

void * serealizarInsert(structInsert * insert, size_t  * length){
    stInsertSize sizeStruct;
    sizeStruct.size_operacion = sizeof(insert->operacion);
    sizeStruct.size_nametable = strlen(insert->nameTable) + 1;
    sizeStruct.size_key = sizeof(insert->key);
    sizeStruct.size_value = strlen(insert->value) + 1;
    sizeStruct.size_timestamp = sizeof(insert->timestamp);
    size_t sizeBuffer = sizeof(sizeStruct) + sizeStruct.size_operacion + sizeStruct.size_nametable + sizeStruct.size_key + sizeStruct.size_value + sizeStruct.size_timestamp;
    void * buffer = malloc(sizeBuffer);
    int offset = 0;
    memcpy(buffer,&sizeStruct, sizeof(sizeStruct));
    offset += sizeof(sizeStruct);

    memcpy((buffer + offset),&insert->operacion,sizeStruct.size_operacion);
    offset += sizeStruct.size_operacion;

    memcpy((buffer + offset),insert->nameTable,sizeStruct.size_nametable);
    offset += sizeStruct.size_nametable;

    memcpy((buffer + offset),&insert->key,sizeStruct.size_key);
    offset += sizeStruct.size_key;

    memcpy((buffer + offset),insert->value,sizeStruct.size_value);
    offset += sizeStruct.size_value;

    memcpy((buffer + offset),&insert->timestamp,sizeStruct.size_timestamp);
    *length = sizeBuffer;
    return buffer;
}

structInsert * desserealizarInsert(void * buffer){
    stInsertSize sizeStruct;
    structInsert * insert = malloc(sizeof(structInsert *));
    int offset = 0;

    memcpy(&sizeStruct,(buffer + offset), sizeof(sizeStruct));
    offset += sizeof(sizeStruct);

    memcpy(&insert->operacion,(buffer + offset), sizeStruct.size_operacion);
    offset += sizeStruct.size_operacion;

    insert->nameTable = malloc(sizeStruct.size_nametable);
    memcpy(insert->nameTable,(buffer + offset), sizeStruct.size_nametable);
    offset += sizeStruct.size_nametable;

    memcpy(&insert->key,(buffer + offset), sizeStruct.size_key);
    offset += sizeStruct.size_key;

    insert->value = malloc(sizeStruct.size_value);
    memcpy(insert->value,(buffer + offset), sizeStruct.size_value);
    offset += sizeStruct.size_value;

    memcpy(&insert->timestamp,(buffer + offset), sizeStruct.size_timestamp);

    return insert;
}
