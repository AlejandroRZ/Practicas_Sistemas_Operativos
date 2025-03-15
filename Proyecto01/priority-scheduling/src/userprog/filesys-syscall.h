#ifndef USERPROG_FILESYS_SYSCALL_H
#define USERPROG_FILESYS_SYSCALL_H

#include <stdlib.h>
#include <stdbool.h>
#include <list.h>
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "threads/synch.h"

struct process_open_file 
  {
    int32_t file_descriptor;
    struct file* file_handle;
    struct list_elem elem;
  };

void filesys_syscall_init (void);
bool filesys_syscall_create (const char* file_name, unsigned initial_size);
bool filesys_syscall_remove (const char* file_name);
int32_t filesys_syscall_open (const char* file_name);
void filesys_syscall_close (int32_t file_descriptor);
void filesys_syscall_close_all (void);
int32_t filesys_syscall_write (int32_t file_descriptor, const void* buffer, unsigned size);
int32_t filesys_syscall_read (int32_t file_descriptor, void* buffer, unsigned size);
int32_t filesys_syscall_size (int32_t file_descriptor);
void filesys_syscall_deny_write (int32_t file_descriptor);
void filesys_syscall_seek (int32_t file_descriptor, unsigned position);
unsigned filesys_syscall_tell (int32_t file_descriptor);
struct file* filesys_syscall_get_file_handle (int32_t file_descriptor);
int32_t filesys_syscall_locate_fd (const struct file* file);

#endif /* userprog/filesys-syscall.h */
