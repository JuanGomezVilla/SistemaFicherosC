#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "cabeceras.h"
#define LONGITUD_COMANDO 100

int main() {
    char comando[LONGITUD_COMANDO]; // Arreglo para almacenar la entrada

    int i, j;


    EXT_DATOS datosFichero[MAX_BLOQUES_PARTICION];
    EXT_SIMPLE_SUPERBLOCK ext_superblock;
    EXT_BYTE_MAPS ext_bytemaps;
    EXT_BLQ_INODOS ext_blq_inodos;
    EXT_ENTRADA_DIR directorio[MAX_FICHEROS];
    EXT_DATOS memdatos[MAX_BLOQUES_DATOS];
    FILE *archivoParticion;

    archivoParticion = fopen("particion.bin","r+b");
    fread(&datosFichero, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, archivoParticion);

    memcpy(&ext_superblock, (EXT_SIMPLE_SUPERBLOCK *) &datosFichero[0], SIZE_BLOQUE);
    memcpy(&directorio, (EXT_ENTRADA_DIR *) &datosFichero[3], SIZE_BLOQUE);
    memcpy(&ext_bytemaps, (EXT_BLQ_INODOS *) &datosFichero[1], SIZE_BLOQUE);
    memcpy(&ext_blq_inodos, (EXT_BLQ_INODOS *) &datosFichero[2], SIZE_BLOQUE);
    memcpy(&memdatos, (EXT_DATOS *) &datosFichero[4],MAX_BLOQUES_DATOS*SIZE_BLOQUE);

    

    


    for (;;) { // Bucle infinito
        printf(">> ");
        
        // Leer entrada del usuario
        if (fgets(comando, LONGITUD_COMANDO, stdin) == NULL) {
            printf("Error leyendo la entrada.\n");
            continue;
        }

        // Eliminar el salto de línea del comando
        comando[strcspn(comando, "\n")] = '\0';

        if(strcmp(comando, "info") == 0){
            //Mostrar información acerca de la partición, estos datos se guardan en ext_superblock
            printf("Bloque %d Bytes\n", ext_superblock.s_block_size);
            printf("inodos particion = %d\n", ext_superblock.s_inodes_count);
            printf("inodos libres = %d\n", ext_superblock.s_free_inodes_count);
            printf("Bloques particion = %d\n", ext_superblock.s_blocks_count);
            printf("Bloques libres = %d\n", ext_superblock.s_free_blocks_count);
            printf("Primer bloque de datos = %d\n", ext_superblock.s_first_data_block);
        
        } else if(strcmp(comando, "bytemaps") == 0){
            printf("Inodos: ");
            for(i=0; i<MAX_INODOS; i++) printf("%d ", ext_bytemaps.bmap_inodos[i]);
            printf("\nBloques [0-25]: ");
            for(i=0; i<MAX_INODOS; i++) printf("%d ", ext_bytemaps.bmap_bloques[i]);
            printf("\n");
            
        } else if(strcmp(comando, "salir") == 0) {
            // Si el usuario escribe "salir", terminamos el bucle
            printf("Saliendo...\n");
            break;
        } else {
            // Comando no reconocido
            printf("Comando no reconocido: %s\n", comando);
        }
    }

    fclose(archivoParticion);

    return 0;
}