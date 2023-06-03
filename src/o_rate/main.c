#include <stdio.h>
#include <stdlib.h>

#include <o_rate.h>

int main() {
  int mode;
  char filename[100];
  char *error_msg = malloc(sizeof(char) * 200);
  printf("Данная программа поможет вам подобрать оптимальный тариф, исходя "
         "из ваших расходов по пакетам.\nВыберите режим подбора:\n\n1 - "
         "Подбор осуществляется в соответствии с максимальными расходами\n2 "
         "- Подбор осуществляется в соответствии с усреднёнными значениями "
         "расходов (менее оптимальный, но экономный вариант)\n");
  printf("Введите номер режима: ");
  scanf("%d", &mode);
  mode--;
  while (mode != 0 && mode != 1) {
    printf("Введите номер режима (1 или 2): ");
    scanf("%d", &mode);
    mode--;
  }
  printf("Введите название json файла, содержащего данные о ваших "
         "расходах (файл должен находиться в директории thirdparty):\n");
  scanf("%s", filename);
  struct Expense expense = get_expense_from_file(filename, mode, error_msg);
  if (expense.minutes < 0) {
    printf("\nОшибка: %s\n", error_msg);
    free(error_msg);
    return -1;
  }
  printf("\nВаши расходы:\nМинуты: %d, гигабайты: %d, сообщения: %d\n\n",
         expense.minutes, expense.internet, expense.sms);
  struct Rate optimal_rate = find_optimal_rate(expense, error_msg);
  if (optimal_rate.minutes < 0) {
    printf("\nОшибка: %s\n", error_msg);
    free(error_msg);
    return -1;
  }
  if (optimal_rate.minutes != 2147483647)
    printf("Был подобран оптимальный тариф %s за %dр.\nВ него "
           "входит\nМинуты: %d, гигабайты: %d, сообщения: %d\n",
           optimal_rate.name, optimal_rate.price, optimal_rate.minutes,
           optimal_rate.internet, optimal_rate.sms);
  else
    printf("Был подобран оптимальный тариф %s за %dр.\nВ него "
           "входит\nМинуты: безлимит, гигабайты: %d, сообщения: %d\n",
           optimal_rate.name, optimal_rate.price, optimal_rate.internet,
           optimal_rate.sms);
  free(error_msg);
  return 0;
}
