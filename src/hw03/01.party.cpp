#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <string>
#include <sstream>
#include <queue>

using namespace std;

#define NULL_NODE -1
#define NOT_NEEDED -2
#define IS_VALID_ID(x) (x >= 0)

typedef struct PersonNode
{
    string name;
    int social;
    PersonNode *child;   // left
    PersonNode *sibling; // right
    PersonNode *parent;
    int parentId = NULL_NODE;
    int childId = NULL_NODE;
    int siblingId = NULL_NODE;

    /**
     * @brief 将人员关系括号表达式转换成树结构
     * 
     * @param expression 括号表达式，有唯一一个根节点——Boss。例如：0#Boss(5#SalHe(6#Tom,4#Jerry),5#Letty(6#What,8#GEM,9#Superman))。
     * @param nodes 用于储存所有节点指针的[vector]。
     * @param rootId 用于存储当前表达式根节点在[nodes]中的索引。
     * @return PersonNode* 二叉树根节点。
     */
    static PersonNode *from(string expression, vector<PersonNode *> *nodes = nullptr, int *rootId = nullptr,
                            PersonNode *parent = nullptr, int parentId = NULL_NODE)
    {
        PersonNode *personnel = new PersonNode{.parent = parent, .parentId = parentId};
        int id = nodes->size();
        if (rootId && nodes)
        {
            *rootId = id;
            nodes->push_back(personnel);
        }
        else if (rootId && !nodes)
        {
            *rootId = NOT_NEEDED;
        }
        else if (nodes)
        {
            nodes->push_back(personnel);
        }

        // 获取评分
        stringstream ss(expression);
        ss >> personnel->social;

        // 获取名字
        char ch;
        ss >> ch; // '#'
        ss >> ch;
        stringstream namestream;
        while (!ss.eof() && ch != '(' && ch != ',')
        {
            namestream << ch;
            ss >> ch;
        }
        personnel->name = namestream.str();

        // 获取子节点信息
        int leftCount = 1;
        stringstream childrenExpression;
        if (ch == '(')
        {
            ss >> ch;
            while (ch != ')' && leftCount <= 0 || leftCount > 0)
            {
                childrenExpression << ch;
                ss >> ch;
                if (ch == '(')
                    leftCount++;
                else if (ch == ')')
                    leftCount--;
            }
            personnel->child = from(childrenExpression.str(), nodes, &personnel->childId, personnel, id);
        }

        // 处理兄弟节点
        string siblingExpression;
        if (ch == ')')
            ss >> ch;
        if (ch == ',')
        {
            getline(ss, siblingExpression);
            personnel->sibling = from(siblingExpression, nodes, &personnel->siblingId,
                                      personnel->parent, personnel->parentId);
        }

        return personnel;
    }
} Personnel;

/**
 * @brief 主要负责括号表达式转换函数的单元测试。
 * 
 */
TEST_CASE("括号表达式转换")
{
    vector<PersonNode *> nodes;
    Personnel *p = Personnel::from("0#Boss(5#SalHe(6#Tom,4#Jerry),5#Letty(6#What,8#GEM,9#Superman))", &nodes);

    SUBCASE("表达式解析正确性")
    {
        CHECK_EQ(p->name, "Boss");
        CHECK_EQ(p->social, 0);
        CHECK_EQ(p->child->name, "SalHe");
        CHECK_EQ(p->child->social, 5);
        CHECK_EQ(p->child->child->name, "Tom");
        CHECK_EQ(p->child->child->social, 6);
        CHECK_EQ(p->child->child->sibling->name, "Jerry");
        CHECK_EQ(p->child->child->sibling->social, 4);
        CHECK_EQ(p->child->sibling->name, "Letty");
        CHECK_EQ(p->child->sibling->social, 5);
        CHECK_EQ(p->child->sibling->child->name, "What");
        CHECK_EQ(p->child->sibling->child->social, 6);
        CHECK_EQ(p->child->sibling->child->sibling->name, "GEM");
        CHECK_EQ(p->child->sibling->child->sibling->social, 8);
        CHECK_EQ(p->child->sibling->child->sibling->sibling->name, "Superman");
        CHECK_EQ(p->child->sibling->child->sibling->sibling->social, 9);
    }

    SUBCASE("节点终止测试")
    {
        CHECK_EQ(p->sibling, nullptr);
        CHECK_EQ(p->child->sibling->child->sibling->sibling->sibling, nullptr);
    }

    SUBCASE("nodes测试")
    {
        for (int i = 0; i < nodes.size(); i++)
        {
            auto node = nodes[i];
            if (node->child)
                CHECK_EQ(node->child, nodes[node->childId]);
            if (node->sibling)
                CHECK_EQ(node->sibling, nodes[node->siblingId]);
        }
    }

    SUBCASE("节点父子关系")
    {
        for (int i = 0; i < nodes.size(); i++)
        {
            auto node = nodes[i];
            if (node->child)
            {
                CHECK_EQ(node, nodes[node->child->parentId]);
                CHECK_EQ(node, node->child->parent);
            }
        }
    }
}

#define MAX(x, y) (x > y ? x : y)

typedef struct
{
    int social;
    vector<int> participants;
} Answer;

Answer arrangeParty(vector<PersonNode *> &nodes, int rootId = 0)
{
    /**
     * @brief DP数组中对节点相关信息的记录
     * 
     */
    typedef struct
    {
        int participating;   // In(n): 与会时可以获得的最大评分和
        int noParticipating; // NotIn(n): 不与会时可以获得的最大评分和
        int childs;          // 直接子节点个数
        bool consumed;       // 是否已被计算
    } PersonScore;
    vector<PersonScore> scores(nodes.size(), PersonScore{0, 0, 0, false}); // DP数组

    // 对节点的直接子节点个数做出统计
    for (int i = 0; i < nodes.size(); i++)
    {
        int pid = nodes[i]->parentId;
        if (IS_VALID_ID(pid))
        {
            scores[pid].childs++;
        }
    }

    queue<int> candidates; // 可以开始计算评分和的节点（只有当节点的直接子节点没有或计算完成时才可以计算）
    // 最开始我们需要选出叶节点
    for (int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i]->child == nullptr)
        {
            candidates.push(i);
        }
    }

    while (!candidates.empty())
    {
        int id = candidates.front();
        candidates.pop();

        // 计算候选节点的最大评分情况
        scores[id].participating = nodes[id]->social;
        scores[id].noParticipating = 0;
        int child = nodes[id]->childId;
        while (IS_VALID_ID(child))
        {
            scores[id].participating += scores[child].noParticipating;
            scores[id].noParticipating += MAX(scores[child].noParticipating, scores[child].participating);
            child = nodes[child]->siblingId;
        }

        // 标记当前节点已处理
        scores[id].consumed = true;
        // 通知父节点当前节点已处理
        int pid = nodes[id]->parentId;
        if (IS_VALID_ID(pid))
            scores[pid].childs--;

        // 加入下一次的候选节点（也就是考虑那些节点的直接子节点已经计算完毕）
        if (candidates.empty())
        {
            for (int i = 0; i < nodes.size(); i++)
            {
                if (!scores[i].consumed && scores[i].childs <= 0) // == 0
                    candidates.push(i);
            }
        }
    }

    Answer ans = {MAX(scores[rootId].participating, scores[rootId].noParticipating)};
    // candidates.empty() == true
    candidates.push(rootId);
    while (!candidates.empty())
    {
        int id = candidates.front();
        candidates.pop();

        if (scores[id].participating > scores[id].noParticipating)
        {
            ans.participants.push_back(id);
            int child = nodes[id]->childId;
            while (IS_VALID_ID(child))
            {
                int childOfChild = nodes[child]->childId;
                while (IS_VALID_ID(childOfChild))
                {
                    candidates.push(childOfChild);
                    childOfChild = nodes[childOfChild]->siblingId;
                }
                child = nodes[child]->siblingId;
            }
        }
        else
        {
            int child = nodes[id]->childId;
            while (IS_VALID_ID(child))
            {
                candidates.push(child);
                child = nodes[child]->siblingId;
            }
        }
    }

    return ans;
}

/**
 * @brief 本次题目核心算法[单位新年聚会人员安排计划]的单元测试。
 * 
 */
TEST_CASE("单位新年聚会人员安排计划")
{

    typedef struct
    {
        string expression;
        Answer expected;
    } TestCase;
    vector<TestCase> cases = {
        // 参选者ID跟树的生成方式有关：可将树节点按深度优先从0开始编号
        {"0#Boss(5#SalHe(6#Tom,4#Jerry),5#Letty(6#What,8#GEM,9#Superman))", {33, {2, 3, 5, 6, 7}}},
        {"0#Boss(11#SalHe(6#Tom,4#Jerry),5#Letty(6#What,8#GEM,9#Superman))", {34, {1, 5, 6, 7}}},
        {"0#Boss(11#SalHe(6#Tom,4#Jerry(11#SalHe(6#Tom,9#Jerry))),5#Letty(6#What,8#GEM,9#Superman))", {49, {1, 5, 6, 8, 9, 10}}},
    };

    for (int i = 0; i < cases.size(); i++)
    {
        SUBCASE(cases[i].expression.c_str())
        {
            vector<PersonNode *> nodes;
            int rootId;
            Personnel *p = Personnel::from(cases[i].expression, &nodes, &rootId);

            Answer actual = arrangeParty(nodes, rootId);
            sort(actual.participants.begin(), actual.participants.end()); // 排序方便用于CHECK_EQ中对比
            CHECK_EQ(cases[i].expected.social, actual.social);
            CHECK_EQ(cases[i].expected.participants, actual.participants);
        }
    }
}