#include <stdio.h>
#include "userprog/filesys-syscall.h"
#include "userprog/process.h"
#include "threads/synch.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "devices/input.h"
#include "threads/thread.h"
#include "threads/malloc.h"

static struct rlock mutex;

static int32_t add_open_file_to_thread (struct file*);
static struct process_open_file* find_open_file (int32_t file_descriptor);
static void remove_open_file_from_thread (struct process_open_file* open_file);

void filesys_syscall_init (void) 
{
  rlock_init (&mutex);
}

bool 
filesys_syscall_create (const char* file_name, unsigned initial_size)
{
  if (!file_name) {
    process_fail ();
  }
  bool success = false;
  rlock_acquire (&mutex);
  success = filesys_create (file_name, initial_size);
  rlock_release (&mutex);
  return success;
}

bool 
filesys_syscall_remove (const char* file_name)
{
  if (!file_name) {
    process_fail ();
  }
  bool success = false;
  rlock_acquire (&mutex);
  success = filesys_remove (file_name);
  rlock_release (&mutex);
  return success;
}

int32_t 
filesys_syscall_open (const char* file_name)
{
  if (!file_name) {
    process_fail ();
  }
  struct file* file;
  rlock_acquire (&mutex);
  file = filesys_open (file_name);
  rlock_release (&mutex);

  if (!file) {
    return -1;
  }

  int32_t fd = add_open_file_to_thread (file);
  if (fd == -1) {
    rlock_acquire (&mutex);
    file_close (file);
    rlock_release (&mutex);
  }
  return fd;
}


void 
filesys_syscall_close (int32_t file_descriptor)
{
  if (file_descriptor < 2) {
    return;
  }
  struct process_open_file* open_file = find_open_file (file_descriptor);
  if (open_file) {
    remove_open_file_from_thread (open_file);
  }
}

void 
filesys_syscall_close_all (void)
{
  struct process* p = thread_current ()->self;
  while (!list_empty (&p->open_files)) {
    struct process_open_file* file = list_entry (list_begin (&p->open_files), struct process_open_file, elem);
    remove_open_file_from_thread (file);
  }
}

int32_t 
filesys_syscall_write (int32_t file_descriptor, const void* buffer, unsigned size)
{
  if (!buffer) {
    process_fail ();
  }
  if (file_descriptor == STDOUT_FILENO) {
    putbuf (buffer, size);
    return size;
  }
  struct process_open_file* open_file = find_open_file (file_descriptor);
  if (!open_file) {
    return 0;
  }
  int32_t writen_bytes;
  rlock_acquire (&mutex);
  writen_bytes = file_write (open_file->file_handle, buffer, size);
  rlock_release (&mutex);
  return writen_bytes;
}

int32_t 
filesys_syscall_read (int32_t file_descriptor, void* buffer, unsigned size)
{
  if (!buffer) {
    process_fail ();
  }
  if (file_descriptor == STDIN_FILENO) {
    uint8_t* buffer_copy = buffer;
    for (unsigned i = 0; i < size; i++) {
      buffer_copy[i] = input_getc();
    }
    return size;
  }
  struct process_open_file* open_file = find_open_file (file_descriptor);
  if (!open_file) {
    return 0;
  }
  int32_t read_bytes;
  rlock_acquire (&mutex);
  read_bytes = file_read (open_file->file_handle, buffer, size);
  rlock_release (&mutex);
  return read_bytes;
}

int32_t 
filesys_syscall_size (int32_t file_descriptor)
{
  struct process_open_file* open_file = find_open_file (file_descriptor);
  if (!open_file) {
    return 0;
  }
  int32_t file_size;
  // rlock_acquire (&mutex);
  file_size = file_length (open_file->file_handle);
  // rlock_release (&mutex);
  return file_size;
}

void 
filesys_syscall_seek (int32_t file_descriptor, unsigned position)
{
  struct process_open_file* open_file = find_open_file (file_descriptor);
  if (!open_file) {
    return;
  }
  // rlock_acquire (&mutex);
  file_seek (open_file->file_handle, position);
  // rlock_release (&mutex);
}

unsigned 
filesys_syscall_tell (int32_t file_descriptor)
{
  struct process_open_file* open_file = find_open_file (file_descriptor);
  if (!open_file) {
    return 0;
  }
  unsigned position;
  // rlock_acquire (&mutex);
  position = file_tell (open_file->file_handle);
  // rlock_release (&mutex);
  return position;
}

void
filesys_syscall_deny_write (int32_t file_descriptor)
{
  struct process_open_file* open_file = find_open_file (file_descriptor);
  if (!open_file) {
    return;
  }
  rlock_acquire (&mutex);
  file_deny_write (open_file->file_handle);
  rlock_release (&mutex);
}

struct file*
filesys_syscall_get_file_handle (int32_t file_descriptor)
{
  struct process_open_file* open_file = find_open_file (file_descriptor);
  if (!open_file) {
    return NULL;
  }
  return open_file->file_handle;
}

int32_t 
filesys_syscall_locate_fd (const struct file* file)
{
  return -1;
}

// Auxiliary Functions

static int32_t 
add_open_file_to_thread (struct file* file)
{
  struct process* p = thread_current ()->self;
  struct process_open_file* open_file = (struct process_open_file*) malloc (sizeof (struct process_open_file));
  if (!open_file) {
    return -1;
  }
  open_file->file_handle = file;
  open_file->file_descriptor = p->next_file_descriptor++;
  list_push_back (&p->open_files, &open_file->elem);
  return open_file->file_descriptor;
}

static struct process_open_file* 
find_open_file (int32_t file_descriptor)
{
  struct process* p = thread_current ()->self;
  struct list_elem* e;
  for (
    e = list_begin (&p->open_files); 
    e != list_end (&p->open_files);
    e = list_next (e)
  ) {
    struct process_open_file* open_file = list_entry (e, struct process_open_file, elem);
    if (file_descriptor == open_file->file_descriptor) {
      return open_file;
    }
  }
  return NULL;
}

static void 
remove_open_file_from_thread (struct process_open_file* open_file)
{
  rlock_acquire (&mutex);
  file_close (open_file->file_handle);
  rlock_release (&mutex);
  list_remove (&open_file->elem);
  free (open_file);
}