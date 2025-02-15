#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>

using namespace std;

typedef long long LL;

int n;
vector<int> w[10];

int main()
{
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        int a, b;
        scanf("%d%d", &a, &b);
        w[a].push_back(b);
    }

    LL res = 0;
    int avg = n / 10;
    for (int i = 0; i < 10; i++)
        if (w[i].size() > avg)
        {
            sort(w[i].begin(), w[i].end());
            for (int j = 0; j < w[i].size() - avg; j++)
                res += w[i][j];
        }

    printf("%lld\n", res);
    return 0;
}