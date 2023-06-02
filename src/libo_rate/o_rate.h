#pragma once

struct Expense {
  int minutes;
  int internet;
  int sms;
};

struct Expense get_expense_from_file(char *filename, int average);