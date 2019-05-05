#include <stdio.h>
#include <funcionesCompartidas/log.h>
#include "load_config.h"
#include "consola.h"
#include "threadMetadata.h"
#include "threadPoolMemoria.h"
#include "threadPlanificador.h"
#include <commons/collections/list.h>
#include <pthread.h>


t_log *file_log;
config * configuracion;
t_list * poolMemoria;
pthread_t plafinifcador;


int inicializar(char * pathConfig){
    file_log = crear_archivo_log("Kernel", true,"./kernelLog");
    log_info(file_log, "cargando el archivo de configuracion");
    configuracion = load_config(pathConfig);
    if(!configuracion) {
        log_error(file_log, "no se pudo cargar el archivo de configuracion");
        log_destroy(file_log);
        return -1;
    }
}

int main(int argc, char **argv){
    if(inicializar(argv[1]) < 0){
        return -1;
    }
    pthread_create(&plafinifcador,NULL, inicialPlanificador, NULL);
    loadPoolMemori();
    consola();
    pthread_cancel(plafinifcador);
    log_destroy(file_log);
    return 0;
}