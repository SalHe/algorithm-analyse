/**
 * @file mst.h
 * @author SalHe (salhe@domain.com)
 * @brief 本文件主要实现了[最短路径长度问题]的核心算法，同时支持使用剪枝优化版。要使用剪枝优化，需要在引入头文件之前定义宏[REMOVE_UNNECESSARY_BRANCHES]。
 * @version 0.1
 * @date 2021-11-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <vector>
#include "routes.h"

void shortestRouteSub(vector<vector<int>> &A, int destination, vector<int> &path, vector<bool> &visited, int distance, int depth, Routes &routes)
{
    if (path[depth - 1] == destination)
    {
        if (distance <= routes.minDistance)
        {
            if (distance < routes.minDistance)
            {
                routes.routes.clear();
                routes.minDistance = distance;
            }
            vector<int> actualPath;
            actualPath.assign(path.begin(), path.begin() + depth);
            routes.routes.push_back(actualPath);
        }
    }
    else
    {
        int last = path[depth - 1];
        for (int i = 0; i < A.size(); i++)
        {
            if (!visited[i] && A[last][i] > 0)
            {
#ifdef REMOVE_UNNECESSARY_BRANCHES  // 需要剪枝时就保留以下代码，否则删除，这里使用定义宏REMOVE_UNNECESSARY_BRANCHES作为开关
                if (A[last][i] + distance > routes.minDistance)
                {
                    continue;
                }
#endif
                path[depth] = i;
                distance += A[last][i];
                visited[i] = true;
                shortestRouteSub(A, destination, path, visited, distance, depth + 1, routes);

                path[depth] = -1; // 非必须
                distance -= A[last][i];
                visited[i] = false;
            }
        }
    }
}

Routes shortestRoute(vector<vector<int>> A, int start, int destination)
{
    Routes routes = {
        numeric_limits<int>::max()};
    if (A.size() <= 0)
        return routes;

    vector<int> path(A.size(), int(-1));
    vector<bool> visited(A.size(), bool(false));
    path[0] = start;
    visited[start] = true;
    shortestRouteSub(A, destination, path, visited, 0, 1, routes);
    return routes;
}