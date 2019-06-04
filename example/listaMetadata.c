#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <commons/collections/list.h>
#include <funcionesCompartidas/listaMetadata.h>


void cargarLista(t_list *listaMetaData) {
    st_metadata *metadata = malloc(sizeof(st_metadata));
    metadata->nameTable = strdup("TABLA_A");
    metadata->consistency = strdup("SC");
    metadata->partitions = 4;
    metadata->compaction_time = 5000;
    list_add(listaMetaData, metadata);
    metadata = malloc(sizeof(st_metadata));
    metadata->nameTable = strdup("TABLA_B");
    metadata->consistency = strdup("EC");
    metadata->partitions = 3;
    metadata->compaction_time = 6000;
    list_add(listaMetaData, metadata);
    metadata = malloc(sizeof(st_metadata));
    metadata->nameTable = strdup("TABLA_C");
    metadata->consistency = strdup("SHC");
    metadata->partitions = 5;
    metadata->compaction_time = 10000;
    list_add(listaMetaData, metadata);
}

void showLista(t_list *listaMetaData) {
    int i = 0;
    st_metadata *metadata;
    for (i = 0; i < listaMetaData->elements_count; ++i) {
        metadata = list_get(listaMetaData, i);
        printf("%s\n", metadata->nameTable);
        printf("%s\n", metadata->consistency);
        printf("%d\n", metadata->partitions);
        printf("%d\n", metadata->compaction_time);
    }
}

int main() {
    t_list *listametadata = list_create();
    cargarLista(listametadata);
    size_t size_buffer;
    void *buffer = serealizarListaMetaData(listametadata, &size_buffer);
    t_list *lista_metadata_deserealizado = deserealizarListaMetaData(buffer, size_buffer);
    showLista(lista_metadata_deserealizado);

    //liberar memoria
    destroyListaMetaData(lista_metadata_deserealizado);
    destroyListaMetaData(listametadata);
    free(buffer);
}