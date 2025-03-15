# PROJECT 1: THREADS - MLFQ SCHEDULER. DESIGN DOCUMENT

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

## ADVANCED SCHEDULER

### ESTRUCTURAS DE DATOS

> B1: A cada declaración nueva o modificación de un `struct` o `struct member`,
 > variable global o estática, `typedef` o `enum`. Agrega comentarios en el
 > código en el que describas su propósito en 25 palabras o menos.
 
 R: Va directo en el código.
 
 Realizamos comentarios y cambios en los siguientes archivos
 -timer.c
  -synch.h
 -synch.c
 -thread.h
 -thread.c
 -fixpoint.h
 -fixpoint.c
 -mlfqs-calculations.h
 -mlfqs-calculations.c

### ALGORITMOS

> C2: Dados los threads A, B y C con el valor nice de 0, 1 y 2 respectivamente.
> Cada uno con un valor recent_cpu de 0. Llena la tabla que se muestra a
> continuación con los valores de recent_cpu, prioridad y el thread que esta
> corriendo para cada tick de la primera columna.

timer |   recent_cpu     |        prioridad       |  	thread
ticks  |  	A     B     C     |       A     B     C     |      corriendo
-----      --     --     --            --      --     --              ------------
 0         0      0     0            63     61    59                A
 4         4      0     0            62     61    59                A
 8         8      0     0            61     61    59                A
12        12    0     0            60     61    59                B
16        12    4     0            6       60    59                B
20        12    8     0            60     59    59	    A     
24        16    8     0            59     59    59                A
28        20    8     0            58     59    59	    B
32        20    12   0            58     58    59                C
36        20    12   4            58     58    58                C

> C3: ¿Alguna ambigüedad en la especificación del _scheduler_ hace que los valores en la tabla sean inciertos?
> En caso de ser así, ¿Qué regla usaste para resolver dichos
> valores? ¿Esta regla está implementada en tu solución?

Sí. Tanto en nuestra implementación como en las pruebas 'thread yielding' no ocurre después de que se haya cambiado la prioridad. Además esta prohibida en el contexto de interrupción. Por lo cual, hicimos que no ocurra un cambio automático de un hilo en ejecución a uno con una prioridad más alta a menos que se llame explícitamente en _thread_yield()_. 

> C4. En tu implementación, ¿Como afecta el rendimiento del sistema operativo
> el costo que agregan las operaciones del scheduler dentro y fuera del contexto
> de interrupción?

`Dentro del Contexto de Interrupción :`
Puesto que recent cpu se actualiza una vez cada tick para el hilo actual (excepto si es el idle_thread), existirá un costo adicional en términos de tiempo de CPU que el scheduler utiliza durante cada interrupción del temporizador, ya que según el número de hilos en ejecución, el recálculo consumirá más tiempo de forma directamente
proporcional a tal número de hilos.
La actualización de la prioridad y load average de forma global, una vez cada 4 ticks y una vez cada segundo (timer_ticks() % TIMER_FREQ == 0) respectivamente, se realiza con menos frecuencia, por lo que su impacto será en principio menor  durante cada interrupción generada por tick, ya que requieren de más de estos últimos. Sin embargo, el hecho de que la prioridad se actualice para cada hilo, tendrá un impacto considerable cada vez que ocurre, esto en el tiempo consumido durante el manejo de las interrupciones. 

`Fuera del Contexto de Interrupción :`
Como mencionamos antes (y como se especifico para la práctica), algunas operaciones se realizan con una mayor frecuencia que otras, algunas para hilos en concreto mientras que otras se efectúan para conjuntos de hilos. Dichas operaciones, fuera del tiempo durante el cuál detienen las interrupciones para evitar errores
de sincronización, consumen recursos del sistema operativo para así poder iterar y ordenar listas con las nuevas prioridades, así como recursos adicionales para buscar los datos necesarios dentro del sistema para hacer los recálculos. Los datos necesarios que mencionamos antes, incluyen la búsqueda del número total de hilos activos. Un aspecto en el que esta implementación tiene un impacto sobre el rendimiento del sistema operativo, es que puede volverlo menos eficiente en su reacción ante cambios. Al pausar la recepción de nuevas interrupciones y si no manejamos adecuadamente el tiempo invertido en los recálculos, el sistema tendrá que esperar bastante 
antes de recibir una nueva interrupción, afortunadamente, la implementación presente es consciente de ese riesgo y maneja los hilos de forma que permite la optimización del manejo de hilos e interrupciones. 
La optimización ocurre en tanto que la implementación del MLFQ_scheduling, se basa en el historial de operaciones del sistema operativo para darle prioridad a aquellos procesos que requieren de una pronta ejecución, sin por ello descuidar a aquellos menos relevantes. Otro aspecto favorable es que proporciona un equilibrio entre procesos de alta y baja prioridad, lo cuál evita que los procesos de baja prioridad sean llevados a la inanición, por procesos de alta prioridad.

### ARGUMENTACIÓN

> C5: Brevemente critica tu diseño, añade ventajas y desventajas que
> tiene la solución que elegiste. Si tuvieras más tiempo para trabajar
> en el scheduler, ¿Que cambios harías para refinar y mejorar tu
> diseño?

DISEÑO: 
La implementación de nuestro Multi-level Feedback Queue (MLFQ) sigue los siguientes puntos:
*Atributos de hilo: Cada hilo tiene atributos como nice y recent_cpu que influyen en su prioridad.
*Cálculo de la carga promedio: Se mantiene la variable global load_avg que estima el número promedio de hilos listos para ejecutar en el último minuto.
*Cálculo de recent_cpu: recent_cpu es una media móvil ponderada exponencialmente de nice. Se actualiza con cada interrupción de temporizador y se recalcula cada segundo.
*Cálculo de prioridad: La prioridad de un hilo se calcula en función de recent_cpu y nice. 
*Planificación: Se llama a thread_yield() cuando hay hilos listos con prioridades más altas que el hilo actual.
*Sincronización: Para evitar conflictos, se deshabilitan las interrupciones al cambiar y utilizar variables globales como load_avg.
*Cálculos en punto fijo: Los cálculos en punto fijo se gestionan en fixpoint.h.

_VENTAJAS_

*Simplicidad: El diseño es relativamente simple y fácil de entender. No implica complicadas políticas de prioridad dinámica o donación de prioridad.
*Uso eficiente de recursos: Al calcular la prioridad y el tiempo de CPU reciente en función de fórmulas matemáticas simples. Nuestro diseño utiliza de manera eficiente los recursos de la CPU sin la necesidad de operaciones costosas.
*Control de carga equilibrado: El uso de 'load_avg' para estimar la cantidad promedio de hilos listos para ejecutar permite un control de carga equilibrado. 

_DESVENTAJAS_

*Falta de prioridad dinámica: Nuestro diseño no permite una prioridad dinámica ajustada en función del comportamiento del hilo ya que la prioridad esta determinada principalmente por 'nice', lo que no podría ser flexible. 
*Limitaciones en la gestión de la congestión: Aunque 'load_avg' la utilizamos para estimar la carga del sistema, el diseño no incluye mecanismos avanzados para gestionar la congestión cuando hay una alta demanda en el CPU. 
*Ineficiencia en casos extremos: En situaciones donde la cantidad de hilos listos es muy baja o alta, las fórmulas utilizadas no podrían calcular de forma exacta la situación real, y por ende, causar ineficiencias en la asignación de CPU. 

