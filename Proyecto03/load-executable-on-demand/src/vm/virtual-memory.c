#include <string.h>
#include <stdio.h>
#include "threads/thread.h"
#include "threads/synch.h"
#include "threads/vaddr.h"
#include "threads/palloc.h"
#include "userprog/pagedir.h"
#include "vm/supplemental-pagedir.h"
#include "vm/virtual-memory.h"
#include "userprog/filesys-syscall.h"


void
vm_remove_frame_from_owner (void* frame_table_entry UNUSED)
{
  // TODO: resolver en práctica de swapping
}

bool
vm_load_frame_if_present_in_spd(void* fault_addr)
{
  
  void *page_start = pg_round_down(fault_addr);

  spde_t *spde = spd_get_page(&current_process->spd, page_start);
  if (spde == NULL)
    return false;
  
  uint8_t *kpage = palloc_get_page(PAL_USER);
  if (kpage == NULL)
    return false;

  file_seek(spde->file_location.fd, spde->file_location.offset);
  if (file_read_at(spde->file_location.fd, kpage, spde->file_location.read_bytes, spde->file_location.offset) != (int)spde->file_location.read_bytes)
  {
    palloc_free_page(kpage);
    return false;
  }
  memset(kpage + spde->file_location.read_bytes, 0, spde->file_location.zero_bytes);

  if (!spde->writable)
    spd_clear_page(&current_process->spd, page_start);
  
  if (!pagedir_set_page(current_process->pagedir, page_start, kpage, spde->writable))
  {
    palloc_free_page(kpage);
    return false;
  }
  
  return true;
}
