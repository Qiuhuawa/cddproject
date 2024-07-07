#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

typedef long long LL;

const int P = 998244353;

int n;

LL qmi(int a, int b)
{
    LL res = 1;
    while (b)
    {
        if (b & 1) res = res * a % P;
        a = (LL)a * a % P;
        b >>= 1;
    }
    return res;
}

int main()
{
    scanf("%d", &n);

    int res = 0;
    while (n -- )
    {
        int x, y;
        scanf("%d%d", &x, &y);

        res = (res + 1ll) * y % P * qmi(y - x, P - 2) % P;
    }

    printf("%d\n", res);
    return 0;
}
