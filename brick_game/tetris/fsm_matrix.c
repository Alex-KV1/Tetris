#include "fsm_matrix.h"

// This is a finite state machine realisation based on matrix of "actions".
/*
    Function sigact() takes an action function from fsm_table.
    Game state defines an index of line in matrix (where to get).
    User signal defines an index of column in matrix (what to get).

    Pros:
        1) By instantly taking needed action, speed of processing is higher than
   switch-case realisation. 2) Code is easy to read. 3) Flexible (easy to add
   new state) Cons: 1) More memory usage.
*/

// action fsm_table[8][7] = {
//     {NULL, NULL, NULL, NULL, exitstate, spawn, NULL},
//     {spawn, spawn, spawn, spawn, spawn, spawn, spawn},
//     {moveup, movedown, moveright, moveleft, exitstate, check, check},
//     {shifting, shifting, shifting, shifting, shifting, shifting, shifting},
//     {reach, reach, reach, reach, reach, reach, reach},
//     {collide, collide, collide, collide, collide, collide, collide},
//     {gameover, gameover, gameover, gameover, gameover, gameover, gameover},
//     {exitstate, exitstate, exitstate, exitstate, exitstate, exitstate,
//     exitstate}
// };

action fsm_table[10][9] = {
    {NULL, NULL, NULL, NULL, exitstate, start_game, NULL, get_help, NULL},
    {spawn, spawn, spawn, spawn, spawn, spawn, spawn, spawn, spawn},
    {rotate, movedown_key, moveright, moveleft, exitstate, NULL, pause, NULL,
     NULL},
    {rotate, movedown, moveright, moveleft, exitstate, NULL, NULL, NULL, NULL},
    {state_atatch, state_atatch, state_atatch, state_atatch, state_atatch,
     state_atatch, state_atatch, state_atatch, state_atatch},
    {pause, pause, pause, pause, pause, pause, state_atatch, get_help, NULL},
    {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    {game_over, game_over, game_over, game_over, exitstate, start_game,
     game_over, NULL, NULL},
    {exitstate, exitstate, exitstate, exitstate, exitstate, exitstate,
     exitstate, exitstate, NULL},  // EXIT_STATE
    {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}};

void sigact(UserAction_t sig, game_state *state, game_stats_t *stats,
            board_t *map, tetra_t *block_pos) {
  int speed[10] = {1000, 900, 800, 700, 600, 500, 400, 300, 200, 100};
  action act = NULL;
  params_t prms;
  prms.stats = stats;
  prms.state = state;
  prms.map = map;
  prms.block_pos = block_pos;

  if (*state == MOVING) {
    if ((prms.stats->time + speed[prms.stats->speed - 1]) < get_time()) {
      prms.stats->time = get_time();
      movedown(&prms);
    }
  }
  if (*state != FILE_ERROR_STATE) act = fsm_table[*state][sig];

  if (act) act(&prms);
}

UserAction_t get_signal(int user_input) {
  UserAction_t rc = NOSIG;
  if (user_input == SPACE)
    rc = SPACE_BTN;
  else if (user_input == KEY_DOWN)
    rc = MOVE_DOWN;
  else if (user_input == KEY_LEFT)
    rc = MOVE_LEFT;
  else if (user_input == KEY_RIGHT)
    rc = MOVE_RIGHT;
  else if (user_input == ESCAPE)
    rc = ESCAPE_BTN;
  else if (user_input == KEY_H)
    rc = HELP_BTN;
  else if (user_input == ENTER_KEY)
    rc = ENTER_BTN;
  else if (user_input == KEY_Q)
    rc = ESCAPE_BTN;
  else if (user_input == KEY_P)
    rc = PAUSE_BTN;
  return rc;
}

void pause(params_t *prms) {
  print_pause_baner();
  *prms->state = PAUSE;
}

void game_over(params_t *prms) {
  print_game_over_baner();
  if (prms->stats->score > prms->stats->hight_score) {
    save_statistic(prms->stats);
  }
}

void get_help() { print_help(); }

void exitstate(params_t *prms) { *prms->state = EXIT_STATE; }

void start_game(params_t *prms) {
  board_t map;
  prms->stats->hight_score = 0;
  prms->stats->speed = 1;
  prms->stats->level = 1;
  prms->stats->score = 0;
  init_board(&map);
  *prms->map = map;
  prms->map->figure_p_next = create_block();
  load_statistic(prms->stats);
  print_game_space(prms->map);
  print_stats(prms->stats);

  *prms->state = SPAWN;
}

void spawn(params_t *prms) {
  prms->map->figure_p_current = prms->map->figure_p_next;
  prms->map->figure_p_next = create_block();
  print_game_space(prms->map);
  print_block(prms->map);
  print_next_block(prms->map);
  print_stats(prms->stats);

  track_fill_line(prms);
  *prms->state = MOVING;
  if (floor_colision(prms->map)) *prms->state = GAMEOVER;
}

void state_atatch(params_t *prms) {
  if (floor_colision(prms->map)) {
    finalize_figure(prms);
    print_game_space(prms->map);
    track_fill_line(prms);
    *prms->state = SPAWN;
  } else
    *prms->state = MOVING;
}

void rotate(params_t *prms) {
  board_t next_rotate = *prms->map;
  if (prms->map->figure_p_current.rotation <
      prms->map->figure_p_current.figure.max_rotation) {
    next_rotate.figure_p_current.rotation += 1;
    colischeack_correction(&next_rotate);
    if (!floor_colision(&next_rotate) && !left_colision(&next_rotate) &&
        !right_colision(&next_rotate))
      prms->map->figure_p_current.rotation += 1;
    else if (!floor_colision(&next_rotate) && !left_colision(&next_rotate) &&
             !right(&next_rotate) && right_colision(&next_rotate)) {
      prms->map->figure_p_current.rotation =
          next_rotate.figure_p_current.rotation;
      colischeack_correction(prms->map);
    }
  } else {
    next_rotate.figure_p_current.rotation = 0;
    if (!floor_colision(&next_rotate) && !left_colision(&next_rotate) &&
        !right_colision(&next_rotate))
      prms->map->figure_p_current.rotation = 0;
    else if (!floor_colision(&next_rotate) && !left_colision(&next_rotate) &&
             !right(&next_rotate) && right_colision(&next_rotate) &&
             next_rotate.figure_p_current.row_pos < 17) {
      prms->map->figure_p_current.rotation =
          next_rotate.figure_p_current.rotation;
      colischeack_correction(prms->map);
    }
  }
  print_game_space(prms->map);
  print_block(prms->map);
}

void movedown(params_t *prms) {
  if (!floor_colision(prms->map)) {
    prms->map->figure_p_current.row_pos += 1;
    *prms->state = ATTACHING;
  }
  track_fill_line(prms);
  print_game_space(prms->map);
  print_block(prms->map);
}

void movedown_key(params_t *prms) {
  while (!floor_colision(prms->map)) {
    movedown(prms);
  }
  track_fill_line(prms);
}

void moveright(params_t *prms) {
  if (prms->map->figure_p_current.col_pos != 10 && !right_colision(prms->map)) {
    prms->map->figure_p_current.col_pos += 1;
  }
  print_game_space(prms->map);
  print_block(prms->map);
}

void moveleft(params_t *prms) {
  board_t temp = *prms->map;
  temp.figure_p_current.col_pos -= 1;
  if (prms->map->figure_p_current.col_pos != 0 && !left_colision(&temp)) {
    prms->map->figure_p_current.col_pos -= 1;
  }
  print_game_space(prms->map);
  print_block(prms->map);
}
