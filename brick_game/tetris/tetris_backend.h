#ifndef BACKEND
#define BACKEND

#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <sys/time.h>

#include "../../gui/cli/frontend.h"
#include "defines.h"
#include "fsm_matrix.h"
#include "object.h"
#include "stdlib.h"
#include "tetris.h"

void init_board(board_t *board);    // initialization of the playing field;
tetra_t create_block(void);         // creating a block;
bool left_colision(board_t *map);   // we check if the figure does not come out
                                    // after turning out of the field
bool right_colision(board_t *map);  // we check if the figure does not come out
                                    // after turning out of the field
bool right(board_t *map);
bool floor_colision(board_t *map);  // Checking the bottom
long long int get_time();           // taking a millisecond

void colischeack_correction(board_t *map);

int save_statistic(game_stats_t *stat);   // save statistic
void load_statistic(game_stats_t *stat);  // load statistic

void finalize_figure(
    params_t *prms);  // pinning a shape when it collides with another shape
                      // or the bottom of the playing field.
void track_fill_line(params_t *game);
void erase_line(int board[BOARD_ROWS + 1][BOARD_COLS + 1], int i);
void shift_lines_down(int board[BOARD_ROWS + 1][BOARD_COLS + 1], int row);
void game_stat(game_stats_t *score, int mult);  // we earn points

#endif