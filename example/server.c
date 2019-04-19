#include <funcionesCompartidas/funcionesNET.h>

typedef struct{
    int key;
    char letra;
    int listNumero[5];
} structSinPuntero;

typedef struct {
    char * nombre;
    char * apellido;
    char * telefono;
    int edad;
}structDatosPersonales;

typedef struct {
    size_t sizeNombre;
    size_t sizeApellido;
    size_t sizeTelefono;
    size_t sizeEdad;
} __attribute__((packed)) structDataPersonalesSize;

void armarStructuraConPuntero(void * buffer){
    structDataPersonalesSize dataPersonalSize;
    structDatosPersonales dataPersonal;
    int offset = 0;
    //como copiamo primero la strucutra don la data size cargamos eso primero por que nos va ayudar a cargar los otros datos
    memcpy(&dataPersonalSize,(buffer + offset), sizeof(structDataPersonalesSize));
    //sumamos lo que ya se copio
    offset += sizeof(structDataPersonalesSize);
    //copiamos las propiedades en el orden que se copiaron el cliente
    dataPersonal.nombre = malloc(dataPersonalSize.sizeNombre);
    memcpy(dataPersonal.nombre,(buffer + offset), dataPersonalSize.sizeNombre);
    offset += dataPersonalSize.sizeNombre;

    dataPersonal.apellido = malloc(dataPersonalSize.sizeApellido);
    memcpy(dataPersonal.apellido,(buffer + offset), dataPersonalSize.sizeApellido);
    offset += dataPersonalSize.sizeApellido;

    dataPersonal.telefono = malloc(dataPersonalSize.sizeTelefono);
    memcpy(dataPersonal.telefono,(buffer + offset), dataPersonalSize.sizeTelefono);
    offset += dataPersonalSize.sizeTelefono;

    memcpy(&dataPersonal.edad,(buffer + offset), dataPersonalSize.sizeEdad);

    printf("nombre = %s \n", dataPersonal.nombre);
    printf("apellido = %s \n", dataPersonal.apellido);
    printf("telefono = %s \n", dataPersonal.telefono);
    printf("edad = %d \n", dataPersonal.edad);

}

int main(){
    t_log * log = crear_archivo_log("Server",true,"./logS");
    int control = 0;
    //instanciar un socket server
    log_info(log,"creando server");
    int fdServer = makeListenSock("4020",log,&control);
    //se queda a la espera de alquien que se conecte
    log_info(log,"esperando a que se conecte alquien");
    int sockConect = aceptar_conexion(fdServer,log,&control);
    header request;
    void * buffer;
    while (1) {
        //recibir el mensaje definimos un header para que se nos cargue el tipo de dato enviado
        //y un buffer con para cargarlo con el dato
        log_info(log,"esperando a que ese alquien me envie un mjs");
         buffer = getMessage(sockConect, &request, &control);
        //verificamos si nos llego el mjs si es NULL fallo;
        if(buffer){
            //verificamos tipo de mjs
            switch(request.codigo){
                case 1:{
                    //si es caso 1 nos llega un string
                    log_info(log, "entro mensaje con el codigo 1");
                    printf("mensaje --> %s \n", (char *)buffer);
                    break;
                }
                case 2:{
                    log_info(log, "entro mensaje con el codigo 2");
                    structSinPuntero dataAux;
                    //al ser una structura sin punteros cargamos la dataAux de forma directa en una estrucura
                    //le pasamos la direccion de la structura, el buffer q nos llego del getMesaage
                    //y le pasamos el tamaño del buffer que nos llega en el header
                    memcpy(&dataAux,buffer,request.sizeData);
                    printf("key [%d], letra[%c] \n",dataAux.key,dataAux.letra);
                    int i = 0;
                    for (i = 0; i < 5; ++i) {
                        printf("listNumero[%d] = %d \n",i,dataAux.listNumero[i]);
                    }
                    break;
                }
                case 3: {
                    log_info(log, "entro mensaje con el codigo 3");
                    armarStructuraConPuntero(buffer);
                    break;
                }
                default:{
                    printf("no entiendo el mjs\n");
                }
            }
        }else{
            log_error(log,"se desconecto el cliente");
            return -1;
        }
    }
}