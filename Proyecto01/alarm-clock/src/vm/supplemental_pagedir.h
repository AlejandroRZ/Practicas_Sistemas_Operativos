#ifndef VM_SPD_H
#define VM_SPD_H

#include <hash.h>
#include "devices/block.h"

typedef block_sector_t swap_sector_t;

struct supplemental_pagedir_entry_file_location 
  {
    int32_t fd;
    int offset;
    int read_bytes;
    int zero_bytes;
  };
typedef struct supplemental_pagedir_entry_file_location spde_file_location_t;

enum supplemental_pagedir_entry_location {SPDE_LOCATION_FILESYS, SPDE_LOCATION_SWAP};
typedef enum supplemental_pagedir_entry_location spde_location_t;

struct supplemental_pagedir_entry 
  {
    void* upage;                          /* Virtual address in swap */
    spde_location_t location;          
    bool writable;   
    swap_sector_t swap_slot;              /* which index in the swap this page is stored into */
    spde_file_location_t file_location;
    struct hash_elem hash_elem;           /* Hash table element. */
  };
typedef struct supplemental_pagedir_entry spde_t;

struct supplemental_pagedir 
  {
    struct hash table;
  };
typedef struct supplemental_pagedir spd_t;

void spd_init (spd_t* spd);
void spd_destroy (spd_t* spd);

bool spd_set_page (spd_t* spd, void* upage, bool writable, spde_location_t location, void* aux);
spde_t* spd_get_page (spd_t* spd, void* upage);
void spd_clear_page (spd_t* spd, void* upage);

#endif
