#ifndef CLI_H
#define CLI_H

/*****************************************************************************
 * @file tetris_cli.h
 * @brief GUI CLI Header File
 *****************************************************************************/

#define _XOPEN_SOURCE_EXTENDED

#include <locale.h>
#include <ncurses.h>
#include <unistd.h>
#include <wchar.h>

#include "../../brick_game/tetris/tetris_logic.h"

#define FIELD_SIZE_X 10
#define FIELD_SIZE_Y 20
#define INFO_SIZE_X 10
#define INFO_SIZE_Y 20
#define SPEED_RATE 0.16
#define FRAME_RATE 60     // Hz
#define READ_DELAY 16.67  // ms

/*****************************************************************************
 * @brief GUI initialization
 *
 * Initialize ncurses CLI window, settings and colors
 *****************************************************************************/
void initGUI(void);

/*****************************************************************************
 * @brief Main loop of game
 *
 * Main loop of game with drawing screens and processing user input
 *****************************************************************************/
void gameLoop(void);

/*****************************************************************************
 * @brief Draw start screen
 *
 * Draw static start screen
 *****************************************************************************/
void drawStartScreen(GameInfo_t *data);

/*****************************************************************************
 * @brief Draw GUI
 *
 * Draw static part of GUI
 *****************************************************************************/
void drawGUI(void);

/*****************************************************************************
 * @brief Draw info block
 *
 * Draw info block with game data and preview with colored next figure
 *****************************************************************************/
void drawInfo(GameInfo_t *data);

/*****************************************************************************
 * @brief Draw field of game
 *
 * Draw colored field of game
 *****************************************************************************/
void drawField(int **field);

/*****************************************************************************
 * @brief Draw screen of game over
 *
 * Draw static screen of game over
 *****************************************************************************/
void drawGameOver(GameInfo_t *data);

/*****************************************************************************
 * @brief Get user action
 *
 * Get user action enum value depending on pressed button
 *
 * @param pressedKey Keyboard button char or int
 * @return UserAction_t
 *****************************************************************************/
UserAction_t getAction(int pressedKey);

/*****************************************************************************
 * @brief GUI destruction
 *
 * Destroy ncurses window and closing application
 *****************************************************************************/
void destroyGUI(void);

#endif  // CLI_H
