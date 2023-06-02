#pragma once

struct Expense {
  int minutes;
  int internet;
  int sms;
};

struct Expense get_expense_from_file(char *filename, int average);
struct Rate
{
    char name[15];
    int price;
    int minutes;
    int internet;
    int sms;
};
