#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

typedef long long LL;

const int N = 100010;

int n;
char str[N];
int l[N], r[N], p[26];

int main()
{
    scanf("%s", str + 1);
    n = strlen(str + 1);

    for (int i = 1; i <= n; i ++ )
    {
        int t = str[i] - 'a';
        l[i] = p[t];
        p[t] = i;
    }

    for (int i = 0; i < 26; i ++ ) p[i] = n + 1;
    for (int i = n; i; i -- )
    {
        int t = str[i] - 'a';
        r[i] = p[t];
        p[t] = i;
    }

    LL res = 0;
    for (int i = 1; i <= n; i ++ )
        res += (LL)(i - l[i]) * (r[i] - i);

    printf("%lld\n", res);
    return 0;
}