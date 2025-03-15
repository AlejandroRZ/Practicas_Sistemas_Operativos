#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "connect4.h"
#include "test_helpers.h"
#include "tests_to_string.h"

void test_empty_board ();
void test_nonempty_board ();

void 
execute_to_string_tests (char* test_name) 
{
  if (strcmp ("empty_board", test_name) == 0) {
    test_empty_board ();
  } else if (strcmp ("nonempty_board", test_name) == 0) {
    test_nonempty_board ();
  } else {
    printf ("unknown to string test case\n");
    assert (false);
  }
}

void 
test_empty_board () 
{
  connect4 game; // está en el stack, en registro de activación de test_empty_board
  // Similar a  "connect4 game = new connect4()", pero java siempre crea los objetos en el stack
  connect4_init (&game, 4, 4, DEFAULT_PLAYERS, DEFAULT_WIN_SIZE);
  print_board (&game);
  connect4_free (&game);
}

void 
test_nonempty_board () 
{
  connect4 game;
  connect4_init(&game, 4, 4, 2, DEFAULT_WIN_SIZE);
  connect4_make_play (&game, 1, 0);
  connect4_make_play (&game, 2, 1);
  connect4_make_play (&game, 1, 1);
  connect4_make_play (&game, 2, 2);
  connect4_make_play (&game, 1, 3);
  print_board(&game);
  connect4_free (&game);
}