# MyShell basic
![Banner](img/banner.png)
### SOI - FCEFyN - UNC - 2022

## Objetivo
Con el desarrollo del siguiente Trabajo Práctico (TP), se busca:

- Utilizar mecanismos de creación de Procesos.
- Utilizar mecanismos de concurrencia e IPC.
- Diseñar un intérprete de línea de comandos al estilo Bourne shell.

## Introducción
Este trabajo práctico consta en la elaboración de un programa en lenguaje C sobre _GNU/Linux_. El trabajo se divide en soluciones incrementales.


## Actividades
### 1. Command line prompt
_myshell_ debe contar con un prompt que contenga el camino al directorio actual.

Por ejemplo, para el home: _username@hostname:~$_
![](img/image1.png)

### 2. Internal commands
_myshell_ debe soportar los siguientes comandos internos:

> __cd \<directorio\>__ : cambia el directorio actual a \<directorio\>. Si \<directorio\> no está presente, reporta el directorio actual. Si el directorio no existe se debe imprimir un error apropiado. Además, este comando debe cambiar la variable de entorno PWD. 
Este comando debe soportar la opción *cd -*, que retorna al último directorio de trabajo (__OLDPWD__).

> __clr__: limpia la pantalla

> __echo \<comentario\|env var\>__ : muestra \<comentario\> en la pantalla seguido por una línea nueva. (múltiples espacios\/tabs pueden ser reducidos a un espacio).

> __quit__ : cierra myshell

![](img/image2.png)

### 3. Program invocation
Las entradas del usuario que no sean comandos internos deben ser interpretados como la invocación de un programa. La misma tiene que ser realizada mediante _fork_ y _execl_. Debe además soportar paths relativos y absolutos.
![](img/image3.png)


### 9. Ejecución de instrucciones desde archivo de texto
Si se desea ejecutar un set de instrucciones desde un archivo de texto, este se debe pasar como parámetro al ejecutar MyShell, de la siguiente manera:
```bash
./bin/myshell batchfile           
```
Donde batchfile corresponde al archivo de texto que se desee leer. Se debe colocar una instruccion por linea.

Al terminar de ejecutar las instrucciones, MyShell finalizará su ejecución.

Se provee un batchfile a modo de ejemplo.

![](img/image4.png)

### 5. Background execution
Un ampersand & al final de la línea de comando indica que la shell debe retornar al prompt inmediatamente luego de lanzar al programa a ejecutarse en background.

Cuando se comienza un trabajo en background, se debe imprimir un mensaje indicando su _Trabajo_ y su _ID de proceso_.

`[<job id>] <process id>`

Ejemplo:
`$ echo 'hola' &
[1] 10506
hola`

![](img/image5.png)

### 6. Soporte de comandos ctrl-c, ctrl-z y ctrl-|
Se puede ingresar cualquiera de estas combinaciones de teclas para abortar o pausar procesos en ejecución en MyShell.

![](img/image6.png)

### 7. Pipes
Se soporta el uso de pipes (el caracter "|") para "encadenar procesos", utilizando la stdout de uno como stdin del siguiente, y asi sucesivamente. Por ejemplo:
```bash
ls | grep .txt           
```
Correrá el comando "ls", y la salida que este genere será enviada al comando "grep" como entrada. En el caso del ejemplo, el comando "grep" filtrará los archivos que contengan la cadena ".txt" de los elementos listados por "ls".

![](img/image7.png)

### 8. Redirección de entrada y salida
Se puede redireccionar la entrada, la salida o ambos de un programa. Por defecto, la entrada de un programa son los caracteres ingresados por teclado por el usuario, y la terminal es la salida. Con esta funcionalidad, podemos cambiar este comportamiento, pudiendo hacer que un programa tome como entrada el texto de un archivo, por ejemplo, o bien pedirle que escriba la salida en un archivo. Por ejemplo:
```bash
ls > outputfile.txt          
```
Generará que los resultados del comando ls se escriban en el archivo de nombre outputfile.txt. En caso de que este no exista se creará. En caso de que el archivo contuviera texto, este se sobreescribirá. Para el caso de redirección de entrada, el archivo elegido como entrada debe existir, de lo contrario el comando será inválido.

![](img/image8.png)

### ¿Dónde se encuentran los pipes en el filesystem, qué atributos tienen?
Los pipes no se encuentran en el filesystem que se encuentran los demás binarios, ya que se encuentra en otro filesystem "propio" virtual. Este filesystem virtual se llama PipeFS. Se encuentra montado en el kernel. A diferencia de otros filesystems que están montados bajo "/", este se encuentra montado en "pipe", es decir que tiene su propio directorio root. La entrada a este fylesistem es la llamada al sistema "pipe()". Este filesystem no puede ser visto por los usuarios, y es un sisteama de archivos en memoria.
