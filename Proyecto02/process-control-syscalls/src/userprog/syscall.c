#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/malloc.h"
#include "devices/shutdown.h"
#include "devices/input.h"
#include "userprog/filesys-syscall.h"
#include "userprog/validate-user-memory.h"
#include "process.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "threads/synch.h"


static void syscall_handler (struct intr_frame *);
static int32_t write_wrapper(void *esp);
static int32_t write (int32_t fd, const void *buffer_, unsigned size);
static int32_t wait (tid_t pid);
static int32_t wait_wrapper(int32_t *esp);
static int32_t exec (const char *cmdline);
static int32_t exec_wrapper(int32_t *esp);
static void exit (int status);
static void exit_wrapper(void *esp);

void
syscall_init (void)
{
    intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
    lock_init(&file_lock);
}


/* System Call Handler:
This function is responsible for handling various system calls. It extracts the
system call number from intr_frame, determines the specific system call,
and then invokes the corresponding wrapper function to handle the syscall, passing
the appropriate arguments. If the syscall is not recognized, it prints a message
and exits the thread.*/

static void
syscall_handler (struct intr_frame *f )
{
    void *esp = f -> esp;
    int32_t syscall = get_user_int_or_fail (esp);
    esp+=4;    
    switch(syscall)
    {    
    case SYS_EXIT:                   
        exit_wrapper(esp);
        break;
    case SYS_EXEC:                   
        f->eax = exec_wrapper((int32_t*)esp);
        break;
    case SYS_WAIT:                   
        f-> eax = wait_wrapper((int32_t*)esp);
        break;    
    case SYS_WRITE:                 
        f->eax = write_wrapper(esp);
        break;   
    default:
        printf ("unsupported syscall\n");
        thread_exit ();
    }
}

/* Exit Wrapper:
This function serves as a wrapper for the exit system call. It takes the exit status
from the user stack and then calls the actual exit function, passing the status as an argument.*/

static void exit_wrapper(void *esp) {
    int status = *((int*)esp);
    exit(status);
}


/* Exit System Call:
This function is responsible for terminating the current thread. It updates the exit status
in the corresponding child element, marks the current status (killed or exited), and exits
the thread.*/
static void exit (int status)
{
    struct thread *cur = thread_current();
    printf ("%s: exit(%d)\n", cur -> name, status);
    
    struct child_element *child = get_child(cur->tid, &cur -> parent -> child_list); 
    child -> exit_status = status;
   
    if (status == -1)
    {
        child -> cur_status = WAS_KILLED;
    }
    else
    {
        child -> cur_status = HAD_EXITED;
    }
    thread_exit();
}

/* Exec Wrapper:
This function serves as a wrapper for the exec system call. It extracts the command
line argument from the user stack and then calls the actual exec function, passing
the argument as a parameter.*/
static int32_t exec_wrapper(int32_t *esp)
{
    int32_t argv = *(esp);
    esp += 4;
    return exec((const char *)argv);   
}

/* Exec System Call:
This function is responsible for executing a new program. It retrieves the command
line from the user space, initiates the execution using process_execute, and waits
for the child process to load successfully. Returns the process ID on success or -1 on failure.*/    
static int32_t exec (const char *cmd_line)
{
    struct thread* parent = thread_current();
    int32_t pid = -1;    
    pid = process_execute(cmd_line);
    
    struct child_element *child = get_child(pid,&parent -> child_list);
    sema_down(&child-> real_child -> sema_exec);    
    if(!child -> loaded_success)
    {
        
        return -1;
    }
    return pid;
}

// Wait Wrapper:
// This function serves as a wrapper for the wait system call. It extracts the process
// ID argument from the user stack and then calls the actual wait function, passing
// the argument as a parameter.
static int32_t wait_wrapper(int32_t *esp){
    tid_t pid = (tid_t)*((int32_t*)esp);
    return wait(pid);
}

/* Wait System Call:
This function is responsible for waiting until the specified child process terminates.
It calls process_wait with the given process ID and returns the exit status of the child.*/
static int32_t wait (tid_t pid)
{
    return process_wait(pid);
}

/* Write Wrapper:
This function serves as a wrapper for the write system call. It extracts the file
descriptor, buffer, and size arguments from the user stack and then calls the actual
write function, passing the arguments.*/
static int32_t write_wrapper(void *esp)
{
    int32_t arg = *((int32_t*) esp);
    esp += 4;
    int32_t arg1 = *((int32_t*) esp);
    esp += 4;
    int32_t arg2 = *((int32_t*) esp);
    esp += 4;     
    return write (arg,(void *) arg1,(unsigned) arg2);    
}

/* Write System Call:
This function is responsible for writing to either the console or a file,
depending on the specified file descriptor. It retrieves the file descriptor
element, checks for validity, and performs the write operation either to the
console using putbuf or to a file using file_write.*/
static int32_t write (int32_t fd, const void *buffer_, unsigned size)
{
    uint8_t * buffer = (uint8_t *) buffer_;
    int32_t ret = -1;
    if (fd == 1)
    {
        putbuf( (char *)buffer, size);
        return (int32_t)size;
    }
    else
    {        
        struct fd_element *fd_elem = get_fd(fd);
        if(fd_elem == NULL || buffer_ == NULL )
        {
            return -1;
        }        
        struct file *myfile = fd_elem->myfile;
        lock_acquire(&file_lock);
        ret = file_write(myfile, buffer_, size);
        lock_release(&file_lock);
    }
    return ret;
}

/* Get File Descriptor Element:
This function searches for a file descriptor element in the current thread's
file descriptor list based on the given file descriptor. It iterates through
the list and returns the file descriptor element if found; otherwise, it returns NULL.*/

struct fd_element*
get_fd(int fd)
{
    struct list_elem *e;
    for (e = list_begin (&thread_current()->fd_list); e != list_end (&thread_current()->fd_list);
            e = list_next (e))
    {
        struct fd_element *fd_elem = list_entry (e, struct fd_element, element);
        if(fd_elem->fd == fd)
        {
            return fd_elem;
        }
    }
    return NULL;
}



/* Get Child Element:
This function searches for a child element in the given list based on the
provided process ID. It iterates through the list and returns the child element
if found; otherwise, it returns NULL.*/

struct child_element*
get_child(tid_t tid, struct list *mylist)
{
    struct list_elem* e;
    for (e = list_begin (mylist); e != list_end (mylist); e = list_next (e))
    {
        struct child_element *child = list_entry (e, struct child_element, child_elem);
        if(child -> child_pid == tid)
        {
            return child;
        }
    }
}

/* Close All File Descriptors:
This function closes and frees all file descriptors associated with the
given file descriptor list. It iterates through the list, closes each file,
removes the corresponding list entry, and frees the associated memory.*/

void close_all(struct list *fd_list)
{
    struct list_elem *e;
    while(!list_empty(fd_list))
    {
        e = list_pop_front(fd_list);
        struct fd_element *fd_elem = list_entry (e, struct fd_element, element);
        file_close(fd_elem->myfile);
        list_remove(e);
        free(fd_elem);
    }
}