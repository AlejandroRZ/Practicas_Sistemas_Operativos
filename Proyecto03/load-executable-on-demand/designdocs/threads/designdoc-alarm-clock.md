# PROJECT 1: THREADS. DESIGN DOCUMENT

## EQUIPO
> Pon aquí los nombres y correos electrónicos de los integrantes de tu equipo.

<Nombre_Completo> <email@domain.example>
<Nombre_Completo> <email@domain.example>

##  PRELIMINARES
> Si tienes algún comentario preliminar a tu entrega, notas para los ayudantes
> o extra créditos, por favor déjalos aquí. 

> Por favor cita cualquier recurso offline u online que hayas consultado
> mientras preparabas tu entrega, otros que no sean la documentación de Pintos,
> las notas del curso, o el equipo de enseñanza

## ALARM CLOCK

### ESTRUCTURAS DE DATOS

> A1: A cada declaración nueva o modificación de un `struct` o `struct member`, 
> variable global o estática, `typedef` o enumeración. Agrega comentarios en el
> código en el que describas su propósito en 25 palabras o menos.

R: Va directo en el código.

### ALGORITMOS

> A2: Describe brevemente qué sucede en una llamada a `timer_sleep ()`
> incluyendo los efectos del __interrupt_handler__ del __timer__.

> A3: Qué pasos son tomados para minimizar la cantidad de tiempo gastada
> por el __interrupt_handler__ del __timer__?

### SINCRONIZACIÓN

> A4: Cómo evitas __race conditions__ cuando multiples __threads__ llaman
> `timer_sleep ()` simultaneamente?

> A5: Cómo evitas __race conditions__ cuando ocurre una interrupción del __timer__
> durante una llamada a `timer_sleep ()`?

### ARGUMENTACIÓN

> A6: Por qué elegiste este diseño? En qué formas es superior a otros diseños 
> que hayas considerado?
