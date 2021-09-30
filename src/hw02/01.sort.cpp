#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <vector>

using Numbers = std::vector<int>;

int findMiniumId(Numbers &numbers, int start, int end)
{
    if (start == end)
        return start;
    else if (start + 1 == end)
        return numbers[start] < numbers[end] ? start : end;
    else
    {
        int mid = (start + end) / 2;
        int id1 = findMiniumId(numbers, start, mid);
        int id2 = findMiniumId(numbers, mid, end);
        return numbers[id1] < numbers[id2] ? id1 : id2;
    }
}

void selectionSortByRef(Numbers &numbers)
{
    for (int i = 0; i < numbers.size(); i++)
    {
        int minId = findMiniumId(numbers, i, numbers.size() - 1);
        auto temp = numbers[i];
        numbers[i] = numbers[minId];
        numbers[minId] = temp;
    }
}

Numbers selectionSort(Numbers numbers)
{
    selectionSortByRef(numbers);
    return numbers;
}

TEST_CASE("分治选择排序")
{
    CHECK_EQ(selectionSort({7, 6, 1, 3, 4, 8, 5, 9, 2}), (Numbers){1, 2, 3, 4, 5, 6, 7, 8, 9});
}