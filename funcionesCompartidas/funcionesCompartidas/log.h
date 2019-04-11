#ifndef FUNCIONES_LOG_H_
#define FUNCIONES_LOG_H_

#include <commons/log.h>

//Funcion para crear archivo de log
//Toma como parametros la cabecera del archivo, booleano de print y el path del archivo
t_log *crear_archivo_log(char *nombre_cabecera, int imprimir, char *file);

//Recibe y genera log de ERROR recibiendo dos textos
void escribir_log_error_compuesto(t_log *log, char *mensaje, char *otro_mensaje);

//Elimina el archivo de log
void liberar_log(t_log *log);

#endif /* FUNCIONES_LOG_H_ */
