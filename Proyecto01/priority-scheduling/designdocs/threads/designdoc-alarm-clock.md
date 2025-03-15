# PROJECT 1: THREADS. DESIGN DOCUMENT

## EQUIPO
> Pon aquí los nombres y correos electrónicos de los integrantes de tu equipo.

<Gabriela_López_Diego> <gabriela_164@ciencias.unam.mx>
<Javier_Alejandro_Rivera_Zavala> <alejandro_rz@ciencias.unam.mx>

##  PRELIMINARES
> Si tienes algún comentario preliminar a tu entrega, notas para los ayudantes
> o extra créditos, por favor déjalos aquí. 
> Sobre el punto A1, no hicimos uso en realidad de muchas estructuras salvo un "atributo" para los hilos
> y una lista para guardar los procesos en espera.

> Por favor cita cualquier recurso offline u online que hayas consultado
> mientras preparabas tu entrega, otros que no sean la documentación de Pintos,
> las notas del curso, o el equipo de enseñanza
> Se le preguntó a Chatgpt sobre el funcionamiento de ciertas funciones.
> Algunos conceptos sobre C se estudiaron en los cursos de: <https://www.youtube.com/@SistemasOperativosCienciasUNAM>
> Guía de referencia de pintos: <https://jeason.gitbooks.io/pintos-reference-guide-sysu/content/>
## ALARM CLOCK

### ESTRUCTURAS DE DATOS

> A1: A cada declaración nueva o modificación de un `struct` o `struct member`, 
> variable global o estática, `typedef` o enumeración. Agrega comentarios en el
> código en el que describas su propósito en 25 palabras o menos.

R: Va directo en el código.

### ALGORITMOS

> A2: Describe brevemente qué sucede en una llamada a `timer_sleep ()`
> incluyendo los efectos del __interrupt_handler__ del __timer__.
>  En la linea int64_t start = timer_ticks (); registra el valor actual del temporizador para conocer el tiempo en ticks en que comenzará a dormir el hilo, 
> luego la linea ASSERT (intr_get_level () == INTR_ON); verifica que las interrupciones se encuentren activas puesto que timer_interrupt se basa 
> en las interrupciones para realizar un seguimiento del tiempo y "despertar" los hilos. Finalmente se llama a timer_wait con un argumento start + ticks
> para indicar cuanto tiempo debe de dormir el hilo antes de despertar.
> Cada vez que el timer genera una interrupción de temporizador, incrementa en 1 el número de ticks para efectuar un seguimiento del tiempo transcurrido,
> la linea thread_tick(); se emplea para realizar tareas de planificación del hilo y finalmente timer_wakeup(); se utiliza para verificar si ya es el 
> tiempo de que algún hilo en espera despierte. En resumen, el handler se encarga de actualizar el contador de "ticks" y de verificar si los hilos que estaban 
> esperando deben despertar.


> A3: Qué pasos son tomados para minimizar la cantidad de tiempo gastada
> por el __interrupt_handler__ del __timer__?
> El control de la situación dentro del interrupt handler es pasado a timer_wakeup dentro de timer.c, dicha función se encarga de recorrer la lista de
> hilos en espera (que se encuentra ordenada de menor a mayor según su tiempo de espera)y se encarga de verificar cuales de ellos ya han de "despertar" hasta que 
> encuentra uno que no, de tal forma no necesitamos que cada hilo en espera, verifique constantemente si ya es su momento de despertar, 
> esto haciendo uso de recursos y tiempo del procesador.

### SINCRONIZACIÓN

> A4: Cómo evitas __race conditions__ cuando multiples __threads__ llaman
> `timer_sleep ()` simultaneamente?
> Recordemos que nuestra implementación hace uso de una función auxiliar timer_wait, que almacena en una lista los hilos que llaman a timer_sleep, dicha función
> los ingresa de forma ordenada mediante list_insert_ordered, para después bloquearlos y que puedan permanecer dormidos hasta que es preciso 
> despertarlos. Anterior a ello, se apunta al hilo actual para que con él se opere, es entonces a través de dicha inserción ordenada en una lista (mediante la 
> comparación provista por timer_less) que se garantiza que los hilos no pretendan acceder exactamente a los mismos recursos cuando se manda a llamar timer_sleep, pues > su gestión se deja a una función externa. 
> Consideremos que en este punto se detuvieron las interrupciones a cada vez que se precisaba, como se explica en el punto siguiente.

> A5: Cómo evitas __race conditions__ cuando ocurre una interrupción del __timer__
> durante una llamada a `timer_sleep ()`?
> El control sobre tal situación es pasado a la función timer_wait dentro de timer.c, misma que gestiona las race conditions en la 
> situación planteada, esto se logra al incluir la instrucción enum intr_level old_level = intr_disable (); misma que desactiva temporalmente las interrupciones,
> para finalmente hacer uso de intr_set_level (old_level); que restaura el nivel de interrupción a su valor previo, una vez que ya puso en espera al hilo. Es 
> importante considerar que la función timer_ticks, detiene las interrupciones momentáneamente cuando es llamada desde timer_sleep
> para evitar conflictos cuando es llamada simultáneamente.


### ARGUMENTACIÓN

> A6: Por qué elegiste este diseño? En qué formas es superior a otros diseños 
> que hayas considerado?
> En primer instancia por qué se evita en lo posible el uso de estructuras de datos, structs, listas o cualquier otro componente que
> requiera de ser iterado adicionalmente, lo cuál suele traer un incremento en la complejidad de los algoritmos, sobre todo cuando las 
> dimensiones de dicha estructura se incrementan. Del mismo modo, como se mencionó en A3, evitamos que los hilos se mantengan activos verificando
> todo el tiempo si ya es su turno de desperar, al pasar el control de tal situación a la función timer_wakeup. El diseño elegido no sólo requiere de
> cambios más sutiles que mejoran la legibilidad del código, sino que como ya se explico, hace un uso más eficiente de los recursos del ordenador en su
> interacción con el sistema operativo, una cualidad que cuando nos referimos a cualquier software, pero sobre todo cuando se trata de aquel que precisamente
> gestiona los recursos de la computadora, se agradece muchisimo.
