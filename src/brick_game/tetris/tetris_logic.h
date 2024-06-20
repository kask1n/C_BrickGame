#ifndef TETRIS_H
#define TETRIS_H

/*****************************************************************************
 * @file tetris_logic.h
 * @brief Header File with Logic of the Tetris Game
 *****************************************************************************/

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI_2 1.57079632679489661923

#define FIELD_WIDTH 16
#define FIELD_HEIGHT 26
#define FIGURE_WIDTH 4
#define FIGURE_HEIGHT 2
#define BORDER_SIZE 3
#define PIXEL_EMPTY 0

#define STATES_COUNT 3
#define SIGNALS_COUNT 8
#define FIGURES_COUNT 7

#define DATA_PATH "./data"

#define SPEED_MIN 1
#define SPEED_MAX 10
#define LEVEL_MIN 1
#define LEVEL_MAX 10
#define LEVEL_THRESHOLD 600

#define SCORE_ROWS_1 100
#define SCORE_ROWS_2 300
#define SCORE_ROWS_3 700
#define SCORE_ROWS_4 1500

#define ROTATION_MIN 0
#define ROTATION_MAX 3

/*****************************************************************************
 * @brief Game data struct
 *
 * Game logic data
 *
 * @param field Game field with borders
 * @param next Next spawn figure for preview
 * @param score Current game score
 * @param high_score Game high score from file
 * @param level Current game level: [1..10]
 * @param speed Current game speed: [1..10]
 * @param pause Pause flag
 *****************************************************************************/
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

/*****************************************************************************
 * @brief States for FSM
 *
 * States of game, used as rows for FSM table
 *****************************************************************************/
typedef enum { START = 0, GAME, GAME_OVER } GameState_t;

/*****************************************************************************
 * @brief Figure data struct
 *
 * Current and next figure parameters
 *
 * @param typeNext Type of next figure
 * @param type Type of current figure
 * @param rotation Number of rotation to PI/2 angle: [0..3]
 * @param x X coordinate of figure center
 * @param y Y coordinate of figure center
 *****************************************************************************/
typedef struct {
  int typeNext;
  int type;
  int rotation;
  int x;
  int y;
} Figure_t;

/*****************************************************************************
 * @brief Struct of game parameters
 *
 * All game parameters with current field, figure and state
 *
 * @param data Game data with field
 * @param state Game current state
 * @param isActive Flag for activate game loop
 * @param figure Current figure data
 *****************************************************************************/
typedef struct {
  GameInfo_t *data;
  GameState_t state;
  bool isActive;
  Figure_t *figure;
} GameParameters_t;

/*****************************************************************************
 * @brief Signals for FSM
 *
 * User input, used as signals (columns) for FSM table
 *****************************************************************************/
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

/*****************************************************************************
 * @brief Initialize game parameters
 *
 * Initialize game parameters: allocate memory for arrays and assign initial
 *values to game data
 *
 * @param parameters Pointer to struct of GameParameters_t
 *****************************************************************************/
void initializeParameters(GameParameters_t *parameters);

/*****************************************************************************
 * @brief Update game parameters
 *
 * Update game parameters in static variable
 *
 * @param parameters Pointer to struct of GameParameters_t
 * @return GameParameters_t
 *****************************************************************************/
GameParameters_t *updateParameters(GameParameters_t *parameters);

/*****************************************************************************
 * @brief Update current game state
 *
 * Update current game state by shifting current figure down one pixel
 * @return GameInfo_t
 *****************************************************************************/
GameInfo_t updateCurrentState(void);

/*****************************************************************************
 * @brief Shift figure down
 *
 * Shift figure down one pixel if possible or activate attach function
 *
 * @param parameters Pointer to struct of GameParameters_t
 *****************************************************************************/
void shiftFigure(GameParameters_t *parameters);

/*****************************************************************************
 * @brief Remove figure from field
 *
 * Remove figure from field
 *
 * @param parameters Pointer to struct of GameParameters_t
 *****************************************************************************/
void clearFigure(GameParameters_t *parameters);

/*****************************************************************************
 * @brief Check if figure collides
 *
 * Check if figure collides during moving, shifting and rotating
 *
 * @param parameters Pointer to struct of GameParameters_t
 * @return bool
 *****************************************************************************/
bool isFigureNotCollide(GameParameters_t *parameters);

/*****************************************************************************
 * @brief Add figure to the field
 *
 * Add figure to the field
 *
 * @param parameters Pointer to struct of GameParameters_t
 *****************************************************************************/
void addFigure(GameParameters_t *parameters);

/*****************************************************************************
 * @brief Attach figure to game field
 *
 * Attach figure to game field followed by clear filled rows, update score,
 *spawn next figure and check if game over
 *
 * @param parameters Pointer to struct of GameParameters_t
 *****************************************************************************/
void attachFigure(GameParameters_t *parameters);

/*****************************************************************************
 * @brief Spawn next figure
 *
 * Spawn next figure on game field above the visible screen
 *
 * @param parameters Pointer to struct of GameParameters_t
 *****************************************************************************/
void spawnNextFigure(GameParameters_t *parameters);

/*****************************************************************************
 * @brief Generate random figure
 *
 * Generate random figure from possible variants
 *
 * @param next Pointer to array of the next figure for preview
 * @return Figure type number in figures array
 *****************************************************************************/
int generateRandomFigure(int **next);

/*****************************************************************************
 * @brief User's input processing
 *
 * Activate function, assigned to game state and action into FSM table
 *
 * @param action User's action
 * @param hold Parameter that checks whether key is hold
 *****************************************************************************/
void userInput(UserAction_t action, bool hold);

/*****************************************************************************
 * @brief Remove game parameters
 *
 * Clear allocated memory for arrays and assign null pointers
 *
 * @param parameters Pointer to struct of GameParameters_t
 *****************************************************************************/
void removeParameters(GameParameters_t *parameters);

/*****************************************************************************
 * @brief Allocate memory for 2D array
 *
 * Dynamically allocate memory for 2D array
 *
 * @param nRows Number of rows
 * @param nCols Number of cols
 * @return int** Return pointer to 2D array
 *****************************************************************************/
int **allocate2DArray(int nRows, int nCols);

/*****************************************************************************
 * @brief Reset game field
 *
 * Reset game field to initial state
 *
 * @param parameters Pointer to struct of GameParameters_t
 *****************************************************************************/
void resetField(GameParameters_t *parameters);

/*****************************************************************************
 * @brief Start game
 *
 * Change game state to START and spawns first (next) figure
 *
 * @param parameters Pointer to struct of GameParameters_t
 *****************************************************************************/
void startGame(GameParameters_t *parameters);

/*****************************************************************************
 * @brief Move current figure to the left
 *
 * Move current figure one pixel to the left if possible
 *
 * @param parameters Pointer to struct of GameParameters_t
 *****************************************************************************/
void moveLeft(GameParameters_t *parameters);

/*****************************************************************************
 * @brief Move current figure to the right
 *
 * Move current figure one pixel to the right if possible
 *
 * @param parameters Pointer to struct of GameParameters_t
 *****************************************************************************/
void moveRight(GameParameters_t *parameters);

/*****************************************************************************
 * @brief Move current figure down to the end
 *
 * Move current figure down while possible
 *
 * @param parameters Pointer to struct of GameParameters_t
 *****************************************************************************/
void moveDown(GameParameters_t *parameters);

/*****************************************************************************
 * @brief Rotate figure clockwise
 *
 * Move figure clockwise to 90 degrees if possible
 *
 * @param parameters Pointer to struct of GameParameters_t
 *****************************************************************************/
void rotateFigure(GameParameters_t *parameters);

/*****************************************************************************
 * @brief Pause game
 *
 * Pause game
 *
 * @param parameters Pointer to struct of GameParameters_t
 *****************************************************************************/
void pauseGame(GameParameters_t *parameters);

/*****************************************************************************
 * @brief Pointer type for finite state machine table
 *
 * Pointer type for finite state machine table
 *
 * @param parameters Pointer to struct of GameParameters_t
 *****************************************************************************/
typedef void (*funcPointer)(GameParameters_t *parameters);

#endif  // TETRIS_H
