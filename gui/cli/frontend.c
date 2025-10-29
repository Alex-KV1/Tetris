#define frontend
#ifdef frontend

#include "frontend.h"

void print_welcom_scr(void) {
  print_rectangle(0, SCREEN_Y, 0, SCREEN_X);
  print_rectangle(0, SCREEN_Y, SCREEN_X + 1, SCREEN_X + 20);

  print_rectangle(1, 3, SCREEN_Y + 4, SCREEN_Y + 21);
  print_rectangle(4, 6, SCREEN_Y + 4, SCREEN_Y + 21);
  print_rectangle(7, 9, SCREEN_Y + 4, SCREEN_Y + 21);
  print_rectangle(10, 15, SCREEN_X + 2, SCREEN_Y + 21);
  print_rectangle(16, SCREEN_Y - 1, SCREEN_X + 2, SCREEN_Y + 21);

  MVPRINTW(2, SCREEN_Y + 5, "LVL:");
  MVPRINTW(2, SCREEN_Y + 14, "SPD:");
  MVPRINTW(5, SCREEN_Y + 5, "SCORE:");
  MVPRINTW(8, SCREEN_Y + 5, "HI SCOR:");
  MVPRINTW(11, SCREEN_Y + 5, "NEXT");
  MVPRINTW(12, SCREEN_Y + 5, "FIG.");

  MVPRINTW(17, SCREEN_Y + 5, "LEGEND: < left");
  MVPRINTW(18, SCREEN_Y + 5, "> right, v down");
  MVPRINTW(19, SCREEN_Y + 5, "space - rotate");

  MVPRINTW(5, (SCREEN_Y / 2) - 4, "press ENTER");
  MVPRINTW(6, (SCREEN_Y / 2) - 2, "to start");

  MVPRINTW(SCREEN_X / 2, (SCREEN_Y / 2) - 4, "Develop by");
  MVPRINTW((SCREEN_X / 2) + 1, (SCREEN_Y / 2) - 3, "Ahrigray");

  MVPRINTW(18, (SCREEN_Y / 2) - 6, "For help press h");
  MVPRINTW(19, (SCREEN_Y / 2) - 6, "For EXIT press q");
}

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  MVADDCH(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;

  for (; i < right_x; i++) {
    MVADDCH(top_y, i, ACS_HLINE);
  }
  MVADDCH(top_y, i, ACS_URCORNER);

  for (int i = top_y + 1; i < bottom_y; i++) {
    MVADDCH(i, left_x, ACS_VLINE);
    MVADDCH(i, right_x, ACS_VLINE);
  }

  MVADDCH(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) MVADDCH(bottom_y, i, ACS_HLINE);
  MVADDCH(bottom_y, i, ACS_LRCORNER);
}

void print_help(void) {
  clear();
  print_rectangle(0, SCREEN_Y, 0, SCREEN_X);

  MVPRINTW(1, 1, "      How to play?");
  MVPRINTW(3, 1, "  For move the block, ");
  MVPRINTW(4, 1, " use arrow keys - left");
  MVPRINTW(5, 1, "      down right   ");
  MVPRINTW(6, 1, " The down key lowers ");
  MVPRINTW(7, 1, "      the block");
  MVPRINTW(8, 1, " For rotate of block,");
  MVPRINTW(9, 1, "     use space");
  MVPRINTW(19, 1, " Exit from help - z");
  refresh();
  while (1) {
    if ('z' == getch()) break;
  }
  clear();
  print_welcom_scr();
}

void print_stats(game_stats_t* stats) {
  MVPRINTW(2, SCREEN_Y + 9, "  ");
  MVPRINTW(2, SCREEN_Y + 18, "  ");
  MVPRINTW(5, SCREEN_Y + 11, "          ");
  MVPRINTW(8, SCREEN_Y + 14, "       ");

  MVPRINTW(2, SCREEN_Y + 9, "%d", stats->level);
  MVPRINTW(2, SCREEN_Y + 18, "%d", stats->speed);
  MVPRINTW(5, SCREEN_Y + 11, "%d", stats->score);
  MVPRINTW(8, SCREEN_Y + 13, "%d", stats->hight_score);
}

// -----------------------------------------------------------------------------------

void print_overlay(void) {
  print_rectangle(0, SCREEN_Y, 0, SCREEN_X);
  print_rectangle(0, SCREEN_Y, SCREEN_X + 1, SCREEN_X + 20);

  print_rectangle(1, 3, SCREEN_Y + 4, SCREEN_Y + 21);
  print_rectangle(4, 6, SCREEN_Y + 4, SCREEN_Y + 21);
  print_rectangle(7, 9, SCREEN_Y + 4, SCREEN_Y + 21);
  print_rectangle(10, 15, SCREEN_X + 2, SCREEN_Y + 21);
  print_rectangle(16, SCREEN_Y - 1, SCREEN_X + 2, SCREEN_Y + 21);
}

void print_game_space(board_t* board) {
  for (int i = BOARDS_BEGIN - 1; i < BOARD_ROWS; ++i) {
    for (int j = BOARDS_BEGIN - 1; j < BOARD_COLS; ++j) {
      if (board->board[i][j] == 0)
        MVADDCH(i, j, SPACE);
      else {
        attron(COLOR_PAIR(1));
        MVADDCH(i, j, SPACE);
        attroff(COLOR_PAIR(1));
      }
    }
  }
}

void print_block(board_t* board) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (board->figure_p_current.figure
              .sprite[board->figure_p_current.rotation][i][j] != 0) {
        attron(COLOR_PAIR(2));
        MVADDCH(i + 1 + board->figure_p_current.row_pos,
                BOARDS_BEGIN - 1 + 2 * (j + board->figure_p_current.col_pos),
                SPACE);
        MVADDCH(
            i + 1 + board->figure_p_current.row_pos,
            BOARDS_BEGIN - 1 + 2 * (j + board->figure_p_current.col_pos) + 1,
            SPACE);
        attroff(COLOR_PAIR(2));
      }
    }
  }
}

void print_next_block(board_t* board) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (board->figure_p_next.figure.sprite[0][i][j] != 0) {
        attron(COLOR_PAIR(3));
        MVADDCH(11 + i, 33 + 1 + 2 * j, SPACE);
        MVADDCH(11 + i, 33 + 1 + 2 * j + 1, SPACE);
        attroff(COLOR_PAIR(3));
      } else {
        attron(COLOR_PAIR(0));
        MVADDCH(11 + i, 33 + 1 + 2 * j, SPACE);
        MVADDCH(11 + i, 33 + 1 + 2 * j + 1, SPACE);
        attroff(COLOR_PAIR(0));
      }
    }
  }
}

void print_debug_state(board_t board) {
  int c = 1;
  print_rectangle(0, SCREEN_Y, 50, 60);

  MVPRINTW(1, 50 + 1, "print_debag %d ", c);
  MVPRINTW(2, 50 + 1, "pos row %d", board.figure_p_current.row_pos);
  MVPRINTW(3, 50 + 1, "pos col %d", board.figure_p_current.col_pos);
}

void print_pause_baner() {
  print_rectangle(BOARDS_BEGIN + 4, BOARDS_BEGIN + 7, BOARDS_BEGIN + 3,
                  SCREEN_Y - 4);
  MVPRINTW(7, 6, "   PAUSE   ");
  MVPRINTW(8, 6, "  press P  ");
  refresh();
}

void print_game_over_baner() {
  print_rectangle(BOARDS_BEGIN + 4, BOARDS_BEGIN + 9, BOARDS_BEGIN + 1,
                  SCREEN_Y);
  MVPRINTW(7, 4, "    GAME OVER    ");
  MVPRINTW(8, 4, "                 ");
  MVPRINTW(9, 4, "for restart press");
  MVPRINTW(10, 4, "       ENTER     ");
  refresh();
}

#endif
