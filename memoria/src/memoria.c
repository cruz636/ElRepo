#include <stdio.h>
#include <funcionesCompartidas/log.h>
#include <funcionesCompartidas/funcionesNET.h>

int main(){
    t_log *file_log = crear_archivo_log("Kernel", true,"./memoriaLog");
    log_info(file_log, "cargando el archivo de configuracion");
    int control = 0;
    log_info(file_log,"estableciendo conexion");
    int socketClient = establecerConexion("10.148.106.248","4020",file_log,&control);
    header headSend;
    headSend.letra = 'D';
    headSend.codigo = 1;
    headSend.sizeData = 13;
    void * bloqueData = "hola Como va";
    message *bufferRes = createMessage(&headSend, bloqueData);
    if (enviar_message(socketClient, bufferRes, file_log, &control) < 0) {
        log_info(file_log, "Error al enviar el bloque");
        return -1;
    }
}