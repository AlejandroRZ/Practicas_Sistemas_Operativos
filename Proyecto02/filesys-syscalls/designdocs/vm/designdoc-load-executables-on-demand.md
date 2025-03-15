# PROJECT 3: VIRTUAL MEMORY. LOAD EXECUTABLES ON DEMAND

## EQUIPO
> Pon aquí los nombres y correos electrónicos de los integrantes de tu equipo.

<Nombre_Completo> <email@domain.example>
<Nombre_Completo> <email@domain.example>

##  PRELIMINARES
> Si tienes algún comentario preliminar a tu entrega, notas para los ayudantes o extra créditos, por favor déjalos aquí.

> Por favor cita cualquier recurso offline u online que hayas consultado mientras preparabas tu entrega, otros que no sean la documentación de Pintos, las notas del curso, o el equipo de enseñanza

## PAGE TABLE MANAGEMENT

### ESTRUCTURAS DE DATOS

> B1: A cada declaración nueva o modificación de un `struct` o `struct member`, variable global o estática, `typedef` o `enum`. Agrega comentarios en el código en el que describas su propósito en 25 palabras o menos.

 R: Va directo en el código.

### ALGORITMOS

> A2: En pocos parrafos, describe tu código para acceder a los datos de una página dada almacenada en la STP (supplemental page table).

En el momento en que un programa intenta leer una dirección que no es encuentra en el pagedir, se llama la interrupción llamada `execption`. Si la dirección que se intento leer es una dirección de usuario y además esta presente en el `supplemental_page_dir`, entonces se carga la página en memoria y se continua el proceso en donde se quedo.

Para cargar una página en memoria se hace lo siguiente:
1. Se verifca que esta este en el `supplemental_page_dir` (en caso de no ser así se termina el proceso de usuario).
2. Se obtiene una nueva página usando `palloc_get_page`
3. Se copia el contenido de la nueva página en la paǵina usando `filesys_syscall_read` y se rellena de `0` en caso de que el contenido del archivo no cubra todo el tamaño de la nueva página.

### SYNCHRONIZATION

> No hay problemas de soncronización que resolver en esta práctica.

### RATIONALE

> A5: Por qué elegiste la(s) estructura(s) de datos que seleccionaste para representar los mapeos de memoria virtual a memoria física?

Se utilizo un `hash` para representar los elementos de la `supplemental page table`, de esta manera se obtienen operaciones de `find` O(1) y `remove`O(1) que tiene un mejor desempeño que una lista en donde la operación `find` toma O(n).