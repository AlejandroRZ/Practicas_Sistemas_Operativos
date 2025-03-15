#ifndef VM_VM_H
#define VM_VM_H

#include <stdbool.h>

void vm_remove_frame_from_owner (void* frame_table_entry);
bool vm_load_frame_if_present_in_spd (void* fault_addr);

#endif
