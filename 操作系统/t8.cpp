#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

const int N = 105, M = 105;

int n, m;
int g[N][M];

void add(int x1, int y1, int x2, int y2, int c)
{
    g[x1][y1] += c;
    g[x2 + 1][y1] -= c;
    g[x1][y2 + 1] -= c;
    g[x2 + 1][y2 + 1] += c;
}

int main()
{
    cin >> n >> m;

    int T;
    cin >> T;

    memset(g, 0, sizeof g);
    while (T--)
    {
        int r1, r2, c1, c2;
        cin >> r1 >> c1 >> r2 >> c2;
        add(r1, c1, r2, c2, 1);
    }

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            g[i][j] += g[i - 1][j] + g[i][j - 1] - g[i - 1][j - 1];

    int sum = 0;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            if (!g[i][j])
                sum++;

    cout << sum << endl;
    return 0;
}