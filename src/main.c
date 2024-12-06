#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "cabeceras.h"

#define LONGITUD_COMANDO 100 //Longitud máxima de un comando


//FUNCIONES PROCESADAS
void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);
void LeerSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup);


int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2);

int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
              char *nombre);
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos);
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
              char *nombreantiguo, char *nombrenuevo);
int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
             EXT_DATOS *memdatos, char *nombre);
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           char *nombre,  FILE *fich);
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich);
void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich);
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich);
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich);
void GrabarDatos(EXT_DATOS *memdatos, FILE *fich);

int main() {
	char comando[LONGITUD_COMANDO];
	char orden[LONGITUD_COMANDO];
	char argumento1[LONGITUD_COMANDO];
	char argumento2[LONGITUD_COMANDO];

    int i, j;


    EXT_DATOS datosFichero[MAX_BLOQUES_PARTICION];
    EXT_SIMPLE_SUPERBLOCK ext_superblock;
    EXT_BYTE_MAPS ext_bytemaps;
    EXT_BLQ_INODOS ext_blq_inodos;
    EXT_ENTRADA_DIR directorio[MAX_FICHEROS];
    EXT_DATOS memdatos[MAX_BLOQUES_DATOS];
    FILE *archivoParticion;

    archivoParticion = fopen("particion.bin","r+b");
    if (archivoParticion == NULL) {
        perror("Error al abrir el archivo de partición");
        return 1;
    }

    fread(&datosFichero, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, archivoParticion);

    memcpy(&ext_superblock, (EXT_SIMPLE_SUPERBLOCK *) &datosFichero[0], SIZE_BLOQUE);
    memcpy(&directorio, (EXT_ENTRADA_DIR *) &datosFichero[3], SIZE_BLOQUE);
    memcpy(&ext_bytemaps, (EXT_BLQ_INODOS *) &datosFichero[1], SIZE_BLOQUE);
    memcpy(&ext_blq_inodos, (EXT_BLQ_INODOS *) &datosFichero[2], SIZE_BLOQUE);
    memcpy(&memdatos, (EXT_DATOS *) &datosFichero[4],MAX_BLOQUES_DATOS*SIZE_BLOQUE);

    //Bucle infinito, alternativa a for(;;), se "rompe" del mismo modo, con un break
    while(1){
        printf(">> ");
        
        //Leer entrada del usuario
        if(fgets(comando, LONGITUD_COMANDO, stdin) == NULL) {
            printf("Error leyendo la entrada.\n");
            continue;
        }

        //Eliminar el salto de línea del comando
        comando[strcspn(comando, "\n")] = '\0';

        if(strcmp(comando, "info") == 0){
            LeerSuperBloque(&ext_superblock);
        
        } else if(strcmp(comando, "bytemaps") == 0){
            Printbytemaps(&ext_bytemaps);
            
        
        } else if(strcmp(comando, "dir") == 0){
            //Bucle que recorre el máximo de ficheros que pueden existir
            for(i=0; i<MAX_FICHEROS; i++) {

                //Si el directorio es distinto de nulo, hay un archivo
                //Obviamos el directorio raíz, que es el 2
                if(directorio[i].dir_inodo != NULL_INODO && directorio[i].dir_inodo != directorio->dir_inodo) {
                    //Creamos un inodo temporal, que guarda el número de bloques y el tamaño del archivo
                    EXT_SIMPLE_INODE inodo = ext_blq_inodos.blq_inodos[directorio[i].dir_inodo];
                    
                    //Mostramos por salida formateada los datos del fichero
                    //He escrito size porque la ñ de tamaño no es aceptada por la consola
                    printf("%-20s size:%-10d inodo:%-10d bloques:",
                        directorio[i].dir_nfich,
                        inodo.size_fichero,
                        directorio[i].dir_inodo);

                    //Bucle anidado para el número de bloque
                    for(j=0; j<MAX_NUMS_BLOQUE_INODO; j++) if(inodo.i_nbloque[j] != NULL_BLOQUE) printf("%d ", inodo.i_nbloque[j]);

                    //Salto de línea para el siguiente archivo o el final
                    printf("\n");
                }
            }
        } else if(sscanf(comando, "imprimir %s", argumento1)){
            int archivoEncontrado = 0;
            for(i=0; i<MAX_FICHEROS; i++) {
                if(directorio[i].dir_inodo != NULL_INODO && directorio[i].dir_inodo != directorio->dir_inodo) {
                    if(strcmp(directorio[i].dir_nfich, argumento1) == 0){
                        archivoEncontrado = 1;
                        unsigned short int inodoIdx = directorio[i].dir_inodo;
                        if(inodoIdx < MAX_INODOS) {
                            EXT_SIMPLE_INODE inodo = ext_blq_inodos.blq_inodos[inodoIdx];
                            for (j = 0; j < MAX_NUMS_BLOQUE_INODO; j++) {
                                unsigned short int numBloque = inodo.i_nbloque[j];
                                if (numBloque != NULL_BLOQUE) {
                                    printf("%.*s", SIZE_BLOQUE, memdatos[numBloque - PRIM_BLOQUE_DATOS].dato);
                                } else {
                                    break;
                                }
                            }
                        } else {
                            printf("Inodo inválido\n");
                        }
                        printf("\n");
                        break;
                    }
                }
            }
            if(!archivoEncontrado) printf("Archivo no encontrado...\n");

        } else if(strcmp(comando, "salir") == 0) {
            // Si el usuario escribe "salir", terminamos el bucle
            break;
        } else {
            // Comando no reconocido
            printf("Comando no reconocido: %s\n", comando);
        }
    }

    fclose(archivoParticion);

    return 0;
}

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps){
    printf("Inodos: ");
    for(int i=0; i<MAX_INODOS; i++) printf("%d ", ext_bytemaps->bmap_inodos[i]);
    printf("\nBloques [0-25]: ");
    for(int i=0; i<MAX_INODOS; i++) printf("%d ", ext_bytemaps->bmap_bloques[i]);
    printf("\n");
}

int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2){
   return 0;
}

void LeerSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup){
    //Mostrar información acerca de la partición, estos datos se guardan en ext_superblock
    printf("Bloque %d Bytes\n",             psup->s_block_size);
    printf("inodos particion = %d\n",       psup->s_inodes_count);
    printf("inodos libres = %d\n",          psup->s_free_inodes_count);
    printf("Bloques particion = %d\n",      psup->s_blocks_count);
    printf("Bloques libres = %d\n",         psup->s_free_blocks_count);
    printf("Primer bloque de datos = %d\n", psup->s_first_data_block);
}


int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombre){
   return 0;
}

void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos){

}

int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombreantiguo, char *nombrenuevo){
   return 0;
}


int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_DATOS *memdatos, char *nombre){
   return 0;
}

int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps,
            EXT_SIMPLE_SUPERBLOCK *ext_superblock, char *nombre,  FILE *fich){
   return 0;
}

int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich){
   return 0;
}

void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich){

}

void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich){

}

void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich){

}

void GrabarDatos(EXT_DATOS *memdatos, FILE *fich){

}