#include <stdio.h>
#include <funcionesCompartidas/funcionesNET.h>
#include <funcionesCompartidas/listaMemoria.h>
#include <funcionesCompartidas/log.h>
#include <funcionesCompartidas/API.h>
#include <commons/collections/list.h>
#include <sys/select.h>

t_log *file_log;

t_list *cargarListMemoria()
{
  t_list *listaMemoria = list_create();
  st_data_memoria *memoria = malloc(sizeof(st_data_memoria));
  memoria->ip = strdup("3001");
  memoria->puerto = strdup("127.0.0.1");
  list_add(listaMemoria, memoria);
  st_data_memoria *memoria2 = malloc(sizeof(st_data_memoria));
  memoria2->ip = strdup("3002");
  memoria2->puerto = strdup("127.0.0.1");
  list_add(listaMemoria, memoria2);
  return listaMemoria;
}

void atenderMjs(header *req, void *buffer, int sockeClient)
{
  header response;
  message *bufferMensaje = NULL;
  size_t sizebuffer = 0;
  int control = 0;
  switch (req->codigo)
  {
  case INSERT:
  {
    structInsert *insert = desserealizarInsert(buffer);
    printf("SE RECIBIO UN INSERT\n");
    printf("tabla [%s]\n", insert->nameTable);
    printf("key [%d]\n", insert->key);
    printf("value [%s]\n", insert->value);
    printf("timeStamp [%d]\n", insert->timestamp);
    response.letra = 'M';
    response.codigo = 1;
    response.sizeData = 0;
    bufferMensaje = createMessage(&response, "");
    enviar_message(sockeClient, bufferMensaje, file_log, &control);
    break;
  }
  case 5:
  {
    t_list * memorias = cargarListMemoria();
    void * bufferList = sereliazarListaDataMemoria(memorias,&sizebuffer);
    response.letra = 'M';
    response.codigo = 5;
    response.sizeData = sizebuffer;
    bufferMensaje = createMessage(&response,bufferList);
    enviar_message(sockeClient,bufferMensaje,file_log,&control);
    break;
  }
  }
}

void main(int argc, char **argv)
{
  int control = 0;
  char *puerto = argv[1];
  puerto = "3001";
  file_log = crear_archivo_log("serverMemoria", true, "./logTest.log");
  int socketServer = makeListenSock(puerto, file_log, &control);
  fd_set listaDeSockets;
  fd_set listaDeSocketsTemporal;
  int valorSocketMasAlto = 0;
  int newClientSocket;
  //limpiamos las listas de socket
  FD_ZERO(&listaDeSockets);
  FD_ZERO(&listaDeSocketsTemporal);
  //cargamos el socket server a la lista no temporal de socket
  FD_SET(socketServer, &listaDeSockets);
  //cargamos el valor mas alto del socket mayor q tengamos en este saco es el server
  //cada socket nuevo q nos del el SO va ser siempre creeciente
  valorSocketMasAlto = socketServer;
  header request;
  void *bufferMensaje;
  while (1)
  {
    listaDeSocketsTemporal = listaDeSockets;
    if (select(valorSocketMasAlto + 1, &listaDeSocketsTemporal, NULL, NULL, NULL) == -1)
    {
      log_error(file_log, "Error en el select");
      break;
    }
    log_info(file_log, "Se detecto algun cambio en la listaDesSockets");
    int FDsocket;
    log_info(file_log, "buscamos que socket de la lista hay q atender");
    for (FDsocket = 0; FDsocket <= valorSocketMasAlto; FDsocket++)
    {
      if (FD_ISSET(FDsocket, &listaDeSocketsTemporal))
      {
        if (FDsocket == socketServer)
        {
          log_info(file_log, "hay una nueva conexcion");
          newClientSocket = aceptar_conexion(socketServer, file_log, &control);
          if (control == 0)
          {
            //agregamos el nuevo socket a la lista no temporal
            FD_SET(newClientSocket, &listaDeSockets);
            //actulizamos el valor mas alto del socket
            valorSocketMasAlto = newClientSocket;
          }
        }
        else
        {
          log_info(file_log, "nos llego un nuevo mensaje del socket client %d", FDsocket);
          bufferMensaje = getMessage(FDsocket, &request, &control);
          if (bufferMensaje)
          {
            atenderMjs(&request, bufferMensaje, FDsocket);
          }
          else
          {
            log_info(file_log, "se desconecto el socket client %d", FDsocket);
            //cerramos el socket que se desconecto
            close(FDsocket);
            //lo sacamos de la lista de socket no temporal
            FD_CLR(FDsocket, &listaDeSockets);
          }
        }
      }
    }
  }
}