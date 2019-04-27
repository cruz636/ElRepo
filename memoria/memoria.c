#include <funcionesCompartidas/funcionesNET.h>
#include <funcionesCompartidas/log.h>
#include <pthread.h>



struct log{
  char IP[16];
  char PUERTO[5];
};



t_log *log_server;

void *funcionServer(char *PUERTO) {

  int control = 0;
  int socketServer = makeListSocket(PUERTO,log_server, &control);


  int cliente = aceptar_conexion(socketServer,log_server,&control);
  if(control == 0){
    log_info(log_server,"todo ok\n");
  }

  printf("[+] Se conectó el cliente %d\n", cliente );

  void buffer;
  header request;


  buffer = getMessage(cliente,&request,&control);
  
  for(;;);

}


void *funcionClient(struct log *memoria){
  struct sockaddr_in direccionServer;
  char paquete[] = "Este es un mensaje del cliente";
  char buffer[1024];

  printf("IP: %s\n",memoria -> IP );
  printf("PUERTO: %d\n",atoi(memoria->PUERTO) );

  direccionServer.sin_family = AF_INET;
  direccionServer.sin_addr.s_addr = inet_addr(memoria->IP);
  direccionServer.sin_port = htons(atoi(memoria->PUERTO));

  int cliente = socket(AF_INET,SOCK_STREAM,0);
  if(connect(cliente,(void *) &direccionServer,sizeof(direccionServer)) != 0){
    perror("[-] Error al conectarse con el servidor \n");
    //exit(-1);
  }

  send(cliente,paquete,strlen(paquete),0);
  recv(cliente,buffer,1024,0);
  printf("Buffer: %s\n",buffer );

}



int main(int argc, char *argv[]) {

  pthread_t servidor;
  pthread_t cliente;

  struct log memoria1 ;

  FILE *logIps;
  FILE *logPorts;
  FILE *miLog;

//  char direccionIP[16];
  //char direccionPuerto[5];
  char miPuerto[5];
  log_server = crear_archivo_log("Server",true,"./LogS");




  //creamos la struct con los datos sacados del log

  //leemos la ip de otra memoria para conectarnos
  logIps = fopen("ips.txt","r");
  fscanf(logIps,"%s",memoria1.IP);
  fclose(logIps);

  //leemos el puerto para conectarnos
  logPorts = fopen("puertos.txt","r");
  fscanf(logPorts,"%s",memoria1.PUERTO);
  fclose(logPorts);

  //abrimos nuestro log, y nos fijamos que puerto tenemos asignado
  miLog = fopen("miLog.txt","r");
  fscanf(miLog,"%s",miPuerto);
  fclose(miLog);

  printf("Mi puerto: %s\n",miPuerto);


  pthread_create(&servidor,NULL, &funcionServer, miPuerto);
 // pthread_create(&cliente,NULL, &funcionClient,(void *) &memoria1); //pasamos puerto y direccion con un struct
  pthread_join(servidor,NULL);
  //pthread_join(cliente,NULL);



  return 0;
}
