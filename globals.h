#pragma once
#include <genesis.h>

//===============================================================================
//								VARIABLES
//===============================================================================
//Playfield consts
const int LEFT_EDGE = 0;
const int RIGHT_EDGE = 320;
const int TOP_EDGE = 0;
const int BOTTOM_EDGE = 224;

//ball variables
Sprite* ball;
int ball_pos_x = 100;
int ball_pos_y = 100;
float ball_vel_x = 1.0f;
float ball_vel_y = 1.0f;
int ball_width = 8;
int ball_height = 8;

//Player variables
Sprite* player;
int player_pos_x = 144;
const int player_pos_y = 200;
int player_vel_x = 0;
const int player_width = 32;
const int player_height = 8;

//Scores
int score = 0;
char label_score[6] = "SCORE\0";
char str_score[3] = "0";

//Game state
game_on = FALSE;
char msg_start[13] = "PRESS START\0";
char msg_reset[10] = "TRY AGAIN?";

//Flashing
int flashing = FALSE;
int frames = 0;
int ball_color = 0; //unused?
int ball_palette[10];
int ball_pal_nums[10];
