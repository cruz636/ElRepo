#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <commons/collections/list.h>
#include <funcionesCompartidas/listaMemoria.h>

t_list *cargarListMemoria()
{
  t_list *listaMemoria = list_create();
  st_data_memoria *memoria = malloc(sizeof(st_data_memoria));
  memoria->ip = strdup("3001");
  memoria->puerto = strdup("127.0.0.1");
  list_add(listaMemoria, memoria);
  st_data_memoria *memoria2 = malloc(sizeof(st_data_memoria));
  memoria2->ip = strdup("3002");
  memoria2->puerto = strdup("127.0.0.1.5.6");
  list_add(listaMemoria, memoria2);
  return listaMemoria;
}

int main(int argc, char **argv)
{
  t_list *de = cargarListMemoria();
  size_t sizeBuffer = 0;
  void *buffer = sereliazarListaDataMemoria(de, &sizeBuffer);
  t_list *ok = deserealizarListaDataMemoria(buffer, sizeBuffer);
  free(buffer);

  for (int i = 0; i < ok->elements_count; i++)
  {
    st_data_memoria *memoria = list_get(ok, i);
    printf("%s\n", memoria->ip);
    printf("%s\n", memoria->puerto);
  }
  destroyListaDataMemoria(de);
  destroyListaDataMemoria(ok);
}