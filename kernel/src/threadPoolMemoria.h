//
// Created by miguelchauca on 28/04/19.
//

#ifndef KERNEL_THREADPOOLMEMORIA_H
#define KERNEL_THREADPOOLMEMORIA_H

#include <commons/collections/list.h>
#include "load_config.h"

typedef struct {
    char * ip;
    char * puerto;
} memoriaData;

void loadPoolMemori();

#endif //KERNEL_THREADPOOLMEMORIA_H
