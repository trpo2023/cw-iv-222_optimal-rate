#include <ctest.h>
#include <ctype.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#include <o_rate.h>

CTEST(searchFile, find) {
  char filename[100] = "expenses.json";
  const char reldir[] = "../thirdparty/";
  int result = searchFile(filename, reldir);

  ASSERT_EQUAL(0, result);
  ASSERT_STR("../thirdparty/expenses.json", filename);
};

CTEST(searchFile, not_exist_file) {
  char filename[100] = "not_exist.h";
  const char reldir[] = "../thirdparty/";
  int result = searchFile(filename, reldir);

  ASSERT_EQUAL(1, result);
  ASSERT_STR("not_exist.h", filename);
};

CTEST(error, test) {
  char error_var[200];
  const char error_msg[200] =
      "Не удалось найти файл, возможно вам стоит проверить его имя или "
      "поменять текущую директорию";
  error(error_var, error_msg);
  ASSERT_STR(error_msg, error_var);
};

CTEST(get_expense_from_file, incorrect_json) {
  int mode = 1;
  char filename[100] = "expenses_test.json";
  char *error_msg = malloc(sizeof(char) * 200);
  get_expense_from_file(filename, mode, error_msg);
  ASSERT_STR("некорректный json файл", error_msg);
};

CTEST(get_expense_from_file, not_exist_file) {
  int mode = 1;
  char filename[100] = "not_expenses_test.json";
  char *error_msg = malloc(sizeof(char) * 200);
  get_expense_from_file(filename, mode, error_msg);
  ASSERT_STR("не удалось найти файл, возможно вам стоит проверить его "
             "имя или поменять текущую директорию",
             error_msg);
};

CTEST(get_expense_from_file, check_max) {
  int mode = 0;
  char filename[100] = "expenses_test_2.json";
  char *error_msg = malloc(sizeof(char) * 200);
  struct Expense expense = get_expense_from_file(filename, mode, error_msg);
  ASSERT_EQUAL(431, expense.minutes);
  ASSERT_EQUAL(45, expense.internet);
  ASSERT_EQUAL(11, expense.sms);
}

CTEST(get_expense_from_file, check_average) {
  int mode = 1;
  char filename[100] = "expenses_test_2.json";
  char *error_msg = malloc(sizeof(char) * 200);
  struct Expense expense = get_expense_from_file(filename, mode, error_msg);
  ASSERT_EQUAL(284, expense.minutes);
  ASSERT_EQUAL(23, expense.internet);
  ASSERT_EQUAL(7, expense.sms);
}

CTEST(find_optimal_rate, too_many_minutes) {
  char *error_msg = malloc(sizeof(char) * 200);
  struct Expense expense = {4560000, 40, 200};
  struct Rate rate = find_optimal_rate(expense, error_msg);
  ASSERT_EQUAL(2147483647, rate.minutes);
};

CTEST(find_optimal_rate, too_many_internet) {
  char *error_msg = malloc(sizeof(char) * 200);
  struct Expense expense = {456, 400, 200};
  find_optimal_rate(expense, error_msg);
  ASSERT_STR("не удалось найти подходящий тариф - ваши расходы слишком большие",
             error_msg);
};

CTEST(find_optimal_rate, too_many_sms) {
  char *error_msg = malloc(sizeof(char) * 200);
  struct Expense expense = {456, 40, 20000};
  find_optimal_rate(expense, error_msg);
  ASSERT_STR("не удалось найти подходящий тариф - ваши расходы слишком большие",
             error_msg);
}
