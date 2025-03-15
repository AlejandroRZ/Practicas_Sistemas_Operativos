#include <stdio.h>
#include <stdlib.h>
#include "connect4.h"
#include "test_helpers.h"

void 
print_board (connect4* game)
{
    char* board = connect4_to_string (game);
    printf ("%s\n", board);
}