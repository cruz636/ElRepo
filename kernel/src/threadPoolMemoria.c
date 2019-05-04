//
// Created by miguelchauca on 28/04/19.
//

#include "threadPoolMemoria.h"

extern t_list *poolMemoria;
extern config *configuracion;

void loadPoolMemori() {
    //conectarse con la memoria y pelirle la lista
    poolMemoria = list_create();
    memoriaData *memoria1 = malloc(sizeof(memoriaData *));
    memoria1->puerto = "3020";
    memoria1->ip = "127.0.0.1";
    list_add(poolMemoria, memoria1);
    memoriaData *memoria2 = malloc(sizeof(memoriaData *));
    memoria1->puerto = "3021";
    memoria1->ip = "127.0.0.1";
    list_add(poolMemoria, memoria2);
}