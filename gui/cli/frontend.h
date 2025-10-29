#ifndef FRONTEND
#define FRONTEND

#include "../../brick_game/tetris/defines.h"
#include "../../brick_game/tetris/object.h"
#include "../../brick_game/tetris/tetris.h"

void print_welcom_scr(void);
void print_help(void);
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);
void print_stats(game_stats_t* stats);
void print_overlay(void);
void print_game_space(board_t* board);
void print_block(board_t* board);
void print_next_block(board_t* board);

void print_pause_baner();
void print_game_over_baner();

void print_debug_state(board_t board);
#endif