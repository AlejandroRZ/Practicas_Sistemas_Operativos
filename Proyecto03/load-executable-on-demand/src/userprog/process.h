#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"
#include "threads/synch.h"
#include "vm/supplemental-pagedir.h"

typedef int tid_t;

struct process 
  {
    tid_t tid;
    int exit_status;
    struct list children;
    struct list_elem elem;
    struct semaphore wait;
    int32_t next_file_descriptor;
    struct list open_files;
    struct supplemental_pagedir spd; /*Suplemental pagedir*/
  };

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

void process_init (struct process*, tid_t);
void process_fail (void) NO_RETURN;
void process_set_exit_status (int);

#endif /* userprog/process.h */
