# PROJECT 3: VIRTUAL MEMORY. LOAD EXECUTABLES ON DEMAND

## EQUIPO
> Pon aquí los nombres y correos electrónicos de los integrantes de tu equipo.

<Gabriela_López_Diego> <gabriela_164@ciencias.unam.mx>
<Javier_Alejandro_Rivera_Zavala> <alejandro_riveraz@ciencias.unam.mx>

##  PRELIMINARES
> Si tienes algún comentario preliminar a tu entrega, notas para los ayudantes o extra créditos, por favor déjalos aquí.

No logramos concluir la práctica por falta de tiempo y sobrecarga de trabajo, esto último nos retrasó un poco con los temas abordados para esta práctica.
Si aún piensan descontarnos una práctica de la evaluación, como lo habían comentado, ojalá puedan descontarnos esta.

> Por favor cita cualquier recurso offline u online que hayas consultado mientras preparabas tu entrega, otros que no sean la documentación de Pintos, las notas del curso, o el equipo de enseñanza

## PAGE TABLE MANAGEMENT

### ESTRUCTURAS DE DATOS

> B1: A cada declaración nueva o modificación de un `struct` o `struct member`, variable global o estática, `typedef` o `enum`. Agrega comentarios en el código en el que describas su propósito en 25 palabras o menos.

 R: Va directo en el código.

### ALGORITMOS

> A2: En pocos parrafos, describe tu código para acceder a los datos de una página dada almacenada en la STP (supplemental page table).

En nuesto intento, el código utiliza la tabla de páginas suplementaria (STP) para rastrear y gestionar la carga de páginas de forma eficiente. La estructura spd_t almacena información importante sobre cada página, como la ubicación en el archivo, si soporta escritura y su posición en el espacio de direcciones virtuales.

La función load_segment se encarga de cargar las páginas iniciales de un archivo en la memoria, utilizando la STP para almacenar información clave y además la función vm_load_frame_if_present_in_spd se activa en caso de un fallo de página, utilizando la STP para recuperar y cargar la página necesaria.

#Esto ya estaba en el repo#

En el momento en que un programa intenta leer una dirección que no es encuentra en el pagedir, se llama la interrupción llamada `execption`. Si la dirección que se intento leer es una dirección de usuario y además esta presente en el `supplemental_page_dir`, entonces se carga la página en memoria y se continua el proceso en donde se quedo.

Para cargar una página en memoria se hace lo siguiente:
1. Se verifca que esta este en el `supplemental_page_dir` (en caso de no ser así se termina el proceso de usuario).
2. Se obtiene una nueva página usando `palloc_get_page`
3. Se copia el contenido de la nueva página en la paǵina usando `filesys_syscall_read` y se rellena de `0` en caso de que el contenido del archivo no cubra todo el tamaño de la nueva página.

### SYNCHRONIZATION

> No hay problemas de soncronización que resolver en esta práctica.

### RATIONALE

> A5: Por qué elegiste la(s) estructura(s) de datos que seleccionaste para representar los mapeos de memoria virtual a memoria física?
En nuestro intento la estructura para representar los mapeos de memoria virtual a memoria física es la provista por los ayudantes para crear una tabla de páginas suplementaria (STP o spd_t), que a su vez está implementada sobre una hashtable. Lo anterior nos permite una búsqueda y acceso eficientes a las entradas de la tabla mediante la dirección virtual de una página y la complejidad de búsqueda y acceso en una tabla hash suele ser constante en promedio.

Además la STP cuenta con otras ventajas usuales para las estructuras definidas sobre tablas hash y otras particulares de su definición: 
- Puede almacenar información adicional asociada con cada página, como la ubicación en el archivo, si permite escritura, entre otros detalles. 

- En caso de conflictos de páginas, como dos páginas intentando ocupar la misma dirección virtual, la estructura de tabla hash de la STP facilita la resolución de estos conflictos.

- La STP puede trabajar en conjunto con otras estructuras de datos, como la tabla de páginas del directorio (pagedir), para facilitar la gestión global de la memoria virtual.



#Esto ya estaba en el repo#
Se utilizo un `hash` para representar los elementos de la `supplemental page table`, de esta manera se obtienen operaciones de `find` O(1) y `remove`O(1) que tiene un mejor desempeño que una lista en donde la operación `find` toma O(n).