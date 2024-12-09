
//LIBRERÍAS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "cabeceras.h"

#define LONGITUD_COMANDO 100 /* Longitud máxima de un comando */

enum Comandos {
    ERROR           = -1,   /* Código de error, se puede utilizar en otros contextos */
    NOTHING         = 0,    /* El usuario no realiza acción alguna */
    INFO            = 1,    /* Comando para obtener la información */
    BYTEMAPS        = 2,    /* Comando para obtener los bytemaps */
    DIR             = 3,    /* Comando para mostrar el contenido del directorio */
    RENAME          = 4,    /* Comando para renombrar un archivo */
    PRINT           = 5,    /* Comando para imprimir el contenido de un archivo */
    REMOVE          = 6,    /* Comando para eliminar un archivo */
    COPY            = 7,    /* Comando para copiar un archivo */
    EXIT            = 8,    /* Comando para terminar la ejecución del programa */
    NOT_RECOGNIZED  = 9     /* Comando no reconocido */
};

/**
 * @brief Función para comprobar un comando
 * @param strcomando Comando a analizar
 * @param orden Sinceramente, desconozco para que sirve este parámetro o que aplicación podía tener en el esqueleto
 * @param argumento1 Posible argumento 1 que va después del comando clave
 * @param argumento2 Posible argumento 2 que va después del argumento 1
 * @returns Número entero, código asociado a la acción a realizar en base a la interpretación del comando
 */
int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2);

/**
 * @brief Muestra información detallada sobre el superbloque
 * @param psup Puntero a la estructura EXT_SIMPLE_SUPERBLOCK con la información
 * @returns void
 */
void LeerSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup);

/**
 * @brief Imprime los inodos y bloques del sistema de archivos
 * @param ext_bytemaps Puntero a la estructura EXT_BYTE_MAPS, contiene la información a imprimir
 * @returns void
 */
void PrintBytemaps(EXT_BYTE_MAPS *ext_bytemaps);

/**
 * @brief Muestra los archivos existentes, con la información de cada uno
 * @param directorio Puntero a la entrada del directorio
 * @param inodos Puntero a los inodos del sistema
 * @returns void
 */
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos);

/**
 * @brief Método para buscar un fichero
 * @param directorio Puntero a la entrada del directorio
 * @param inodos Puntero al conjunto de inodos
 * @param nombre Nombre del archivo por el que se va a buscar
 * @returns Número entero, -1 si no se encuentra, en caso contrario el índice del nodo del archivo
 */
int BuscarFichero(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombre);

/**
 * @brief Renombra un archivo con un nombre antiguo y nuevo pasado por parámetro
 * @param directorio Puntero a la entrada del directorio
 * @param inodos Puntero al conjunto de inodos
 * @param nombreAntiguo Nombre antiguo a buscar
 * @param nombreNuevo Nombre nuevo del archivo
 * @returns Devuelve 0 en caso de éxito, -1 si no se encuentra el archivo, -2 si no se puede reemplazar
 */
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombreAntiguo, char *nombreNuevo);

/**
 * @brief Método para imprimir el contenido de un archivo
 * @param directorio Puntero a la entrada del directorio
 * @param inodos Puntero al conjunto de inodos
 * @param memdatos Puntero al conjunto de bloques de datos
 * @param nombre Nombre del archivo a imprimir
 * @returns Devuelve un número entero, -1 si hubo errores, 1 en caso de éxito
 */
int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_DATOS *memdatos, char *nombre);

/**
 * @brief Función para eliminar un fichero
 * @param directorio Puntero a la entrada del directorio
 * @param inodos Puntero al conjunto de inodos
 * @param ext_bytemaps Puntero a los bytemaps
 * @param ext_superblock Puntero al superbloque de información
 * @param nombre Nombre del archivo a eliminar
 * @param fichero Puntero del fichero a eliminar
 * @returns Devuelve 0 si todo ha salido correcto, -1 si el archivo no se ha encontrado
 */
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           char *nombre, FILE *fichero);

/**
 * @brief Método para copiar un archivo en uno nuevo
 * @param directorio Puntero a la entrada del directorio
 * @param inodos Puntero al conjunto de inodos
 * @param ext_bytemaps Puntero a los bytemaps
 * @param ext_superblock Puntero al superbloque de información
 * @param memdatos Puntero al conjunto de bloques de datos
 * @param nombreOrigen Nombre del archivo a copiar
 * @param nombreDestino Nombre del archivo final, no puede existir un archivo con el mismo nombre
 * @param fichero Puntero del fichero sobre el que escribir los resultados
 * @returns Devuelve un número entero, 0 si está todo correcto, -1 si la acción no se ha podido realizar
 */
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           EXT_DATOS *memdatos, char *nombreOrigen, char *nombreDestino,  FILE *fichero);

/**
 * @brief Graba en la partición los inodos y el directorio
 * @param directorio Puntero que apunta al directorio que se debe grabar
 * @param inodos Puntero a los inodos existentes
 * @param fichero Archivo (la partición) donde escribir el directorio y los inodos
 * @returns void
 */
void GrabarInodosyDirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fichero);

/**
 * @brief Función que escribe en el fichero de la partición el contenido de los bytemaps
 * @param ext_bytemaps Puntero que apunta al bloque de bytemaps
 * @param fichero Puntero del fichero que viene siendo la partición
 * @returns void
 */
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fichero);

/**
 * @brief Método para grabar el contenido del superbloque en el fichero de la partición
 * @param ext_superblock Puntero que apunta al superbloque de información
 * @param fichero Fichero donde insertar los datos del superbloque (en este caso, al principio)
 * @returns void
 */
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fichero);

/**
 * @brief Método para grabar datos
 * @param memdatos Puntero al conjunto de bloques de datos
 * @param fichero Fichero donde escribir los datos
 * @returns void
 */
void GrabarDatos(EXT_DATOS *memdatos, FILE *fichero);

int main(){
    //Variables para los comandos a ejecutar
    char comando[LONGITUD_COMANDO];
    char orden[LONGITUD_COMANDO];
    char argumento1[LONGITUD_COMANDO];
    char argumento2[LONGITUD_COMANDO];

    //Iteradores para los bucles, de todos modos, se crean directamente en cada función
    //(No serían accesibles por alcance a este punto)
    int i, j;

    //Variables con los datos del fichero, superbloque, bytemaps, inodos, directorio y bloques
    EXT_DATOS datosFichero[MAX_BLOQUES_PARTICION];
    EXT_SIMPLE_SUPERBLOCK ext_superblock;
    EXT_BYTE_MAPS ext_bytemaps;
    EXT_BLQ_INODOS ext_blq_inodos;
    EXT_ENTRADA_DIR directorio[MAX_FICHEROS];
    EXT_DATOS memdatos[MAX_BLOQUES_DATOS];

    //Archivo de partición, se abre en formato de lectura, escritura y en formato binario
    FILE *archivoParticion = fopen("particion.bin", "r+b");

    //Comprueba que el archivo se ha abierto correctamente
    if(archivoParticion == NULL){
        //Mensaje de error, termina el programa
        printf("Error al abrir el archivo de partición...");
        return 1;
    }

    //Comprobado que está abierto, lee el contenido del archivo
    fread(&datosFichero, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, archivoParticion);

    //Va guardando en las variables creadas anteriormente los distintos ajustes del fichero
    memcpy(&ext_superblock, (EXT_SIMPLE_SUPERBLOCK *) &datosFichero[0], SIZE_BLOQUE);
    memcpy(&directorio,     (EXT_ENTRADA_DIR *) &datosFichero[3], SIZE_BLOQUE);
    memcpy(&ext_bytemaps,   (EXT_BLQ_INODOS *) &datosFichero[1], SIZE_BLOQUE);
    memcpy(&ext_blq_inodos, (EXT_BLQ_INODOS *) &datosFichero[2], SIZE_BLOQUE);
    memcpy(&memdatos,       (EXT_DATOS *) &datosFichero[4], MAX_BLOQUES_DATOS*SIZE_BLOQUE);

    //Bucle infinito, alternativa a for(;;), se "rompe"/termina del mismo modo, con un break
    while(1){
        //Texto para indicar al usuario la escritura de consola
        printf(">> ");
        
        //Leer entrada del usuario, si fuera un texto NULO, continúa la ejecución
        if(fgets(comando, LONGITUD_COMANDO, stdin) == NULL) continue;

        //Eliminar el salto de línea del comando
        comando[strcspn(comando, "\n")] = '\0';

        int operacion = ComprobarComando(comando, orden, argumento1, argumento2);

        //Comparativa de cada comando
        if(operacion == NOTHING){                       continue; //El usuario no escribe valores
        } else if(operacion == EXIT){                   break;
        } else if(operacion == INFO){                   LeerSuperBloque(&ext_superblock);
        } else if(operacion == BYTEMAPS){               PrintBytemaps(&ext_bytemaps);
        } else if(operacion == DIR){                    Directorio(directorio, &ext_blq_inodos);
        } else if(operacion == PRINT){                  Imprimir(directorio, &ext_blq_inodos, memdatos, argumento1);
        } else if(operacion == RENAME){                 
            switch(Renombrar(directorio, &ext_blq_inodos, argumento1, argumento2)){
                case 0:
                    //El archivo se ha cambiado el nombre correctamente
                    printf("El archivo se ha renombrado...\n\n");

                    //Escribe en el directorio, el bytemap y el superbloque
                    //IMPORTANTE: esta acción se realiza sobre el fichero particion.bin
                    GrabarInodosyDirectorio(directorio, &ext_blq_inodos, archivoParticion);
                    GrabarByteMaps(&ext_bytemaps, archivoParticion);
                    GrabarSuperBloque(&ext_superblock, archivoParticion);
                    break;
                case -1:
                    //Interpretación del código de error, en este caso no existe
                    printf("El archivo a renombrar no existe...\n\n");
                    break;
                case -2:
                    //El primer argumento es correcto, pero el nombre nuevo ya existe
                    printf("El nuevo nombre del archivo ya existe...\n\n");
                    break;
            }
        } else if(operacion == REMOVE){
            //Realiza la acción de borrar y comprueba con un switch los resultados
            switch(Borrar(directorio, &ext_blq_inodos, &ext_bytemaps, &ext_superblock, argumento1, archivoParticion)){
                //Caso de éxito (teórico, a menos que surga una excepción no controlada)
                case 0:
                    //El usuario es informado de que se ha eliminado correctamente
                    printf("El archivo se ha eliminado correctamente...\n\n");

                    //Graba en los inodos, el bytemaps y el superbloque la información nueva
                    GrabarInodosyDirectorio(directorio, &ext_blq_inodos, archivoParticion);
                    GrabarByteMaps(&ext_bytemaps, archivoParticion);
                    GrabarSuperBloque(&ext_superblock, archivoParticion);
                    break;
                case -1:
                    //Mensaje error
                    printf("El archivo a eliminar no existe...\n\n");
                    break;
            }
        } else if(operacion == COPY){
            //A partir de un switch con la función Copiar (que devuelve un número) se interpreta la situación
            switch(Copiar(directorio, &ext_blq_inodos, &ext_bytemaps, &ext_superblock, memdatos, argumento1, argumento2, archivoParticion)){
                case 0:
                    //Mensaje de hecho con éxito
                    printf("El archivo se ha copiado...\n\n");
                    
                    //Graba en los inodos, el bytemaps y el superbloque la información nueva
                    GrabarInodosyDirectorio(directorio, &ext_blq_inodos, archivoParticion);
                    GrabarByteMaps(&ext_bytemaps, archivoParticion);
                    GrabarSuperBloque(&ext_superblock, archivoParticion);

                    break;
                case -1:
                    //El código -1 indica que el nombre de destino ya existe
                    printf("El nombre de destino ya existe...\n\n");
                    break;
                case -2:
                    //El código -2 indica que el archivo a copiar no existe
                    printf("El archivo a copiar no existe...\n\n");
                    break;
                case -3:
                    //El archivo no se puede crear, código -3
                    printf("El archivo no se puede crear...\n\n");
                    break;
            }
        } else if(operacion == NOT_RECOGNIZED) {
            //Comando no reconocido (operacion será equivalente a NOT_RECOGNIZED)
            printf("Comando no reconocido: %s\n\n", comando);
        }
    }

    //Antes de cerrar el archivo se graban los datos
    GrabarDatos(memdatos, archivoParticion);

    //Cierra el archivo y finaliza la ejecución del programa
    fclose(archivoParticion);
    return 0;
}

/**
 * @brief Función para comprobar un comando
 * @param strcomando Comando a analizar
 * @param orden Sinceramente, desconozco para que sirve este parámetro o que aplicación podía tener en el esqueleto
 * @param argumento1 Posible argumento 1 que va después del comando clave
 * @param argumento2 Posible argumento 2 que va después del argumento 1
 * @returns Número entero, código asociado a la acción a realizar en base a la interpretación del comando
 */
int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2){
    //Variable para el resultado de un posible sscanf
    int resultadoSscanf;
    
    //Si el usuario presiona ENTER, el primer caracter del comando es final
    if(strcomando[0] == '\0'){
        return NOTHING; //Devuelve NOTHING

    //Si el comando es info, devuelve el valor de INFO, que tiene un código asociado
    } else if(!strcmp(strcomando, "info")){
        return INFO;
    
    //Devuelve el código asociado a BYTEMAPS si el comando es bytemaps
    } else if(!strcmp(strcomando, "bytemaps")){
        return BYTEMAPS;
    
    //Devuelve el valor de DIR (código único) si el comando escrito es dir
    } else if(!strcmp(strcomando, "dir")){
        return DIR;

    //Escanea un posible comando rename, y guarda el resultado en la variable declarada al principio
    } else if(resultadoSscanf = sscanf(strcomando, "rename %s %s", argumento1, argumento2)){
        //Si  el resultado es diferente de 2 (2 argumentos adicionales)
        if(resultadoSscanf != 2){
            //Muestra un mensaje de error, y devuelve el código asociado de ERROR
            printf("Faltan argumentos para este comando...\n");
            return ERROR;
        } else {
            //Todo está correcto, devuelve el código para renombrar el archivo
            return RENAME;
        }
    
    //Mismos pasos para rename, pero escanea un solo comando y un argumento
    } else if(resultadoSscanf = sscanf(strcomando, "imprimir %s", argumento1)){
        //Si falta un argumento, o hay más de los descritos
        if(resultadoSscanf != 1){
            //Muestra un mensaje de error, con una estructura de ejemplo, devuelve ERROR
            printf("ERROR, estructura del comando: imprimir <nombre_del_archivo>\n\n");
            return ERROR;
        } else {
            //El proceso es correcto, puede imprimir, por lo que se devuelve PRINT
            return PRINT;
        }
    
    //Si el comando es eliminar un archivo
    } else if(resultadoSscanf = sscanf(strcomando, "remove %s", argumento1)){
        //Si falta un argumento (el nombre del archivo a eliminar)
        if(resultadoSscanf != 1){
            //MUestra un mensaje de error
            printf("ERROR, falta el nombre del archivo...\n\n");
            return ERROR;
        } else {
            //El comando es correcto, envía el código asociado a eliminaciones
            return REMOVE;
        }

    //Si el comando es copiar un archivo, comprueba también por los argumentos
    } else if(resultadoSscanf = sscanf(strcomando, "copy %s %s", argumento1, argumento2)){
        //Si faltan 2 argumentos, no permite la ejecución 
        if(resultadoSscanf != 2){
            //Muestra un mensaje de error
            printf("ERROR, debe haber dos argumentos...\n\n");
            return ERROR;
        } else {
            //El comando es correcto, envía el código asociado para copiar el archivo
            return COPY;
        }

    //Si el comando es salir, el programa devuelve el código asociado a EXIT
    } else if(!strcmp(strcomando, "salir")){
        return EXIT;
    
    //Situación por defecto, cuando no se encuentra el comando
    } else {
        return NOT_RECOGNIZED; //Devuelve no reconocido
    }
    
    /*
        Aunque esta línea nunca se alcanza, por nomenclatura
        se define que devuelva un valor no reconocido
    */
    return NOT_RECOGNIZED;
}

/**
 * @brief Muestra información detallada sobre el superbloque
 * @param psup Puntero a la estructura EXT_SIMPLE_SUPERBLOCK con la información
 * @returns void
 */
void LeerSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup){
    //Mostrar información acerca de la partición, estos datos se guardan en ext_superblock
    printf("Bloque                 = %d Bytes\n",   psup->s_block_size);            //Tamaño de cada bloque
    printf("inodos particion       = %d\n",         psup->s_inodes_count);          //Cantidad de inodos disponibles en la partición
    printf("inodos libres          = %d\n",         psup->s_free_inodes_count);     //Cantidad de inodos libres en la partición
    printf("Bloques particion      = %d\n",         psup->s_blocks_count);          //Cantidad de bloques disponibles en la partición
    printf("Bloques libres         = %d\n",         psup->s_free_blocks_count);     //Cantidad de bloques libres en la partición
    printf("Primer bloque de datos = %d\n\n",       psup->s_first_data_block);      //Índice del primer bloque de datos
}

/**
 * @brief Imprime los inodos y bloques del sistema de archivos
 * @param ext_bytemaps Puntero a la información de bytemaps
 * @returns void
 */
void PrintBytemaps(EXT_BYTE_MAPS *ext_bytemaps){
    //Imprime por consola cada inodo, utilizando un bucle de una sola línea
    printf("Inodos: ");
    for(int i=0; i<MAX_INODOS; i++) printf("%d ", ext_bytemaps->bmap_inodos[i]);

    //Imprime por consola cada bloque, utilizando del mismo modo un bucle de una sola línea 
    printf("\nBloques [0-25]: ");
    for(int i=0; i<MAX_INODOS; i++) printf("%d ", ext_bytemaps->bmap_bloques[i]);

    //Añade dos saltos de línea para el siguiente comando a ejecutar por el usuario
    printf("\n\n");
}

/**
 * @brief Muestra los archivos existentes, con la información de cada uno
 * @param directorio Puntero a la entrada del directorio
 * @param inodos Puntero a los inodos del sistema
 * @returns void
 */
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos){
    //Bucle que recorre el máximo de ficheros que pueden existir
    for(int i=0; i<MAX_FICHEROS; i++) {
        //Si el directorio es distinto de nulo, hay un archivo
        //Obviamos el directorio raíz, que es el 2
        if(directorio[i].dir_inodo != NULL_INODO && directorio[i].dir_inodo != directorio->dir_inodo) {
            //Creamos un inodo temporal, que guarda el número de bloques y el tamaño del archivo
            EXT_SIMPLE_INODE inodo = inodos->blq_inodos[directorio[i].dir_inodo];
                    
            //Mostramos por salida formateada los datos del fichero
            printf("%-15s tama%co:%-5d inodo:%-5d bloques:",
                directorio[i].dir_nfich,
                164, //Dado que la ñ es un carácter especial, escribo su código para que se muestre correctamente por consola
                inodo.size_fichero,
                directorio[i].dir_inodo
            );

            //Bucle anidado para el número de bloque por archivo
            for(int j=0; j<MAX_NUMS_BLOQUE_INODO; j++) if(inodo.i_nbloque[j] != NULL_BLOQUE) printf("%d ", inodo.i_nbloque[j]);

            //Salto de línea para el siguiente archivo o el final
            printf("\n");
        }
    }
    //Se añade un salto de línea para dejar margen entre el final de archivos y la entrada de comandos
    printf("\n");
}

/**
 * @brief Método para buscar un fichero
 * @param directorio Puntero a la entrada del directorio
 * @param inodos Puntero al conjunto de inodos
 * @param nombre Nombre del archivo por el que se va a buscar
 * @returns Número entero, -1 si no se encuentra, en caso contrario el índice del nodo del archivo
 */
int BuscarFichero(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombre){
    //El archivo por defecto no se ha encontrado
    int archivoEncontrado = -1;

    //Bucle para ir buscando a lo largo de todos los archivos
    for(int i=0; i<MAX_FICHEROS; i++) {
        //El nodo tiene que ser diferente de un nodo nulo
        //El nodo no puede ser el directorio raíz
        //El nombre del fichero tiene que coincidir con el pasado por parámetro
        //Se devuelve el índice
        if(
            directorio[i].dir_inodo != NULL_INODO &&
            directorio[i].dir_inodo != directorio->dir_inodo &&
            !strcmp(directorio[i].dir_nfich, nombre)
        ) return directorio[i].dir_inodo;
    }
    //Al llegar terminal el archivo, se devuelve -1 ya que no se ha devuelto índice
    return archivoEncontrado;
}

/**
 * @brief Renombra un archivo con un nombre antiguo y nuevo pasado por parámetro
 * @param directorio Puntero a la entrada del directorio
 * @param inodos Puntero al conjunto de inodos
 * @param nombreAntiguo Nombre antiguo a buscar
 * @param nombreNuevo Nombre nuevo del archivo
 * @returns Devuelve 0 en caso de éxito, -1 si no se encuentra el archivo, -2 si no se puede reemplazar
 */
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombreAntiguo, char *nombreNuevo){
    //Buscar el archivo con el nombre antiguo
    for(int i=0; i<MAX_FICHEROS; i++){
        //Si el nombre del fichero coincide con el antiguo, se ha encontrado
        if(!strcmp(directorio[i].dir_nfich, nombreAntiguo)){
            //Comprueba para todos los ficheros que el nombre no está en uso, en ese caso, devuelve -2
            for(int j=0; j<MAX_FICHEROS; j++) if(!strcmp(directorio[j].dir_nfich, nombreNuevo)) return -2;
            
            //Copia el nombre del archivo en el array de nombres del directorio
            strncpy(directorio[i].dir_nfich, nombreNuevo, LEN_NFICH);

            //Devuelve 0, por lo que la acción se ha completado con éxito
            return 0;
        }
    }

    //Devuelve -1 dado que el bucle ha terminado y no ha devuelto datos
    //Esto quiere decir que el archivo no se encontró
    return -1;
}

/**
 * @brief Método para imprimir el contenido de un archivo
 * @param directorio Puntero a la entrada del directorio
 * @param inodos Puntero al conjunto de inodos
 * @param memdatos Puntero al conjunto de bloques de datos
 * @param nombre Nombre del archivo a imprimir
 * @returns Devuelve un número entero, -1 si hubo errores, 1 en caso de éxito
 */
int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_DATOS *memdatos, char *nombre){
    //Primero busca el archivo, puede que no exista o el usuario ha escrito un valor incorrecto
    int indiceInodo = BuscarFichero(directorio, inodos, nombre);

    //Si el índice es -1, es que el archivo no se ha encontrado correctamente
    if(indiceInodo == -1){
        //Muestra un mensaje de error por consola
        printf("ERROR: Fichero %s no encontrado\n\n", nombre);
        return -1; //Devuelve -1, código de error de que el archivo no existe, el usuario luego puede tratar con este valor

    //En caso contrario, comprueba que el índice es inferior al máximo permitido
    } else if(indiceInodo < MAX_INODOS) {
        //Extrae la información de ese nodo y lo guarda en uno temporal
        EXT_SIMPLE_INODE inodo = inodos->blq_inodos[indiceInodo];
        
        //Ejecuta un bucle para leer las líneas del archivo
        for(int i=0; i<MAX_NUMS_BLOQUE_INODO; i++) {
            //Obtiene el número de bloque
            int numeroBloque = inodo.i_nbloque[i];

            //Si el número de bloque es nulo, rompe/termina el bucle, de lo contrario imprime el contenido
            if(numeroBloque != NULL_BLOQUE) printf("%.*s", SIZE_BLOQUE, memdatos[numeroBloque - PRIM_BLOQUE_DATOS].dato);
            else break;
        }
    }
    //Imprime un salto de línea para separarlo del contenido del archivo
    printf("\n");
    return 1; //Devuelve 1, la ejecución hipotéticamente se ha ejecutado correctamente 
}

/**
 * @brief Función para eliminar un fichero
 * @param directorio Puntero a la entrada del directorio
 * @param inodos Puntero al conjunto de inodos
 * @param ext_bytemaps Puntero a los bytemaps
 * @param ext_superblock Puntero al superbloque de información
 * @param nombre Nombre del archivo a eliminar
 * @param fichero Puntero del fichero a eliminar
 * @returns Devuelve 0 si todo ha salido correcto, -1 si el archivo no se ha encontrado
 */
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps,
        EXT_SIMPLE_SUPERBLOCK *ext_superblock, char *nombre, FILE *fichero){
    
    //Busca el indice del archivo, devuelve como código de error -1 si no se encuentra
    int indiceInodo = BuscarFichero(directorio, inodos, nombre);
    if(indiceInodo == -1) return -1;
    EXT_SIMPLE_INODE inodo = inodos->blq_inodos[indiceInodo]; //Inodo del archivo, que sí existe al llegar a esta línea

    //Vaciar los BLOQUES del archivo en el bytemap
    //Se marca cada uno con un 0 (porque está vacío)
    for(int i=0; i<MAX_NUMS_BLOQUE_INODO; i++) if(inodo.i_nbloque[i] != NULL_BLOQUE) ext_bytemaps->bmap_bloques[inodo.i_nbloque[i]] = 0;
    ext_bytemaps->bmap_inodos[indiceInodo] = 0; //Establecer a 0 el INODO en el bytemap (para que esté libre)

    //Establecer el tamaño del fichero a 0
    //Marcar los 7 punteros a bloque de ese inodo con el valor FFFFH
    //MAX_NUMS_BLOQUE_INODO = 7; NULL_BLOQUE = FFFFH
    inodo.size_fichero = 0;
    for(int i=0; i<MAX_NUMS_BLOQUE_INODO; i++) inodo.i_nbloque[i] = NULL_BLOQUE;

    //Guardar los cambios en el conjunto de inodos
    inodos->blq_inodos[indiceInodo] = inodo;

    //Busca la entrada del directorio para eliminarla a lo largo de todos los ficheros existentes
    for(int i=0; i<MAX_FICHEROS; i++){
        //Una vez encontrado el índice del directorio
        if(directorio[i].dir_inodo == indiceInodo) {
            memset(directorio[i].dir_nfich, 0, LEN_NFICH); //Elimina el nombre (valor 0)
            directorio[i].dir_inodo = NULL_INODO; //Elimina el nodo estableciendo uno nulo
            break;
        }
    }

    //Actualiza el superbloque, aumentando el número de inodos y bloques libres
    ext_superblock->s_free_inodes_count += 1;
    ext_superblock->s_free_blocks_count += 1;

    //Devuelve 0, lo que quiere decir que todo se ha realizado con éxito (hipotéticamente)
    return 0;
}

/**
 * @brief Método para copiar un archivo en uno nuevo
 * @param directorio Puntero a la entrada del directorio
 * @param inodos Puntero al conjunto de inodos
 * @param ext_bytemaps Puntero a los bytemaps
 * @param ext_superblock Puntero al superbloque de información
 * @param memdatos Puntero al conjunto de bloques de datos
 * @param nombreOrigen Nombre del archivo a copiar
 * @param nombreDestino Nombre del archivo final, no puede existir un archivo con el mismo nombre
 * @param fichero Puntero del fichero sobre el que escribir los resultados
 * @returns Devuelve un número entero, 0 si está todo correcto, -1 si la acción no se ha podido realizar
 */
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           EXT_DATOS *memdatos, char *nombreOrigen, char *nombreDestino, FILE *fichero){

    //Buscar el fichero origen en el directorio
    int inodoArchivoOrigen = BuscarFichero(directorio, inodos, nombreOrigen);    
    if(inodoArchivoOrigen == -1) return -2;  //El fichero origen no se encuentra

    //Verificar si el fichero destino ya existe en el directorio
    if(BuscarFichero(directorio, inodos, nombreDestino) != -1) return -1; //El fichero de destino ya existe

    //Buscar el primer inodo libre
    int inodoLibre = -1;
    for(int i=0; i<MAX_INODOS; i++){
        //Si el inodo es 0, está libre, por lo que guarda el valor del iterador en la variable
        if(ext_bytemaps->bmap_inodos[i] == 0){
            inodoLibre = i;
            ext_bytemaps->bmap_inodos[inodoLibre] = 1; //Se establece como ocupado
            break; //Finaliza el bucle
        }
    }
    
    //Si no se encuentra un inodo libre, el valor seguirá siendo -1, se devuelve -3
    if(inodoLibre == -1) return -3;

    //Copia la información del inodo origen al inodo destino
    EXT_SIMPLE_INODE *ext_inodoDestino = &inodos->blq_inodos[inodoLibre];
    EXT_SIMPLE_INODE *ext_inodoOrigen = &inodos->blq_inodos[inodoArchivoOrigen];
    
    //Guarda en el inodo destino el tamaño del fichero de origen
    ext_inodoDestino->size_fichero = ext_inodoOrigen->size_fichero;

    //Inicia los bloques de destino a valores nulos
    for(int i=0; i<MAX_NUMS_BLOQUE_INODO; i++) ext_inodoDestino->i_nbloque[i] = NULL_BLOQUE;

    //Copia los bloques del fichero origen al destino
    for(int i=0; i<MAX_NUMS_BLOQUE_INODO && ext_inodoOrigen->i_nbloque[i] != NULL_BLOQUE; i++){
        //Guarda el bloque de origen para buscar posteriormente uno de destinoa
        int bloqueOrigen = ext_inodoOrigen->i_nbloque[i];
        int bloqueDestino = -1;

        //Busca el primer bloque libre en el bytemap
        for(int j=PRIM_BLOQUE_DATOS; j<MAX_BLOQUES_PARTICION; j++){
            //Si el bloque en el bytemap es 0, es un bloque libre
            if(ext_bytemaps->bmap_bloques[j] == 0){
                //Establece el bloque de destino el del iterador y lo marca como ocupado
                bloqueDestino = j;
                ext_bytemaps->bmap_bloques[j] = 1;  
                break; //Rompe el bucle
            }
        }
        
        //Si no hay bloques libres, retorna error (tener en cuenta que el -3 es que no se puede crear por falta de espacio libre, ya sea inodo o bloque)
        if(bloqueDestino == -1) return -3;

        //Copia los datos del bloque origen al bloque destino
        memcpy(memdatos[bloqueDestino - PRIM_BLOQUE_DATOS].dato, memdatos[bloqueOrigen - PRIM_BLOQUE_DATOS].dato, SIZE_BLOQUE);

        //Asigna el bloque destino al inodo bloque destino
        ext_inodoDestino->i_nbloque[i] = bloqueDestino;
    }

    //Crea una entrada para el fichero destino en el directorio
    for(int i=0; i<MAX_FICHEROS; i++){
        //Si encuentra un inodo nulo, esstablece ahí el valor
        if(directorio[i].dir_inodo == NULL_INODO){
            //Copia el string de nombreDestino en la dirección libre, guarda también el inodo libre 
            strncpy(directorio[i].dir_nfich, nombreDestino, LEN_NFICH);
            directorio[i].dir_inodo = inodoLibre;
            break; //Rompe el bucle
        }
    }

    //Reduce la cantidad de inodos y bloques libres
    ext_superblock->s_free_inodes_count--;
    ext_superblock->s_free_blocks_count--;

    //Devuelve el código 0 (el fichero se ha copiado correctamente)
    return 0;
}


/**
 * @brief Graba en la partición los inodos y el directorio
 * @param directorio Puntero que apunta al directorio que se debe grabar
 * @param inodos Puntero a los inodos existentes
 * @param fichero Archivo (la partición) donde escribir el directorio y los inodos
 * @returns void
 */
void GrabarInodosyDirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fichero){
    //Mover el puntero del archivo a donde debe ir los inodos y escribirlo en la partición/fichero
    fseek(fichero, SIZE_BLOQUE * 2, 0);
    fwrite(inodos, SIZE_BLOQUE, 1, fichero);

    //Mover el puntero del archivo a la posición donde debe ir el directorio y escribirlo en el fichero
    fseek(fichero, SIZE_BLOQUE * 3, 0);
    fwrite(directorio, SIZE_BLOQUE, 1, fichero);
}

/**
 * @brief Función que escribe en el fichero de la partición el contenido de los bytemaps
 * @param ext_bytemaps Puntero que apunta al bloque de bytemaps
 * @param fichero Puntero del fichero que viene siendo la partición
 * @returns void
 */
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fichero){
    //Mueve el puntero a la ubicación donde van los bytemaps
    fseek(fichero, SIZE_BLOQUE, 0);

    //Escribe en el fichero de la partición el contenido de bytemaps
    fwrite(ext_bytemaps, SIZE_BLOQUE, 1, fichero);
}

/**
 * @brief Método para grabar el contenido del superbloque en el fichero de la partición
 * @param ext_superblock Puntero que apunta al superbloque de información
 * @param fichero Fichero donde insertar los datos del superbloque (en este caso, al principio)
 * @returns void
 */
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fichero){
    //Mueve el puntero del archivo al principio del todo (al primer superbloque)
    fseek(fichero, 0, SEEK_SET);

    //Escribir el contenido del superbloque en el archivo
    fwrite(ext_superblock, SIZE_BLOQUE, 1, fichero);
}

/**
 * @brief Método para grabar datos
 * @param memdatos Puntero al conjunto de bloques de datos
 * @param fichero Fichero donde escribir los datos
 * @returns void
 */
void GrabarDatos(EXT_DATOS *memdatos, FILE *fichero){
    //Mover el puntero del archivo a la posición donde deben ir los bloques de datos
    fseek(fichero, PRIM_BLOQUE_DATOS * SIZE_BLOQUE, 0);

    //Escribir tantos bloques de datos como existan en el archivo
    for(int i=0; i<MAX_BLOQUES_DATOS; i++) fwrite(&memdatos[i], SIZE_BLOQUE, 1, fichero);
}