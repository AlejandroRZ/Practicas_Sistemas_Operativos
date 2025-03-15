#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "connect4.h"
#include "tests_to_string.h"
#include "tests_make_play.h"
#include "tests_player_won.h"

int main(int argc, char** argv) 
{
  if(argc != 3) {
    printf("Error\n");
  } else {
    if (strcmp ("to_string", argv[1]) == 0) {
      execute_to_string_tests (argv[2]);
    } else if ((strcmp ("make_play", argv[1]) == 0)) {
      execute_make_play_tests (argv[2]);
    } else if ((strcmp ("player_won", argv[1]) == 0)) {
      execute_player_won_test (argv[2]);
    } else {
      printf ("unknown test suite\n");
    }
  }    
  return EXIT_SUCCESS;
}