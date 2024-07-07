#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

int main()
{

    int n;
    scanf("%d", &n);

    int days = 0, date = 0;
    while (n)
    {
        days++;
        if (!date && n % 3 == 1)
            date = days;
        n -= (n + 2) / 3;//相当于n除以三的向上取整。
    }

    printf("%d %d\n", days, date);
    return 0;
}