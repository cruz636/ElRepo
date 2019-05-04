//
// Created by miguelchauca on 04/05/19.
//

#include "planificador.h"

t_list * listProcesos;

void showAllProceso(){
    int i,j;
    for (i = 0; i < listProcesos->elements_count ; ++i) {
        t_list * procesoInstrucciones = list_get(listProcesos,i);
        printf("proceso numero [%d]\n",i);
        for (j = 0; j < procesoInstrucciones->elements_count; ++j) {
            stinstruccion * instruccion = list_get(procesoInstrucciones,j);
            printf("instrucion numero [%d]\n",instruccion->operacion);
        }
    }
}

void mockDeRequest(){
    structInsert * insert = malloc(sizeof(structInsert));
    insert->operacion = INSERT;
    insert->nameTable = "TABLAB";
    insert->timestamp= 213123213;
    insert->value= "miguel";
    insert->key = 3;
    stinstruccion * intrucion = malloc(sizeof(stinstruccion));
    intrucion->operacion = INSERT;
    intrucion->instruccion = insert;

    t_list * proceso1 = list_create();
    list_add(proceso1,intrucion);
    list_add(proceso1,intrucion);
    list_add(proceso1,intrucion);
    list_add(proceso1,intrucion);
    list_add(proceso1,intrucion);

    t_list * proceso2 = list_create();
    list_add(proceso2,intrucion);
    list_add(proceso2,intrucion);
    list_add(proceso2,intrucion);
    list_add(proceso2,intrucion);
    list_add(proceso2,intrucion);
    list_add(proceso2,intrucion);
    list_add(proceso2,intrucion);
    list_add(proceso2,intrucion);
    list_add(proceso2,intrucion);
    list_add(proceso2,intrucion);

    list_add(listProcesos,proceso1);
    list_add(listProcesos,proceso2);

    showAllProceso();
}

void atenderRequest(){
    while (1){
        //optener el proceso y sacarlo de la lista
        t_list * proceso = list_remove(listProcesos,0);
        //remover las 4 primeras intrucciones
        t_list * instrucciones = list_take_and_remove(proceso,3);
        //ejecutar esas 4 instrucciones

        //validar resultado de las 4 instruciones

        //verificar si quedan instrucciones para volver a ponerlo en la cola o terminarlo
        if(proceso->elements_count > 0){
            cargarNuevoScript(proceso);
        }
        if(listProcesos->elements_count == 0){
            printf("no hay mas procesos que atender");
            return;
        }
        showAllProceso();
    }
}

void inicialPlanificador(){
    listProcesos = list_create();
    mockDeRequest();
    atenderRequest();
}

void cargarNuevoScript(t_list * listInstrucciones){
    list_add(listProcesos,listInstrucciones);
}

