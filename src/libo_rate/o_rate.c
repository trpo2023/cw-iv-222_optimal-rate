#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include <json-c/json.h>
#include <o_rate.h>

void error(char *error_var, const char *error_msg) {
  memcpy(error_var, error_msg, 200);
  return;
}

int searchFile(char *filename, const char *reldir) {
  DIR *dir;
  struct dirent *entry;

  dir = opendir(reldir);
  if (dir == NULL) {
    return 1;
  }

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, filename) == 0) {
      strcpy(filename, reldir);
      strcat(filename, entry->d_name);
      closedir(dir);
      return 0;
    }
  }

  closedir(dir);
  return 1;
}

struct Expense get_expense_from_file(char *filename, int average,
                                     char *error_msg) {
  char buffer[MAX_LEN];
  struct json_object *json;
  struct json_object *expenses;
  struct Expense temp_expense = {0, 0, 0};
  struct Expense err_expense = {-1, -1, -1};
  int n_dates;
  int correct = 1;

  if (searchFile(filename, "") && searchFile(filename, "thirdparty/") &&
      searchFile(filename, "../thirdparty/")) {
    error(error_msg, "не удалось найти файл, возможно вам стоит проверить его "
                     "имя или поменять текущую директорию");
    return err_expense;
  }

  FILE *fp = fopen(filename, "r");
  if (!fp) {
    char temp[50];
    sprintf(temp, "не удалось открыть файл '%s'", filename);
    error(error_msg, temp);
    return err_expense;
  }
  fread(buffer, 4096, 1, fp);
  fclose(fp);

  json = json_tokener_parse(buffer);

  correct *= json_object_object_get_ex(json, "expenses", &expenses);
  int start_point = 0;
  n_dates = json_object_array_length(expenses);

  if (n_dates > 12)
    start_point = n_dates - 12;

  for (int i = start_point; i < n_dates; i++) {
    if (!correct) {
      error(error_msg, "некорректный json файл");
      return err_expense;
    }
    struct json_object *estimated_date;
    struct json_object *consumption;
    struct json_object *minutes;
    struct json_object *internet;
    struct json_object *sms;
    int int_minutes;
    int int_internet;
    int int_sms;
    estimated_date = json_object_array_get_idx(expenses, i);
    correct *=
        json_object_object_get_ex(estimated_date, "consumption", &consumption);
    correct *= json_object_object_get_ex(consumption, "minutes", &minutes);
    correct *= json_object_object_get_ex(consumption, "internet", &internet);
    correct *= json_object_object_get_ex(consumption, "sms", &sms);
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

struct Rate find_optimal_rate(struct Expense expense, char *error_msg) {
  struct Rate err_rate = {"", -1, -1, -1, -1};

  char buffer[4096];
  struct json_object *json;
  struct json_object *rates;
  size_t n_rates;

  char filename[100] = "rates.json";
  searchFile(filename, "");
  searchFile(filename, "thirdparty/");
  searchFile(filename, "../thirdparty/");

  FILE *fp = fopen(filename, "r");
  if (!fp) {
    error(error_msg, "Не удалось загрузить базу тарифов, проверьте целостность "
                     "файлов программы");
    return err_rate;
  }
  fread(buffer, 4096, 1, fp);
  fclose(fp);

  json = json_tokener_parse(buffer);

  json_object_object_get_ex(json, "rates", &rates);
  n_rates = json_object_array_length(rates);

  for (int i = 0; i < n_rates; i++) {
    struct Rate temp_rate;
    struct json_object *rate;
    struct json_object *price;
    struct json_object *name;
    struct json_object *packages;
    struct json_object *minutes;
    struct json_object *internet;
    struct json_object *sms;
    rate = json_object_array_get_idx(rates, i);
    json_object_object_get_ex(rate, "packages", &packages);
    json_object_object_get_ex(packages, "minutes", &minutes);
    json_object_object_get_ex(packages, "internet", &internet);
    json_object_object_get_ex(packages, "sms", &sms);
    temp_rate.minutes = json_object_get_int(minutes);
    temp_rate.internet = json_object_get_int(internet);
    temp_rate.sms = json_object_get_int(sms);

    if (expense.minutes <= temp_rate.minutes &&
        expense.internet <= temp_rate.internet &&
        expense.sms <= temp_rate.sms) {
      json_object_object_get_ex(rate, "price", &price);
      json_object_object_get_ex(rate, "name", &name);
      temp_rate.price = json_object_get_int(price);
      memcpy(temp_rate.name, json_object_get_string(name), sizeof(char) * 20);

      return temp_rate;
    }
  }
  error(error_msg,
        "не удалось найти подходящий тариф - ваши расходы слишком большие");
  return err_rate;
}
