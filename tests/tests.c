#include "tests.h"

START_TEST(init_stats_test) {
  game_stats_t stats = {0};
  stats.hight_score = 0;
  stats.score = 0;
  stats.level = 1;
  stats.speed = 1;
  ck_assert_int_eq(stats.level, 1);
  ck_assert_int_eq(stats.score, 0);
}
END_TEST

START_TEST(init_board_test) {
  board_t board = {0};
  init_board(&board);
  init_board(&board);
  ck_assert_int_eq(board.board[0][0], 0);
  ck_assert_int_eq(board.board[19][9], 0);
}
END_TEST

START_TEST(gen_next_block_test) {
  tetra_t tet = create_block();
  int tmp = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tet.figure.sprite[0][i][j] != 0) {
        tmp++;
      }
    }
  }
  ck_assert_int_eq(tmp, 4);
  ck_assert_int_eq(tet.col_pos, 10 / 2);
  ck_assert_int_eq(tet.row_pos, 0);
  ck_assert_int_eq(tet.rotation, 0);
}
END_TEST

// Test the check_lborder_collide function
START_TEST(test_check_lborder_collide) {
  board_t board = {0};
  init_board(&board);
  game_state state = START;
  game_stats_t stats = {0};

  stats.speed = 1;
  stats.level = 1;

  params_t prms = {0};
  prms.stats = &stats;
  prms.state = &state;
  prms.map = &board;

  prms.map->figure_p_current = create_block();

  prms.map->figure_p_current.col_pos = -4;
  prms.map->figure_p_current.row_pos = 0;
  prms.map->figure_p_current.rotation = 0;

  ck_assert_int_eq(left_colision(prms.map), TRUE);
}
END_TEST

// Test the check_rborder_collide function
START_TEST(test_check_rborder_collide) {
  board_t board = {0};
  init_board(&board);
  game_state state = START;
  game_stats_t stats = {0};
  stats.speed = 1;
  stats.level = 1;

  params_t prms = {0};
  prms.stats = &stats;
  prms.state = &state;
  prms.map = &board;

  prms.map->figure_p_current = create_block();

  prms.map->figure_p_current.col_pos = BOARD_COLS + 4;
  prms.map->figure_p_current.row_pos = 0;
  prms.map->figure_p_current.rotation = 0;
  ck_assert_int_eq(right_colision(prms.map), TRUE);
}
END_TEST

// Test the check_board_collide function
START_TEST(test_check_board_collide) {
  board_t board = {0};
  init_board(&board);
  game_state state = START;
  game_stats_t stats = {0};

  stats.speed = 1;
  stats.level = 1;

  params_t prms = {0};
  prms.stats = &stats;
  prms.state = &state;
  prms.map = &board;

  prms.map->figure_p_current = create_block();

  prms.map->figure_p_current.col_pos = 5;
  prms.map->figure_p_current.row_pos = 5;
  prms.map->figure_p_current.rotation = 0;

  for (int i = 0; i < BOARD_ROWS; ++i)
    for (int j = 0; j < BOARD_COLS; ++j) prms.map->board[i][j] = 1;

  ck_assert_int_eq(right_colision(prms.map), TRUE);
  prms.map->figure_p_current.row_pos = 10;
  ck_assert_int_eq(right_colision(prms.map), TRUE);
}
END_TEST

// Test the attach_block function
START_TEST(test_attach_block) {
  board_t board = {0};
  init_board(&board);
  game_state state = START;
  game_stats_t stats = {0};

  stats.speed = 1;
  stats.level = 1;

  params_t prms = {0};
  prms.stats = &stats;
  prms.state = &state;
  prms.map = &board;
  prms.map->figure_p_current = create_block();

  prms.map->figure_p_current.col_pos = 5;
  prms.map->figure_p_current.row_pos = 5;
  prms.map->figure_p_current.rotation = 0;
  while (!floor_colision(prms.map)) prms.map->figure_p_current.row_pos += 1;
  finalize_figure(&prms);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(
          prms.map->board[prms.map->figure_p_current.row_pos + i + 1]
                         [1 + 2 * (prms.map->figure_p_current.col_pos + j)],
          prms.map->figure_p_current.figure.sprite[0][i][j]);
    }
  }
}
END_TEST

// Test the find_full_rows function
START_TEST(test_find_full_rows) {
  board_t board = {0};
  init_board(&board);
  game_state state = START;
  game_stats_t stats = {0};

  stats.speed = 1;
  stats.level = 1;

  params_t prms = {0};
  prms.stats = &stats;
  prms.state = &state;
  prms.map = &board;
  prms.map->figure_p_current = create_block();
  for (int i = 0; i < BOARD_COLS; i++) {
    board.board[5][i] = 1;
  }
  track_fill_line(&prms);
  ck_assert_int_eq(prms.stats->score, 100);
}
END_TEST

// Test the shift_board function
START_TEST(test_shift_board) {
  board_t board = {0};
  init_board(&board);
  game_state state = START;
  game_stats_t stats = {0};

  stats.speed = 1;
  stats.level = 1;

  params_t prms = {0};
  prms.stats = &stats;
  prms.state = &state;
  prms.map = &board;
  prms.map->figure_p_current = create_block();

  for (int i = 0; i < BOARD_COLS; i++) {
    board.board[BOARD_ROWS - 1][i] = 1;
  }
  shift_lines_down(prms.map->board, BOARD_ROWS - 1);
  for (int i = 0; i < BOARD_COLS; i++) {
    ck_assert_int_eq(prms.map->board[0][i], 0);
  }
}
END_TEST

// Test the update_score function
START_TEST(test_update_score) {
  game_stats_t stats = {0};
  game_stat(&stats, 1);
  ck_assert_int_eq(stats.score, 100);
  game_stat(&stats, 2);
  ck_assert_int_eq(stats.score, 400);
  game_stat(&stats, 3);
  ck_assert_int_eq(stats.score, 1100);
  game_stat(&stats, 4);
  ck_assert_int_eq(stats.score, 2600);
  game_stat(&stats, 5);
  ck_assert_int_eq(stats.score, 4100);
}
END_TEST

// Test the save_high_score function
START_TEST(test_save_high_score) {
  game_stats_t stats = {0};
  stats.score = 333;
  ck_assert_int_eq(save_statistic(&stats), 1);
}
END_TEST

// Test the load_high_score function
START_TEST(test_load_high_score) {
  game_stats_t stats = {0};
  stats.hight_score = 333;
  load_statistic(&stats);
  ck_assert_int_eq(stats.hight_score, 333);
}
END_TEST

// FSM UNIT TESTS
// Test get_signal
START_TEST(test_get_signal) {
  ck_assert_int_eq(get_signal(' '), SPACE_BTN);
  ck_assert_int_eq(get_signal(KEY_DOWN), MOVE_DOWN);
  ck_assert_int_eq(get_signal(KEY_LEFT), MOVE_LEFT);
  ck_assert_int_eq(get_signal(KEY_RIGHT), MOVE_RIGHT);
  ck_assert_int_eq(get_signal(ESCAPE), ESCAPE_BTN);
  ck_assert_int_eq(get_signal(ENTER_KEY), ENTER_BTN);
  ck_assert_int_eq(get_signal('p'), PAUSE_BTN);
  ck_assert_int_eq(get_signal('h'), HELP_BTN);
  ck_assert_int_eq(get_signal('q'), ESCAPE_BTN);
}
END_TEST

// Test rotate
START_TEST(test_rotate) {
  board_t board = {0};
  init_board(&board);
  game_state state = MOVING;
  game_stats_t stats = {0};

  stats.speed = 1;
  stats.level = 1;

  params_t prms = {0};
  prms.stats = &stats;
  prms.state = &state;
  prms.map = &board;
  prms.map->figure_p_current = create_block();
  prms.map->figure_p_current.col_pos = 3;
  prms.map->figure_p_current.row_pos = 2;

  ck_assert_int_eq(prms.map->figure_p_current.rotation, 0);
  rotate(&prms);
  ck_assert_int_eq(prms.map->figure_p_current.rotation, 1);
  prms.map->figure_p_current.rotation =
      prms.map->figure_p_current.figure.max_rotation;
  rotate(&prms);
  ck_assert_int_eq(prms.map->figure_p_current.rotation, 0);
  prms.map->figure_p_current.rotation =
      prms.map->figure_p_current.figure.max_rotation;
  prms.map->figure_p_current.col_pos = 11;
  rotate(&prms);
  ck_assert_int_eq(prms.map->figure_p_current.rotation,
                   prms.map->figure_p_current.figure.max_rotation);
  prms.map->figure_p_current.rotation =
      prms.map->figure_p_current.figure.max_rotation;
  prms.map->figure_p_current.col_pos = 5;
  rotate(&prms);
  ck_assert_int_eq(prms.map->figure_p_current.rotation, 0);
  prms.map->figure_p_current.rotation = 0;
  prms.map->figure_p_current.col_pos = 7;
  prms.map->figure_p_current.row_pos = 8;
  rotate(&prms);
  ck_assert_int_eq(prms.map->figure_p_current.rotation, 1);
  prms.map->figure_p_current.rotation = 0;
  prms.map->figure_p_current.col_pos = 6;
  prms.map->figure_p_current.row_pos = 5;
  rotate(&prms);
  ck_assert_int_eq(prms.map->figure_p_current.rotation, 1);
}
END_TEST

// Test movedown
START_TEST(test_movedown) {
  board_t board = {0};
  init_board(&board);
  game_state state = MOVING;
  game_stats_t stats = {0};

  stats.speed = 1;
  stats.level = 1;

  params_t prms = {0};
  prms.stats = &stats;
  prms.state = &state;
  prms.map = &board;
  prms.map->figure_p_current = create_block();

  prms.map->figure_p_current.row_pos = 0;
  movedown(&prms);
  ck_assert_int_eq(prms.map->figure_p_current.row_pos, 1);
  movedown(&prms);
  ck_assert_int_eq(prms.map->figure_p_current.row_pos, 2);
  movedown_key(&prms);
  ck_assert_int_eq(*prms.state, ATTACHING);
}
END_TEST

// Test moveright
START_TEST(test_moveright) {
  board_t board = {0};
  init_board(&board);
  game_state state = MOVING;
  game_stats_t stats = {0};

  stats.speed = 1;
  stats.level = 1;

  params_t prms = {0};
  prms.stats = &stats;
  prms.state = &state;
  prms.map = &board;
  prms.map->figure_p_current = create_block();

  prms.map->figure_p_current.col_pos = 0;

  moveright(&prms);
  ck_assert_int_eq(prms.map->figure_p_current.col_pos, SPAWN);

  moveright(&prms);
  ck_assert_int_eq(prms.map->figure_p_current.col_pos, MOVING);
}
END_TEST

// Test moveleft
START_TEST(test_moveleft) {
  board_t board = {0};
  init_board(&board);
  game_state state = MOVING;
  game_stats_t stats = {0};

  stats.speed = 1;
  stats.level = 1;

  params_t prms = {0};
  prms.stats = &stats;
  prms.state = &state;
  prms.map = &board;
  prms.map->figure_p_current = create_block();

  prms.map->figure_p_current.col_pos = 2;
  moveleft(&prms);
  ck_assert_int_eq(prms.map->figure_p_current.col_pos, 1);

  moveleft(&prms);
  ck_assert_int_eq(prms.map->figure_p_current.col_pos, 0);
}
END_TEST

// Test on_start_state
START_TEST(test_on_start_state) {
  board_t board = {0};
  init_board(&board);
  game_state state = MOVING;
  game_stats_t stats = {0};

  stats.speed = 1;
  stats.level = 1;

  params_t prms = {0};
  prms.stats = &stats;
  prms.state = &state;
  prms.map = &board;
  prms.map->figure_p_current = create_block();

  int signal = 0;
  signal = ENTER_BTN;
  sigact(get_signal(signal), &state, &stats, &board,
         &prms.map->figure_p_current);

  ck_assert_int_eq(state, MOVING);

  signal = ESCAPE_BTN;
  sigact(get_signal(signal), &state, &stats, &board,
         &prms.map->figure_p_current);
  ck_assert_int_eq(state, MOVING);

  signal = MOVE_DOWN;
  sigact(get_signal(signal), &state, &stats, &board,
         &prms.map->figure_p_current);
  ck_assert_int_eq(state, MOVING);
  start_game(&prms);
  ck_assert_int_eq(*prms.state, SPAWN);
  signal = NOSIG;
  sigact(get_signal(signal), &state, &stats, &board,
         &prms.map->figure_p_current);
  ck_assert_int_eq(state, MOVING);
  start_game(&prms);
  ck_assert_int_eq(*prms.state, SPAWN);
}
END_TEST

// Test on_spawn_state
START_TEST(test_on_spawn_state) {
  board_t board = {0};
  init_board(&board);
  game_state state = MOVING;
  game_stats_t stats = {0};

  stats.speed = 1;
  stats.level = 1;
  params_t prms = {0};
  prms.stats = &stats;
  prms.state = &state;
  prms.map = &board;
  prms.map->figure_p_current = create_block();
  prms.map->figure_p_current.row_pos = 9;
  int signal = 0;

  sigact(get_signal(signal), &state, &stats, &board,
         &prms.map->figure_p_current);

  ck_assert_int_eq(state, MOVING);
  exitstate(&prms);
  ck_assert_int_eq(*prms.state, EXIT_STATE);

  spawn(&prms);
  ck_assert_int_eq(*prms.state, MOVING);
  prms.map->figure_p_current.col_pos = 19;
  prms.map->figure_p_next.col_pos = 19;
  spawn(&prms);
  ck_assert_int_eq(*prms.state, 2);
}
END_TEST

// Test on_pause_state
START_TEST(test_on_pause_state) {
  board_t board = {0};
  init_board(&board);
  game_state state = MOVING;
  game_stats_t stats = {0};

  stats.speed = 1;
  stats.level = 1;
  params_t prms = {0};
  prms.stats = &stats;
  prms.state = &state;
  prms.map = &board;
  prms.map->figure_p_current = create_block();
  pause(&prms);
  ck_assert_int_eq(state, PAUSE);
}
END_TEST

// Test on_attach_state
START_TEST(test_on_attach_state) {
  board_t board = {0};
  init_board(&board);
  game_state state = ATTACHING;
  game_stats_t stats = {0};

  stats.speed = 1;
  stats.level = 1;
  params_t prms = {0};
  prms.stats = &stats;
  prms.state = &state;
  prms.map = &board;
  prms.map->figure_p_current = create_block();
  // get_help();
  state_atatch(&prms);
  ck_assert_int_eq(state, MOVING);

  for (int i = BOARD_ROWS; i > 5; i--)
    for (int j = 0; j < BOARD_COLS; j++) prms.map->board[i][j] = 1;
  prms.map->figure_p_current.rotation = 1;
  prms.map->figure_p_current.row_pos = 19;
  state_atatch(&prms);
  ck_assert_int_eq(state, SPAWN);
}
END_TEST

// Test on_game_over_state
START_TEST(test_on_game_over_state) {
  UserAction_t sig = ESCAPE_BTN;
  board_t board = {0};
  init_board(&board);
  game_state state = GAMEOVER;
  game_stats_t stats = {0};
  tetra_t block_pos = {0};
  stats.speed = 1;
  stats.level = 1;
  params_t prms = {0};
  prms.stats = &stats;
  prms.state = &state;
  prms.map = &board;
  prms.map->figure_p_current = create_block();

  sig = ENTER_BTN;
  sigact(sig, &state, &stats, &board, &block_pos);
  ck_assert_int_eq(state, SPAWN);
  state = GAMEOVER;
  sig = ESCAPE_BTN;
  sigact(sig, &state, &stats, &board, &block_pos);
  ck_assert_int_eq(state, EXIT_STATE);
  game_over(&prms);
}
END_TEST

START_TEST(test_gui_1) {
  board_t board = {0};
  init_board(&board);
  game_state state = START;
  game_stats_t stats = {0};
  tetra_t block = {0};
  stats.speed = 1;
  stats.level = 1;

  params_t prms = {0};
  prms.stats = &stats;
  prms.state = &state;
  prms.map = &board;
  prms.block_pos = &block;
  int x = 2;
  print_welcom_scr();
  prms.map->figure_p_current = create_block();
  prms.map->figure_p_next = create_block();
  print_block(prms.map);
  print_debug_state(*prms.map);
  print_game_over_baner();
  prms.map->board[2][4] = 1;
  prms.map->board[2][5] = 1;
  prms.map->board[2][6] = 1;
  print_game_space(prms.map);
  print_overlay();
  print_pause_baner();
  print_next_block(prms.map);
  print_stats(prms.stats);
  attron(COLOR_PAIR(1));
  attroff(COLOR_PAIR(1));
  ck_assert_int_eq(x, BOARDS_BEGIN);
}
END_TEST

START_TEST(right_test) {
  board_t board = {0};
  init_board(&board);
  game_state state = START;
  game_stats_t stats = {0};
  tetra_t block = {0};
  stats.speed = 1;
  stats.level = 1;

  params_t prms = {0};
  prms.stats = &stats;
  prms.state = &state;
  prms.map = &board;
  prms.block_pos = &block;
  prms.map->figure_p_current = create_block();
  prms.map->figure_p_current.col_pos = 20;

  ck_assert_int_eq(right(prms.map), 1);
}
END_TEST

START_TEST(right_test_1) {
  board_t board = {0};
  init_board(&board);
  game_state state = START;
  game_stats_t stats = {0};
  tetra_t block = {0};
  stats.speed = 1;
  stats.level = 1;

  params_t prms = {0};
  prms.stats = &stats;
  prms.state = &state;
  prms.map = &board;
  prms.block_pos = &block;
  prms.map->figure_p_current = create_block();
  prms.map->figure_p_current.col_pos = 5;

  ck_assert_int_eq(right(prms.map), 0);
}
END_TEST

START_TEST(right_test_2) {
  board_t board = {0};
  init_board(&board);
  game_state state = START;
  game_stats_t stats = {0};
  tetra_t block = {0};
  stats.speed = 1;
  stats.level = 1;

  params_t prms = {0};
  prms.stats = &stats;
  prms.state = &state;
  prms.map = &board;
  prms.block_pos = &block;
  prms.map->figure_p_current = create_block();
  prms.map->figure_p_current.col_pos = 6;
  prms.map->figure_p_current.row_pos = 5;
  for (int i = 6; i < 11; i++)
    for (int j = 1; j < 15; j++) prms.map->board[i][j] = 1;
  ck_assert_int_eq(right(prms.map), 0);
}
END_TEST

START_TEST(right_test_3) {
  board_t board = {0};
  init_board(&board);
  game_state state = START;
  game_stats_t stats = {0};
  tetra_t block = {0};
  stats.speed = 1;
  stats.level = 1;

  params_t prms = {0};
  prms.stats = &stats;
  prms.state = &state;
  prms.map = &board;
  prms.block_pos = &block;
  prms.map->figure_p_current = create_block();
  prms.map->figure_p_current.row_pos = 5;

  ck_assert_int_eq(right(prms.map), 0);
}

START_TEST(time_chek) {
  long long int time = get_time();
  long long int time2 = get_time();
  ck_assert_uint_eq(time, time2);
}
END_TEST

Suite *tetris_suite() {
  Suite *test_tetris = suite_create("tetris_suite");
  TCase *tetris_t = tcase_create("tetris_tc");

  tcase_add_test(tetris_t, init_stats_test);
  tcase_add_test(tetris_t, init_board_test);
  tcase_add_test(tetris_t, gen_next_block_test);
  tcase_add_test(tetris_t, test_check_lborder_collide);
  tcase_add_test(tetris_t, test_check_rborder_collide);
  tcase_add_test(tetris_t, test_check_board_collide);
  tcase_add_test(tetris_t, test_attach_block);
  tcase_add_test(tetris_t, test_find_full_rows);
  tcase_add_test(tetris_t, test_shift_board);
  tcase_add_test(tetris_t, test_update_score);
  tcase_add_test(tetris_t, test_save_high_score);
  tcase_add_test(tetris_t, test_load_high_score);
  tcase_add_test(tetris_t, test_get_signal);
  tcase_add_test(tetris_t, test_rotate);
  tcase_add_test(tetris_t, test_movedown);
  tcase_add_test(tetris_t, test_moveright);
  tcase_add_test(tetris_t, test_moveleft);
  tcase_add_test(tetris_t, test_on_start_state);
  tcase_add_test(tetris_t, test_on_spawn_state);
  tcase_add_test(tetris_t, test_on_attach_state);
  tcase_add_test(tetris_t, test_on_game_over_state);
  tcase_add_test(tetris_t, test_gui_1);
  tcase_add_test(tetris_t, right_test);
  tcase_add_test(tetris_t, right_test_1);
  tcase_add_test(tetris_t, right_test_2);
  tcase_add_test(tetris_t, right_test_3);
  tcase_add_test(tetris_t, time_chek);
  tcase_add_test(tetris_t, test_on_pause_state);

  suite_add_tcase(test_tetris, tetris_t);
  return test_tetris;
}

int main() {
  Suite *test_tetris = tetris_suite();
  SRunner *tetris = srunner_create(test_tetris);
  int count = 0;

  srunner_set_fork_status(tetris, CK_NOFORK);
  srunner_run_all(tetris, CK_VERBOSE);
  count = srunner_ntests_failed(tetris);
  srunner_free(tetris);
  return (count == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}