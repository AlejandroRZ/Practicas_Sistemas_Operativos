#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "userprog/filesys-syscall.h"
#include "userprog/validate-user-memory.h"

static void syscall_handler (struct intr_frame *);
static int32_t write_wrapper (int32_t *esp);
static void exit_wrapper (int32_t *esp);
static void exit (int exit_status);
static tid_t exec_wrapper (int32_t* esp);
static tid_t exec (char* cmd_line);
static int32_t wait_wrapper (int32_t* esp);
static int32_t wait (int32_t pid);
static bool file_create_wrapper (int32_t* esp);
static bool file_remove_wrapper (int32_t* esp);
static int32_t file_open_wrapper (int32_t* esp);
static void file_close_wrapper (int32_t* esp);
static int32_t read_wrapper (int32_t* esp);
static int32_t file_size_wrapper (int32_t* esp);
static void file_seek_wrapper (int32_t* esp);
static unsigned file_tell_wrapper (int32_t* esp);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  filesys_syscall_init ();
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  int32_t* esp = f->esp;
  int32_t syscall = get_user_int_or_fail ((uint32_t*) esp++);
  
  switch(syscall) {
    case SYS_WRITE: {
      f->eax = write_wrapper (esp);
      break;
    }
    case SYS_READ: {
      f->eax = read_wrapper (esp);
      break;
    }
    case SYS_CREATE: {
      f->eax = file_create_wrapper (esp);
      break;
    }
    case SYS_REMOVE: {
      f->eax = file_remove_wrapper (esp);
      break;
    }
    case SYS_OPEN: {
      f->eax = file_open_wrapper (esp);
      break;
    }
    case SYS_CLOSE: {
      file_close_wrapper (esp);
      break;
    }
    case SYS_FILESIZE: {
      f->eax = file_size_wrapper (esp);
      break;
    }
    case SYS_SEEK: {
      file_seek_wrapper (esp);
      break;
    }
    case SYS_TELL: {
      f->eax = file_tell_wrapper (esp);
      break;
    }
    case SYS_EXIT: {
      exit_wrapper (esp);
      break;
    }
    case SYS_WAIT: {
      f->eax = wait_wrapper (esp);
      break;
    }
    case SYS_EXEC: {
      f->eax = exec_wrapper (esp);
      break;
    }
    default: {
      printf ("unsupported syscall\n");
      process_fail ();
    }
  }
}

/* Write Syscall Implementation */
static int32_t 
write_wrapper (int32_t *esp) {
  int file_descriptor = get_user_int_or_fail ((uint32_t*) esp++);
  char *buffer = (char*) get_user_int_or_fail ((uint32_t*) esp++);
  unsigned size = (unsigned) get_user_int_or_fail ((uint32_t*) esp++);
  return filesys_syscall_write (file_descriptor, buffer, size);
}

static int32_t
read_wrapper (int32_t *esp) {
  int file_descriptor = get_user_int_or_fail ((uint32_t*) esp++);
  char *buffer = (char*) get_user_int_or_fail ((uint32_t*) esp++);
  unsigned size = (unsigned) get_user_int_or_fail ((uint32_t*) esp++);
  return filesys_syscall_read (file_descriptor, buffer, size);
}

/* Write Syscall Implementation */
static void 
exit_wrapper (int32_t *esp) {
  int exit_status = get_user_int_or_fail ((uint32_t*) esp);
  exit (exit_status);
}

static void 
exit (int exit_status) {
  process_set_exit_status (exit_status);
  thread_exit ();
}

static tid_t
exec_wrapper (int32_t* esp) 
{
  char* cmd = (char*) get_user_int_or_fail ((uint32_t*) esp);
  return exec(cmd);
}

static tid_t 
exec (char* cmd_line)
{
  return process_execute(cmd_line);
}

static int32_t
wait_wrapper (int32_t* esp) 
{
  int pid = get_user_int_or_fail ((uint32_t*) esp);
  return wait(pid);
}

static int32_t 
wait (int32_t pid)
{
  return process_wait(pid);
}

static bool 
file_create_wrapper (int32_t* esp)
{
  char* file_name = (char*) *esp++;
  unsigned initial_size = (unsigned) *esp;
  return filesys_syscall_create (file_name, initial_size);
}

static bool 
file_remove_wrapper (int32_t* esp)
{
  char* file_name = (char*) *esp++;
  return filesys_syscall_remove (file_name);
}

static int32_t 
file_open_wrapper (int32_t* esp)
{
  char* file_name = (char*) *esp++;
  return filesys_syscall_open (file_name);
}

static void 
file_close_wrapper (int32_t* esp)
{
  int32_t fd = *esp;
  filesys_syscall_close (fd);
}

static int32_t 
file_size_wrapper (int32_t* esp)
{
  int32_t fd = *esp;
  return filesys_syscall_size (fd);
}

static void 
file_seek_wrapper (int32_t* esp)
{
  int32_t fd = *esp++;
  unsigned position = *esp++;
  filesys_syscall_seek (fd, position);
}

static unsigned 
file_tell_wrapper (int32_t* esp)
{
  int32_t fd = *esp;
  return filesys_syscall_tell (fd);
}
