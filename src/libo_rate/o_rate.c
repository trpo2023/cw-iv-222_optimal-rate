#include <stdio.h>

#include <json-c/json.h>
#include <o_rate.h>

struct Expense get_expense_from_file(char *filename, int average) {
  FILE *fp = fopen(filename, "r");
  char buffer[4096];
  struct json_object *json;
  struct json_object *expenses;
  struct Expense temp_expense = {0, 0, 0};
  int n_dates;

  fread(buffer, 4096, 1, fp);
  fclose(fp);

  json = json_tokener_parse(buffer);

  json_object_object_get_ex(json, "expenses", &expenses);
  int start_point = 0;
  n_dates = json_object_array_length(expenses);

  if (n_dates > 12)
    start_point = n_dates - 12;

  for (int i = start_point; i < n_dates; i++) {
    struct json_object *estimated_date;
    struct json_object *consumption;
    struct json_object *minutes;
    struct json_object *internet;
    struct json_object *sms;
    int int_minutes;
    int int_internet;
    int int_sms;
    estimated_date = json_object_array_get_idx(expenses, i);
    json_object_object_get_ex(estimated_date, "consumption", &consumption);
    json_object_object_get_ex(consumption, "minutes", &minutes);
    json_object_object_get_ex(consumption, "internet", &internet);
    json_object_object_get_ex(consumption, "sms", &sms);
    int_minutes = json_object_get_int(minutes);
    int_internet = json_object_get_int(internet);
    int_sms = json_object_get_int(sms);

    if (average) {
      average++;
      temp_expense.minutes += int_minutes;
      temp_expense.internet += int_internet;
      temp_expense.sms += int_sms;
    } else {
      if (temp_expense.minutes < int_minutes)
        temp_expense.minutes = int_minutes;
      if (temp_expense.internet < int_internet)
        temp_expense.internet = int_internet;
      if (temp_expense.sms < int_sms)
        temp_expense.sms = int_sms;
    }
  }

  if (average) {
    temp_expense.minutes /= (average - 1);
    temp_expense.internet /= (average - 1);
    temp_expense.sms /= (average - 1);
  }

  return temp_expense;
}
