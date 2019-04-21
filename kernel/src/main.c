#include <stdio.h>
#include <funcionesCompartidas/log.h>
#include "load_config.h"

int main(){
    t_log *file_log = crear_archivo_log("Kernel", true,"./kernelLog");
    log_info(file_log, "cargando el archivo de configuracion");
    config * configuracion = load_config("./kernel.config");
    if(!configuracion){
        log_error(file_log,"no se pudo cargar el archivo de configuracion");
    }
    log_destroy(file_log);
    return 0;
}