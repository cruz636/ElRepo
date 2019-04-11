#include <stdio.h>
#include <funcionesCompartidas/log.h>
int main(){
    t_log *file_log = crear_archivo_log("Kernel", true,"/home/miguelchauca/log/kernelLog");
    log_info(file_log, "cargando el archivo de configuracion");

    return 0;
}