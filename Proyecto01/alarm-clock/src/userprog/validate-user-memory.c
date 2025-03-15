#include <stdio.h>
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/process.h"
#include "userprog/validate-user-memory.h"

static int get_user (const uint8_t *uaddr);
static bool put_user (uint8_t *udst, uint8_t byte);
static int32_t get_user_int (const uint32_t *uaddr);

void
verify_str_is_valid (const uint8_t* uaddr UNUSED) {
  // TODO: implement other missing validations
}

void 
verify_buffer_is_valid (uint8_t* uaddr UNUSED, unsigned size UNUSED, bool writable UNUSED)
{
  // TODO: implement other missing validations
}

int8_t
get_user_byte_or_fail (const uint8_t *uaddr)
{
  // TODO: implement other missing validations
  return get_user (uaddr);
}

void 
put_user_byte_or_fail (uint8_t *udst, uint8_t byte)
{
  // TODO: implement other missing validations
  put_user (udst, byte);
}

int32_t
get_user_int_or_fail (const uint32_t* uaddr)
{
  // TODO: implement other missing validations
  return get_user_int (uaddr);
}
 
/* Reads a byte at user virtual address UADDR.
   UADDR must be below PHYS_BASE.
   Returns the byte value if successful, -1 if a segfault
   occurred. */
static int
get_user (const uint8_t *uaddr)
{
  int result;
  asm ("movl $1f, %0; movzbl %1, %0; 1:"
       : "=&a" (result) : "m" (*uaddr));
  return result;
}
 
/* Writes BYTE to user address UDST.
   UDST must be below PHYS_BASE.
   Returns true if successful, false if a segfault occurred. */
static bool
put_user (uint8_t *udst, uint8_t byte)
{
  int error_code;
  asm ("movl $1f, %0; movb %b2, %1; 1:"
       : "=&a" (error_code), "=m" (*udst) : "q" (byte));
  return error_code != -1;
}

/* Reads a 32-bit integer at user virtual address UADDR.
   UADDR must be below PHYS_BASE.
   Returns the i32-bit integer value if successful, -1 if a segfault
   occurred. */
static int32_t
get_user_int (const uint32_t *uaddr)
{
  int32_t result;
  asm ("movl $1f, %0; movl %1, %0; 1:"
       : "=&a" (result) : "m" (*uaddr));
  return result;
}