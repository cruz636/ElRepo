//
// Created by miguelchauca on 18/05/19.
//

#ifndef KERNEL_LISTAMETADATA_H
#define KERNEL_LISTAMETADATA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <commons/collections/list.h>


typedef struct {
    char *consistency;
    int partitions;
    int compaction_time;

} st_metadata;

typedef struct {
    size_t consistency;
    size_t partitions;
    size_t compaction_time;
} __attribute__((packed)) st_size_metadata;

void *serealizarMetaData(st_metadata *metadata, size_t *sizeBuffer);
st_metadata *deserealizarMetaData(void *buffer, size_t *sizeBuffer);
void destroyMetaData(st_metadata *metadata);
void *serealizarListaMetaData(t_list *listaMetaData, size_t *sizeBuffer);
t_list *deserealizarListaMetaData(void *buffer, size_t size_buffer);
void destroyListaMetaData(t_list * lista_metadata);
#endif //KERNEL_LISTAMETADATA_H
