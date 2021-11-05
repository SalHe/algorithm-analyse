/**
 * @file routes.h
 * @author SalHe (salhe@qq.com)
 * @brief 本文件主要定义了[最短路径长度问题]的解信息结构，包括了：最短路径长度，所有路径。同时为该结构体的"=="运算符做了重载，方便使用doctest做单元测试时比较算法实现给出的解是否与参考解匹配。
 * @version 0.1
 * @date 2021-11-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <vector>
#include <limits>

using namespace std;

typedef struct Routes
{
    int minDistance;
    vector<vector<int>> routes;
} Routes;


bool operator==(const Routes x, const Routes y)
{
    if (x.minDistance != y.minDistance || x.routes.size() != y.routes.size())
        return false;

    vector<bool> visited(x.routes.size(), bool(false));
    for (int i = 0; i < y.routes.size(); i++)
    {
        bool isFound = false;
        for (int j = 0; j < x.routes.size(); j++)
        {
            if (!visited[j] && x.routes[j] == y.routes[i])
            {
                visited[j] = isFound = true;
                break;
            }
        }

        if (!isFound)
            return false;
    }
    return true;
}

bool operator!=(const Routes x, const Routes y)
{
    return !(x == y);
}