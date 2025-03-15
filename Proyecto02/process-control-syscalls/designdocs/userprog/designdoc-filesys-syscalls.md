# PROJECT 2: USER PROGRAMS. FILE SYSTEM, SYSTEM CALLS

## EQUIPO
> Pon aquí los nombres y correos electrónicos de los integrantes de tu equipo.

<Nombre_Completo> <email@domain.example>
<Nombre_Completo> <email@domain.example>

##  PRELIMINARES
> Si tienes algún comentario preliminar a tu entrega, notas para los ayudantes o extra créditos, por favor déjalos aquí.

> Por favor cita cualquier recurso offline u online que hayas consultado mientras preparabas tu entrega, otros que no sean la documentación de Pintos, las notas del curso, o el equipo de enseñanza

## SYSTEM CALLS

### ESTRUCTURAS DE DATOS

> C1: A cada declaración nueva o modificación de un `struct` o `struct member`,
> variable global o estática, `typedef` o `enum`. Agrega comentarios en el código
> en el que describas su propósito en 25 palabras o menos.

R: Va directo en el código.

> C2: Describe cómo asocias _file descriptors_ con archivos _open files_. En tu 
> implementación los _file descriptors_ son únicos dentro del _sistema operativo_ 
> completo, o solamente dentro de un proceso?


### ALGORITMOS

> No hay preguntas en esta práctica

### SYNCHRONIZATION

> C8: El acceso al _File System_ en pintos no es _thread-safe_, en consecuencia necesitas
> manejar la concurrencia cuando manipulas archivos, para este propósito puedes utilizar un
> _semaphore_ o un _lock_. Cuales operaciones deben de ser protegidas y cómo? Es un único 
> _semaphore_ o _lock_ global suficiente para resolver los problemas de concurrencia de esta
> tarea?


### RATIONALE

> C10: Qué ventajas o desventajas puedes ver en tu diseño de _file descriptors_? Menciona por lo menos
> dos alternativas de diseño y expón los _pros_ y _contras_ de cada una de ellas.
