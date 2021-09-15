#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

TEST_CASE("simple test")
{
    CHECK(1 == 1);
    CHECK(1 == 2);
    CHECK(3*3 == 9);
    CHECK(3*3 == 28);
}