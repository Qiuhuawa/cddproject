#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

typedef long long LL;

const int N = 100010;

int main()
{
    int n;
    scanf("%d", &n);

    LL res = 0, sum = 0;
    while (n -- )
    {
        int x;
        scanf("%d", &x);
        res += sum * x;
        sum += x;
    }

    printf("%lld\n", res);
    return 0;
}