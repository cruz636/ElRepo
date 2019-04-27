#include <stdio.h>
#include <funcionesCompartidas/log.h>
#include "load_config.h"
#include "consola.h"

t_log *file_log;


int main(int argc, char **argv){
    file_log = crear_archivo_log("Kernel", true,"./kernelLog");
    log_info(file_log, "cargando el archivo de configuracion");
    config * configuracion = load_config(argv[1]);
    if(!configuracion) {
        log_error(file_log, "no se pudo cargar el archivo de configuracion");
    }
    consola();
    log_destroy(file_log);
    return 0;
}