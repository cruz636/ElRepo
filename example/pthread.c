#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <commons/collections/list.h>
#include <readline/readline.h>
#include <semaphore.h>

t_list *listaNombres;
sem_t nombresDisponibles;


void *consumirNombre(void *arg)
{
  while (1)
  {
    sem_wait(&nombresDisponibles);
    sleep(1);
    char * nombre = list_remove(listaNombres, 0);
    printf("se tomo el nombre [%s]\n",nombre);
  }
  
}

int main(void)
{
  listaNombres = list_create();
  sem_init(&nombresDisponibles, 0, 0);
  pthread_t consumidor;

  pthread_create(&consumidor,NULL,consumirNombre, NULL);

  char *palabra = readline(">");
  while (strcmp(palabra, "exit") != 0)
  {
    list_add(listaNombres, palabra);
    sem_post(&nombresDisponibles);
    palabra = readline(">");
  }
  pthread_join(consumidor,NULL);
  
  return 1;
}