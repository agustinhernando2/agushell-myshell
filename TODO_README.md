# agushell-myshell
## MyShell basic
### SOI - FCEFyN - UNC - 2022


## Objetivo
Con el desarrollo del siguiente Trabajo Práctico (TP), se busca:

- Utilizar mecanismos de creación de Procesos.
- Utilizar mecanismos de concurrencia e IPC.
- Diseñar un intérprete de línea de comandos al estilo Bourne shell.


## Duración
Con el material dado en clase, este laboratorio está diseñado para resolverse entre 8 y 10 horas.


## Introducción
Este trabajo práctico consta en la elaboración de un programa en lenguaje C sobre _GNU/Linux_. El trabajo se divide en soluciones incrementales.


## Actividades
### 1. Command line prompt
_myshell_ debe contar con un prompt que contenga el camino al directorio actual.

Por ejemplo, para el home: _username@hostname:~$_



### 2. Internal commands
_myshell_ debe soportar los siguientes comandos internos:

- __cd \<directorio\>__ : cambia el directorio actual a \<directorio\>. Si \<directorio\> no está presente, reporta el directorio actual. Si el directorio no existe se debe imprimir un error apropiado. Además, este comando debe cambiar la variable de entorno PWD. 
Este comando debe soportar la opción *cd -*, que retorna al último directorio de trabajo (__OLDPWD__).

- __clr__: limpia la pantalla

- __echo \<comentario\|env var\>__ : muestra \<comentario\> en la pantalla seguido por una línea nueva. (múltiples espacios\/tabs pueden ser reducidos a un espacio).

- __quit__ : cierra myshell

### 3. Program invocation
Las entradas del usuario que no sean comandos internos deben ser interpretados como la invocación de un programa. La misma tiene que ser realizada mediante _fork_ y _execl_. Debe además soportar paths relativos y absolutos.


### 4. Batch file
_myshell_ debe ser capaz de tomar sus comandos a ejecutar desde un archivo. Por ejemplo, si la shell es invocada con un argumento _myshell batchfile_. El _batchfile_ contiene un conjunto de comandos de línea para que la shell ejecute. 

Cuando se alcance el fin de archivo (EOF), _myshell_ debe cerrarse.

Notar que si myshell se ejecuta sin argumento (.\/myshell), se tiene que mostrar el command prompt y se debe esperar a comandos del usuario vía stdin.

### 5. Background execution
Un ampersand & al final de la línea de comando indica que la shell debe retornar al prompt inmediatamente luego de lanzar al programa a ejecutarse en background.

Cuando se comienza un trabajo en background, se debe imprimir un mensaje indicando su _Trabajo_ y su _ID de proceso_.

`[<job id>] <process id>`

Ejemplo:
`$ echo 'hola' &
[1] 10506
hola`

## 6. Soporte de comandos ctrl-c, ctrl-z y ctrl-|
Se puede ingresar cualquiera de estas combinaciones de teclas para abortar o pausar procesos en ejecución en MyShell.

## 7. Pipes
Se soporta el uso de pipes (el caracter "|") para "encadenar procesos", utilizando la stdout de uno como stdin del siguiente, y asi sucesivamente. Por ejemplo:
```bash
ls | grep .txt           
```
Correrá el comando "ls", y la salida que este genere será enviada al comando "grep" como entrada. En el caso del ejemplo, el comando "grep" filtrará los archivos que contengan la cadena ".txt" de los elementos listados por "ls".

## 8. Redirección de entrada y salida
Se puede redireccionar la entrada, la salida o ambos de un programa. Por defecto, la entrada de un programa son los caracteres ingresados por teclado por el usuario, y la terminal es la salida. Con esta funcionalidad, podemos cambiar este comportamiento, pudiendo hacer que un programa tome como entrada el texto de un archivo, por ejemplo, o bien pedirle que escriba la salida en un archivo. Por ejemplo:
```bash
ls > outputfile.txt          
```
Generará que los resultados del comando ls se escriban en el archivo de nombre outputfile.txt. En caso de que este no exista se creará. En caso de que el archivo contuviera texto, este se sobreescribirá. Para el caso de redirección de entrada, el archivo elegido como entrada debe existir, de lo contrario el comando será inválido.

## Criterios de Corrección
- Se debe compilar el código con los flags de compilación: -Wall -Pedantic -Werror 
- Dividir el código en módulos de manera juiciosa.
- Estilo de código.
- El código no debe contener errores, ni warnings.
- El código no debe contener errores de cppcheck.
- _myshell_ no debe crear procesos zombies.

## Qué se debe Entregar
- Informe del desarrollo del proyecto (markdown).
- Código (funcionando bajo las especificaciones dadas y bajo cualquier caso de test de parámetros de entrada).
- Makefile / CMake
- Todo en el GitHub


