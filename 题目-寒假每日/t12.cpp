#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

const int N = 100010;

int n;
char a[N], b[N];
int f[N][3];

int main()
{
    scanf("%d%s%s", &n, a, b);

    memset(f, 0x3f, sizeof f);
    f[n][1] = 0;
    for (int i = n - 1; i >= 0; i -- )
        for (int j = 0; j < 3; j ++ )
            for (int k = -9; k <= 9; k ++ )
                for (int t = 0; t < 3; t ++ )
                    if (a[i] + k + t - 1 - b[i] == (j - 1) * 10)
                        f[i][j] = min(f[i][j], f[i + 1][t] + abs(k));

    printf("%d\n", min({f[0][0], f[0][1], f[0][2]}));
    return 0;
}