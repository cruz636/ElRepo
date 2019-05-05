//
// Created by miguelchauca on 04/05/19.
//

#include "threadPlanificador.h"

t_list * listProcesos;
sem_t colaDeListos;
extern t_log * file_log;

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

void atenderRequest(){
    while (1){
        sem_wait(&colaDeListos);
        log_info(file_log,"atendiendo nuevo proceso");
        //optener el proceso y sacarlo de la lista
        t_list * proceso = list_remove(listProcesos,0);
        //remover las 4 primeras intrucciones
        t_list * instrucciones = list_take_and_remove(proceso,3);
        //ejecutar esas 4 instrucciones
        sleep(6);
        //validar resultado de las 4 instruciones

        //verificar si quedan instrucciones para volver a ponerlo en la cola o terminarlo
        if(proceso->elements_count > 0){
            cargarNuevoScript(proceso);
        }
    }
}

void * inicialPlanificador(){
    listProcesos = list_create();
    sem_init(&colaDeListos,0,0);
    atenderRequest();
}

void cargarNuevoScript(t_list * listInstrucciones){
    list_add(listProcesos,listInstrucciones);
    sem_post(&colaDeListos);
}

