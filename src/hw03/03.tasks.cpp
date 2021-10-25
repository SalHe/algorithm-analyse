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
    // 3.第二条可能有点问题需要修正，假如较迟DDL的任务无法大部分及时完成带来的惩罚比DDL更早的任务规划到以后在执行(即便会超过DDL带来惩罚)的惩罚更大，那么可以优先考虑尽可能多的完成那种惩罚较大、又无法及时完成的任务

    vector<int> schedule(tasks.size(), int(NOT_FOUND)); // 规划任务顺序
    vector<bool> arranged(tasks.size(), bool(false));   // 用于记录某个任务是否已规划

    vector<int> ddl(tasks.size() + 1, int(0));        // 刚好截止至某个DDL的任务个数
    vector<int> punishment(tasks.size() + 1, int(0)); // 刚好超过某个DDL的任务总惩罚
    for (int i = 0; i < tasks.size(); i++)
    {
        ddl[tasks[i].deadline]++;
        punishment[tasks[i].deadline] += tasks[i].punishment;
    }

    int t = 0;
    while (true)
    {
        int candidate = NOT_FOUND;
        for (int i = 0; i < tasks.size(); i++)
        {
            if (t < tasks[i].deadline)
            {
                // 如果任务已被安排那么跳过
                if (arranged[i])
                    continue;

                if (

                    // 如果当前循环还没有candidate，那么需要选择初次遇到的任务作为candidate
                    !IS_FOUND(candidate) ||

                    // 任务都还没过ddl，并且ddl更早（对应情况1）
                    (t < tasks[i].deadline && tasks[i].deadline < tasks[candidate].deadline) ||

                    // ddl更早或一样，惩罚力度更大的情况（对应情况2）
                    (tasks[i].deadline <= tasks[candidate].deadline &&
                     tasks[i].punishment > tasks[candidate].punishment) ||

                    // 如果：（对应补充的情况3）
                    (
                        // 当前的任务惩罚力度更大，
                        tasks[i].punishment > tasks[candidate].punishment &&
                        // 而且她所对应的DDL刚好截止的任务刚好能够全部完成或者无法全部完成
                        tasks[i].deadline - tasks[candidate].deadline <= ddl[tasks[i].deadline] &&
                        // 而且【先做任务i带来的惩罚】比【先不做任务candidate带来的惩罚】大
                        punishment[tasks[i].deadline] - tasks[i].punishment >
                            punishment[tasks[candidate].deadline])

                    //
                )
                {
                    candidate = i;
                }
            }
        }
        if (!IS_FOUND(candidate))
            break;
        arranged[candidate] = true;
        ddl[tasks[candidate].deadline]--;
        punishment[tasks[candidate].deadline] -= tasks[candidate].punishment;
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
             (vector<int>)/* {3, 2, 1, 6, 0, 4, 5} 惩罚10 */ {4, 2, 1, 6, 0, 3, 5});
    CHECK_EQ(arrangeTasks({{2, 1},
                           {4, 2},
                           {2, 9},
                           {1, 6},
                           {2, 7},
                           {4, 8},
                           {5, 1}}),
             (vector<int>)/* {3, 2, 5, 1, 6, 0, 4} 惩罚8 */ {4, 2, 5, 1, 6, 0, 3});
    CHECK_EQ(arrangeTasks({{1, 3},
                           {2, 4},
                           {2, 4}}),
             (vector<int>){1, 2, 0});
}