#include <stdio.h>
#include <funcionesCompartidas/log.h>
#include "load_config.h"
#include "consola.h"
#include <funcionesCompartidas/API.h>
#include <funcionesCompartidas/funcionesNET.h>

t_log *file_log;


int main(int argc, char **argv){
    file_log = crear_archivo_log("Kernel", true,"./kernelLog");
    log_info(file_log, "cargando el archivo de configuracion");
    config * configuracion = load_config(argv[1]);
    if(!configuracion){
        log_error(file_log,"no se pudo cargar el archivo de configuracion");
    }
    structInsert insert;
    insert.nameTable = "Tabla";
    insert.operacion = INSERT;
    insert.key = 23;
    insert.value = "miguel";
    insert.timestamp = 1000;
    header * request = malloc(sizeof(header));
    request->codigo = 3;
    request->letra = 'D';
    request->sizeData = 23;
    void * buffer = serealizarInsert(&insert,&request->sizeData);
    //consola();
    structInsert * m3 = desserealizarInsert(buffer);
    log_destroy(file_log);
    printf("[nameTable] %s\n", m3->nameTable);
    return 0;
}