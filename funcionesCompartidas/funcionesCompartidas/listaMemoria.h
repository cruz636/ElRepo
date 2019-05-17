//
// Created by miguelchauca on 05/05/19.
//

#ifndef KERNEL_LISTAMEMORIA_H
#define KERNEL_LISTAMEMORIA_H
#include <stdio.h>
#include <commons/collections/list.h>
typedef struct
{
  char *ip;
  char *puerto;
} st_data_memoria;

typedef struct
{
  size_t ip;
  size_t puerto;
} __attribute__((packed)) st_size_memoria;
void *serealizarDataMemoria(st_data_memoria *memoria, size_t *size_buffer);
st_data_memoria *deserealizarDataMemoria(void *buffer, size_t *size_buffer);
void *sereliazarListaDataMemoria(t_list *listaDeMemorias, size_t *size_buffer);
t_list *deserealizarListaDataMemoria(void *buffer, size_t size_buffer);
void destroyMemoria(st_data_memoria *memoria);
void destroyListaDataMemoria(t_list *listaDeMemorias);

#endif //KERNEL_LISTAMEMORIA_H
