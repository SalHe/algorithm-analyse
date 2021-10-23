#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <vector>

using namespace std;

typedef struct
{
    int deadline;
    int punishment;
} Task;

#define NOT_FOUND -1
#define IS_FOUND(x) (x >= 0)

vector<int> arrangeTasks(vector<Task> tasks)
{
    // 任务分类：
    // 1.未超过DDL的
    // 2.超过DDL

    // 希望所有的任务都要被调度到：
    // 1.在同样截止时间之前，优先考虑先调度惩罚大的任务（毫无疑问）
    // 2.有超过DDL的，也有在DDL之前的，超过DDL的任务的惩罚不会再变了，所以不如先把在DDL之前的解决了，防止过多不必要的惩罚

    vector<int> schedule(tasks.size(), int(NOT_FOUND));
    vector<bool> arranged(tasks.size(), bool(false));
    int t = 0;
    while (true)
    {
        int candidate = NOT_FOUND;
        for (int i = 0; i < tasks.size(); i++)
        {
            if (t < tasks[i].deadline)
            {
                if (arranged[i])
                    continue;
                if (!IS_FOUND(candidate) ||
                    (t < tasks[i].deadline && tasks[i].deadline < tasks[candidate].deadline) ||
                    (tasks[i].deadline <= tasks[candidate].deadline &&
                     tasks[i].punishment > tasks[candidate].punishment))
                {
                    candidate = i;
                }
            }
        }
        if (!IS_FOUND(candidate))
            break;
        arranged[candidate] = true;
        schedule[t] = candidate;
        t++;
    }

    for (int i = 0; i < tasks.size(); i++)
    {
        if (!arranged[i])
        {
            arranged[i] = true;
            schedule[t] = i;
            t++;
        }
    }
    return schedule;
}

TEST_CASE("带惩罚的任务调度算法")
{
    CHECK_EQ(arrangeTasks({
                 {2, 1},
                 {1, 6},
                 {2, 7},
             }),
             (vector<int>){1, 2, 0});
    CHECK_EQ(arrangeTasks({{2, 1},
                           {3, 8},
                           {2, 9},
                           {1, 6},
                           {2, 7},
                           {3, 2},
                           {4, 1}}),
             (vector<int>){3, 2, 1, 6, 0, 4, 5});
    CHECK_EQ(arrangeTasks({{2, 1},
                           {4, 2},
                           {2, 9},
                           {1, 6},
                           {2, 7},
                           {4, 8},
                           {5, 1}}),
             (vector<int>){3, 2, 5, 1, 6, 0, 4});
}