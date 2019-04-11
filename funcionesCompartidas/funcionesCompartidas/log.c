#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <commons/log.h>
#include <commons/string.h>

t_log *crear_archivo_log(char *nombre_cabecera, int imprimir, char *file)
{
    t_log *log = log_create(file,nombre_cabecera,imprimir, LOG_LEVEL_TRACE);
    if(log){
        log_info(log, "Se crea el archivo de log");
    }
    return log;
}


void escribir_log_error_compuesto(t_log *log, char *mensaje, char *otro_mensaje)
{
    char *final = strdup("");
    string_append(&final, mensaje);
    string_append(&final, otro_mensaje);
    log_error(log, final);
    free(final);
}

void liberar_log(t_log *log)
{
    log_destroy(log);
}
