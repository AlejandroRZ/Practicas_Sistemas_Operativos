# PROJECT 1: THREADS - MLFQ SCHEDULER. DESIGN DOCUMENT

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

## ADVANCED SCHEDULER

### ESTRUCTURAS DE DATOS

> B1: A cada declaración nueva o modificación de un `struct` o `struct member`,
 > variable global o estática, `typedef` o `enum`. Agrega comentarios en el
 > código en el que describas su propósito en 25 palabras o menos.
 
 R: Va directo en el código.

### ALGORITMOS

> C2: Dados los threads A, B y C con el valor nice de 0, 1 y 2 respectivamente.
> Cada uno con un valor recent_cpu de 0. Llena la tabla que se muestra a
> continuación con los valores de recent_cpu, prioridad y el thread que esta
> corriendo para cada tick de la primera columna.

timer  recent_cpu    prioridad  thread
ticks   A   B   C   A   B   C   corriendo
-----  --  --  --  --  --  --   ------
 0
 4
 8
12
16
20
24
28
32
36

> C3: ¿Alguna ambigüedad en la especificación del _scheduler_ hace que los valores en la tabla sean inciertos?
> En caso de ser así, ¿Qué regla usaste para resolver dichos
> valores? ¿Esta regla está implementada en tu solución?

> C4. En tu implementación, ¿Como afecta el rendimiento del sistema operativo
> el costo que agregan las operaciones del scheduler dentro y fuera del contexto
> de interrupción?

### ARGUMENTACIÓN

> C5: Brevemente critica tu diseño, añade ventajas y desventajas que
> tiene la solución que elegiste. Si tuvieras más tiempo para trabajar
> en el scheduler, ¿Que cambios harías para refinar y mejorar tu
> diseño?