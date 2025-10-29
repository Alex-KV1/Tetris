#include "tetris_backend.h"

void init_board(board_t *board) {
  for (int i = 0; i < BOARD_ROWS + 1; ++i) {
    for (int j = 0; j < BOARD_COLS + 1; ++j) {
      board->board[i][j] = 0;
    }
  }
  board->figure_p_next = create_block();
}

bool right(board_t *map) {
  bool res = false;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (map->figure_p_current.figure
                  .sprite[map->figure_p_current.rotation][i][j] != 0 &&
          map->board[map->figure_p_current.row_pos - i]
                    [map->figure_p_current.col_pos - j] != 0 &&
          map->board[map->figure_p_current.row_pos + i]
                    [map->figure_p_current.col_pos - j] != 0) {
        res = false;
        break;
      }
      if (map->figure_p_current.figure
                  .sprite[map->figure_p_current.rotation][i][j] != 0 &&
          map->figure_p_current.col_pos + j > 10)
        res = true;
    }
  }
  return res;
}

bool right_colision(board_t *map) {
  bool res = false;
  int y = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (map->figure_p_current.figure
                  .sprite[map->figure_p_current.rotation][i][j] != 0 &&
          y < j)
        y = j;

      if (map->figure_p_current.figure
                  .sprite[map->figure_p_current.rotation][i][j] != 0 &&
          (1 + map->board[i + map->figure_p_current.row_pos + i + 1]
                         [1 + 2 * (map->figure_p_current.col_pos + j)] !=
               0 &&
           map->board[map->figure_p_current.row_pos + i + 1]
                     [2 + 2 * (map->figure_p_current.col_pos + j + 1)])) {
        res = true;
      }
    }
  }

  if (map->figure_p_current.col_pos + y > (BOARD_COLS - 4) / 2) res = true;
  return res;
}

bool left_colision(board_t *map) {
  bool res = false;
  int y = 0, x = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (map->figure_p_current.figure
                  .sprite[map->figure_p_current.rotation][i][j] != 0 &&
          y < j)
        y = j;
      if (map->figure_p_current.figure
                  .sprite[map->figure_p_current.rotation][i][j] != 0 &&
          x < i)
        x = i;

      if (map->figure_p_current.figure
                  .sprite[map->figure_p_current.rotation][i][j] != 0 &&
          (1 + map->board[i + map->figure_p_current.row_pos + i + 1]
                         [1 + 2 * (map->figure_p_current.col_pos + j)] !=
               0 &&
           map->board[map->figure_p_current.row_pos + i + 1]
                     [2 + 2 * (map->figure_p_current.col_pos + j)])) {
        res = true;
      }
    }
  }
  if (map->figure_p_current.col_pos + y < 0 ||
      map->figure_p_current.col_pos + x < 0)
    res = true;

  return res;
}

bool floor_colision(board_t *map) {
  int rotate = map->figure_p_current.rotation;
  bool res = false;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (map->figure_p_current.figure.sprite[rotate][i][j] != 0 &&
          (1 + map->board[i + map->figure_p_current.row_pos + i + 1]
                         [1 + 2 * (map->figure_p_current.col_pos + j)] !=
               0 &&
           map->board[1 + map->figure_p_current.row_pos + i + 1]
                     [2 + 2 * (map->figure_p_current.col_pos + j)])) {
        res = true;
        break;
      }
      if (map->figure_p_current.figure.sprite[rotate][i][j] != 0 &&
          map->figure_p_current.row_pos + i > 18)
        res = true;
    }
  }
  return res;
}

void colischeack_correction(board_t *map) {
  while (right(map)) map->figure_p_current.col_pos -= 1;

  while (left_colision(map)) map->figure_p_current.col_pos += 1;
}

void finalize_figure(params_t *prms) {
  if (floor_colision(prms->map)) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (prms->map->figure_p_current.figure
                .sprite[prms->map->figure_p_current.rotation][i][j] != 0) {
          prms->map->board[prms->map->figure_p_current.row_pos + i + 1]
                          [1 + 2 * (prms->map->figure_p_current.col_pos + j)] =
              1;
          prms->map->board[prms->map->figure_p_current.row_pos + i + 1]
                          [2 + 2 * (prms->map->figure_p_current.col_pos + j)] =
              1;
        }
      }
    }
  }
}

void shift_lines_down(int board[BOARD_ROWS + 1][BOARD_COLS + 1], int row) {
  for (int i = row; i > 1; i--) {
    for (int j = BOARDS_BEGIN - 1; j < BOARD_COLS; j++) {
      board[i][j] = board[i - 1][j];
    }
  }
}

void erase_line(int board[BOARD_ROWS + 1][BOARD_COLS + 1], int i) {
  for (int j = BOARDS_BEGIN - 1; j < BOARD_COLS; j++) board[i][j] = 0;
}

void track_fill_line(params_t *game) {
  int count_line = 0;
  for (int i = BOARDS_BEGIN; i < BOARD_ROWS; ++i) {
    int count = 0;
    for (int j = BOARDS_BEGIN; j < BOARD_COLS; ++j) {
      if (game->map->board[i][j] != 0) {
        count++;
      }
      if (count == 21) {
        erase_line(game->map->board, i);
        shift_lines_down(game->map->board, i);
        count_line++;
      }
    }
  }
  game_stat(game->stats, count_line);
}

tetra_t create_block(void) {
  figure_t fig[7] = {0};
  // Палка
  fig[0].sprite[0][0][0] = 1;
  fig[0].sprite[0][1][0] = 1;
  fig[0].sprite[0][2][0] = 1;
  fig[0].sprite[0][3][0] = 1;

  fig[0].sprite[1][0][0] = 1;
  fig[0].sprite[1][0][1] = 1;
  fig[0].sprite[1][0][2] = 1;
  fig[0].sprite[1][0][3] = 1;
  fig[0].max_rotation = 1;

  //  квдрат
  fig[1].sprite[0][0][0] = 1;
  fig[1].sprite[0][0][1] = 1;
  fig[1].sprite[0][1][0] = 1;
  fig[1].sprite[0][1][1] = 1;

  fig[1].max_rotation = 0;
  //  Г
  fig[2].sprite[0][0][0] = 1;
  fig[2].sprite[0][0][1] = 1;
  fig[2].sprite[0][1][1] = 1;
  fig[2].sprite[0][2][1] = 1;

  fig[2].sprite[1][0][0] = 1;
  fig[2].sprite[1][0][1] = 1;
  fig[2].sprite[1][0][2] = 1;
  fig[2].sprite[1][1][0] = 1;

  fig[2].sprite[2][0][0] = 1;
  fig[2].sprite[2][1][0] = 1;
  fig[2].sprite[2][2][0] = 1;
  fig[2].sprite[2][2][1] = 1;

  fig[2].sprite[3][0][2] = 1;
  fig[2].sprite[3][1][2] = 1;
  fig[2].sprite[3][1][1] = 1;
  fig[2].sprite[3][1][0] = 1;

  fig[2].max_rotation = 3;

  // z

  fig[3].sprite[0][0][0] = 1;
  fig[3].sprite[0][1][0] = 1;
  fig[3].sprite[0][1][1] = 1;
  fig[3].sprite[0][2][1] = 1;

  fig[3].sprite[1][0][1] = 1;
  fig[3].sprite[1][0][2] = 1;
  fig[3].sprite[1][1][0] = 1;
  fig[3].sprite[1][1][1] = 1;

  fig[3].max_rotation = 1;

  // ...
  //  `

  fig[4].sprite[0][0][0] = 1;
  fig[4].sprite[0][1][0] = 1;
  fig[4].sprite[0][1][1] = 1;
  fig[4].sprite[0][2][0] = 1;

  fig[4].sprite[1][0][1] = 1;
  fig[4].sprite[1][1][0] = 1;
  fig[4].sprite[1][1][1] = 1;
  fig[4].sprite[1][1][2] = 1;

  fig[4].sprite[2][0][1] = 1;
  fig[4].sprite[2][1][0] = 1;
  fig[4].sprite[2][1][1] = 1;
  fig[4].sprite[2][2][1] = 1;

  fig[4].sprite[3][1][0] = 1;
  fig[4].sprite[3][1][1] = 1;
  fig[4].sprite[3][1][2] = 1;
  fig[4].sprite[3][2][1] = 1;

  fig[4].max_rotation = 3;

  // Г в другую сторону

  fig[5].sprite[0][0][0] = 1;
  fig[5].sprite[0][0][1] = 1;
  fig[5].sprite[0][1][0] = 1;
  fig[5].sprite[0][2][0] = 1;

  fig[5].sprite[1][0][0] = 1;
  fig[5].sprite[1][0][1] = 1;
  fig[5].sprite[1][0][2] = 1;
  fig[5].sprite[1][1][2] = 1;

  fig[5].sprite[2][0][1] = 1;
  fig[5].sprite[2][1][1] = 1;
  fig[5].sprite[2][2][1] = 1;
  fig[5].sprite[2][2][0] = 1;

  fig[5].sprite[3][0][0] = 1;
  fig[5].sprite[3][1][0] = 1;
  fig[5].sprite[3][1][1] = 1;
  fig[5].sprite[3][1][2] = 1;

  fig[5].max_rotation = 3;

  // z в другую сторону

  fig[6].sprite[0][0][0] = 1;
  fig[6].sprite[0][0][1] = 1;
  fig[6].sprite[0][1][1] = 1;
  fig[6].sprite[0][1][2] = 1;

  fig[6].sprite[1][0][1] = 1;
  fig[6].sprite[1][1][1] = 1;
  fig[6].sprite[1][1][0] = 1;
  fig[6].sprite[1][2][0] = 1;

  fig[6].max_rotation = 1;

  tetra_t res;
  res.col_pos = rand() % 9;
  res.row_pos = 0;
  res.rotation = 0;
  res.figure = fig[rand() % 7];
  return res;
}

long long int get_time() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (((long long int)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
}

void game_stat(game_stats_t *score, int mult) {
  if (mult == 1)
    score->score += 100;
  else if (mult == 2)
    score->score += 300;
  else if (mult == 3)
    score->score += 700;
  else if (mult >= 4)
    score->score += 1500;

  if (score->hight_score < score->score) {
    save_statistic(score);
    score->hight_score = score->score;
  }

  if (score->score >= 600 && score->level <= 10) {
    int res = (score->score / 600) + 1;
    score->level = res;
    score->speed = res;
  }

  // if (score->score < 2000) {score->level =1; score->speed=1;}
  // else if (score->score > 2000 && score->score < 5000) {score->level =2;
  // score->speed=2;} else if (score->score > 5000 && score->score < 10000)
  // {score->level = 3; score->speed= 3;} else if (score->score > 10000 &&
  // score->score < 15000) {score->level = 4; score->speed= 4;} else if
  // (score->score > 15000) {score->level =4 ; score->speed= 4;}
}

int save_statistic(game_stats_t *stat) {
  FILE *file = fopen("score.txt", "w+");
  char buf[20];
  sprintf(buf, "%d", stat->score);
  if (file != NULL) {
    fputs(buf, file);
    fclose(file);
    return 1;
  }
  stat->hight_score = stat->score;
  return 0;
}

void load_statistic(game_stats_t *stat) {
  FILE *file = fopen("score.txt", "r");
  if (file == NULL) {
    stat->hight_score = 0;
  } else {
    if (fscanf(file, "%d", &stat->hight_score) == 1)
      ;
    else
      stat->hight_score = 0;
    fclose(file);
  }
}