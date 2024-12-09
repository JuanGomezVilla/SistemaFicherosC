# SistemaFicherosC

[![SistemaFicherosC](https://img.shields.io/badge/version-v1.0.0-blue.svg)](#)
[![SistemaFicherosC](https://img.shields.io/github/license/JuanGomezVilla/FlangerJS)](#)



SistemaFicherosC es un sistema de gestión de ficheros simple basado en C, simulando de forma básica el sistema de gestión de directorios de un sistema operativo convencional. Utiliza un archivo de nombre _particion.bin_ que es considerado el archivo donde se almacenan los datos a trabajar, que son ficheros de texto.

> [!NOTE]
> Es importante tener en cuenta que esto tiene una finalidad meramente educativa

<br>


## Índice
1. [Instalación y desarrollo](#punto1)
2. [Comandos](#punto2)



<br><div id="punto1"></div>

## 1. Instalación y desarrollo
1. Clona el repositorio en un directorio de tu ordenador:
    ```bash
    git clone https://github.com/JuanGomezVilla/SistemaFicherosC.git
    ```
2. Para compilar y ejecutar el proyecto, lanza (desde la carpeta _src_ el siguiente comando):
    ```bash
    cd src
    gcc -o main simul_ext_esqueleto.c && main
    ```
    Alternativamente, puedes optar por llamar al programa _run.bat_, que se encargará de automatizar esta labor:
    ```bash
    run
    ```
    Hay que tener en cuenta que si detenemos la ejecución del programa de forma abrupta (sin recurrir al comando _salir_), la ubicación será _src_, por lo que habría que retornar a la raíz para ejecutar _run_



<br><div id="punto2"></div>

## 2. Comandos
- Mostrar la información del superbloque:
    ```bash
    >> info
    Bloque                 = 512 Bytes
    inodos particion       = 24       
    inodos libres          = 19       
    Bloques particion      = 100      
    Bloques libres         = 92       
    Primer bloque de datos = 4
    ```
- Ver el bytemap de inodos y los 25 primeros elementos del bytemap de bloques:
    ```bash
    >> bytemaps
    Inodos: 1 1 1 1 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
    Bloques [0-25]: 1 1 1 1 0 0 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    ```
    > Se recomienda utilizar este comando durante la fase de desarrollo
- Mostrar los archivos del directorio:
    ```bash
    >> dir
    BelloGal.txt    tamaño:1289  inodo:3     bloques:6 9 8 
    HOLA.txt        tamaño:11    inodo:5     bloques:4
    CHISTE.txt      tamaño:44    inodo:7     bloques:7
    ```
    > El comando obvia la entrada especial del directorio raíz, por lo tanto, no se mostrará.
- Cambiar el nombre de un fichero. La estructura de este comando es la siguiente:
    ```bash
    >> rename nombre_antiguo nombre_nuevo
    ```
    Para ejecutarlo, por ejemplo con el archivo _HOLA.txt_ a _ADIOS.txt_:
    ```bash
    >> rename HOLA.txt ADIOS.txt
    El archivo se ha renombrado...
    ```
    > Si el nuevo nombre ya existe o no se encuentra el nombre del archivo, la acción no se llevará a cabo
- Imprimir el contenido de un archivo:
    ```bash
    >> imprimir nombre_del_archivo
    ```
    Por ejemplo, para imprimir el contenido de _HOLA.txt_:
    ```bash
    >> imprimir HOLA.txt
    OLA KE ASE?
    ```
- Para eliminar un archivo de la partición:
    ```bash
    >> remove nombre_del_archivo
    ```
    Si queremos eliminar el archivo _CHISTE.txt_ procedemos con:
    ```bash
    >> remove CHISTE.txt
    El archivo se ha eliminado correctamente...
    ```
- Para copiar un archivo en uno nuevo, que se crea automáticamente:
    ```bash
    copy ADIOS.txt Prueba.txt
    ```
- Cerrar el programa (evitar el uso de Ctrl + C):
    ```bash
    salir
    ``` 