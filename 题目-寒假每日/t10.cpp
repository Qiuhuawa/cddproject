#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

typedef long long LL;

const int N = 100010;

int n;
int a[N], b[N], c[N], w[N];

int work(int x[], int y[], int z[])
{
    for (int i = 0; i < n; i ++ )
        w[i] = x[i] - y[i] - z[i];
    sort(w, w + n, greater<int>());

    int res = -1;
    LL sum = 0;
    for (int i = 0; i < n; i ++ )
    {
        sum += w[i];
        if (sum > 0) res = i + 1;
        else break;
    }

    return res;
}

int main()
{
    scanf("%d", &n);
    for (int i = 0; i < n; i ++ ) scanf("%d", &a[i]);
    for (int i = 0; i < n; i ++ ) scanf("%d", &b[i]);
    for (int i = 0; i < n; i ++ ) scanf("%d", &c[i]);

    int res = max({work(a, b, c), work(b, a, c), work(c, a, b)});
    printf("%d\n", res);

    return 0;
}