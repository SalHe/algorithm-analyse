#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <vector>
#include <limits>

using namespace std;

void travelSub(vector<vector<int>> &A, int depth, vector<int> &path, vector<bool> &visited, int currentDistance, vector<int> &bestPath, int &distance)
{
    if (depth == A.size() + 1)
    {
        if (currentDistance < distance)
        {
            bestPath = path;
            distance = currentDistance;
        }
    }
    else if (depth == A.size())
    {
        int first = path[0];
        int last = path[depth - 1];
        path[depth] = first;
        currentDistance += A[last][first];
        travelSub(A, depth + 1, path, visited, currentDistance, bestPath, distance);
        path[depth] = -1; // 非必须
        currentDistance -= A[last][first];
    }
    else
    {
        for (int i = 0; i < A.size(); i++)
        {
            int last = depth > 0 ? path[depth - 1] : i;
            if (depth == 0 || (!visited[i] && A[last][i] > 0 && last != i)) // 最后一个条件(last != i)非必须，只是为了描述清晰
            {
                path[depth] = i;
                visited[i] = true;
                currentDistance += A[last][i];
                travelSub(A, depth + 1, path, visited, currentDistance, bestPath, distance);
                path[depth] = -1; // 非必须
                visited[i] = false;
                currentDistance -= A[last][i];
            }
        }
    }
}

vector<int> travel(vector<vector<int>> A)
{
    int locationsCount = A.size();
    vector<int> bestPath(locationsCount + 1, int(-1));
    vector<int> path(locationsCount + 1, int(-1));
    vector<bool> visited(locationsCount, bool(false));
    int distance = numeric_limits<int>::max();
    travelSub(

        A, 0,

        path,
        visited,
        0,

        bestPath, distance

    );
    return bestPath;
}

/**
 * @brief 对称矩阵（压缩储存）转一个完全矩阵，仅仅为了方便编程，完全可以在对称矩阵做到O(1)时间复杂度的索引。
 * 
 * @param tri 对称矩阵
 * @return vector<vector<int>> 完全矩阵 
 */
vector<vector<int>> T2R(vector<vector<int>> tri)
{
    int size = tri.size();
    vector<vector<int>> rect(size, vector<int>(size));
    for (int i = 0; i < size; i++)
        for (int j = 0; j <= i; j++)
            rect[i][j] = rect[j][i] = tri[i][j];
    return rect;
}

TEST_CASE("旅行商问题")
{
    CHECK_EQ(travel(T2R({
                 {0},
                 {1, 0},
                 {2, 6, 0},
                 {3, 4, 5, 0},
             })),
             (vector<int>){0, 1, 3, 2, 0});
    CHECK_EQ(travel(T2R({
                 {0},
                 {1, 0},
                 {2, 1, 0},
                 {1, 4, 1, 0},
             })),
             (vector<int>){0, 1, 2, 3, 0});
}
