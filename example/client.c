#include <stdio.h>
#include <funcionesCompartidas/log.h>
#include <funcionesCompartidas/funcionesNET.h>
#include <readline/readline.h>


void enviarMjsString(int socket, t_log * log){
    // creamos el header
    char * bloqueData = "hola Como va";
    int control = 0;
    header headSend;
    //para identificar de que componente viene
    headSend.letra = 'M';
    //para identificar el tipo de mensaje
    headSend.codigo = 1;
    //tamaño de la data que se va enviar en este caso es un string usamos strlen
    //y le sumamos 1 por que hay q agregarle el \0 el fin de cadena
    headSend.sizeData = strlen(bloqueData) + 1;
    //creamos el mjs y nos devuelve el paquetes de datos a enviar pasamos la direccion del header y el bloque de data a enviar
    message *bufferRes = createMessage(&headSend, bloqueData);
    //enviamos el mjs se pasa el socket , la direccion del log para logear error interno y un direccion de int para tomar accion
    //sobre los resultados
    if (enviar_message(socket, bufferRes, log, &control) < 0) {
        log_info(log, "Error al enviar el bloque");
    }
    log_info(log, "Se envio el mjs valla a mirar la respuesta en el server");
}

typedef struct{
    int key;
    char letra;
    int listNumero[5];
} __attribute__((packed)) structSinPuntero;

void enviarMjsStructSinPuntero(int socket, t_log * log){
    structSinPuntero bloqueData;
    bloqueData.key = 4;
    bloqueData.letra = 'M';
    //limpiamos el array cargamos todo con cero
    memset(bloqueData.listNumero,0, sizeof(bloqueData.listNumero));
    //cargamos un par de posiciones
    bloqueData.listNumero[0] = 12;
    bloqueData.listNumero[1] = 39;
    bloqueData.listNumero[2] = 33;
    int control = 0;
    header headSend;
    //para identificar de que componente viene
    headSend.letra = 'M';
    //para identificar el tipo de mensaje
    headSend.codigo = 2;
    //tamaño de la data que se va enviar en este caso sizeof nos devuelve el tamaño de la structura
    headSend.sizeData = sizeof(bloqueData);
    //creamos el mjs y nos devuelve el paquetes de datos a enviar pasamos la direccion del header y el bloque de data a enviar
    message *bufferRes = createMessage(&headSend, &bloqueData);
    //enviamos el mjs se pasa el socket , la direccion del log para logear error interno y un direccion de int para tomar accion
    //sobre los resultados
    if (enviar_message(socket, bufferRes, log, &control) < 0) {
        log_info(log, "Error al enviar el bloque");
    }
    log_info(log, "Se envio el mjs valla a mirar la respuesta en el server");
}

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

void crearMjsStructConPuntero(int socket, t_log * log){
    structDatosPersonales dataPersonal;
    //creamos esta structura para guardar los tamaños de los datos de la structura
    structDataPersonalesSize dataPersonalSize;
    dataPersonal.nombre = "Miguel Angel";
    dataPersonal.apellido = "Chauca Sanchez";
    dataPersonal.telefono = "11-2308-1763";
    dataPersonal.edad = 29;
    //guardamos todos los tamaños de cada propiedad de la structura
    dataPersonalSize.sizeNombre = strlen(dataPersonal.nombre)+ 1;
    dataPersonalSize.sizeApellido = strlen(dataPersonal.apellido)+ 1;
    dataPersonalSize.sizeTelefono = strlen(dataPersonal.telefono) +1;
    dataPersonalSize.sizeEdad = sizeof(int);
    //sumamos el tamaño de cada propiedad mas el tamaño de la structura que guarda los tamaños
    size_t sizeBufferAEnviar = sizeof(dataPersonalSize) + dataPersonalSize.sizeNombre + dataPersonalSize.sizeApellido + dataPersonalSize.sizeTelefono + dataPersonalSize.sizeEdad;
    //creamos un buffer con el tamaño de dato que vamos a enviar
    void * bufferAEnviar = malloc(sizeBufferAEnviar);
    //offset para posicionar el puntero al final de lo copiado en el buffer
    int offset = 0;
    //copiamos al buffer la structura con los size
    memcpy((bufferAEnviar + offset),&dataPersonalSize, sizeof(dataPersonalSize));
    //sumamos lo copiado
    offset += sizeof(dataPersonalSize);
    //copiamos las propiedades que se va enviar
    memcpy((bufferAEnviar + offset),dataPersonal.nombre, dataPersonalSize.sizeNombre);
    offset += dataPersonalSize.sizeNombre;

    memcpy((bufferAEnviar + offset),dataPersonal.apellido,dataPersonalSize.sizeApellido);
    offset += dataPersonalSize.sizeApellido;

    memcpy((bufferAEnviar + offset),dataPersonal.telefono,dataPersonalSize.sizeTelefono);
    offset += dataPersonalSize.sizeTelefono;

    memcpy((bufferAEnviar+ offset),&dataPersonal.edad,dataPersonalSize.sizeEdad);

    //ahora creamos el mensaje a enviar ya que tenemos todo el buffer cargado
    //cargamos el header
    header request;
    request.letra = 'M';
    request.codigo = 3;
    request.sizeData = sizeBufferAEnviar;

    int control= 0;
    void * bufferDeMensaje = createMessage(&request,bufferAEnviar);

    if (enviar_message(socket, bufferDeMensaje, log, &control) < 0) {
        log_info(log, "Error al enviar el bloque");
    }
    log_info(log, "Se envio el mjs valla a mirar la respuesta en el server");
}

int main(){
    t_log *file_log = crear_archivo_log("Kernel", true,"./logC");
    int control = 0;
    log_info(file_log,"estableciendo conexion");
    int socketClient = establecerConexion("127.0.0.1","4020",file_log,&control);
    if (control != 0) {
        log_error(file_log, "Error al intentar establecer conneccion");
        return -1;
    }
    printf("option 1 para un mensaje con string\n");
    printf("option 2 para un mensaje con structura sin punteros\n");
    printf("option 3 para un mensaje con structura con punteros\n");
    printf("option -1 para salir\n");
    char * option;
    int optinNumber;
    option = readline(">");
    while (strcmp(option,"-1") != 0){
        optinNumber = strtol(option, NULL, 10);
        switch (optinNumber){
            case 1:{
                log_info(file_log,"enviando solo un string");
                enviarMjsString(socketClient,file_log);
                break;
            }
            case 2: {
                log_info(file_log,"enviando con structura sin puntero");
                enviarMjsStructSinPuntero(socketClient,file_log);
                break;
            }
            case 3:{
                log_info(file_log,"enviando con structura con puntero");
                crearMjsStructConPuntero(socketClient,file_log);
                break;
            }
            default: {
                printf("no se reconoce ese mjs\n");
            }
        }
        option = readline(">");
    }
}