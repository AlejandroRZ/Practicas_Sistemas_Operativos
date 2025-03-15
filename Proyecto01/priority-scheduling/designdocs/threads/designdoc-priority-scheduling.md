# PROJECT 1: THREADS - PRIORITY SCHEDULER. DESIGN DOCUMENT

## EQUIPO
 > Pon aquí los nombres y correos electrónicos de los integrantes de tu equipo.

 <Gabriela_López_Diego> <gabriela_164@ciencias.unam.mx>
 <Javier_Alejandro_Rivera_Zavala> <alejandro_riveraz@ciencias.unam.mx>

 ##  PRELIMINARES
 > Si tienes algún comentario preliminar a tu entrega, notas para los ayudantes
 > o extra créditos, por favor déjalos aquí.

 > Por favor cita cualquier recurso offline u online que hayas consultado
 > mientras preparabas tu entrega, otros que no sean la documentación de Pintos,
 > las notas del curso, o el equipo de enseñanza

## PRIORITY SCHEDULING

### ESTRUCTURAS DE DATOS

> B1: A cada declaración nueva o modificación de un `struct` o `struct member`,
 > variable global o estática, `typedef` o `enum`. Agrega comentarios en el
 > código en el que describas su propósito en 25 palabras o menos.

 R: Va directo en el código.
Descripción de los cambios hechos:
--------src/devices/timer.c----------
Se agrego:
struct list sleeping_threads;
Se modifico los metodos:
timer_init
timer_sleep
timer_interrupt 
Se comento los metodos
tick_less
timer_wait
timer_wakeup

--------------src/threads/synch.h---------------
se agrego:
conditional_var_comparator

--------------src/threads/synch.c---------------
se modifico:
sema_down
sema_up
lock_acquire
lock_try_acquire
lock_release
cond_signal
se agrego:
conditional_var_comparator

--------------src/threads/thread.h-------------------
Se modifico:
struct thread
se agrego 
sleeptime_comparator
priority_comparator_reverse
priority_comparator

--------------src/threads/thread.c-------------------
se modifico:
thread_create
thread_unblock 
thread_yield
thread_set_priority
init_thread
se agrego:
sleeptime_comparator
priority_comparator_reverse
priority_comparator
### ALGORITMOS

> B3: (Punto Extra) Cómo aseguras que el _thread_ de más alta
> prioridad que está esperando en un _semaphore_, _lock_ o 
> _condition_ despierta primero?
Para lograr el objetivo planteado, la función sema_up utiliza list_sort para ordenar la lista de espera (sema->waiters) 
en orden descendente de prioridad. Esto significa que el hilo con la prioridad más alta se colocará al frente de la lista y
luego de ello, se utiliza list_pop_front para eliminar al hilo con la mayor prioridad de la lista al cuál almacenaremos en father_element.
Para concluir, se llama a thread_unblock para desbloquear el hilo almacenado en father_element, así que el hilo de mayor prioridad será 
el primero en ser despertado y programado para ejecutarse, como detalle exta, es importante considerar que se incrementa el valor del semáforo 
(sema->value) y se restaura el nivel de interrupción al valor original. 
Además de todo lo anterior, si la función no se llama desde un contexto de interrupción (intr_context()), se llama a thread_yield() 
para que el hilo actual ceda el CPU, permitiendo que el hilo de mayor prioridad comience su ejecución de inmediato.

> B4: Qué tan eficientes es tu estrategia para insertar y borrar
> elementos de la `ready_list`? Puedes imaginar una estructura de
> datos para representar la `ready_list` en la que las inserciones y 
> operaciones de borrado tomen __O(1)__?
El borrado se ve apenas modificado ya que seguimos haciendo pop de la cabeza de la lista, operación que
en general podemos decir que se efectúa en tiempo constante, los cambios ocurren cuando consideramos la inserción
dentro de la ready_list mediante las funciones thread_unblock y thread_yield. Dichos cambios exigen que la lista en la
que encolamos los hilos, cada hilo nuevo compare su prioridad con aquellos que ya se encuentran dentro de dicha lista.
Lo anterior hace que en el peor de los casos se requiera de recorrer toda la lista para insertar un nuevo hilo a cada vez.

***EXTRA: Para efectuar las operaciones de eliminación e inserción en tiempo constante podría emplearse un arreglo, con acceso
mediante indice de acuerdo a la prioridad del hilo, el problema es entonces que perderíamos el tamaño dinámico de la estructura
que almacena los hilos y ello podría suponer un problema.
Otra posible elección sería usar un montículo de Fibonacci, que permite insertar y eliminar elementos en tiempo promedio del orden
__O(1)__, sin embargo tal complejidad no es estable y además los algoritmos empleados para tal tarea, son más difíciles de implementar
y requerirían de modificar aún más la estructura de las clases que forman el sistema operativo, sus métodos y atributos.


### SINCRONIZACIÓN

> B6: Describe una posible _race condition_ en la función
> `thread_set_priority ()` y explica cómo tu implementación la evita.
> Puedes utilizar un _lock_ para evitarla?
Puede surgir una race condition si múltiples hilos intentan cambiar la prioridad del thread actual al mismo tiempo. 
Si múltiples hilos intentan modificar la misma estructura, sobre la cual gestionamos los threads de acuerdo a su prioridad,
puede surgir una race condition. 
En este caso, nuestra implementación gestiona dicho problema al deshabilitar temporalmente las interrupciones, sin embargo, 
si distintas llamadas a la función intentan deshabilitar las interrupciones, sólo una de ellas lo logrará por vez (la primera), 
las demás quedarán bloqueadas hasta que la primer llamada a la función termine su ejecución.

### RATIONALE

> B7: Por qué elegiste este diseño? En qué maneras es superior a otros
> diseños que hayas considerado?
Entre los motivos por los que decidimos emplear esta implementación en lugar de alguna otra
se encuentran que, el uso de prioridades y la ordenación de acuerdo a ellas, permite que los hilos 
más importantes se ejecuten primero, eso mejora la capacidad de pintos para responder a eventos críticos y garantiza 
que los recursos sean asignados de manera eficiente. Por otro lado la implementación elegida hace uso de un algoritmo de ordenamiento
estable y que es fácil de redactar en el código. 
Esta implementación es preferible sobre otras ya que al establecer prioridades sobre los hilos, podemos evitar
posibles conflictos de sincronización y evitamos recorrer listados o estructuras de hilos, de forma innecesaria, el
orden nos asegura un acceso optimo a los mismos y su correspondiente gestión.
