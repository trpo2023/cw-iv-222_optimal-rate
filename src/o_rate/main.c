#include <stdio.h>

#include <o_rate.h>

int main()
{
    int mode;
    char filename[25];
    printf("Данная программа поможет вам подобрать оптимальный тариф, исходя "
           "из ваших расходов по пакетам.\nВыберите режим подбора:\n\n1 - "
           "Подбор осуществляется в соответствии с максимальными расходами\n2 "
           "- Подбор осуществляется в соответствии с усреднёнными значениями "
           "расходов (менее оптимальный, но экономный вариант)\n");
    printf("Введите номер режима: ");
    mode = getchar() - 49;
    printf("Введите название json файла, содержащего данные о ваших "
           "расходах: ");
    scanf("%s", filename);
    struct Expense expense = get_expense_from_file(filename, mode);
    printf("\nВаши расходы:\nМинуты: %d, гигабайты: %d, сообщения: %d\n\n",
           expense.minutes,
           expense.internet,
           expense.sms);
    struct Rate optimal_rate = find_optimal_rate(expense);
    if (optimal_rate.minutes != 2147483647)
        printf("Был подобран оптимальный тариф %s за %dр.\nВ него "
               "входит\nМинуты: %d, гигабайты: %d, сообщения: %d\n",
               optimal_rate.name,
               optimal_rate.price,
               optimal_rate.minutes,
               optimal_rate.internet,
               optimal_rate.sms);
    else
        printf("Был подобран оптимальный тариф %s за %dр.\nВ него "
               "входит\nМинуты: безлимит, гигабайты: %d, сообщения: %d\n",
               optimal_rate.name,
               optimal_rate.price,
               optimal_rate.internet,
               optimal_rate.sms);

    return 0;
}
