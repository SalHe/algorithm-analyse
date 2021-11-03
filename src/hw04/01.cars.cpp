#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <vector>

using namespace std;

/**
 * @brief 求解汽车加油问题。
 * 
 * @param onceDistance 加满一次油汽车最大可以行驶的距离。
 * @param totalDistance 原点到达目的地的距离。
 * @param stations 加油站的位置(假设起点处没有加油站)。
 * @return vector<int> 需要停下来加油的加油站编号。
 * 
 * @example
 * 
 * 假设路线（小数点为加油站，下划线代表一个单位距离）：<起点> _ _ _ . _ _ _ _ . _ _ <终点>
 * 则 stations = {3, 4}, totalDistance = 3 + 4 + 7 = 9;
 * 注意到stations[0]是起点到第一个加油站的距离。
 */
vector<int> driveCar(int onceDistance, int totalDistance, vector<int> stations)
{
    if (stations.size() <= 0)
        return {};

    for (auto &&d : stations)
    {
        totalDistance -= d;
    }
    stations.push_back(totalDistance);
    // stations = {3, 4}, totalDistance = 9
    // <起点> _ _ _ . _ _ _ _ . _ _ <终点>
    //
    // 预处理为 => stations = {3, 4, 2};

    vector<int> stops;
    int currentGas = onceDistance;
    for (int i = 0; i < stations.size(); i++)
    {
        if (stations[i] > currentGas)
        {
            // 当前的汽油不足以开往下一站，那么本站必须加油
            stops.push_back(i);
            currentGas = onceDistance - stations[i];
        }
        else
        {
            currentGas -= stations[i];
        }
    }
    return stops;
}

TEST_CASE("求解汽车加油问题")
{
    // ^ 代表汽车位置，后接车油剩余里程，加号代表在对应站点加油
    // <起点>^4 _ _ _ . _ _ _ _ . _ _ <终点>
    // <起点> _ _ _ .^1+ _ _ _ _ . _ _ <终点>
    // <起点> _ _ _ . _ _ _ _ .^0+ _ _ <终点>
    CHECK_EQ(driveCar(4, 9, {3, 4}), (vector<int>){1, 2});

    // <起点> _ _ _ . _ _ _ _ . _ . _ . _ . _ _ . _ _ _ _ <终点>
    CHECK_EQ(driveCar(4, 16, {3, 4, 1, 1, 1, 2}), (vector<int>){1, 2, 5, 6});
}