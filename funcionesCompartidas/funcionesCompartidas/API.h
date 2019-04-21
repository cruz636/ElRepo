//
// Created by miguelchauca on 21/04/19.
//

#ifndef KERNEL_API_H
#define KERNEL_API_H
#include <commons/string.h>
#include <stdlib.h>
#include <string.h>
enum OPERACION{
    INSERT = 1,
    SELECT = 2,
    CREATE = 3,
    DROP = 4
};

typedef struct {
    enum OPERACION operacion;
    char * nameTable;
    char * tipoConsistencia;
    int numeroParticiones;
    int compactionTime;
}__attribute__((packed)) structCreate;

typedef struct {
    enum OPERACION operacion;
    char * nameTable;
    int key;
}__attribute__((packed)) structSelect;

typedef struct {
    enum OPERACION operacion;
    char * nameTable;
    int key;
    char * value;
    int timestamp;
} __attribute__((packed)) structInsert;

typedef struct {
    enum OPERACION operacion;
    char * nameTable;
}__attribute__((packed)) structDrop;

structInsert * cargarInsert(char * comando, bool requireTimeStamp);
structSelect * cargarSelect(char * comando);
structCreate * cargarCreate(char * comando);
structDrop * cargarDrop(char * comando);

#endif //KERNEL_API_H
