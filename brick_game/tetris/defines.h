#ifndef DEFINES_H
#define DEFINES_H

#define WINDOW_INIT(time) \
  {                       \
    initscr();            \
    noecho();             \
    curs_set(0);          \
    keypad(stdscr, TRUE); \
    timeout(time);        \
    start_color();        \
  }

#define MVPRINTW(y, x, ...) \
  mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)
#define CLEAR_BACKPOS(y, x) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), ' ')

#define SCREEN_X 23
#define SCREEN_Y 21
#define BOARDS_BEGIN 2

#define BOARD_ROWS 21
#define BOARD_COLS 23

#define ESCAPE 27
#define ENTER_KEY 10
#define KEY_H 104
#define KEY_Q 113
#define KEY_P 112
#define SPACE ' '

#endif