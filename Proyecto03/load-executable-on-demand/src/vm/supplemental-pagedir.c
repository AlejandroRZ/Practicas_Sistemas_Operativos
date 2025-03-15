#include <debug.h>
#include "threads/vaddr.h"
#include "threads/malloc.h"
#include "vm/supplemental-pagedir.h"

static unsigned page_hash (const struct hash_elem *, void*);
static bool page_less (const struct hash_elem*, const struct hash_elem*, void*);

static unsigned page_hash(const struct hash_elem *p_, void *aux UNUSED)
{
  const spde_t *p = hash_entry(p_, struct supplemental_pagedir_entry, hash_elem);
  return hash_bytes(&p->upage, sizeof p->upage);
}

static bool page_less(const struct hash_elem *a_, const struct hash_elem *b_,
                      void *aux UNUSED)
{
  const spde_t *a = hash_entry(a_, struct supplemental_pagedir_entry, hash_elem);
  const spde_t *b = hash_entry(b_, struct supplemental_pagedir_entry, hash_elem);
  return a->upage < b->upage;
}

void spd_init(spd_t *spd)
{
  hash_init(&spd->table, page_hash, page_less, NULL);
}

void spd_destroy(spd_t *spd)
{
  hash_destroy(&spd->table, NULL);
}

bool spd_set_page(spd_t *spd, void *upage, bool writable, spde_location_t location, void *aux)
{
  spde_t *entry = malloc(sizeof(spde_t));
  if (entry == NULL)
    return false;

  entry->upage = upage;
  entry->location = location;
  entry->writable = writable;

  // Copia la información del archivo
  memcpy(&entry->file_location, aux, sizeof(spde_file_location_t));

  // Agrega la entrada a la tabla hash
  hash_insert(&spd->table, &entry->hash_elem);

  return true;
}

spde_t *spd_get_page(spd_t *spd, void *upage)
{
  struct supplemental_pagedir_entry target;
  target.upage = upage;

  struct hash_elem *e = hash_find(&spd->table, &target.hash_elem);

  return e != NULL ? hash_entry(e, struct supplemental_pagedir_entry, hash_elem) : NULL;
}

void spd_clear_page(spd_t *spd, void *upage)
{
  struct supplemental_pagedir_entry target;
  target.upage = upage;

  struct hash_elem *e = hash_delete(&spd->table, &target.hash_elem, NULL);
  if (e != NULL)
  {
    spde_t *entry = hash_entry(e, struct supplemental_pagedir_entry, hash_elem);
    free(entry);
  }
}
