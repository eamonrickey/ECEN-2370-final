#include "ApplicationCode.h"

volatile uint8_t drop_requested = 0;
static uint8_t board[ROWS][COLS];
static uint32_t game_time = 0;
static uint32_t player_one_score = 0;
static uint32_t player_two_score = 0;

extern void initialise_monitor_handles(void); 

#if COMPILE_TOUCH_FUNCTIONS == 1
static STMPE811_TouchData StaticTouchData;
#endif // COMPILE_TOUCH_FUNCTIONS


void ApplicationInit()
{
	initialise_monitor_handles(); // Allows printf functionality
    LTCD__Init();
    LTCD_Layer_Init(0);
    LCD_Clear(0,LCD_COLOR_WHITE);
    ButtonInit();
    RNG_Init();
    Timer_Init();

    #if COMPILE_TOUCH_FUNCTIONS == 1
	InitializeLCDTouch();

	// This is the orientation for the board to be direclty up where the buttons are vertically above the screen
	// Top left would be low x value, high y value. Bottom right would be low x value, low y value.
	StaticTouchData.orientation = STMPE811_Orientation_Portrait_2;

	#endif // COMPILE_TOUCH_FUNCTIONS
}

void Main_menu_screen()
{
	LCD_Clear(0,LCD_COLOR_WHITE);
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_SetFont(&Font16x24);

	LCD_Draw_Horizontal_Line(1, 160, 238, LCD_COLOR_BLACK);
	//LCD_Draw_Horizontal_Line(1, 240, 238, LCD_COLOR_BLACK);

	LCD_DisplayChar(85,90,'1'); //  (y position, x position)
	LCD_DisplayChar(90,90,'-');
	LCD_DisplayChar(100,90,'P');
	LCD_DisplayChar(110,90,'l');
	LCD_DisplayChar(120,90,'a');
	LCD_DisplayChar(130,90,'y');
	LCD_DisplayChar(140,90,'e');
	LCD_DisplayChar(150,90,'r');

	LCD_DisplayChar(83,240,'2'); //  (y position, x position)
	LCD_DisplayChar(90,240,'-');
	LCD_DisplayChar(100,240,'P');
	LCD_DisplayChar(110,240,'l');
	LCD_DisplayChar(120,240,'a');
	LCD_DisplayChar(130,240,'y');
	LCD_DisplayChar(140,240,'e');
	LCD_DisplayChar(150,240,'r');
}

void Game_board_screen()
{
	LCD_Clear(0,LCD_COLOR_WHITE);

	LCD_Draw_Horizontal_Line(1, 35, 238, LCD_COLOR_BLACK);
	LCD_Draw_Horizontal_Line(1, 70, 238, LCD_COLOR_BLACK);
	LCD_Draw_Horizontal_Line(1, 105, 238, LCD_COLOR_BLACK);
	LCD_Draw_Horizontal_Line(1, 140, 238, LCD_COLOR_BLACK);
	LCD_Draw_Horizontal_Line(1, 175, 238, LCD_COLOR_BLACK);
	LCD_Draw_Horizontal_Line(1, 210, 238, LCD_COLOR_BLACK);
	LCD_Draw_Horizontal_Line(1, 245, 238, LCD_COLOR_BLACK);

	LCD_Draw_Vertical_Line(40,1,245,LCD_COLOR_BLACK);
	LCD_Draw_Vertical_Line(80,1,245,LCD_COLOR_BLACK);
	LCD_Draw_Vertical_Line(120,1,245,LCD_COLOR_BLACK);
	LCD_Draw_Vertical_Line(160,1,245,LCD_COLOR_BLACK);
	LCD_Draw_Vertical_Line(200,1,245,LCD_COLOR_BLACK);

	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
		    if (board[row][col] == 1)
		    {
		    	LCD_Draw_Circle_Fill(20 + (row*ROW_STEP), 18 + (col*COL_STEP), 15, LCD_COLOR_RED);
		    }
		    else if (board[row][col] == 2)
		    {
		    	LCD_Draw_Circle_Fill(20 + (row*ROW_STEP), 18 + (col*COL_STEP), 15, LCD_COLOR_BLACK);
		    }
		}
	}
}

void Game_Over_Screen()
{
	LCD_Clear(0,LCD_COLOR_WHITE);

	game_time = Timer_GetGameTime();

	char red_score_text[10];
	char black_score_text[10];
	char game_time_text[10];
	sprintf(red_score_text, "%lu", player_one_score);
	sprintf(black_score_text, "%lu", player_two_score);
	sprintf(game_time_text, "%lu", game_time);

	LCD_DisplayChar(80,40,'R'); //  (y position, x position)
	LCD_DisplayChar(90,40,'e');
	LCD_DisplayChar(100,40,'d');
	LCD_DisplayChar(105,40,':');
	LCD_DisplayChar(115,40, red_score_text[0]);

	LCD_DisplayChar(80,80,'B'); //  (y position, x position)
	LCD_DisplayChar(90,80,'l');
	LCD_DisplayChar(100,80,'a');
	LCD_DisplayChar(110,80,'c');
	LCD_DisplayChar(120,80,'k');
	LCD_DisplayChar(125,80,':');
	LCD_DisplayChar(135,80, black_score_text[0]);

	LCD_DisplayChar(80,120,'T'); //  (y position, x position)
	LCD_DisplayChar(90,120,'i');
	LCD_DisplayChar(100,120,'m');
	LCD_DisplayChar(110,120,'e');
	LCD_DisplayChar(120,120,':');

	uint32_t x = 130;
	for (int i = 0; game_time_text[i] != '\0'; i++)
	{
	    LCD_DisplayChar(x, 120, game_time_text[i]);
	    x += 10;
	}

	LCD_Draw_Horizontal_Line(1, 160, 238, LCD_COLOR_BLACK);

	LCD_DisplayChar(80,240,'R'); //  (y position, x position)
	LCD_DisplayChar(90,240,'e');
	LCD_DisplayChar(100,240,'p');
	LCD_DisplayChar(110,240,'l');
	LCD_DisplayChar(120,240,'a');
	LCD_DisplayChar(130,240,'y');
	LCD_DisplayChar(140,240,'?');

}

void Game_loop()
{
	enum status game_status = main_menu;
	enum mode game_mode;
	while(1)
	{
		switch(game_status)
		{
		case main_menu:
			Main_menu_screen();
			game_mode = MM_Touch_Polling();
			game_status = game_board;
			break;
		case game_board:
			Reset_Board(board);
			Reset_Time(&game_time);
			Timer_Start();
			if(game_mode == one_player)
			{
				One_Player_Gameplay_loop();
			}
			else
			{
				Two_Player_Gameplay_loop();
			}
			Timer_Stop();
			game_status = game_over;
			break;
		case game_over:
			Game_Over_Screen();
			game_status = Replay_Touch_Polling();
			break;
		}
	}

}


enum mode MM_Touch_Polling()
{
	while (1)
	{
		//1player
		if(returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed && (StaticTouchData.y<160))
		{
			return two_player;
		}
		//2player
		else if(returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed && (StaticTouchData.y>160))
		{
			/* Touch not pressed */
			return one_player;
		}
	}
}

void Game_Touch_Polling(uint8_t pos, uint8_t player_touch)
{
	bool is_turn = 1;
	uint8_t col = 0;
	while (is_turn)
	    {
	        HAL_Delay(DELAY);
	        Game_board_screen();
	        // Draw preview coin
	        if (player_touch == PLAYER_ONE)
	        {
	            LCD_Draw_Circle_Fill(20, pos, 15, LCD_COLOR_RED);
	        }
	        else
	        {
	            LCD_Draw_Circle_Fill(20, pos, 15, LCD_COLOR_BLACK);
	        }

	        if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed)
	        {
	            if (StaticTouchData.y > 160)
	            {
	                if (pos > POS_MIN)
	                {
	                    pos -= COL_STEP;
	                }
	            }
	            else
	            {
	                if (pos < POS_MAX)
	                {
	                    pos += COL_STEP;
	                }
	            }
	        }

	        if(drop_requested)
	        {
	        	col = pos / COL_STEP;
	        	if(drop_coin(board, player_touch, col))
	        	{
	        		drop_requested = 0;
	        		is_turn = 0;
	        	}
	        }
	    }
}

enum status Replay_Touch_Polling()
{
	while(1)
	{
		if(returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed && (StaticTouchData.y<160))
		{
			return game_board;
		}
	}
}

void RNG_drop_coin()
{
	uint32_t randomNum;
	bool is_turn = 1;
	while(is_turn)
	{
		HAL_Delay(DELAY);
		Game_board_screen();
		RNG_Get_Number(&randomNum);
		randomNum = randomNum % 7;
		if(drop_coin(board, PLAYER_TWO, randomNum))
		{
			is_turn = 0;
		}
	}
}

void Two_Player_Gameplay_loop()
{
	uint8_t turn = PLAYER_ONE;
	bool game_on = true;
	uint8_t start_position = 53;
	printf("Two Player Mode\n");

	while(game_on)
	{
		switch(turn)
		{
		case PLAYER_ONE:
			while(turn == PLAYER_ONE)
			{
				Game_Touch_Polling(start_position, PLAYER_ONE);
				game_on = check_win(board, PLAYER_ONE);
				turn = PLAYER_TWO;
			}
			break;
		case PLAYER_TWO:
			while(turn == PLAYER_TWO)
			{
				Game_Touch_Polling(start_position, PLAYER_TWO);
				game_on = check_win(board, PLAYER_TWO);
				turn = PLAYER_ONE;
			}
			break;
		}
	}
}

void One_Player_Gameplay_loop()
{
	uint8_t turn = PLAYER_ONE;
	bool game_on = true;
	uint8_t start_position = 53;
	printf("One Player Mode\n");

	while(game_on)
	{
		switch(turn)
		{
		case PLAYER_ONE:
			while(turn == PLAYER_ONE)
			{
				Game_Touch_Polling(start_position, PLAYER_ONE);
				game_on = check_win(board, PLAYER_ONE);
				turn = PLAYER_TWO;
			}
			break;
		case PLAYER_TWO:
			while(turn == PLAYER_TWO)
			{
				RNG_drop_coin();
				game_on = check_win(board, PLAYER_TWO);
				turn = PLAYER_ONE;
			}
			break;
		}
	}
}

bool drop_coin(uint8_t board[ROWS][COLS], uint8_t player_drop, uint8_t col)
{
    if (col < 0 || col >= COLS)
        return false;  // Invalid column

    for (int row = ROWS - 1; row >= 0; row--)
    {
        if (board[row][col] == 0)
        {
            board[row][col] = player_drop;
            return true;
        }
    }

    return false;  // Column is full
}


bool check_win(uint8_t board[ROWS][COLS], uint8_t player)
{
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
        	if (board[row][col] != player)
        	{
            	continue;
        	}

        	// ---- Check Horizontal (right) ----
        	if (col + 3 < COLS &&
        	board[row][col + 1] == player &&
        	board[row][col + 2] == player &&
			board[row][col + 3] == player)
        	{
        		if(player == PLAYER_ONE)
        	    {
        	    player_one_score++;
        	    }
        	    else
        	    {
        	    player_two_score++;
        	    }
        	    return false;
        	}
        	// ---- Check Vertical (down) ----
        	if (row + 3 < ROWS &&
        	board[row + 1][col] == player &&
        	board[row + 2][col] == player &&
        	board[row + 3][col] == player)
        	{
        		if(player == PLAYER_ONE)
        		{
        			player_one_score++;
        		}
        		else
        		{
        			player_two_score++;
        		}
        		return false;
        	}

        	// ---- Check Diagonal (down-right) ----
        	if (row + 3 < ROWS && col + 3 < COLS &&
        	board[row + 1][col + 1] == player &&
        	board[row + 2][col + 2] == player &&
        	board[row + 3][col + 3] == player)
        	{
        		if(player == PLAYER_ONE)
        		{
        			player_one_score++;
        		}
        		else
        		{
        			player_two_score++;
        	    }
        	    return false;
        	}

        	// ---- Check Diagonal (down-left) ----
        	if (row + 3 < ROWS && col - 3 >= 0 &&
        	board[row + 1][col - 1] == player &&
        	board[row + 2][col - 2] == player &&
        	board[row + 3][col - 3] == player)
        	{
        		if(player == PLAYER_ONE)
        	    {
        			player_one_score++;
        	    }
        	    else
        	    {
        	    	player_two_score++;
        	    }
        	    return false;
        	}
        }
    }

    // No win found
    return true;
}

uint32_t Get_Time()
{
	return game_time;
}

void Reset_Time(uint32_t *game_time)
{
	Timer_Reset();
	*game_time = 0;
}

void Reset_Board(uint8_t board[ROWS][COLS])
{
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            board[row][col] = 0;
        }
    }
}

#if COMPILE_TOUCH_FUNCTIONS == 1
void LCD_Touch_Polling_Demo(void)
{
	LCD_Clear(0,LCD_COLOR_GREEN);
	while (1) {
		/* If touch pressed */
		if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
			/* Touch valid */
			printf("\nX: %03d\nY: %03d\n", StaticTouchData.x, StaticTouchData.y);
			LCD_Clear(0, LCD_COLOR_RED);
		} else {
			/* Touch not pressed */
			LCD_Clear(0, LCD_COLOR_GREEN);
		}
	}
}
#endif // COMPILE_TOUCH_FUNCTIONS

void EXTI0_IRQHandler()
{
	IRQ_disableInterrupt(EXTI0_IRQn);
	drop_requested = 1;
	 __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
	IRQ_enableInterrupt(EXTI0_IRQn);
}




