#ifndef DEFINES_TETRIS
#define DEFINES_TETRIS

#include <locale.h>
#include <ncurses.h>
#include <stdio.h>

#include "../../gui/cli/frontend.h"
#include "defines.h"
#include "fsm_matrix.h"
#include "object.h"
#include "tetris_backend.h"

void game_loop(void);

#endif