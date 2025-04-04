#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);
/*****here*****/
struct thread* process_get_child(struct thread* t, tid_t child_tid UNUSED);
/*****here*****/

#endif /* userprog/process.h */
