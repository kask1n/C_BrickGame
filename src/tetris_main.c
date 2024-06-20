/*****************************************************************************
 * @mainpage
 * @author redavero
 * @version 1.0
 * @file tetris_main.c
 * @brief Entry point
 *****************************************************************************/

#include "gui/cli/tetris_cli.h"

int main() {
  srand(time(NULL));

  initGUI();
  gameLoop();
  destroyGUI();

  return 0;
}
