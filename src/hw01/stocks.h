#include <vector>
#include <tuple>

using namespace std;

tuple<double, int, int> predictStock(vector<double> prices)
{
    // 思路分析已放入[src/hw01/README.md]中

    double max = 0, left = 0, right = 0;
    int tryStart = 0;
    for (int i = 1; i < prices.size(); i++)
    {
        double curr = prices[i] - prices[tryStart];
        if (curr > max)
        {
            max = curr;
            left = tryStart;
            right = i;
        }
        else if (curr <= 0)
        {
            tryStart = i;
        }
    }
    return {max, left, right};
}