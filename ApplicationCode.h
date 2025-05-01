/*
 * ApplicationCode.h
 *
 *  Created on: Dec 30, 2023
 *      Author: Xavion
 *  Perfected on: April 30, 2025
 *    Sage: Eamon
 */

#include "LCD_Driver.h"
#include "Button_Driver.h"
#include "Timer_Driver.h"
#include "RNG_Driver.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>


#ifndef INC_APPLICATIONCODE_H_
#define INC_APPLICATIONCODE_H_

#define PLAYER_ONE 	1
#define PLAYER_TWO	2
#define DELAY 		100
#define POS_MIN 	20       // Leftmost column (in pixels)
#define POS_MAX		230      // Rightmost column (adjust based on your screen layout)
#define COL_STEP 	35
#define ROW_STEP	40
#define ROWS 		6
#define COLS 		7


enum mode{
	one_player = 1,
	two_player = 2,
};

enum status{
	main_menu,
	game_board,
	game_over,
};

void ApplicationInit();

//Game screens
void Main_menu_screen();
void Game_board_screen();
void Game_Over_Screen();

// Game Loops
void Game_loop();
void Two_Player_Gameplay_loop();
void One_Player_Gameplay_loop();

//AI functions
void RNG_drop_coin();
void AI_Opponent();
bool Simualte_Move(uint8_t board[ROWS][COLS], uint8_t player_drop);

//Game Polling
void Game_Touch_Polling(uint8_t pos, uint8_t player_touch);
enum mode MM_Touch_Polling();
enum status Replay_Touch_Polling();

//Misc
bool drop_coin(uint8_t board[ROWS][COLS], uint8_t player_drop, uint8_t col);
bool check_win(uint8_t board[ROWS][COLS], uint8_t player);
void Reset_Board(uint8_t board[ROWS][COLS]);

//Timer functions
uint32_t Get_Time();
void Reset_Time(uint32_t *game_time);

#if (COMPILE_TOUCH_FUNCTIONS == 1)
void LCD_Touch_Polling_Demo(void);
#endif // (COMPILE_TOUCH_FUNCTIONS == 1)


#endif /* INC_APPLICATIONCODE_H_ */
