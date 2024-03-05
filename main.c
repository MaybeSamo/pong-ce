/*
 *--------------------------------------
 * Program Name: Pong
 * Author: MaybeSamo
 * License:
 * Description: Pong for the Ce
 *--------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <graphx.h>
#include <fileioc.h>
#include <keypadc.h>
#include <intce.h>
#include <tice.h>
#include <math.h>

#define RIGHT_DOWN 1
#define RIGHT_UP 2
#define LEFT_UP 3
#define LEFT_DOWN 4

#define STATE_MENU 0
#define STATE_GAME 1
#define STATE_GAME_OVER 2

int ballx = 80;
int bally = 80;
int balldirection = RIGHT_DOWN;
int ballspeed = 5;
char ballspeedStr[20];

int paddlex = 0;
int paddley = 0;
int paddle_width = 3;
int paddle_height = 50;

int score = 0;
char scoreStr[10];

state = STATE_MENU;

int mainMenuSelection = 0;

void intToString(int num, char *str) {
    int i = 0;
    int isNegative = 0;

    // Handle negative numbers
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    // Convert each digit to a character
    while (num != 0) {
        str[i++] = '0' + (num % 10);
        num /= 10;
    }

    // Add a negative sign if necessary
    if (isNegative) {
        str[i++] = '-';
    }

    // Null-terminate the string
    str[i] = '\0';

    // Reverse the string
    int len = i;
    for (int j = 0; j < len / 2; j++) {
        char temp = str[j];
        str[j] = str[len - j - 1];
        str[len - j - 1] = temp;
    }
}

void RenderGameOver() {

}

void RenderMainMenu() {
    gfx_SetColor(0x00);
    gfx_SetTextScale(2, 2);
    gfx_PrintStringXY("Pong", 10, 10);
    gfx_PrintStringXY("Play", 10, 50);
    gfx_PrintStringXY("Ball Speed: ", 10, 90);
    intToString(ballspeed, ballspeedStr);
    gfx_PrintStringXY(ballspeedStr, 250, 90);

    if(kb_IsDown(kb_Down) && mainMenuSelection != 1) {
        gfx_FillScreen(0xFF);
        mainMenuSelection = 1;
    }

    if(kb_IsDown(kb_Up) && mainMenuSelection != 0) {
        gfx_FillScreen(0xFF);
        mainMenuSelection = 0;
    }

    if (kb_IsDown(kb_Right) && mainMenuSelection == 1) {
        delay(100);
        gfx_FillScreen(0xFF);
        ballspeed += 5;
    }

    if (kb_IsDown(kb_Left) && mainMenuSelection == 1) {
        delay(100);
        gfx_FillScreen(0xFF);
        ballspeed -= 5;
    }
    
    if (kb_IsDown(kb_Right) && mainMenuSelection == 0) {
        gfx_FillScreen(0x00);
        state = STATE_GAME;
    }

    if (mainMenuSelection == 0) {
        gfx_Rectangle(10, 50, 100, 20);
    }
    if (mainMenuSelection == 1) {
        gfx_Rectangle(10, 90, 200, 20);
    }
}

void RenderMainGame() {
    gfx_SetColor(0x00);
    gfx_SetTextScale(1, 1);
    gfx_PrintStringXY("Score", 10, 10);
    gfx_FillRectangle(paddlex, paddley, paddle_width, paddle_height);
    gfx_FillRectangle(ballx, bally, 5, 5);
    intToString(score, scoreStr);

    if (balldirection == RIGHT_DOWN) {
        ballx += ballspeed;
        bally += ballspeed;
        if(bally > gfx_lcdHeight - 10) {
            balldirection = RIGHT_UP;
        }
        if (ballx > gfx_lcdWidth - 10) {
            balldirection = LEFT_DOWN;
        }
    }

    if (balldirection == RIGHT_UP) {
        ballx += ballspeed;
        bally -= ballspeed;
        if (bally == 0) {
            balldirection = RIGHT_DOWN;
        }
        if (ballx >= gfx_lcdWidth - 10) {
            balldirection = LEFT_UP;
        }
    }

    if (balldirection == LEFT_UP) {
        ballx -= ballspeed;
        bally -= ballspeed;
        if (bally == 0) {
            balldirection = LEFT_DOWN;
        }
        if (ballx == 0) {
            balldirection = RIGHT_UP;
        }
    }

    if (balldirection == LEFT_DOWN) {
        ballx -= ballspeed;
        bally += ballspeed;
        if (bally > gfx_lcdHeight - 10) {
            balldirection = LEFT_UP;
        }
        if (ballx == 0) {
            balldirection = RIGHT_DOWN;
        }
    }


    //Passing the paddle (losing)
    if (ballx <= 0) {
        gfx_FillScreen(0xFF);
        state = STATE_MENU;
    }

    //Hitting the paddle
    if (ballx <= paddlex) {
        if (bally >= paddley && bally <= paddley + 50) {
            score++;
            if (balldirection == LEFT_DOWN) {
                balldirection = RIGHT_DOWN;
            }
            if (balldirection == LEFT_UP) {
                balldirection = RIGHT_UP;
            }
        }
    }

    if (kb_IsDown(kb_Down) && paddley < gfx_lcdHeight - paddle_height) {
        paddley += 10; 
    }
    if (kb_IsDown(kb_Up) && paddley > 0) {
        paddley -= 10; 
    }

    gfx_SetColor(0xFF);
    gfx_SetTextScale(1, 1);
    gfx_PrintStringXY("Score", 10, 10);
    gfx_FillRectangle(paddlex, paddley, paddle_width, paddle_height);
    gfx_FillRectangle(ballx, bally, 5, 5);

    delay(50);
}

void main(void)
{
    gfx_Begin();

    gfx_FillScreen(0xFF);

    paddlex = 10; 
    paddley = 50; 

    while (!kb_IsDown(kb_KeyClear)) {
        if (state == STATE_GAME) {

            RenderMainGame();
        }
        if (state == STATE_MENU) {
            RenderMainMenu();
        }
        if (state == STATE_GAME_OVER) {
            RenderGameOver();
        }
    }

    gfx_End();
}
