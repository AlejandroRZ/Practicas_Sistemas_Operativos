#include <stdio.h>
#include "userprog/filesys-syscall.h"
#include "threads/synch.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "devices/input.h"
#include "threads/thread.h"
#include "threads/malloc.h"

void filesys_syscall_init (void) 
{
}

bool 
filesys_syscall_create (const char* file_name UNUSED, unsigned initial_size UNUSED)
{
  return false;
}

bool 
filesys_syscall_remove (const char* file_name UNUSED)
{
  return false;
}

int32_t 
filesys_syscall_open (const char* file_name UNUSED)
{
  return -1;
}


void 
filesys_syscall_close (int32_t file_descriptor UNUSED)
{
}

void 
filesys_syscall_close_all (void)
{
}

int32_t 
filesys_syscall_write (int32_t file_descriptor UNUSED, const void* buffer UNUSED, unsigned size UNUSED)
{
  // TODO: do something special with STDOUT_FILENO
  return 0;
}

int32_t 
filesys_syscall_read (int32_t file_descriptor UNUSED, void* buffer UNUSED, unsigned size UNUSED)
{
  // TODO: do something special with STDIN_FILENO
  return 0;
}

int32_t 
filesys_syscall_size (int32_t file_descriptor UNUSED)
{
  return 0;
}

void 
filesys_syscall_seek (int32_t file_descriptor UNUSED, unsigned position UNUSED)
{
}

unsigned 
filesys_syscall_tell (int32_t file_descriptor UNUSED)
{
  return 0;
}

void
filesys_syscall_deny_write (int32_t file_descriptor UNUSED)
{
}

struct file*
filesys_syscall_get_file_handle (int32_t file_descriptor UNUSED)
{
  return NULL;
}

int32_t 
filesys_syscall_locate_fd (const struct file* file UNUSED)
{
  return -1;
}
