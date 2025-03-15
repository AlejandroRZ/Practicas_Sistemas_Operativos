#include <stdbool.h>

#ifndef CONNECT4_H
#define CONNECT4_H

#define FULL_COLUMN -1
#define INVALID_COLUMN -2
#define INVALID_PLAYER -3

#define DEFAULT_ROWS 6
#define DEFAULT_COLUMNS 7
#define DEFAULT_PLAYERS 2
#define DEFAULT_WIN_SIZE 4

#define PLAYER_1 1
#define PLAYER_2 2
#define PLAYER_3 3
#define PLAYER_4 4

struct connect4_board {
  // TODO: Here define the necessary data attributes to play this game
};

typedef struct connect4_board connect4;

/* 
 * Initializes the board state, setting all the necesary 
 * values to play this game in a board of `rows` by `columns` size, 
 * the game being played by `num_of_players` players and a player
 * wins when he connect `win_size` consecutive tokens of the
 * same color.
 * By default this game is represented with a 6x7 board, 
 * played by 2 players, and a player wins when he connects 
 * 4 consecutive tokens in the same direction.
 */
void connect4_init (connect4* game, int rows, int columns, int num_of_players, int win_size);

/*
 * Returns a string representation for the board state
 * in this case empty locations are represented with 0,
 * places where there is a token for player N contain the
 * number N.
 * 
 * For example a 3 x 4 board, with two players playing, 
 * and after a few turns would look like this:
 * 
 * 0100
 * 2100
 * 1221
 */ 
char* connect4_to_string (connect4* game);

/*
 * Given a player and a column where that player
 * wants to make a play, this function makes that
 * play at the given column on "game".
 * Note: columns and rows are zero based. 
 * Possible returned values: 
 * - The row where the token is placed,
 * - FULL_COLUMN if the column is full
 * - INVALID_COLUMN if the column is invalid
 * - INVALID_PLAYER if the provided player is invalid
 */
int connect4_make_play (connect4* game, int player, int column);

/*
 * Given the last position where a player threw
 * a token, this function decides whether the given
 * player already won. It considers horizontal,
 * vertical and diagonal wins.
 */ 
bool connect4_player_won (connect4* game, int player, int row, int column);

/* Releases the memory allocated by connect4_init */
void connect4_free (connect4* game);

#endif
