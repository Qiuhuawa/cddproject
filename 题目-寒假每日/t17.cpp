#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

typedef long long LL;

const int MOD = 1e9 + 7;

LL calc(int n)
{
    return n * (__int128)(n + 1) * (n * 2 + 1) / 6 % MOD;
}

int main()
{
    int n;
    cin >> n;

    int res = 0;
    for (int i = 1; i <= n;)
    {
        int x = n / i, y = n / x;
        res = (res + (calc(y) - calc(i - 1)) * x) % MOD;
        i = y + 1;
    }

    cout << (res + MOD) % MOD << endl;
    return 0;
}
