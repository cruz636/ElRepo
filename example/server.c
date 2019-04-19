#include <funcionesCompartidas/funcionesNET.h>
#include <sys/select.h>

typedef struct
{
    int key;
    char letra;
    int listNumero[5];
} structSinPuntero;

typedef struct
{
    char *nombre;
    char *apellido;
    char *telefono;
    int edad;
} structDatosPersonales;

typedef struct
{
    size_t sizeNombre;
    size_t sizeApellido;
    size_t sizeTelefono;
    size_t sizeEdad;
} __attribute__((packed)) structDataPersonalesSize;

void armarStructuraConPuntero(void *buffer)
{
    structDataPersonalesSize dataPersonalSize;
    structDatosPersonales dataPersonal;
    int offset = 0;
    //como copiamo primero la strucutra don la data size cargamos eso primero por que nos va ayudar a cargar los otros datos
    memcpy(&dataPersonalSize, (buffer + offset), sizeof(structDataPersonalesSize));
    //sumamos lo que ya se copio
    offset += sizeof(structDataPersonalesSize);
    //copiamos las propiedades en el orden que se copiaron el cliente
    dataPersonal.nombre = malloc(dataPersonalSize.sizeNombre);
    memcpy(dataPersonal.nombre, (buffer + offset), dataPersonalSize.sizeNombre);
    offset += dataPersonalSize.sizeNombre;

    dataPersonal.apellido = malloc(dataPersonalSize.sizeApellido);
    memcpy(dataPersonal.apellido, (buffer + offset), dataPersonalSize.sizeApellido);
    offset += dataPersonalSize.sizeApellido;

    dataPersonal.telefono = malloc(dataPersonalSize.sizeTelefono);
    memcpy(dataPersonal.telefono, (buffer + offset), dataPersonalSize.sizeTelefono);
    offset += dataPersonalSize.sizeTelefono;

    memcpy(&dataPersonal.edad, (buffer + offset), dataPersonalSize.sizeEdad);

    printf("nombre = %s \n", dataPersonal.nombre);
    printf("apellido = %s \n", dataPersonal.apellido);
    printf("telefono = %s \n", dataPersonal.telefono);
    printf("edad = %d \n", dataPersonal.edad);
}

int leerMensaje(int socketClient, t_log * log){
    int control;
    header request;
    void * bufferMensaje = getMessage(socketClient,&request,&control);
    if(bufferMensaje == NULL){
        return -1;
    }
    switch(request.codigo){
        case 1:{
            //si es caso 1 nos llega un string
            log_info(log, "entro mensaje con el codigo 1");
            printf("mensaje --> %s \n", (char *)bufferMensaje);
            break;
        }
        case 2:{
            log_info(log, "entro mensaje con el codigo 2");
            structSinPuntero dataAux;
            //al ser una structura sin punteros cargamos la dataAux de forma directa en una estrucura
            //le pasamos la direccion de la structura, el buffer q nos llego del getMesaage
            //y le pasamos el tamaño del buffer que nos llega en el header
            memcpy(&dataAux,bufferMensaje,request.sizeData);
            printf("key [%d], letra[%c] \n",dataAux.key,dataAux.letra);
            int i = 0;
            for (i = 0; i < 5; ++i) {
                printf("listNumero[%d] = %d \n",i,dataAux.listNumero[i]);
            }
            break;
        }
        case 3: {
            log_info(log, "entro mensaje con el codigo 3");
            armarStructuraConPuntero(bufferMensaje);
            break;
        }
        default:{
            printf("no entiendo el mjs\n");
        }
    }
    return 0;
}

int main()
{
    t_log *log = crear_archivo_log("Server", true, "./logS");
    int control = 0;
    //instanciar un socket server
    log_info(log, "creando server");
    int socketServer = makeListenSock("4020", log, &control);

    fd_set listaDeSockets;
    fd_set listaDeSocketsTemporal;
    int valorSocketMasAlto = 0;
    int newClientSocket;
    FD_ZERO(&listaDeSockets);
    FD_ZERO(&listaDeSocketsTemporal);

    FD_SET(socketServer, &listaDeSockets);
    valorSocketMasAlto = socketServer;

    while (1)
    {
        listaDeSocketsTemporal = listaDeSockets;
        if (select(valorSocketMasAlto + 1, &listaDeSocketsTemporal, NULL, NULL, NULL) == -1)
        {
            log_error(log, "Error en el select");
            break;
        }
        log_info(log, "Se detecto algun cambio en la listaDesSockets");
        int FDsocket;
        log_info(log, "buscamos que socket de la lista hay q atender");
        for (FDsocket = 0; FDsocket <= valorSocketMasAlto; FDsocket++)
        {
            if (FD_ISSET(FDsocket, &listaDeSocketsTemporal))
            {
                if(FDsocket == socketServer){
                    log_info(log, "hay una nueva conexcion");
                    newClientSocket = aceptar_conexion(socketServer,log,&control);
                    if(control == 0){
                        //agregamos el nuevo socket a la lista no temporal
                        FD_SET(newClientSocket,&listaDeSockets);
                        //actulizamos el valor mas alto del socket
                        valorSocketMasAlto = newClientSocket;
                    }
                }else{
                    log_info(log, "nos llego un nuevo mensaje del socket client %d",FDsocket);
                    if(leerMensaje(FDsocket,log) < 0){
                        log_info(log, "se desconecto el socket client %d",FDsocket);
                        close(FDsocket);
                        FD_CLR(FDsocket, &listaDeSockets);
                    }
                }
            }
        }
    }
}