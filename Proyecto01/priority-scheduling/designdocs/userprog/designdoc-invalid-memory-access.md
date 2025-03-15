# PROJECT 2: USER PROGRAMS. PROCESS MANAGEMENT SYSTEM CALLS

## EQUIPO
> Pon aquí los nombres y correos electrónicos de los integrantes de tu equipo.

<Nombre_Completo> <email@domain.example>
<Nombre_Completo> <email@domain.example>

##  PRELIMINARES
> Si tienes algún comentario preliminar a tu entrega, notas para los ayudantes o extra créditos, por favor déjalos aquí.

> Por favor cita cualquier recurso offline u online que hayas consultado mientras preparabas tu entrega, otros que no sean la documentación de Pintos, las notas del curso, o el equipo de enseñanza

## Invalid Memory Access

### ESTRUCTURAS DE DATOS

> B1: A cada declaración nueva o modificación de un `struct` o `struct member`, variable global o estática, `typedef` o `enum`. Agrega comentarios en el código en el que describas su propósito en 25 palabras o menos.

 R: Va directo en el código.

### ALGORITMOS

> B3: Describe el código encargado de leer y escribir datos de usuario desde el kernel.

> B4: Supón que una __system call__ causa que una página completa (4096 bytes) de datos se copien del espacio de usuario al __kernel__. ¿Cuál es el número mínimo y máximo de inspecciones de la __page table__ (e.g llamadas a `pagedir_get_page()`) que pudieran resultar? ¿Qué ocurre cuando una llamada a sistema copia solo 2 bytes de datos? ¿Existe margen de mejora para estos números, y por cuánto? 


> B6: Cualquier acceso a una dirección de memoria especificada por el usuario en un proceso del usuario podría fallar si la dirección es incorrecta, en cuyo caso se debería terminar el proceso. Estos accesos podrían ocurrir en cualquier momento durante una __system call__, por ejemplo la __system call__ `write` requiere leer el id del stack del usuario, seguido de cada uno de los tres argumentos de la llamada y de una cantidad arbitraria de memoria del usuario, y cada uno de estos accesos podría fallar. Lo anterior supone un desafío de diseño y de manejo de errores. ¿Cómo se evita obscurecer la función principal del código, sin dejar de manejar los errores de acceso a memoria? Además, si un error es detectado, ¿Cómo se se garantiza que se liberen todos los recursos que se habían asignado temporalmente en la __system call__ (locks, buffers, etc.)? Describe las estrategias que se utilizaron para resolver estos problemas. Da un ejemplo.

### SYNCHRONIZATION

> No hay preguntas en esta práctica.

### RATIONALE

> B9: ¿Por qué elegiste implementar el acceso a la memoria del usuario desde el __kernel__ de la forma en que lo hiciste?