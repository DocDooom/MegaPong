#include <genesis.h>
#include <resources.h>
#include "prototypes.h"
#include "globals.h"
#include <string.h>
#include "tools.h"

//Main Entry Point
int main()
{
	//===============================================================================
	//								INIT JOY PADS
	//===============================================================================
	JOY_init();
	JOY_setEventHandler(&paddleHandler);

	//===============================================================================
	//								TILES
	//===============================================================================
	//Set Tiles
	VDP_loadTileSet(bgtile.tileset, 1, DMA);

	//Set Palettes
	VDP_setPalette(PAL2, bgtile.palette->data);
	VDP_setPalette(PAL1, imgball.palette->data);

	VDP_fillTileMapRect(PLAN_B, TILE_ATTR_FULL(PAL2, 0, FALSE, FALSE, 1), 0, 0, 40, 30);
	//===============================================================================
	//								SCORES & SCREEN TEXT
	//===============================================================================
	//Draw Score Text
	VDP_setTextPlan(PLAN_A);
	VDP_drawText(label_score, 1, 1);
	updateScoreDisplay();
	showText(msg_start);
	
	//===============================================================================
	//								SPRITES
	//===============================================================================
	SPR_init();

	ball = SPR_addSprite(&imgball, 100, 100, TILE_ATTR(PAL1, 0, FALSE, FALSE));
	//ball_color = VDP_getPaletteColor(22);

	for(int i = 16, j = 0; i < 26; i++, j++)
	{
		ball_palette[j] = VDP_getPaletteColor(i);
		ball_pal_nums[j] = i;
	}
	
	player = SPR_addSprite(&paddle, player_pos_x, player_pos_y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
	
	while (1)
	{
		if(game_on == TRUE)
		{
			moveBall();
			positionPlayer();

			//Flashing when ball is hit
			if(flashing == TRUE)
			{
				frames++;
				if(frames % 4 == 0)
				{
					for(int i = 0; i < 10; i++)
					{
						VDP_setPaletteColor(ball_pal_nums[i], ball_palette[i]);
					}
				}
				else if(frames % 2 == 0)
				{
					for (int i = 0; i < 10; i++)
					{
						VDP_setPaletteColor(ball_pal_nums[i], RGB24_TO_VDPCOLOR(0xffffff));
					}
				}
				if(frames > 8)
				{
					flashing = FALSE;
					frames = 0;
					for (int i = 0; i < 10; i++)
					{
						VDP_setPaletteColor(ball_pal_nums[i], ball_palette[i]);
					}
				}
			}
		}
		SPR_update();
		VDP_waitVSync(); //wait for screen refresh
	}
	return 0;
}

void moveBall()
{
	//Check bounds
	if (ball_pos_y < TOP_EDGE)
	{
		ball_pos_y = TOP_EDGE;
		ball_vel_y = -ball_vel_y;
	}
	else if (ball_pos_y + ball_height > BOTTOM_EDGE)
	{
		endGame();
	}
	if (ball_pos_x < LEFT_EDGE)
	{
		ball_pos_x = LEFT_EDGE;
		ball_vel_x = -ball_vel_x;
	}
	else if (ball_pos_x + ball_width > RIGHT_EDGE)
	{
		ball_pos_x = RIGHT_EDGE - ball_width;
		ball_vel_x = -ball_vel_x;
	}

	//Collision handling
	if (ball_pos_x < player_pos_x + player_width && ball_pos_x + ball_width > player_pos_x)
	{
		if (ball_pos_y < player_pos_y + player_height && ball_pos_y + ball_height >= player_pos_y)
		{
			//invert the velocity on collision
			ball_pos_y = player_pos_y - ball_height - 1;
			ball_vel_y = -ball_vel_y;
			score++;
			flashing = TRUE;
			updateScoreDisplay();

			//make ball faster when scoring more and more
			if(score % 3 == 0 )
			{
				ball_vel_x += sign(ball_vel_x);
				ball_vel_y += sign(ball_vel_y);
			}
		}
	}

	ball_pos_x += ball_vel_x;
	ball_pos_y += ball_vel_y;

	SPR_setPosition(ball, ball_pos_x, ball_pos_y);
}

void paddleHandler(u16 joy, u16 changed, u16 state)
{
	if (joy == JOY_1)
	{
		if(state & BUTTON_START)
		{
			if(!game_on)
			{
				startGame();
			}
		}
		//Set player velocity if lef or right are pressed
		//Set velocity to 0 if no direction is pressed
		if (state & BUTTON_RIGHT)
		{
			player_vel_x = 6;
		}
		else if (state & BUTTON_LEFT)
		{
			player_vel_x = -6;
		}
		else
		{
			if ((changed & BUTTON_RIGHT) | (changed & BUTTON_LEFT))
			{
				player_vel_x = 0;
			}
		}
	}
}

void positionPlayer()
{
	//Add the player's velocity to its position
	player_pos_x += player_vel_x;

	//Keep the player within the bounds of the screen
	if (player_pos_x < LEFT_EDGE) player_pos_x = LEFT_EDGE;
	if (player_pos_x + player_width > RIGHT_EDGE) player_pos_x = RIGHT_EDGE - player_width;

	//Let the Sprite engine position the sprite
	SPR_setPosition(player, player_pos_x, player_pos_y);
}

int sign(int x)
{
	return (x > 0) - (x < 0);
}

void updateScoreDisplay()
{
	sprintf(str_score, "%d", score);
	VDP_clearText(1, 2, 3);
	VDP_drawText(str_score, 1, 2);
}

void showText(char s[])
{
	VDP_drawText(s, 20 - strlen(s) / 2, 15);
}

void endGame()
{
	showText(msg_reset);
	game_on = FALSE;
}

void startGame()
{
	score = 0;
	updateScoreDisplay();

	ball_pos_x = 0;
	ball_pos_y = 0;

	ball_vel_x = 1;
	ball_vel_y = 1;

	//Clear text from screen
	VDP_clearTextArea(0, 10, 40, 10);

	game_on = TRUE;
}
