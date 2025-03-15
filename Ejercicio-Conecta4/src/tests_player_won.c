#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "connect4.h"
#include "test_helpers.h"
#include "tests_player_won.h"

void horizontal_no_win_from_last_position ();
void horizontal_no_win_from_first_position ();
void horizontal_no_win_from_inner_position ();
void horizontal_win_edge_from_last_position ();
void horizontal_win_edge_from_first_position ();
void horizontal_win_edge_from_inner_position ();
void horizontal_win_in_first_row ();
void horizontal_win_in_inner_row ();
void horizontal_win_in_last_row ();
void vertical_win ();
void vertical_no_win ();
void downwards_diagonal_win ();
void downwards_diagonal_no_win ();
void upwards_diagonal_win ();
void upwards_diagonal_no_win ();

void print_player_wont ();

void 
execute_player_won_test(char* test_name)
{
  if (strcmp ("horizontal_no_win_from_last_position", test_name) == 0) {
    horizontal_no_win_from_last_position ();
  } else if (strcmp ("horizontal_no_win_from_first_position", test_name) == 0) {
    horizontal_no_win_from_first_position ();
  } else if (strcmp ("horizontal_no_win_from_inner_position", test_name) == 0) {
    horizontal_no_win_from_inner_position ();
  } else if (strcmp ("horizontal_win_edge_from_last_position", test_name) == 0) {
    horizontal_win_edge_from_last_position ();
  } else if (strcmp ("horizontal_win_edge_from_first_position", test_name) == 0) {
    horizontal_win_edge_from_first_position ();
  } else if (strcmp ("horizontal_win_edge_from_inner_position", test_name) == 0) {
    horizontal_win_edge_from_inner_position ();
  } else if (strcmp ("horizontal_win_in_first_row", test_name) == 0) {
    horizontal_win_in_first_row ();
  } else if (strcmp ("horizontal_win_in_inner_row", test_name) == 0) {
    horizontal_win_in_inner_row ();
  } else if (strcmp ("horizontal_win_in_last_row", test_name) == 0) {
    horizontal_win_in_last_row ();
  } else if (strcmp ("vertical_win", test_name) == 0) {
    vertical_win ();
  } else if (strcmp ("vertical_no_win", test_name) == 0) {
    vertical_no_win ();
  } else if (strcmp ("downwards_diagonal_win", test_name) == 0) {
    downwards_diagonal_win ();
  } else if (strcmp ("downwards_diagonal_no_win", test_name) == 0) {
    downwards_diagonal_no_win ();
  } else if (strcmp ("upwards_diagonal_win", test_name) == 0) {
    upwards_diagonal_win ();
  } else if (strcmp ("upwards_diagonal_no_win", test_name) == 0) {
    upwards_diagonal_no_win ();
  } else {
    printf ("unknown to string test case\n");
    assert (false);
  }
}

static int init_no_win_board (connect4* game);

void horizontal_no_win_from_last_position () 
{
  connect4 game;
  int last_row = init_no_win_board (&game);
  int last_column = 3;
  print_board(&game);
  print_player_wont (&game, PLAYER_1, last_row, last_column);
  connect4_free (&game);
}

void horizontal_no_win_from_first_position () 
{
  connect4 game;
  int last_row = init_no_win_board (&game);
  int first_column = 1;
  print_board(&game);
  print_player_wont (&game, PLAYER_1, last_row, first_column);
  connect4_free (&game);
}

void horizontal_no_win_from_inner_position ()
{
  connect4 game;
  int last_row = init_no_win_board (&game);
  int inner_column = 2;
  print_board(&game);
  print_player_wont (&game, PLAYER_1, last_row, inner_column);
  connect4_free (&game);
}

static int 
init_no_win_board (connect4* game)
{
  connect4_init (game, DEFAULT_ROWS, DEFAULT_COLUMNS, DEFAULT_PLAYERS, DEFAULT_WIN_SIZE + 1);
  connect4_make_play (game, PLAYER_1, 0);
  connect4_make_play (game, PLAYER_2, 1);
  connect4_make_play (game, PLAYER_1, 2);
  connect4_make_play (game, PLAYER_2, 3);
  connect4_make_play (game, PLAYER_1, 1);
  connect4_make_play (game, PLAYER_1, 2);
  return connect4_make_play (game, PLAYER_1, 3);
}

static int 
init_horizontal_win_board (connect4* game);

void horizontal_win_edge_from_last_position ()
{
  connect4 game;
  int last_row = init_horizontal_win_board (&game);
  int last_column = 3;
  print_board(&game);
  print_player_wont (&game, PLAYER_1, last_row, last_column);
  connect4_free (&game);
}

void horizontal_win_edge_from_first_position ()
{
  connect4 game;
  int last_row = init_horizontal_win_board (&game);
  int first_column = 0;
  print_board (&game);
  print_player_wont (&game, PLAYER_1, last_row, first_column);
  connect4_free (&game);
}

void horizontal_win_edge_from_inner_position ()
{
  connect4 game;
  int last_row = init_horizontal_win_board (&game);
  int inner_column = 1;
  print_board (&game);
  print_player_wont (&game, PLAYER_1, last_row, inner_column);
  connect4_free (&game);
}

static int 
init_horizontal_win_board (connect4* game)
{
  connect4_init (game, 4, 4, DEFAULT_PLAYERS, DEFAULT_WIN_SIZE);
  connect4_make_play (game, PLAYER_1, 0);
  connect4_make_play (game, PLAYER_2, 1);
  connect4_make_play (game, PLAYER_1, 2);
  connect4_make_play (game, PLAYER_2, 3);
  connect4_make_play (game, PLAYER_1, 1);
  connect4_make_play (game, PLAYER_1, 2);
  connect4_make_play (game, PLAYER_1, 3);
  return connect4_make_play (game, PLAYER_1, 0);
}

void horizontal_win_in_first_row ()
{
  connect4 game;
  connect4_init (&game, 3, 5, 3, 3);
  connect4_make_play (&game, PLAYER_1, 1);
  connect4_make_play (&game, PLAYER_1, 2);
  int last_row = connect4_make_play (&game, PLAYER_1, 3);
  print_board (&game);
  print_player_wont (&game, PLAYER_1, last_row, 2);
  connect4_free (&game);
}

void horizontal_win_in_inner_row ()
{
  connect4 game;
  connect4_init (&game, 3, 5, 3, 3);
  connect4_make_play (&game, PLAYER_1, 1);
  connect4_make_play (&game, PLAYER_2, 2);
  connect4_make_play (&game, PLAYER_3, 3);
  connect4_make_play (&game, PLAYER_1, 1);
  connect4_make_play (&game, PLAYER_1, 2);
  int last_row = connect4_make_play (&game, PLAYER_1, 3);
  print_board (&game);
  print_player_wont (&game, PLAYER_1, last_row, 2);
  connect4_free (&game);
}

void horizontal_win_in_last_row ()
{
  connect4 game;
  connect4_init (&game, 3, 5, 3, 3);
  connect4_make_play (&game, PLAYER_1, 1);
  connect4_make_play (&game, PLAYER_2, 2);
  connect4_make_play (&game, PLAYER_3, 3);
  connect4_make_play (&game, PLAYER_2, 1);
  connect4_make_play (&game, PLAYER_3, 2);
  connect4_make_play (&game, PLAYER_1, 3);
  connect4_make_play (&game, PLAYER_1, 1);
  connect4_make_play (&game, PLAYER_1, 2);
  int last_row = connect4_make_play (&game, PLAYER_1, 3);
  print_board (&game);
  print_player_wont (&game, PLAYER_1, last_row, 2);
  connect4_free (&game);
}

void vertical_win ()
{
  connect4 game;
  connect4_init (&game, 3, 1, 2, 3);
  connect4_make_play (&game, PLAYER_1, 0);
  connect4_make_play (&game, PLAYER_1, 0);
  connect4_make_play (&game, PLAYER_1, 0);
  print_board (&game);
  print_player_wont (&game, PLAYER_1, 1, 0);
  connect4_free (&game);
}

void vertical_no_win ()
{
  connect4 game;
  connect4_init (&game, 3, 1, 2, 3);
  connect4_make_play (&game, PLAYER_1, 0);
  connect4_make_play (&game, PLAYER_2, 0);
  connect4_make_play (&game, PLAYER_1, 0);
  print_board (&game);
  print_player_wont (&game, PLAYER_1, 1, 0);
  connect4_free (&game);
}

void downwards_diagonal_win ()
{
  connect4 game;
  connect4_init (&game, 3, 3, 2, 3);
  connect4_make_play (&game, PLAYER_1, 0);
  connect4_make_play (&game, PLAYER_2, 1);
  connect4_make_play (&game, PLAYER_1, 0);
  connect4_make_play (&game, PLAYER_2, 0);
  connect4_make_play (&game, PLAYER_2, 1);
  connect4_make_play (&game, PLAYER_2, 2);
  print_board (&game);
  print_player_wont (&game, PLAYER_2, 1, 1);
  connect4_free (&game);
}

void downwards_diagonal_no_win ()
{
  connect4 game;
  connect4_init (&game, 3, 3, 2, 3);
  connect4_make_play (&game, PLAYER_1, 0);
  connect4_make_play (&game, PLAYER_2, 1);
  connect4_make_play (&game, PLAYER_1, 0);
  connect4_make_play (&game, PLAYER_2, 0);
  connect4_make_play (&game, PLAYER_1, 1);
  connect4_make_play (&game, PLAYER_2, 2);
  print_board (&game);
  print_player_wont (&game, PLAYER_2, 1, 1);
  connect4_free (&game);
}

void upwards_diagonal_win ()
{
  connect4 game;
  connect4_init (&game, 3, 3, 2, 3);
  connect4_make_play (&game, PLAYER_1, 2);
  connect4_make_play (&game, PLAYER_2, 1);
  connect4_make_play (&game, PLAYER_1, 2);
  connect4_make_play (&game, PLAYER_2, 2);
  connect4_make_play (&game, PLAYER_2, 1);
  connect4_make_play (&game, PLAYER_2, 0);
  print_board (&game);
  print_player_wont (&game, PLAYER_2, 1, 1);
  connect4_free (&game);
}

void upwards_diagonal_no_win ()
{
  connect4 game;
  connect4_init (&game, 3, 3, 2, 3);
  connect4_make_play (&game, PLAYER_1, 2);
  connect4_make_play (&game, PLAYER_2, 1);
  connect4_make_play (&game, PLAYER_1, 2);
  connect4_make_play (&game, PLAYER_2, 2);
  connect4_make_play (&game, PLAYER_1, 1);
  connect4_make_play (&game, PLAYER_2, 0);
  print_board (&game);
  print_player_wont (&game, PLAYER_2, 1, 1);
  connect4_free (&game);
}

void print_player_wont (connect4* game, int player, int row, int column)
{
  printf (
    "player %d won %s when throwing at position row %d column %d\n",
    player,
    connect4_player_won (game, player, row, column) ? "true" : "false",
    row,
    column
  );
}