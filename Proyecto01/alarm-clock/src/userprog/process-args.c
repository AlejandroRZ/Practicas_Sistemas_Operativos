#include "userprog/process-args.h"
#include "threads/vaddr.h"

void* 
process_args_put_in_stack (char* cmd_line, int tokens)
{
  char* esp_c = PHYS_BASE - 1;
  *esp_c = 0;
  esp_c--;
  
  int index;
  /* Put the args in the top of the stack in double array of char format. */
  for(index = tokens - 1; index >= 0; index--) {
    /* If there are extra end of string chars, ignore them. */
    if(cmd_line[index] != 0 || cmd_line[index + 1] != 0) {
      *esp_c = cmd_line[index];
      esp_c--;
    }
  }
  
  char* args_first = esp_c;
  
  *((uint8_t*)esp_c) = 0;
  uint32_t* esp_p = (uint32_t*)esp_c;
  esp_p--;
  *esp_p = 0;
  esp_p--;
  
  char* args_last;
  int size_args = 0;
  /* Put the pointers */
  for(args_last = PHYS_BASE - 2; args_last >= args_first; args_last--) {
    if(*args_last == 0) {
      *esp_p = (uint32_t)args_last + 1;
      size_args++;
      esp_p--;
    }
  }
  
  *esp_p = (uint32_t)(esp_p + 1);
  esp_p--;

  /* Put the argc */
  *esp_p = (uint32_t)size_args;
  esp_p--;
  *esp_p = 0;
  
  return (void*)esp_p;
}