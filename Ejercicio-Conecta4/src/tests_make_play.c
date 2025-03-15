#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "connect4.h"
#include "test_helpers.h"
#include "tests_make_play.h"

void single_play ();
void multiple_plays ();
void full_column ();
void invalid_column ();
void invalid_player ();
void print_play (connect4* game, int player, int column);

void 
execute_make_play_tests(char* test_name) 
{
  if (strcmp ("single_play", test_name) == 0) {
    single_play ();
  } else if (strcmp ("multiple_plays", test_name) == 0) {
    multiple_plays ();
  } else if (strcmp ("full_column", test_name) == 0) {
    full_column ();
  } else if (strcmp ("invalid_column", test_name) == 0) {
    invalid_column ();
  } else if (strcmp ("invalid_player", test_name) == 0) {
    invalid_player ();
  }  else {
    printf ("unknown to string test case\n");
    assert (false);
  }
}

void single_play () 
{
  connect4 game;
  connect4_init (&game, 6, 7, 3, DEFAULT_WIN_SIZE);
  print_play (&game, 1, 0);
  print_board (&game);
  connect4_free (&game);
}

void multiple_plays () 
{
  connect4 game;
  connect4_init(&game, 6, 7, 3, DEFAULT_WIN_SIZE);
  print_play (&game, 1, 1);
  print_play (&game, 2, 0);
  print_play (&game, 3, 2);
  print_play (&game, 1, 4);
  print_play (&game, 2, 6);
  print_play (&game, 3, 2);
  print_play (&game, 1, 3);
  print_play (&game, 2, 4);
  print_play (&game, 3, 5);
  print_play (&game, 1, 0);
  print_play (&game, 2, 1);
  print_play (&game, 3, 2);
  print_play (&game, 1, 6);
  print_play (&game, 2, 2);
  print_play (&game, 3, 3);
  print_play (&game, 1, 5);
  print_play (&game, 2, 2);
  print_play (&game, 3, 3);
  print_play (&game, 1, 2);
  print_play (&game, 2, 6);
  print_play (&game, 3, 0);
  print_play (&game, 1, 5);
  print_play (&game, 2, 4);
  print_play (&game, 3, 6);
  print_board (&game);
  connect4_free (&game);
}

void full_column ()
{
  connect4 game;
  connect4_init(&game, 1, 2, 2, DEFAULT_WIN_SIZE);
  print_play (&game, 1, 0);
  print_play (&game, 2, 0);
  print_board (&game);
  connect4_free (&game);
}

void invalid_column ()
{
  connect4 game;
  connect4_init(&game, 2, 1, 2, DEFAULT_WIN_SIZE);
  print_play (&game, 1, -1);
  print_play (&game, 2, 1);
  print_board (&game);
  connect4_free (&game);
}

void invalid_player ()
{
  connect4 game;
  connect4_init(&game, 1, 2, 2, DEFAULT_WIN_SIZE);
  print_play (&game, 0, 0);
  print_play (&game, 3  , 0);
  print_board (&game);
  connect4_free (&game);
}

void 
print_play (connect4* game, int player, int column)
{
  printf (
    "player %d made a play in column %d and token was placed at the row %d\n", 
    player,
    column, 
    connect4_make_play (game, player, column)
  );
}