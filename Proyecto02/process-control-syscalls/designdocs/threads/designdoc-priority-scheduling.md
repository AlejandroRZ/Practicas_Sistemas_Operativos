# PROJECT 1: THREADS - PRIORITY SCHEDULER. DESIGN DOCUMENT

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

## PRIORITY SCHEDULING

### ESTRUCTURAS DE DATOS

> B1: A cada declaración nueva o modificación de un `struct` o `struct member`,
 > variable global o estática, `typedef` o `enum`. Agrega comentarios en el
 > código en el que describas su propósito en 25 palabras o menos.

 R: Va directo en el código.

### ALGORITMOS

> B3: (Punto Extra) Cómo aseguras que el _thread_ de más alta
> prioridad que está esperando en un _semaphore_, _lock_ o 
> _condition_ despierta primero?

> B4: Qué tan eficientes es tu estrategia para insertar y borrar
> elementos de la `ready_list`? Puedes imaginar una estructura de
> datos para representar la `ready_list` en la que las inserciones y 
> operaciones de borrado tomen __O(1)__?


### SINCRONIZACIÓN

> B6: Describe una posible _race condition_ en la función
> `thread_set_priority ()` y explica cómo tu implementación la evita.
> Puedes utilizar un _lock_ para evitarla?

### RATIONALE

> B7: Por qué elegiste este diseño? En qué maneras es superior a otros
> diseños que hayas considerado?
