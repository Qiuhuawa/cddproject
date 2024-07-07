#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

const int N = 1000010;

int n;
char str[N];
int f[N][2];

int main()
{
    scanf("%s", str + 1);//从i=1开始存储，因为要求是i＞1.
    n = strlen(str + 1);

    for (int i = 1; i <= n; i ++ )
    {
        f[i][0] = max(f[i - 1][0], f[i - 1][1]);
        f[i][1] = f[i - 1][0] + str[i] - 'a' + 1;

        //f[i] = max(f[i-1] , f[i-2] + str[i] - 'a' + 1);线性DP。
    }

    printf("%d\n", max(f[n][0], f[n][1]));

    //printf("%d\n", f[n]);即最大值
    return 0;
}