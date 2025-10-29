#ifndef FSM
#define FSM

#include "../../gui/cli/frontend.h"
#include "defines.h"
#include "object.h"
#include "tetris_backend.h"

// typedef struct game_params // структура которая будет аккумулировать
// состояние игры для того чтобы после запихнуть в функции
// {
//     game_stats_t *stats;
//     game_state *state;
//     board_t *map;
//     tetra_t *block_pos;
//     bool *break_flag;
// } params_t;

// typedef void (*action)(params_t *prms); // это объявление нового типа данных
// action который является указателем на функцю шаблона void (params_t *prms),
//                                         // используем для хранения указателей
//                                         на фунции

void get_help();

void rotate(params_t *prms);
void movedown(params_t *prms);
void moveright(params_t *prms);
void moveleft(params_t *prms);
void pause(params_t *prms);

void game_over(params_t *prms);  // params_t *prms

void movedown_key(params_t *prms);

void state_atatch(params_t *prms);
void start_game(params_t *prms);
void exitstate(params_t *prms);
void spawn(params_t *prms);
void state_move(params_t *prms);
void state_pause(params_t *prms);
void state_gameover(params_t *prms);

void sigact(UserAction_t sig, game_state *state, game_stats_t *stats,
            board_t *map, tetra_t *block_pos);

UserAction_t get_signal(int user_input);

#endif