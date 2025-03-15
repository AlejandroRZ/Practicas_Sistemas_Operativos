#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include <stdbool.h>
#include "threads/thread.h"
#include <list.h>
#include "threads/synch.h"
/*****************************************Here**********************************/
struct lock file_lock;       /*lock for file access*/

/*
fd = file id
file = the file that we access
list_element = list for fd_element
*/
struct fd_element
{
    int fd;                        /***file id***/
    struct file *myfile;           /***file****/
    struct list_elem element;      /***list for fd_element***/
};
/*****************************************Here**********************************/
void syscall_init (void); //Inicializa el sistema de llamadas al sistema. 

/****************************************Here***********************************/
struct child_element* get_child(tid_t tid,struct list *mylist);  //Obtiene una referencia al hijo correspondiente en la lista de elem hijos.
void close_all(struct list * fd_list); //Cierra archivos
struct fd_element* get_fd(int fd); //Busca un elem en la lista de descriptores de archivo 
/****************************************Here***********************************/

#endif /* userprog/syscall.h */
