#include <debug.h>
#include "threads/vaddr.h"
#include "threads/malloc.h"
#include "vm/supplemental_pagedir.h"

static unsigned page_hash (const struct hash_elem *, void*);
static bool page_less (const struct hash_elem*, const struct hash_elem*, void*);

void 
spd_init (spd_t* spd UNUSED)
{
}

void
spd_destroy (spd_t* spd UNUSED)
{
}

bool
spd_set_page (spd_t* spd, void* upage UNUSED,  bool writable UNUSED, spde_location_t location UNUSED, void* aux UNUSED)
{
  return false;
}

spde_t* 
spd_get_page (spd_t* spd UNUSED, void* upage UNUSED)
{
  return NULL;
}

void 
spd_clear_page (spd_t* spd UNUSED, void* upage UNUSED)
{
}

static unsigned
page_hash (const struct hash_elem* p_, void* aux UNUSED)
{
  const spde_t* p = hash_entry (p_, struct supplemental_pagedir_entry, hash_elem);
  return hash_bytes (&p->upage, sizeof p->upage);
}

static bool
page_less (const struct hash_elem* a_, const struct hash_elem* b_,
        void* aux UNUSED)
{
  const spde_t* a = hash_entry (a_, struct supplemental_pagedir_entry, hash_elem);
  const spde_t* b = hash_entry (b_, struct supplemental_pagedir_entry, hash_elem);
  return a->upage < b->upage;
}
