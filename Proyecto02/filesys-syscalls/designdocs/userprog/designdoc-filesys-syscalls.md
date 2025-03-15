# PROJECT 2: USER PROGRAMS. FILE SYSTEM, SYSTEM CALLS

## EQUIPO
> Pon aquí los nombres y correos electrónicos de los integrantes de tu equipo.

<Gabriela_López_Diego> <gabriela_164@ciencias.unam.mx>
<Javier_Alejandro_Rivera_Zavala> <alejandro_riveraz@ciencias.unam.mx>

##  PRELIMINARES
> Si tienes algún comentario preliminar a tu entrega, notas para los ayudantes o extra créditos, por favor déjalos aquí.

PASAN 38/42 PRUEBAS EN TOTAL en esta práctica.
IMPORTANTE: Ya no pasan todas las pruebas de la práctica anterior, una falla. Pasan 9/10 de la práctica: "Process Control System Calls"
En github al no pasar todas las pruebas de Process Control System Calls, no se ejecutan las pruebas de esta práctica 5. 
Ya no nos dio tiempo de corregirlo por qué estamos ahogados en tareas y proyectos, una disculpa. 

> Por favor cita cualquier recurso offline u online que hayas consultado mientras preparabas tu entrega, otros que no sean la documentación de Pintos, las notas del curso, o el equipo de enseñanza.

## SYSTEM CALLS - FILE SYSTEM

### ESTRUCTURAS DE DATOS

> C1: A cada declaración nueva o modificación de un `struct` o `struct member`,
> variable global o estática, `typedef` o `enum`. Agrega comentarios en el código
> en el que describas su propósito en 25 palabras o menos.

R: Va directo en el código.

src/threads/thread.c 
src/threads/thread.h
src/userprog/Make.vars
src/userprog/process.c
src/userprog/process.h  <--- (struct)
src/userprog/syscall.c
src/userprog/syscall.h
src/Make.config

> C2: Describe cómo asocias _file descriptors_ con archivos _open files_. En tu 
> implementación los _file descriptors_ son únicos dentro del _sistema operativo_ 
> completo, o solamente dentro de un proceso?
En primer lugar, dentro de la función init_thread la parte relevante para la gestión de descriptores de archivo es la inicialización de la lista fd_list, en la que se almacenan los descriptores de archivo asociados al hilo:
 /* Intialize the list of file descriptors */
  list_init(&t->fd_list);
  /* Initialize file descriptor, which should be 2 since 0 is 
    reserved for STDIN and 1 is for STDOUT */
  t->fd = 2;
Esto establece la base para la gestión de descriptores de archivo dentro del hilo. El hilo inicia con una lista vacía fd_list que se utilizará para almacenar los descriptores de archivo asociados a él. El descriptor de archivo inicial (t->fd) se establece en 2, ya que los descriptores 0 y 1 suelen estar reservados para STDIN y STDOUT respectivamente. Dado que la lista de descriptores de archivo (fd_list) se encuentra dentro de la estructura de hilo (struct thread), cada hilo en el sistema operativo puede tener su propia lista única de descriptores de archivo, aún cuando compartan descriptores entre si. Además de lo anterior, distintas funciones implementadas para las syscalls gestionan los descriptores, por ejemplo las funciones read y write verifican si el descriptor de archivo pertenece a la entrada/salida estándar (STDIN_FILENO o STDOUT_FILENO) o si la lista de descriptores de archivo del hilo está vacía. En estos casos, las funciones retornan 0, indicando que no se realizarán operaciones de lectura o escritura. Estás funciones son cruciales para la gestión de los descriptors de otras funciones, pues las mismas hacen uso de las ya mencionadas.
Para otros descriptores de archivo, se utiliza la función (get_file_from_list) para obtener el puntero al archivo correspondiente en la lista fd_list.
Por último, es preciso notar que se utilizan locks (file_lock) para garantizar la sincronización en operaciones de lectura/escritura, evitando problemas de concurrencia.

### ALGORITMOS

> No hay preguntas en esta práctica

### SYNCHRONIZATION

> C8: El acceso al _File System_ en pintos no es _thread-safe_, en consecuencia necesitas
> manejar la concurrencia cuando manipulas archivos, para este propósito puedes utilizar un
> _semaphore_ o un _lock_. Cuales operaciones deben de ser protegidas y cómo? Es un único 
> _semaphore_ o _lock_ global suficiente para resolver los problemas de concurrencia de esta
> tarea?
Dado que el acceso al sistema de archivos no es thread-safe, es necesario manejar la concurrencia al manipular archivos. Algunas operaciones que deben ser protegidas incluyen la apertura, el cierre, la lectura y la escritura de archivos. Estas operaciones pueden generar conflictos si varios hilos intentan realizarlas simultáneamente. Por ejemplo podría haber problemas sí:
-Dos hilos intentan abrir un archivo al mismo tiempo
-Dos hilos cierran un archivo simultáneamente.
-Si varios hilos intentan leer desde el mismo archivo simultáneamente, se podrían mezclar los datos o provocar otros problemas.
-Si varios hilos intentan escribir en el mismo archivo simultáneamente.

En esta implementación empleados un único lock global para toda la manipulación de archivos. Este lock es adquirido antes de realizar cualquier operación de archivo crítica y liberado después de completarla. En teoría puede emplearse con un sólo lock global tal y como lo hicimos aquí, sin embargo, hay algunas desventajas tales como  que si múltiples hilos intentan realizar operaciones en diferentes archivos, un único lock global podría convertirse en un cuello de botella y reducir la concurrencia. Podría también haber bloqueos innecesarios si un hilo realiza una operación de lectura no relacionada con un archivo mientras otro hilo realiza operaciones de escritura en un archivo diferente.

### RATIONALE

> C10: Qué ventajas o desventajas puedes ver en tu diseño de _file descriptors_? Menciona por lo menos
> dos alternativas de diseño y expón los _pros_ y _contras_ de cada una de ellas.
El diseño de file descriptors presentado es un diseño sencillo que utiliza una lista (fd_list) dentro de cada hilo (struct thread) para mantener un registro de los descriptores de archivo asociados a ese hilo en particular. El descriptor de archivo inicial (t->fd) se asigna al valor 2, ya que los descriptores 0 y 1 se reservan para STDIN y STDOUT.

Ventajas:
Nuestra implementación es fácil de entender e implementar, lo que facilita el mantenimiento y la depuración del código, así mismo, el que cada hilo tenga su propia lista de descriptores de archivo, facilita la asociación directa entre un hilo y sus descriptores sin etapas intermedias.

Desventajas:
El uso de un único lock global (file_lock) como se mencionó antes, podría resultar en bloqueos innecesarios si múltiples hilos intentan realizar operaciones en diferentes archivos simultáneamente, reduciendo la concurrencia, también podría generar cuellos de botella.

Alternativas de Diseño:
-Podríamos usar un lock por archivo para que múltiples hilos operen simultáneamente en archivos diferentes, también evitamos el bloqueo innecesario cuando los hilos operan en archivos diferentes. En contra, tenemos el que la gestión de locks por archivo puede ser más compleja y por ende, más propensa a errores.

-Otra posible alternativa de diseño, incluye construir una tabla global de archivos, lo cuál podría facilitar la gestión centralizada de todos los archivos abiertos en el sistema, ello permite un mayor control sobre la asignación y liberación de descriptores de archivo. Sin embargo, podríamos inducir la formación de cuellos de botella, si muchos hilos acceden frecuentemente a diferentes archivos.

## READ ONLY EXECUTABLES

### RATIONALE

> C11: Cómo haces para mantener abierto un archivo ejecutable asociado a un proceso que no ha concluido su ejecución?
En nuestra implementación, un archivo ejecutable asociado a un proceso se mantiene abierto a través de una estructura de datos llamada struct file que almacena información sobre el programa en ejecución. Esta estructura guarda la información sobre el archivo ejecutable asociado a un proceso y se asocia con un descriptor de archivo específico en la tabla de descriptores del proceso.

Cuando un proceso se inicia, se abre el archivo ejecutable asociado. Esto se realiza a través de la llamada al sistema exec, que carga el programa en memoria y configura la información necesaria para su ejecución. El descriptor de archivo asociado al archivo ejecutable se almacena en la tabla de descriptores de archivo del proceso. Este descriptor de archivo es único dentro del proceso y se utiliza para realizar operaciones de lectura y ejecución en el archivo. 

El archivo ejecutable se mantiene abierto durante toda la duración del proceso pues la estructura thread mantiene información sobre el archivo ejecutable abierto en su miembro fd_list. Cada entrada en esta lista contiene un puntero a la estructura struct file que representa el archivo asociado al descriptor de archivo. Durante la ejecución del proceso, el acceso a datos asociados al archivo ejecutable es posible a través de la estructura struct file obtenida desde la lista fd_list.

Finalmente, cuando el proceso concluye su ejecución, se cierran todos los archivos asociados al proceso, incluyendo el archivo ejecutable. Esto se realiza a través de la función close_all_files que itera sobre la lista fd_list y cierra cada archivo.
En resumen, el archivo ejecutable asociado a un proceso se mantiene abierto a través de la administración de descriptores de archivo en la tabla de descriptores de archivo del proceso. Este enfoque asegura que el archivo permanezca abierto mientras el proceso está en ejecución y se cierra cuando el proceso termina.
