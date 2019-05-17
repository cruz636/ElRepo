//
// Created by miguelchauca on 28/04/19.
//

#ifndef KERNEL_THREADPOOLMEMORIA_H
#define KERNEL_THREADPOOLMEMORIA_H

#include <stdio.h>
#include <unistd.h>
#include <commons/collections/list.h>
#include "load_config.h"
#include <funcionesCompartidas/funcionesNET.h>
#include <funcionesCompartidas/listaMemoria.h>
#include <pthread.h>

void *loadPoolMemori();

#endif //KERNEL_THREADPOOLMEMORIA_H
