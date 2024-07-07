#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

const int N = 1010, M = 10;

int n, m;
int w[N][M];

int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i ++ )
        for (int j = 0; j < m; j ++ )
            scanf("%d", &w[i][j]);

    for (int i = 1; i <= n; i ++ )
    {
        int id = 0;
        for (int j = 1; j <= n; j ++ )
        {
            bool is_bigger = true;
            for (int k = 0; k < m; k ++ )
                if (w[j][k] <= w[i][k])
                {
                    is_bigger = false;
                    break;
                }

            if (is_bigger)
            {
                id = j;
                break;
            }
        }

        printf("%d\n", id);
    }

    return 0;
}