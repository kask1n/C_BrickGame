/*****************************************************************************
 * @file tetris_cli.c
 * @brief GUI CLI Source File
 *****************************************************************************/

#include "tetris_cli.h"

void initGUI(void) {
  setlocale(LC_ALL, "");
  initscr();
  start_color();

  init_pair(1, COLOR_BLUE, COLOR_BLUE);
  init_pair(2, COLOR_CYAN, COLOR_CYAN);
  init_pair(3, COLOR_GREEN, COLOR_GREEN);
  init_pair(4, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(5, COLOR_RED, COLOR_RED);
  init_pair(6, COLOR_WHITE, COLOR_WHITE);
  init_pair(7, COLOR_YELLOW, COLOR_YELLOW);

  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, true);
  timeout(READ_DELAY);
}

void gameLoop(void) {
  GameParameters_t parameters;
  GameInfo_t data;
  parameters.data = &data;
  Figure_t figure;
  parameters.figure = &figure;
  UserAction_t action;
  bool hold = false;
  double counter = 0.;

  initializeParameters(&parameters);
  updateParameters(&parameters);

  while (parameters.isActive) {
    if (counter >= 1.6 - parameters.data->speed * SPEED_RATE) {
      if (parameters.state == GAME && !parameters.data->pause) {
        updateCurrentState();
      }

      counter = 0.;
    }

    counter += READ_DELAY * 0.001;

    if (parameters.state == START) {
      drawStartScreen(parameters.data);
    } else if (parameters.state == GAME) {
      drawGUI();
      drawInfo(parameters.data);
      drawField(parameters.data->field);
    } else if (parameters.state == GAME_OVER) {
      drawGameOver(parameters.data);
    }

    if (parameters.data->pause) {
      mvprintw(FIELD_SIZE_Y / 2 + 1, FIELD_SIZE_X - 1, "PAUSE");
      move(FIELD_SIZE_Y + 1, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 3);
    }

    int pressedKey = getch();
    action = getAction(pressedKey);
    if (action != Up) {
      userInput(action, hold);
    }
  }
}

void drawStartScreen(GameInfo_t *data) {
  drawGUI();
  drawInfo(data);

  mvprintw(FIELD_SIZE_Y / 2 + 1, 1, "Press ENTER to start");

  move(FIELD_SIZE_Y + 1, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 3);
}

void drawGUI(void) {
  clear();

  mvhline(0, 0, ACS_HLINE, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 2);
  mvhline(FIELD_SIZE_Y + 1, 0, ACS_HLINE,
          FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 2);
  mvhline(FIELD_SIZE_Y - 6, FIELD_SIZE_X * 2 + 2, ACS_HLINE, INFO_SIZE_X * 2);

  mvvline(1, 0, ACS_VLINE, FIELD_SIZE_Y);
  mvvline(1, FIELD_SIZE_X * 2 + 1, ACS_VLINE, FIELD_SIZE_Y);
  mvvline(1, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 2, ACS_VLINE, FIELD_SIZE_Y);

  mvaddch(0, 0, ACS_ULCORNER);
  mvaddch(0, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 2, ACS_URCORNER);
  mvaddch(FIELD_SIZE_Y + 1, 0, ACS_LLCORNER);
  mvaddch(FIELD_SIZE_Y + 1, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 2,
          ACS_LRCORNER);
  mvaddch(0, FIELD_SIZE_X * 2 + 1, ACS_TTEE);
  mvaddch(FIELD_SIZE_Y + 1, FIELD_SIZE_X * 2 + 1, ACS_BTEE);

  move(FIELD_SIZE_Y + 1, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 3);
}

void drawInfo(GameInfo_t *data) {
  mvprintw(2, FIELD_SIZE_X * 2 + 3, "HIGH SCORE: %d", data->high_score);
  mvprintw(4, FIELD_SIZE_X * 2 + 3, "SCORE: %d", data->score);
  mvprintw(6, FIELD_SIZE_X * 2 + 3, "LEVEL: %d", data->level);
  mvprintw(8, FIELD_SIZE_X * 2 + 3, "SPEED: %d", data->speed);
  mvprintw(10, FIELD_SIZE_X * 2 + 3, "NEXT:");

  for (int row = 0; row < FIGURE_HEIGHT; ++row) {
    for (int col = 0; col < FIGURE_WIDTH; ++col) {
      if (data->next[row][col]) {
        attron(COLOR_PAIR(data->next[row][col]));
        mvaddch(row + 11, FIELD_SIZE_X * 2 + 6 * 2 + col * 2, ACS_CKBOARD);
        mvaddch(row + 11, FIELD_SIZE_X * 2 + 6 * 2 + col * 2 + 1, ACS_CKBOARD);
        attroff(COLOR_PAIR(data->next[row][col]));
      }
    }
  }

  mvprintw(15, FIELD_SIZE_X * 2 + 4, "ESC  - Pause game");
  mvaddwstr(16, FIELD_SIZE_X * 2 + 5, L"←   - Move left");
  mvaddwstr(17, FIELD_SIZE_X * 2 + 5, L"→   - Move right");
  mvaddwstr(18, FIELD_SIZE_X * 2 + 5, L"↓   - Move down");
  mvprintw(19, FIELD_SIZE_X * 2 + 3, "SPACE - Rotate");
  mvprintw(20, FIELD_SIZE_X * 2 + 5, "Q   - Exit game");
  move(FIELD_SIZE_Y + 1, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 3);
}

void drawField(int **field) {
  for (int row = 0; row < FIELD_SIZE_Y; ++row) {
    for (int col = 0; col < FIELD_SIZE_X; ++col) {
      if (field[row + 3][col + 3]) {
        attron(COLOR_PAIR(field[row + 3][col + 3]));
        mvaddch(row + 1, col * 2 + 1, ACS_CKBOARD);
        mvaddch(row + 1, col * 2 + 2, ACS_CKBOARD);
        attroff(COLOR_PAIR(field[row + 3][col + 3]));
      }
    }
  }

  move(FIELD_SIZE_Y + 1, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 3);
}

void drawGameOver(GameInfo_t *data) {
  drawGUI();
  drawInfo(data);
  drawField(data->field);

  mvprintw(FIELD_SIZE_Y / 2, 6, "GAME OVER");
  mvprintw(FIELD_SIZE_Y / 2 + 1, 5, "Press ENTER");
  mvprintw(FIELD_SIZE_Y / 2 + 2, 4, "to start again");

  move(FIELD_SIZE_Y + 1, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 3);
}

UserAction_t getAction(int pressedKey) {
  UserAction_t action = Up;

  switch (pressedKey) {
    case 10:
      action = Start;
      break;
    case 27:
      action = Pause;
      break;
    case 'q':
    case 'Q':
      action = Terminate;
      break;
    case KEY_LEFT:
      action = Left;
      break;
    case KEY_RIGHT:
      action = Right;
      break;
    case KEY_DOWN:
      action = Down;
      break;
    case ' ':
      action = Action;
      break;
  }

  return action;
}

void destroyGUI(void) {
  mvprintw(FIELD_SIZE_Y + 2, 2, "End of Game. Closing the application...");
  refresh();
  sleep(2);
  clear();
  refresh();
  endwin();
}
