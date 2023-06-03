#include <ctest.h>
#include <ctype.h>
#include <dirent.h>
#include <string.h>

#include <o_rate.h>

CTEST(searchFile, test_1) {
  char filename[100] = "expenses.json";
  const char reldir[] = "../thirdparty/";
  int result = searchFile(filename, reldir);

  ASSERT_EQUAL(0, result);
  ASSERT_STR("../thirdparty/expenses.json", filename);
}
