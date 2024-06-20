/*****************************************************************************
 * @file tetris_logic.c
 * @brief Source File with Logic of the Tetris Game
 *****************************************************************************/

#include "tetris_logic.h"

/*****************************************************************************
 * @brief Finite state machine table
 *
 * Finite state machine table
 *****************************************************************************/
funcPointer fsmTable[STATES_COUNT][SIGNALS_COUNT] = {
    {startGame, NULL, removeParameters, NULL, NULL, NULL, NULL, NULL},  // START
    {NULL, pauseGame, removeParameters, moveLeft, moveRight, NULL, moveDown,
     rotateFigure},  // GAME
    {startGame, NULL, removeParameters, NULL, NULL, NULL, NULL,
     NULL}  // GAME_OVER
};

/*****************************************************************************
 * @brief Relative coordinates of figures
 *
 * Relative coordinates of figures
 *****************************************************************************/
int figures[FIGURES_COUNT][8] = {
    {0, -1, 0, 0, 0, 1, 0, 2},    // Hero
    {-1, -1, 0, -1, 0, 0, 0, 1},  // Blue Ricky
    {0, -1, 0, 0, 0, 1, -1, 1},   // Orange Ricky
    {-1, 0, -1, 1, 0, 0, 0, 1},   // SmashBoy
    {0, -1, 0, 0, -1, 0, -1, 1},  // Rhode Island Z
    {0, -1, 0, 0, -1, 0, 0, 1},   // TeeWee
    {-1, -1, -1, 0, 0, 0, 0, 1}   // Cleveland Z
};

void initializeParameters(GameParameters_t *parameters) {
  parameters->data->field = allocate2DArray(FIELD_HEIGHT, FIELD_WIDTH);
  parameters->data->next = allocate2DArray(FIGURE_HEIGHT, FIGURE_WIDTH);

  if (NULL == parameters->data->field || NULL == parameters->data->next) {
    printf("\nNot enough memory...\n");
    exit(1);
  }

  parameters->data->score = 0;
  resetField(parameters);

  FILE *file = fopen(DATA_PATH, "r");
  if (!file) {
    parameters->data->high_score = 0;
    file = fopen(DATA_PATH, "w");
    fprintf(file, "0\n");
  } else {
    int highScore;

    if (fscanf(file, "%d\n", &highScore) != 1) {
      printf("Error: Unable to read data from external file (%s)", DATA_PATH);
      exit(1);
    }

    parameters->data->high_score = highScore;
  }
  fclose(file);

  parameters->data->level = LEVEL_MIN;
  parameters->data->speed = SPEED_MIN;
  parameters->data->pause = 0;
  parameters->figure->typeNext = generateRandomFigure(parameters->data->next);
  parameters->state = START;
  parameters->isActive = true;
}

GameParameters_t *updateParameters(GameParameters_t *parameters) {
  static GameParameters_t *data;
  if (NULL != parameters) {
    data = parameters;
  }

  return data;
}

GameInfo_t updateCurrentState(void) {
  GameParameters_t *parameters = updateParameters(NULL);
  shiftFigure(parameters);
  return *parameters->data;
}

void shiftFigure(GameParameters_t *parameters) {
  clearFigure(parameters);
  parameters->figure->y++;
  bool canShift = isFigureNotCollide(parameters);

  if (!canShift) parameters->figure->y--;

  addFigure(parameters);

  if (!canShift) {
    attachFigure(parameters);
  }
}

void clearFigure(GameParameters_t *parameters) {
  int y = parameters->figure->y;
  int x = parameters->figure->x;
  int type = parameters->figure->type;
  int rotation = parameters->figure->rotation;

  for (int i = 1; i < 8; i += 2) {
    int xx = (int)round(figures[type][i] * cos(PI_2 * rotation) +
                        figures[type][i - 1] * sin(PI_2 * rotation));
    int yy = (int)round(-figures[type][i] * sin(PI_2 * rotation) +
                        figures[type][i - 1] * cos(PI_2 * rotation));

    parameters->data->field[yy + y][xx + x] = PIXEL_EMPTY;
  }
}

bool isFigureNotCollide(GameParameters_t *parameters) {
  int y = parameters->figure->y;
  int x = parameters->figure->x;
  int type = parameters->figure->type;
  int rotation = parameters->figure->rotation;

  bool isNotCollide = true;
  for (int i = 1; i < 8 && isNotCollide; i += 2) {
    int xx = (int)round(figures[type][i] * cos(PI_2 * rotation) +
                        figures[type][i - 1] * sin(PI_2 * rotation));
    int yy = (int)round(-figures[type][i] * sin(PI_2 * rotation) +
                        figures[type][i - 1] * cos(PI_2 * rotation));

    if (parameters->data->field[yy + y][xx + x]) {
      isNotCollide = false;
    }
  }

  return isNotCollide;
}

void addFigure(GameParameters_t *parameters) {
  int y = parameters->figure->y;
  int x = parameters->figure->x;
  int type = parameters->figure->type;
  int rotation = parameters->figure->rotation;

  for (int i = 1; i < 8; i += 2) {
    int xx = (int)round(figures[type][i] * cos(PI_2 * rotation) +
                        figures[type][i - 1] * sin(PI_2 * rotation));
    int yy = (int)round(-figures[type][i] * sin(PI_2 * rotation) +
                        figures[type][i - 1] * cos(PI_2 * rotation));

    parameters->data->field[yy + y][xx + x] = type + 1;
  }
}

void attachFigure(GameParameters_t *parameters) {
  int rows = 0;
  for (int row = FIELD_HEIGHT - BORDER_SIZE - 1; row > 2; --row) {
    bool cycle = true;
    while (cycle) {
      int rowBlocks = 0;
      for (int col = 3; col < FIELD_WIDTH - 3; ++col) {
        if (parameters->data->field[row][col]) {
          ++rowBlocks;
        }
      }

      if (rowBlocks == FIELD_WIDTH - 6) {
        ++rows;
        for (int i = row; i > 1; --i) {
          for (int col = BORDER_SIZE; col < FIELD_WIDTH - BORDER_SIZE; ++col) {
            parameters->data->field[i][col] =
                parameters->data->field[i - 1][col];
          }
        }
      } else {
        cycle = false;
      }
    }
  }

  if (rows == 1) {
    parameters->data->score += SCORE_ROWS_1;
  } else if (rows == 2) {
    parameters->data->score += SCORE_ROWS_2;
  } else if (rows == 3) {
    parameters->data->score += SCORE_ROWS_3;
  } else if (rows == 4) {
    parameters->data->score += SCORE_ROWS_4;
  }

  if (parameters->data->score > parameters->data->high_score) {
    parameters->data->high_score = parameters->data->score;
    FILE *file = fopen(DATA_PATH, "w");
    fprintf(file, "%d\n", parameters->data->high_score);
    fclose(file);
  }

  parameters->data->level =
      parameters->data->score / LEVEL_THRESHOLD + 1 <= LEVEL_MAX
          ? parameters->data->score / LEVEL_THRESHOLD + 1
          : LEVEL_MAX;
  parameters->data->speed = parameters->data->level;

  spawnNextFigure(parameters);
  clearFigure(parameters);
  parameters->figure->y++;
  bool canShift = isFigureNotCollide(parameters);

  if (!canShift) {
    parameters->figure->y--;
    parameters->state = GAME_OVER;
  }

  addFigure(parameters);
}

void spawnNextFigure(GameParameters_t *parameters) {
  parameters->figure->type = parameters->figure->typeNext;
  parameters->figure->x = FIELD_WIDTH / 2;
  parameters->figure->y = 2;
  parameters->figure->rotation = 0;
  parameters->figure->typeNext = generateRandomFigure(parameters->data->next);
  addFigure(parameters);
}

int generateRandomFigure(int **next) {
  int type = rand() % FIGURES_COUNT;

  for (int row = 0; row < FIGURE_HEIGHT; ++row) {
    for (int col = 0; col < FIGURE_WIDTH; ++col) {
      next[row][col] = PIXEL_EMPTY;
    }
  }

  for (int i = 1; i < 8; i += 2) {
    next[figures[type][i - 1] + 1][figures[type][i] + 1] = type + 1;
  }

  return type;
}

void userInput(UserAction_t action, bool hold) {
  if (hold) {
    printf("\x1b");  // ESC
  }

  GameParameters_t *parameters = updateParameters(NULL);
  GameState_t state = parameters->state;
  funcPointer func = fsmTable[state][action];

  if (func) {
    func(parameters);
  }
}

void removeParameters(GameParameters_t *parameters) {
  if (parameters->data->field) {
    for (int row = 0; row < FIELD_HEIGHT; ++row) {
      free(parameters->data->field[row]);
      parameters->data->field[row] = NULL;
    }

    free(parameters->data->field);
    parameters->data->field = NULL;
  }

  if (parameters->data->next) {
    for (int row = 0; row < FIGURE_HEIGHT; ++row) {
      free(parameters->data->next[row]);
      parameters->data->next[row] = NULL;
    }

    free(parameters->data->next);
    parameters->data->next = NULL;
  }

  parameters->state = GAME_OVER;
  parameters->isActive = false;
}

int **allocate2DArray(int nRows, int nCols) {
  int **arr = (int **)calloc(nRows, sizeof(int *));

  for (int row = 0; row < nRows; ++row) {
    arr[row] = (int *)calloc(nCols, sizeof(int));
  }

  return arr;
}

void resetField(GameParameters_t *parameters) {
  for (int row = 0; row < FIELD_HEIGHT; ++row) {
    for (int col = 0; col < FIELD_WIDTH; ++col) {
      parameters->data->field[row][col] =
          (row > FIELD_HEIGHT - BORDER_SIZE - 1 || col < BORDER_SIZE ||
           col > FIELD_WIDTH - BORDER_SIZE - 1)
              ? 1
              : PIXEL_EMPTY;
    }
  }
}

void startGame(GameParameters_t *parameters) {
  resetField(parameters);

  FILE *file = fopen(DATA_PATH, "r");
  int highScore;

  if (fscanf(file, "%d\n", &highScore) != 1) {
    printf("Error: Unable to read data from external file (%s)", DATA_PATH);
    exit(1);
  }

  parameters->data->high_score = highScore;
  fclose(file);

  parameters->data->score = 0;
  parameters->data->level = LEVEL_MIN;
  parameters->data->speed = SPEED_MIN;
  parameters->state = GAME;

  spawnNextFigure(parameters);
}

void moveLeft(GameParameters_t *parameters) {
  if (!parameters->data->pause) {
    clearFigure(parameters);
    parameters->figure->x--;
    bool canMove = isFigureNotCollide(parameters);

    if (!canMove) {
      parameters->figure->x++;
    }

    addFigure(parameters);
  }
}

void moveRight(GameParameters_t *parameters) {
  if (!parameters->data->pause) {
    clearFigure(parameters);
    parameters->figure->x++;
    bool canMove = isFigureNotCollide(parameters);

    if (!canMove) {
      parameters->figure->x--;
    }

    addFigure(parameters);
  }
}

void moveDown(GameParameters_t *parameters) {
  if (!parameters->data->pause) {
    clearFigure(parameters);

    bool canMove = true;
    while (canMove) {
      parameters->figure->y++;
      canMove = isFigureNotCollide(parameters);

      if (!canMove) {
        parameters->figure->y--;
      }
    }

    addFigure(parameters);
    attachFigure(parameters);
  }
}

void rotateFigure(GameParameters_t *parameters) {
  if (!parameters->data->pause) {
    clearFigure(parameters);
    parameters->figure->rotation =
        parameters->figure->rotation + 1 <= ROTATION_MAX
            ? parameters->figure->rotation + 1
            : ROTATION_MIN;
    bool canRotate = isFigureNotCollide(parameters);

    if (!canRotate) {
      parameters->figure->rotation =
          parameters->figure->rotation - 1 >= ROTATION_MIN
              ? parameters->figure->rotation - 1
              : ROTATION_MAX;
    }

    addFigure(parameters);
  }
}

void pauseGame(GameParameters_t *parameters) {
  parameters->data->pause = !parameters->data->pause;
}
