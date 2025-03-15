# PROJECT 2: USER PROGRAMS. PROCESS MANAGEMENT SYSTEM CALLS

## EQUIPO
> Pon aquí los nombres y correos electrónicos de los integrantes de tu equipo.

 <Gabriela_López_Diego> <gabriela_164@ciencias.unam.mx>
 <Javier_Alejandro_Rivera_Zavala> <alejandro_riveraz@ciencias.unam.mx>

##  PRELIMINARES
> Si tienes algún comentario preliminar a tu entrega, notas para los ayudantes o extra créditos, por favor déjalos aquí.

> Por favor cita cualquier recurso offline u online que hayas consultado mientras preparabas tu entrega, otros que no sean la documentación de Pintos, las notas del curso, o el equipo de enseñanza

Recursos online que consultamos para la realización de esta práctica: 
 
-  https://www.youtube.com/watch?v=sBFJwVeAwEk
-  https://www.youtube.com/watch?v=RbsE0EQ9_dY
-  https://www.youtube.com/watch?v=jfPdiaAG0AQ

## SYSTEM CALLS

### ESTRUCTURAS DE DATOS

> B1: A cada declaración nueva o modificación de un `struct` o `struct member`, variable global o estática, `typedef` o `enum`. Agrega comentarios en el código en el que describas su propósito en 25 palabras o menos.

 R: Va directo en el código.
 
 Realizamos comentarios y cambios en los siguientes archivos. 
 - userprog/syscall.h
 - userprog/syscall.c
 - userprog/process.h 
 - userprog/process.c
 - threads/thread.h
 - threads/thread.c
 
### ALGORITMOS
> B5: Describe brevemente tu implementación de la __system call__ "wait" y cómo interactúa con la terminación del proceso.

R5: process_wait espera a que el hilo hijo con el identificador tid termine su ejecución. Si es la primera llamada a process_wait para ese hijo, se bloquea hasta que el hijo termine y devuelve su estado de salida. Si el hijo ya ha terminado o la función ya ha sido llamada previamente, devuelve -1.

### SYNCHRONIZATION

> B7: La __system call__ `exec` regresa `-1` si falla la carga del ejecutable, por lo cual no puede regresar antes de que se complete la carga en el proceso hijo. ¿Cómo se garantiza que `exec` no regrese antes de que finalice la carga del ejecutable? ¿Cómo se pasa el resultado de la carga (éxito/fallo) hacia el __thread__ que llama a `exec`? 

R7:el proceso hijo comunica su estado de carga al hilo padre a través de la estructura struct child_element y el uso de semáforos para asegurarse de que el hilo padre no regrese antes de que el proceso hijo termine la carga del ejecutable. Esto permite que el hilo padre conozca el resultado de la carga del proceso hijo y tome decisiones basadas en ese resultado.

> B8: Considera un proceso padre P con un proceso hijo C. ¿Cómo se evitan __race conditions__ cuando P llama `wait(C)` antes de que C finalice? ¿Cómo se evitan __race conditions__ cuando P llama `wait(C)` luego de que C finalice? ¿Cómo se garantiza que todos los recursos se liberen en cada caso? ¿Qué ocurre cuando C finaliza su ejecución sin llamar a `wait`, antes de que C finalice? ¿Luego de que C finalice? ¿Existe algún caso especial que se deba tener en cuenta?  

R8:
1. Cuando P llama a process_wait(C) antes de que C finalice:
En este caso, el proceso padre P bloquea su ejecución en el semáforo sema_wait asociado al proceso hijo C, ya que C todavía está en ejecución.
La estructura struct child_element del proceso hijo C registra el estado STILL_ALIVE para indicar que C está en ejecución.
Cuando C finaliza su ejecución, establece su estado de salida en la estructura struct child_element y desbloquea el semáforo sema_wait para que el proceso padre P continúe su ejecución.
El recurso (estructura struct child_element) se libera después de que P obtiene el estado de salida de C.
2. Cuando P llama a process_wait(C) después de que C finalice:
En este caso, el proceso padre P no se bloquea, ya que C ya ha finalizado.
El proceso padre P puede obtener el estado de salida de C directamente de la estructura struct child_element sin bloquearse.
El recurso (estructura struct child_element) se libera después de que P obtiene el estado de salida de C.
3. C finaliza sin llamar a process_wait antes de finalizar:
En este caso, el proceso hijo C marca su estado de salida como WAS_KILLED en la estructura struct child_element para indicar que fue terminado por el sistema operativo antes de que pudiera llamar a process_wait.
La estructura struct child_element del proceso hijo C se libera después de que el proceso hijo C termina su ejecución.
4. Luego de que C finaliza (después de llamar a process_wait o sin hacerlo):
La estructura struct child_element asociada a C se libera correctamente para evitar pérdida de recursos.
El estado de salida de C (éxito o fallo) se mantiene en la estructura struct child_element hasta que se obtiene y se devuelve a través de process_wait o se libera en caso de que C finalice sin llamar a process_wait.

### RATIONALE

> B9: (Punto Extra) ¿Por qué elegiste implementar el acceso a la memoria del usuario desde el __kernel__ de la forma en que lo hiciste?

R9:  Para el acceso a la memoria de usuario desde el núcleo, optamos por utilizar la descomposición de funciones para la detección de errores. Esto reduce la dificultad y es más simple que implementar el manejo de memoria de fallo de página.
 Siempre que un puntero no sea válido, será capturado por el manejador de interrupción de fallo de página. En el manejador de interrupciones se llama al syscall exit(-1).

> B11: El mapeo de `tid_t` a `pid_t` utilizado por defecto es la función identidad. Si se utilizó un mapeo diferente, ¿Cuáles son las ventajas del mapeo utilizado?

R11: Utilizamos tid_t a pid_t. La ventaja del mapeo por defecto (identidad) es la simplicidad y la consistencia en la gestión de identificadores de hilos y procesos. Los identificadores de hilos y procesos son los mismos, lo que facilita el seguimiento y la administración de los mismos. En ciertos casos, un mapeo personalizado ofrece una mayor flexibilidad y control sobre la asignación de identificadores en un sistema, lo que puede llevar a una mejor eficiencia, seguridad y adaptación a las necesidades específicas de la aplicación. La elección entre un mapeo personalizado y/o un mapeo identidad dependerá de las necesidades y diseño del sistema. 
