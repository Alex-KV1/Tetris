#ifndef DEFINES_OBJECT
#define DEFINES_OBJECT

#include "defines.h"
#include "stdbool.h"

typedef struct {
  int sprite[4][4][4];
  int max_rotation;
} figure_t;

typedef struct {
  int score;
  int speed;
  int level;
  int hight_score;
  long long time;
} game_stats_t;

typedef struct {
  int row_pos;
  int col_pos;
  figure_t figure;
  int rotation;
} tetra_t;

typedef struct {
  int board[BOARD_ROWS + 1][BOARD_COLS + 1];
  tetra_t figure_p_current;
  tetra_t figure_p_next;
} board_t;

typedef struct {
  board_t board;
  game_stats_t stats;
} GameInfo_t;

typedef enum {
  START = 0,
  SPAWN,
  MOVING,
  SHIFTING,
  ATTACHING,
  PAUSE,
  HELP,
  GAMEOVER,
  EXIT_STATE,
  FILE_ERROR_STATE

} game_state;

typedef enum {
  SPACE_BTN = 0,
  MOVE_DOWN,
  MOVE_RIGHT,
  MOVE_LEFT,
  ESCAPE_BTN,
  ENTER_BTN,
  PAUSE_BTN,
  HELP_BTN,
  NOSIG
} UserAction_t;

typedef struct game_params

{
  game_stats_t *stats;
  game_state *state;
  board_t *map;
  tetra_t *block_pos;
  bool *break_flag;
} params_t;

typedef void (*action)(
    params_t *prms);  // это объявление нового типа данных action который
                      // является указателем на функцю шаблона void (params_t
                      // *prms), используем для хранения указателей на фунции

typedef enum { BLACK = 0, RED, GREN, BLUE } color_t;  // цвета

#endif
