#ifndef USERPROG_VALIDATE_MEMORY_H
#define USERPROG_VALIDATE_MEMORY_H

#include <stdlib.h>
#include <stdbool.h>

int8_t get_user_byte_or_fail (const uint8_t *uaddr);
int32_t get_user_int_or_fail (const uint32_t *uaddr);
void put_user_byte_or_fail (uint8_t *udst, uint8_t byte);
void verify_str_is_valid (const uint8_t* uaddr);
void verify_buffer_is_valid (uint8_t* uaddr, unsigned size, bool writable);

#endif /* userprog/validate-user-memory.h */
