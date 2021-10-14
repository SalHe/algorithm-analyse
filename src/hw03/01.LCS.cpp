#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <string>
#include <vector>

using namespace std;

typedef struct
{
    int length;
    int di;
    int dj;
} trace_info;

string findLCS(string s1, string s2)
{
    int n1 = s1.length(), n2 = s2.length();
    vector<vector<trace_info>> traces(n1 + 1, vector<trace_info>(n2 + 1, trace_info{0}));
    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < n2; j++)
        {
            if (s1[i] == s2[j])
            {
                traces[i + 1][j + 1].length = traces[i][j].length + 1;
                traces[i + 1][j + 1].di = traces[i + 1][j + 1].dj = -1;
            }
            else if (traces[i + 1][j].length > traces[i][j + 1].length)
            {
                traces[i + 1][j + 1].length = traces[i + 1][j].length;
                traces[i + 1][j + 1].dj = -1;
            }
            else
            {
                traces[i + 1][j + 1].length = traces[i][j + 1].length;
                traces[i + 1][j + 1].di = -1;
            }
        }
    }

    int n = traces[n1][n2].length;
    string result(n, '\0');
    int i = n1, j = n2;
    while (i > 0 && j > 0)
    {
        if (s1[i - 1] == s2[j - 1])
            result[--n] = s1[i - 1];
        trace_info *t = &traces[i][j];
        i += t->di;
        j += t->dj;
    }

    return result;
}

TEST_CASE("LCS")
{
    CHECK_EQ(findLCS("123", ".1.23"), "123");
    CHECK_EQ(findLCS("_1_23", "123"), "123");
    CHECK_EQ(findLCS("_1_23___", "123"), "123");
    CHECK_EQ(findLCS("_1_23___", ".1..2.3"), "123");
}