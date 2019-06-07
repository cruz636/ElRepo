#include <commons/config.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <funcionesCompartidas/funcionesNET.h>
#include <funcionesCompartidas/log.h>
#include <pthread.h>
#include <funcionesCompartidas/API.h>

#define IPFILESYSTEM "192.168.0.21" //shall we save this info in the .config file???
#define PORTFILESYSTEM "8080"
#define DIM 1//total pages (?)

t_log *log_server;
t_config * g_config;

char value; //tamanio máximo para configuración de las páginas
char *IP_FS = IPFILESYSTEM;
char *PORT_FS = PORTFILESYSTEM;



///

typedef struct {
    enum OPERACION operacion;
    char *nameTable;
    int key;
} st_select;

typedef struct {
    size_t operacion;
    size_t nameTable;
    size_t key;
}__attribute__((packed)) st_size_select;

st_select *deserealizarSelect(void *buffer) {
    st_size_select sizeSelect;
    st_select *_select = malloc(sizeof(st_select));
    int offset = 0;

    memcpy(&sizeSelect, buffer, sizeof(st_size_select));
    offset += sizeof(st_size_select);

    memcpy(&_select->operacion, (buffer + offset), sizeSelect.operacion);
    offset += sizeSelect.operacion;

    _select->nameTable = malloc(sizeSelect.nameTable);
    memcpy(_select->nameTable, (buffer + offset), sizeSelect.nameTable);
    offset += sizeSelect.nameTable;

    memcpy(&_select->key, (buffer + offset), sizeSelect.key);

    return _select;
}


////
struct segmentTable{

};

struct PageTables{
    char NAME;
    int KEY;
    char VALUE;

}list[DIM];



void *start_server(char *port);
void *connectToSeeds(int size);
void start_seeding(char *IP,char *PORT);
void leerMensaje(void *recibido,header request);
void enviarMjsString(int socket, t_log * log);
void enviarAFileSystem(enum OPERACION operacion,size_t sizeBuffer,void * buffer);

void connectToFileSystem(char *IP,char *PORT);


int main(int argc, char *argv[]) {

    for(int i=0;i < DIM ; i++){
        printf("[>] Name of page N°%i : ",i);
        scanf("%s",&list[i].NAME);

        printf("[>] Key of page N°%i : ",i);
        scanf("%i",&list[i].KEY);

        printf("[>] Value of page N°%i:",i);
        scanf("%s",&list[i].VALUE);

        printf("\n--------------------------------------\n");
    }


    pthread_t servidor;
    pthread_t client;
    g_config = config_create("/media/root/bodhi/Facultad/Sistemas Operativos/TP0/tp-2019-1c-misc/memoria/src/memoriaConf.config");
    char* port = config_get_string_value(g_config,"PUERTO");
    char* name = config_get_string_value(g_config,"MEM_NUMBER");

    char** ipSeed = config_get_array_value(g_config,"IP_SEED");
    char** portSeed = config_get_array_value(g_config,"PORT_SEED");

    printf("PORT: %s\n", port );
    printf("MEMORY NAME: %s\n", name );

    int totalSeeds = 0;
    while (ipSeed[totalSeeds] != '\0') totalSeeds++;

    printf("Total seeds: %d\n",totalSeeds);

    for(int seed = 0; seed < totalSeeds; seed++){
      printf("[%i] seed address: %s::%s \n", seed ,ipSeed[seed],portSeed[seed]);
    }

    printf("\n\n");
  //that was all the memoriaConf.config info ;)


    connectToFileSystem(IP_FS,PORT_FS); //we have to connect to the FS to recive the handshake




    log_server = crear_archivo_log(name,true,"./LogS");

    pthread_create(&servidor,NULL, &start_server, port);
//    pthread_create(&client,NULL, &connectToSeeds,(void *) totalSeeds);

    pthread_join(servidor,NULL);

  //  pthread_join(client,NULL);

//the client func are for the gossiping, dont need them now

    return 0;
}


void connectToFileSystem(char *IP,char *PORT){

    t_log *file_log = crear_archivo_log("PrimaryMemory", true,"./logC");
    int control = 0;
    log_info(file_log,"getting handshake.."); //delete (?)

    printf("[+]Connecting to FILESYSTEM AT : %s:%s \n",IP,PORT);

    int socketClient = establecerConexion(IP,PORT,file_log,&control);

    if (control != 0) {
        log_error(file_log, "Failed to connect.");
        return -1;
    }

    printf("[+] Getting handshake .\n");
    read(socketClient,value,sizeof(value)); // read(socket,var,size of var)
    printf("[+] The value to config the pages is %s\n",value);
    printf("-------------------------\n");
    sleep(2);



}



void checkPackage(void *package,int packageNumber){

  switch(packageNumber){
    case 1:
      printf("SELECT\n" );
      //its select , we have to :

      // Verifica si existe el segmento de la tabla solicitada y busca en las páginas del
      //mismo si contiene key solicitada. Si la contiene, devuelve su valor y finaliza el proceso.

      //if(package.key.segment = exist) return key;
      //else askToFS(key); //Si no la contiene, envía la solicitud a
                    //FileSystem para obtener el valor solicitado y almacenarlo.
                //Una vez obtenido se debe solicitar una nueva página libre para almacenar el mismo. [sigue..]
  }

}

void leerMensaje(void *recibido,header request){

    //we have to know who is seending us the message
    if(request.letra == 'K'){

        // once we know that the sender is de kernel, we have to know which operation is he given us
        //and send it to the FL

        switch (request.codigo){
            case INSERT:{
                structInsert * instruccionInsert = desserealizarInsert(recibido);
                //verificar si esta en cache
                enviarAFileSystem(INSERT,request.sizeData,recibido);
            }
            case SELECT:{
                st_select * instruccionSelect = deserealizarSelect(recibido); //desserealizarSelect
                //check
                checkPackage(recibido,1);// the number for SELECT is 1 (one)  ( we sould make a dictionary)
                enviarAFileSystem(SELECT,request.sizeData,recibido);
            }

            //we can make a error handler func, in where we put the instruction value & the func decides how to hand it
            // if we dont do this, we have to do that in each instruction







            //the next cases arent necessary 4 tomorrow mates ;) ;) ;)

            case CREATE:{
                structCreate * instruccionCreate = desserealizarInsert(recibido); //desserealizarCreate
                //..
                enviarAFileSystem(CREATE,request.sizeData,recibido);
            }
            case DROP:{
                structDrop * intruccionDrop = desserealizarInsert(recibido); //desserealizarDrop
                //..
                enviarAFileSystem(DROP,request.sizeData,recibido);
            }
        }
    }
}



void enviarMjsString(int socket, t_log * log){
    // creamos el header
    char * bloqueData = "Hello hello";
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
        log_info(log, "[-] Failed to send the block :( \n");
    }
    log_info(log, "[+] Message sended ;) \n");
}


void enviarAFileSystem(enum OPERACION operacion,size_t sizeBuffer,void * buffer){
    printf("[+] Sending to FS ..");

    t_log *file_log = crear_archivo_log("PrimaryMemory", true,"./logC");
    int control = 0;

    printf("[+]Connecting to FILESYSTEM AT : %s:%s \n",IP_FS,PORT_FS);

    int socketClient = establecerConexion(IP_FS,PORT_FS,file_log,&control);

    if (control != 0) {
        log_error(file_log, "[-] Failed to connect. \n");
        return -1;
    }

    //make package & send it

    //send(socketClient,package,..,..);
}


void *start_server(char *port){
  printf("[+] Starting server.. \n");

  int control = 0;
  int socketServer = makeListenSock(port,log_server, &control);


  int client = aceptar_conexion(socketServer,log_server,&control);
  if(control == 0){
    log_info(log_server,"todo ok\n");
  }

  printf("[+] Memory [ID: %d] connected\n", client );

  header request;

  void * recibido;
  recibido = getMessage(client, &request, &control);

  if(recibido == NULL){
     perror("[-] It's an empy package :( \n");
     return -1;
  }

  leerMensaje(recibido,request);


}



// this next functions are for the gossiping , still in development ;)

void *connectToSeeds(int size){
  printf("[+] Connecting to the seeds.. \n");
  printf("-----------------------------\n" );
  g_config = config_create("memoriaConf.config");
  char** ipSeed = config_get_array_value(g_config,"IP_SEED");
  char** portSeed = config_get_array_value(g_config,"PORT_SEED");
  int sleepTime = config_get_int_value(g_config,"SLEEP");

  sleep(sleepTime);

	for(int v = 1; v < size;v++){ //its start from 2 because the first ip&port its our adress

    char *PORT[5];
    char IP[16];

    strcpy(IP,*&ipSeed[v]);
    strcpy(PORT,*&portSeed[v]);

    start_seeding(IP,PORT);
	}

}

void start_seeding(char *IP,char *PORT){

  t_log *file_log = crear_archivo_log("Kernel", true,"./logC");
  int control = 0;
  log_info(file_log,"estableciendo conexion");

  printf("[+]Connecting to : %s:%s \n",IP,PORT);


  int socketClient = establecerConexion(IP,PORT,file_log,&control);

  if (control != 0) {
      log_error(file_log, "Error al intentar establecer connexión");
      return -1;
  }

  enviarMjsString(socketClient,file_log);
  printf("-------------------------\n");
  sleep(2);
}
