#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

typedef long long LL;

int main()
{
    LL n;
    cin >> n;

    int res = 0;
    for (LL i = 2; i * i <= n; i ++ )
        if (n % i == 0)
        {
            while (n % i == 0) n /= i;
            res ++ ;
        }

    if (n > 1) res ++ ;
    cout << res << endl;

    return 0;
}