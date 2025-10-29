#include "tetris.h"

int main(void) {
  color_t color;
  setlocale(LC_ALL, "");
  WINDOW_INIT(50);
  init_pair(color = BLACK, COLOR_WHITE, COLOR_BLACK);
  init_pair(color = RED, COLOR_BLACK, COLOR_RED);
  init_pair(color = GREN, COLOR_BLACK, COLOR_GREEN);
  init_pair(color = BLUE, COLOR_BLACK, COLOR_BLUE);
  print_welcom_scr();
  game_loop();
  endwin();
  return 0;
}

void game_loop() {
  //----------------------
  board_t board = {0};
  init_board(&board);
  //-----------------------------------
  bool break_flag = TRUE;
  int signal = 0;
  game_state state = START;
  game_stats_t stats = {0};
  tetra_t block = {0};
  stats.speed = 1;
  stats.level = 1;
  print_stats(&stats);

  while (break_flag) {
    if (state == EXIT_STATE) break_flag = FALSE;

    sigact(get_signal(signal), &state, &stats, &board, &block);
    if (state == MOVING || state == START || state == PAUSE ||
        state == GAMEOVER) {
      signal = getch();
    }
  }
}
