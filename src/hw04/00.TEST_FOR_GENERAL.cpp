#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include "routes.h"

TEST_CASE("Routes 类型\"==\"运算符重载测试")
{
    CHECK_EQ(Routes{3, {{1, 2}, {3, 4}}},
             Routes{3, {{3, 4}, {1, 2}}});
    CHECK_NE(Routes{5, {{1, 2}, {3, 4}}},
             Routes{3, {{3, 4}, {1, 2}}});
}