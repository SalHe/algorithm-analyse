#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <string>
#include <sstream>

using namespace std;

#define NULL_SUB_NODE -1
#define NOT_NEEDED -2

typedef struct PersonNode
{
    string name;
    int social;
    PersonNode *child;   // left
    PersonNode *sibling; // right
    int childId = NULL_SUB_NODE;
    int siblingId = NULL_SUB_NODE;

    /**
     * @brief 将人员关系括号表达式转换成树结构
     * 
     * @param expression 括号表达式，有唯一一个根节点——Boss。例如：0#Boss(5#SalHe(6#Tom,4#Jerry),5#Letty(6#What,8#GEM,9#Superman))。
     * @param nodes 用于储存所有节点指针的[vector]。
     * @param rootId 用于存储当前表达式根节点在[nodes]中的索引。
     * @return PersonNode* 二叉树根节点。
     */
    static PersonNode *from(string expression, vector<PersonNode *> *nodes = nullptr, int *rootId = nullptr)
    {
        PersonNode *personnel = new PersonNode{};
        if (rootId && nodes)
        {
            *rootId = nodes->size();
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
            personnel->child = from(childrenExpression.str(), nodes, &personnel->childId);
        }

        // 处理兄弟节点
        string siblingExpression;
        if (ch == ')')
            ss >> ch;
        if (ch == ',')
        {
            getline(ss, siblingExpression);
            personnel->sibling = from(siblingExpression, nodes, &personnel->siblingId);
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
}

/**
 * @brief 本次题目核心算法[单位新年聚会人员安排计划]的单元测试。
 * 
 */
TEST_CASE("单位新年聚会人员安排计划")
{
}