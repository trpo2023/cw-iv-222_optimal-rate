#pragma once
#define MAX_LEN 4096

struct Expense {
  int minutes;
  int internet;
  int sms;
};

struct Rate {
  char name[20];
  int price;
  int minutes;
  int internet;
  int sms;
};

struct Expense get_expense_from_file(char *filename, int average,
                                     char *error_msg);

struct Rate find_optimal_rate(struct Expense expense, char *error_msg);
