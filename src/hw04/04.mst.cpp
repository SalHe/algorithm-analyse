#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>


#include "mst.h"

TEST_CASE("最短路径长度问题")
{
    CHECK_EQ(shortestRoute({

                               {0, 1, 3, 5, 2},
                               {-1, 0, 3, -1, 2},
                               {7, 1, 0, 8, 2},
                               {3, 1, -1, 0, 2},
                               {-1, 1, 3, 1, 0},

                           },
                           0, 4),

             Routes{2, {
                           {0, 4},
                       }});

    CHECK_EQ(shortestRoute({

                               {0, 4, 5, -1, 10},
                               {-1, 0, 3, 3, 4},
                               {1, 2, 0, 3, 3},
                               {3, 4, -1, 0, 1},
                               {5, 3, 2, 5, 0},

                           },
                           0, 4),

             Routes{8, {
                           {0, 1, 4},
                           {0, 1, 3, 4},
                           {0, 2, 4},
                       }});
}