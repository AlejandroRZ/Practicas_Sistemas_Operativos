#include <string.h>
#include <stdio.h>
#include "threads/thread.h"
#include "threads/synch.h"
#include "threads/vaddr.h"
#include "threads/palloc.h"
#include "userprog/pagedir.h"
#include "vm/supplemental_pagedir.h"
#include "vm/virtual_memory.h"
#include "userprog/filesys-syscall.h"


void
vm_remove_frame_from_owner (void* frame_table_entry UNUSED)
{
  // TODO: resolver en práctica de swapping
}

bool
vm_load_frame_if_present_in_spd (void* fault_addr UNUSED)
{
  // TODO: resolver en la práctica de load executables on demand
  return false;
}