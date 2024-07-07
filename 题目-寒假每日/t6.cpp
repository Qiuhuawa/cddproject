#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

const int N = 100010, INF = 2e9;

int n;
int x[N], a[N], b[N];
double f[N][2];

double get(int x1, int x2)
{
    if (x1 < x2) return (x2 - x1) / 0.7;
    return (x1 - x2) / 1.3;
}

int main()
{
    scanf("%d", &n);
    for (int i = 1; i <= n; i ++ )
        scanf("%d", &x[i]);
    for (int i = 1; i < n; i ++ )
        scanf("%d%d", &a[i], &b[i + 1]);

    for (int i = 0; i <= n; i ++ )
        f[i][0] = f[i][1] = INF;

    f[1][0] = x[1];
    for (int i = 2; i <= n; i ++ )
    {
        int d = x[i] - x[i - 1];
        f[i][0] = min(
            f[i - 1][0] + d,
            f[i - 1][1] + get(b[i - 1], 0) + d
        );
        f[i][1] = min(
            f[i - 1][0] + get(0, a[i - 1]),
            f[i - 1][1] + get(b[i - 1], a[i - 1])
        );
    }

    printf("%.2lf\n", min(f[n][0], f[n][1] + b[n] / 1.3));
    return 0;
}
