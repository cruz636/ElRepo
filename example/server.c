#include <funcionesCompartidas/funcionesNET.h>

void main(){
    t_log * log = crear_archivo_log("Server",true,"/home/logS");
    log_info(log,"Hola hice lago");
    int control = 0;
    int fdServer = makeListenSock("4020",log,&control);
    int sockConect = aceptar_conexion(fdServer,log,&control);
    header request;
    void * buffer;
    char * result = getMessage(sockConect, &request, &control);
    printf("resultado --- %s", result);
}