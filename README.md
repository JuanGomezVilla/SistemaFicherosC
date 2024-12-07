# SistemaFicherosC
Sistema de ficheros simple basado en C

## Índice
1. [Instalación y desarrollo](#punto1)
2. [Comandos](#punto2)
3. [Anotaciones](#punto3)




<br><div id="punto1"></div>

## 1. Instalación y desarrollo
1. Clona el repositorio en cualquier ubicación de tu ordenador:
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
    
    > [!TIP]
    > Se recomienda utilizar este comando durante la fase de desarrollo
- Mostrar los archivos del directorio:
    ```bash
    >> dir
    BelloGal.txt    tamaño:1289  inodo:3     bloques:6 9 8 
    HOLA.txt        tamaño:11    inodo:5     bloques:4
    CHISTE.txt      tamaño:44    inodo:7     bloques:7
    ```

    > [!NOTE]
    > El comando obvia la entrada especial del directorio raíz, por lo tanto, no se mostrará.
- Cambiar el nombre de un fichero. La estructura de este comando es la siguiente:
    ```bash
    rename nombre_antiguo nombre_nuevo
    ```
    Para ejecutarlo, por ejemplo con el archivo _HOLA.txt_ a _ADIOS.txt_:
    ```bash
    >> rename HOLA.txt ADIOS.txt
    El archivo se ha renombrado...
    ```

    > [!WARNING]
    > Si el nuevo nombre ya existe o no se encuentra el nombre del archivo, la acción no se llevará a cabo



<br><div id="punto3"></div>

## 3. Anotaciones
