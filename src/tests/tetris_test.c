#include <check.h>
#include <locale.h>
#include <stdlib.h>

#include "../brick_game/tetris/tetris_logic.h"

#define AMOUNT 1
#define FALSE 0
#define TRUE 1

// updateCurrentState
START_TEST(tc_logic_1) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  updateParameters(&params);
  spawnNextFigure(&params);
  GameInfo_t dataCheck = updateCurrentState();

  ck_assert_ptr_eq(params.data->field, dataCheck.field);
  ck_assert_ptr_eq(params.data->next, dataCheck.next);
  ck_assert_int_eq(params.data->high_score, dataCheck.high_score);
  ck_assert_int_eq(params.data->score, dataCheck.score);
  ck_assert_int_eq(params.data->level, dataCheck.level);
  ck_assert_int_eq(params.data->pause, dataCheck.pause);
  ck_assert_int_eq(params.data->speed, dataCheck.speed);
  removeParameters(&params);
}
END_TEST

// updateParameters
START_TEST(tc_logic_2) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  GameParameters_t *paramsCheck1 = updateParameters(&params);
  GameParameters_t *paramsCheck2 = updateParameters(NULL);

  ck_assert_ptr_eq(&params, paramsCheck1);
  ck_assert_ptr_eq(&params, paramsCheck2);
  removeParameters(&params);
}
END_TEST

// initializeParameters
START_TEST(tc_logic_3) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);

  ck_assert_int_eq(params.data->score, 0);
  ck_assert_int_eq(params.data->level, 1);
  ck_assert_int_eq(params.data->speed, 1);
  ck_assert_int_eq(params.data->pause, 0);
  ck_assert_int_ge(params.figure->typeNext, 0);
  ck_assert_int_le(params.figure->typeNext, 6);
  ck_assert_int_eq(params.state, START);
  ck_assert_int_eq(params.isActive, true);
  removeParameters(&params);
}
END_TEST

// removeParameters
START_TEST(tc_logic_4) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  removeParameters(&params);

  ck_assert_ptr_null(params.data->field);
  ck_assert_ptr_null(params.data->next);
  ck_assert_int_eq(params.state, GAME_OVER);
  ck_assert_int_eq(params.isActive, false);
}
END_TEST

START_TEST(tc_logic_5) {
  GameParameters_t params;
  GameInfo_t data;
  params.data = &data;
  params.data->field = NULL;
  params.data->next = NULL;

  removeParameters(&params);

  ck_assert_ptr_null(params.data->field);
  ck_assert_ptr_null(params.data->next);
  ck_assert_int_eq(params.state, GAME_OVER);
  ck_assert_int_eq(params.isActive, false);
}
END_TEST

// resetField
START_TEST(tc_logic_6) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  startGame(&params);

  ck_assert_int_eq(params.data->score, 0);
  ck_assert_int_eq(params.data->level, 1);
  ck_assert_int_eq(params.data->speed, 1);
  ck_assert_int_eq(params.state, GAME);
  ck_assert_int_eq(params.isActive, true);
  removeParameters(&params);
}
END_TEST

// startGame
START_TEST(tc_logic_7) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.data->field[FIELD_HEIGHT / 2][FIELD_WIDTH / 2] = 1;
  resetField(&params);

  bool isFieldEqual = true;
  for (int row = 0; row < FIELD_HEIGHT && isFieldEqual; row++)
    for (int col = 0; col < FIELD_WIDTH && isFieldEqual; col++) {
      int checkValue = (row > 22 || col < 3 || col > 12) ? 1 : 0;
      isFieldEqual = params.data->field[row][col] == checkValue;
    }
  ck_assert_int_eq(isFieldEqual, true);
  removeParameters(&params);
}
END_TEST

// generateRandomFigure
START_TEST(tc_logic_8) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.figure->typeNext = generateRandomFigure(params.data->next);

  ck_assert_int_ge(params.figure->typeNext, 0);
  ck_assert_int_le(params.figure->typeNext, 6);
  removeParameters(&params);
}
END_TEST

// spawnNextFigure
START_TEST(tc_logic_9) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.figure->typeNext = 0;
  spawnNextFigure(&params);

  ck_assert_int_eq(params.figure->type, 0);
  ck_assert_int_eq(params.figure->y, 2);
  ck_assert_int_eq(params.figure->x, FIELD_WIDTH / 2);
  ck_assert_int_eq(params.figure->rotation, 0);
  removeParameters(&params);
}
END_TEST

// moveLeft
START_TEST(tc_logic_10) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.figure->y = FIELD_HEIGHT / 2;
  params.figure->x = FIELD_WIDTH / 2;
  int previousX = params.figure->x;
  params.figure->type = 0;  // Hero
  params.figure->rotation = 0;
  moveLeft(&params);

  ck_assert_int_ne(params.figure->x, previousX);
  removeParameters(&params);
}
END_TEST

START_TEST(tc_logic_11) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.figure->y = FIELD_HEIGHT / 2;
  params.figure->x = FIELD_WIDTH / 2;
  int previousX = params.figure->x;
  params.figure->type = 0;  // Hero
  params.figure->rotation = 0;
  params.data->pause = 1;
  moveLeft(&params);

  ck_assert_int_eq(params.figure->x, previousX);
  removeParameters(&params);
}
END_TEST

START_TEST(tc_logic_12) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.figure->y = FIELD_HEIGHT / 2;
  params.figure->x = 4;
  int previousX = params.figure->x;
  params.figure->type = 0;  // Hero
  params.figure->rotation = 0;
  moveLeft(&params);

  ck_assert_int_eq(params.figure->x, previousX);
  removeParameters(&params);
}
END_TEST

// moveRight
START_TEST(tc_logic_13) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.figure->y = FIELD_HEIGHT / 2;
  params.figure->x = FIELD_WIDTH / 2;
  int previousX = params.figure->x;
  params.figure->type = 0;  // Hero
  params.figure->rotation = 0;
  moveRight(&params);

  ck_assert_int_ne(params.figure->x, previousX);
  removeParameters(&params);
}
END_TEST

START_TEST(tc_logic_14) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.figure->y = FIELD_HEIGHT / 2;
  params.figure->x = FIELD_WIDTH / 2;
  int previousX = params.figure->x;
  params.figure->type = 0;  // Hero
  params.figure->rotation = 0;
  params.data->pause = 1;
  moveRight(&params);

  ck_assert_int_eq(params.figure->x, previousX);
  removeParameters(&params);
}
END_TEST

START_TEST(tc_logic_15) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.figure->y = FIELD_HEIGHT / 2;
  params.figure->x = FIELD_WIDTH - 6;
  int previousX = params.figure->x;
  params.figure->type = 0;  // Hero
  params.figure->rotation = 0;
  moveRight(&params);

  ck_assert_int_eq(params.figure->x, previousX);
  removeParameters(&params);
}
END_TEST

// rotateFigure
START_TEST(tc_logic_16) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.figure->y = FIELD_HEIGHT / 2;
  params.figure->x = FIELD_WIDTH / 2;
  params.figure->rotation = 0;
  params.figure->type = 0;  // Hero
  int previousRotation = params.figure->rotation;
  rotateFigure(&params);

  ck_assert_int_ne(params.figure->rotation, previousRotation);
  removeParameters(&params);
}
END_TEST

START_TEST(tc_logic_17) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.figure->y = FIELD_HEIGHT / 2;
  params.figure->x = FIELD_WIDTH / 2;
  params.figure->type = 0;  // Hero
  params.figure->rotation = 0;
  int previousRotation = params.figure->rotation;
  params.data->pause = 1;
  rotateFigure(&params);

  ck_assert_int_eq(params.figure->rotation, previousRotation);
  removeParameters(&params);
}
END_TEST

START_TEST(tc_logic_18) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.figure->y = FIELD_HEIGHT / 2;
  params.figure->x = FIELD_WIDTH - 3;
  params.figure->type = 0;  // Hero
  params.figure->rotation = 0;
  int previousRotation = params.figure->rotation;
  rotateFigure(&params);

  ck_assert_int_eq(params.figure->rotation, previousRotation);
  removeParameters(&params);
}
END_TEST

// moveDown
START_TEST(tc_logic_19) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.figure->y = FIELD_HEIGHT / 2;
  params.figure->x = FIELD_WIDTH / 2;
  //  int previousY = params.figure->y;
  params.figure->type = 0;  // Hero
  params.figure->rotation = 0;
  moveDown(&params);

  bool isFieldEqual = true;
  //  isFieldEqual = params.data->field[params.figure->y][previousY] == 0;
  //  isFieldEqual =
  //      params.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2 - 1] == 1;
  //  isFieldEqual = params.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2] ==
  //  1; isFieldEqual =
  //      params.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2 + 1] == 1;
  isFieldEqual =
      params.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2 + 2] == 1;
  ck_assert_int_eq(isFieldEqual, true);
  removeParameters(&params);
}
END_TEST

START_TEST(tc_logic_20) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.figure->y = FIELD_HEIGHT / 2;
  params.figure->x = FIELD_WIDTH / 2;
  params.figure->type = 0;  // Hero
  params.figure->rotation = 0;
  params.data->pause = 1;
  moveDown(&params);

  bool isFieldEqual = true;
  //  isFieldEqual =
  //      params.data->field[FIELD_HEIGHT / 2][FIELD_HEIGHT / 2 - 1] == 1;
  isFieldEqual = params.data->field[FIELD_HEIGHT / 2][FIELD_HEIGHT / 2] == 1;
  //  isFieldEqual =
  //      params.data->field[FIELD_HEIGHT / 2][FIELD_HEIGHT / 2 + 1] == 1;
  //  isFieldEqual =
  //      params.data->field[FIELD_HEIGHT / 2][FIELD_HEIGHT / 2 + 2] == 1;
  ck_assert_int_eq(isFieldEqual, true);
  removeParameters(&params);
}
END_TEST

START_TEST(tc_logic_21) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.figure->y = FIELD_HEIGHT - 4;
  params.figure->x = FIELD_WIDTH / 2;
  params.figure->type = 0;  // Hero
  params.figure->rotation = 0;
  int previousY = params.figure->y;
  moveDown(&params);

  bool isFieldEqual = true;
  isFieldEqual = params.data->field[previousY][params.figure->x] == 1;
  //  isFieldEqual =
  //      params.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2 - 1] == 1;
  //  isFieldEqual = params.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2] ==
  //  1; isFieldEqual =
  //      params.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2 + 1] == 1;
  //  isFieldEqual =
  //      params.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2 + 2] == 1;
  ck_assert_int_eq(isFieldEqual, true);
  removeParameters(&params);
}
END_TEST

// shiftFigure
START_TEST(tc_logic_22) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.figure->y = FIELD_HEIGHT / 2;
  params.figure->x = FIELD_WIDTH / 2;
  int previousY = params.figure->y;
  params.figure->type = 0;  // Hero
  params.figure->rotation = 0;
  shiftFigure(&params);

  ck_assert_int_ne(params.figure->y, previousY);
  removeParameters(&params);
}
END_TEST

START_TEST(tc_logic_23) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.figure->y = FIELD_HEIGHT - 4;
  params.figure->x = FIELD_WIDTH / 2;
  params.figure->type = 0;  // Hero
  params.figure->rotation = 0;
  int previousY = params.figure->y;
  shiftFigure(&params);

  bool isFieldEqual = true;
  isFieldEqual = params.data->field[previousY][params.figure->x] == 1;
  //  isFieldEqual =
  //      params.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2 - 1] == 1;
  //  isFieldEqual = params.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2] ==
  //  1; isFieldEqual =
  //      params.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2 + 1] == 1;
  //  isFieldEqual =
  //      params.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2 + 2] == 1;
  ck_assert_int_eq(isFieldEqual, true);
  removeParameters(&params);
}
END_TEST

// attachFigure
START_TEST(tc_logic_24) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  for (int col = 0; col < FIELD_WIDTH; col++)
    params.data->field[FIELD_HEIGHT - 4][col] = 1;
  attachFigure(&params);

  ck_assert_int_eq(params.data->score, 100);
  removeParameters(&params);
}
END_TEST

START_TEST(tc_logic_25) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  for (int col = 0; col < FIELD_WIDTH; col++) {
    params.data->field[FIELD_HEIGHT - 4][col] = 1;
    params.data->field[FIELD_HEIGHT - 5][col] = 1;
  }
  attachFigure(&params);

  ck_assert_int_eq(params.data->score, 300);
  removeParameters(&params);
}
END_TEST

START_TEST(tc_logic_26) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  for (int col = 0; col < FIELD_WIDTH; col++) {
    params.data->field[FIELD_HEIGHT - 4][col] = 1;
    params.data->field[FIELD_HEIGHT - 5][col] = 1;
    params.data->field[FIELD_HEIGHT - 6][col] = 1;
  }
  attachFigure(&params);

  ck_assert_int_eq(params.data->score, 700);
  removeParameters(&params);
}
END_TEST

START_TEST(tc_logic_27) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  for (int col = 0; col < FIELD_WIDTH; col++) {
    params.data->field[FIELD_HEIGHT - 4][col] = 1;
    params.data->field[FIELD_HEIGHT - 5][col] = 1;
    params.data->field[FIELD_HEIGHT - 6][col] = 1;
    params.data->field[FIELD_HEIGHT - 7][col] = 1;
  }
  attachFigure(&params);

  ck_assert_int_eq(params.data->score, 1500);
  removeParameters(&params);
}
END_TEST

START_TEST(tc_logic_28) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.data->score = 6000;
  for (int col = 0; col < FIELD_WIDTH; col++) {
    params.data->field[FIELD_HEIGHT - 4][col] = 1;
    params.data->field[FIELD_HEIGHT - 5][col] = 1;
    params.data->field[FIELD_HEIGHT - 6][col] = 1;
    params.data->field[FIELD_HEIGHT - 7][col] = 1;
  }
  attachFigure(&params);

  ck_assert_int_eq(params.data->score, 7500);
  ck_assert_int_eq(params.data->level, 10);
  removeParameters(&params);
}
END_TEST

START_TEST(tc_logic_29) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.data->field[3][FIELD_WIDTH / 2] = 1;
  attachFigure(&params);

  ck_assert_int_eq(params.state, GAME_OVER);
  removeParameters(&params);
}
END_TEST

START_TEST(tc_logic_30) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.data->score = params.data->high_score + 100;
  attachFigure(&params);

  ck_assert_int_eq(params.data->score, params.data->high_score);
  removeParameters(&params);
}
END_TEST

// addFigure
START_TEST(tc_logic_31) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.figure->y = FIELD_HEIGHT / 2;
  params.figure->x = FIELD_WIDTH / 2;
  params.figure->type = 0;  // Hero
  params.figure->rotation = 0;
  addFigure(&params);

  bool isFieldEqual = true;
  //  isFieldEqual = params.data->field[params.figure->y][params.figure->x - 1]
  //  ==
  //                 params.figure->type + 1;
  //  isFieldEqual = params.data->field[params.figure->y][params.figure->x] ==
  //                 params.figure->type + 1;
  //  isFieldEqual = params.data->field[params.figure->y][params.figure->x + 1]
  //  ==
  //                 params.figure->type + 1;
  isFieldEqual = params.data->field[params.figure->y][params.figure->x + 2] ==
                 params.figure->type + 1;
  ck_assert_int_eq(isFieldEqual, true);
  removeParameters(&params);
}
END_TEST

// isFigureNotCollide
START_TEST(tc_logic_32) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.figure->y = FIELD_HEIGHT / 2;
  params.figure->x = FIELD_WIDTH / 2;
  params.figure->type = 0;  // Hero
  params.figure->rotation = 0;
  bool isNotCollide = isFigureNotCollide(&params);

  ck_assert_int_eq(isNotCollide, true);
  removeParameters(&params);
}
END_TEST

START_TEST(tc_logic_33) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.figure->y = FIELD_HEIGHT - 3;
  params.figure->x = FIELD_WIDTH / 2;
  params.figure->type = 0;  // Hero
  params.figure->rotation = 0;
  bool isNotCollide = isFigureNotCollide(&params);

  ck_assert_int_eq(isNotCollide, false);
  removeParameters(&params);
}
END_TEST

// clearFigure
START_TEST(tc_logic_34) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  params.figure->y = FIELD_HEIGHT / 2;
  params.figure->x = FIELD_WIDTH / 2;
  params.figure->type = 0;  // Hero
  params.figure->rotation = 0;
  clearFigure(&params);

  bool isFieldEqual = true;
  //  isFieldEqual =
  //      params.data->field[params.figure->y][params.figure->x - 1] == 0;
  //  isFieldEqual = params.data->field[params.figure->y][params.figure->x] ==
  //  0; isFieldEqual =
  //      params.data->field[params.figure->y][params.figure->x + 1] == 0;
  isFieldEqual =
      params.data->field[params.figure->y][params.figure->x + 2] == 0;
  ck_assert_int_eq(isFieldEqual, true);
  removeParameters(&params);
}
END_TEST

// allocate2DArray
START_TEST(tc_logic_35) {
  GameParameters_t params;
  GameInfo_t data;
  data.field = NULL;
  data.next = NULL;
  params.data = &data;

  data.field = allocate2DArray(FIELD_HEIGHT, FIELD_WIDTH);

  ck_assert_ptr_nonnull(params.data->field);
  removeParameters(&params);
}
END_TEST

// Pause
START_TEST(tc_logic_36) {
  GameParameters_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;

  initializeParameters(&params);
  pauseGame(&params);

  ck_assert_int_eq(params.data->pause, 1);
  removeParameters(&params);
}
END_TEST

Suite *tetris_suite() {
  TCase *tc = tcase_create("[logic] cases");
  tcase_add_test(tc, tc_logic_1);
  tcase_add_test(tc, tc_logic_2);
  tcase_add_test(tc, tc_logic_3);
  tcase_add_test(tc, tc_logic_4);
  tcase_add_test(tc, tc_logic_5);
  tcase_add_test(tc, tc_logic_6);
  tcase_add_test(tc, tc_logic_7);
  tcase_add_test(tc, tc_logic_8);
  tcase_add_test(tc, tc_logic_9);
  tcase_add_test(tc, tc_logic_10);
  tcase_add_test(tc, tc_logic_11);
  tcase_add_test(tc, tc_logic_12);
  tcase_add_test(tc, tc_logic_13);
  tcase_add_test(tc, tc_logic_14);
  tcase_add_test(tc, tc_logic_15);
  tcase_add_test(tc, tc_logic_16);
  tcase_add_test(tc, tc_logic_17);
  tcase_add_test(tc, tc_logic_18);
  tcase_add_test(tc, tc_logic_19);
  tcase_add_test(tc, tc_logic_20);
  tcase_add_test(tc, tc_logic_21);
  tcase_add_test(tc, tc_logic_22);
  tcase_add_test(tc, tc_logic_23);
  tcase_add_test(tc, tc_logic_24);
  tcase_add_test(tc, tc_logic_25);
  tcase_add_test(tc, tc_logic_26);
  tcase_add_test(tc, tc_logic_27);
  tcase_add_test(tc, tc_logic_28);
  tcase_add_test(tc, tc_logic_29);
  tcase_add_test(tc, tc_logic_30);
  tcase_add_test(tc, tc_logic_31);
  tcase_add_test(tc, tc_logic_32);
  tcase_add_test(tc, tc_logic_33);
  tcase_add_test(tc, tc_logic_34);
  tcase_add_test(tc, tc_logic_35);
  tcase_add_test(tc, tc_logic_36);

  Suite *s = suite_create("[s21_tetris] suite");
  suite_add_tcase(s, tc);

  return s;
}

int main(void) {
  setlocale(LC_ALL, "");
  setlocale(LC_NUMERIC, "C");

  Suite *ts = tetris_suite();
  SRunner *sr = srunner_create(ts);

  for (int repeats = 0; repeats < AMOUNT; ++repeats) {
    srunner_run_all(sr, CK_NORMAL);
  }

  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (0 == number_failed) ? EXIT_SUCCESS : EXIT_FAILURE;
}
