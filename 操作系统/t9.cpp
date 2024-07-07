#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

const int N = 110, M = 110;

int n, m;
int g[N][M];
bool vis[N][N];

int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

int dfs(int x, int y)
{
	int dep = 0;
	for (int d = 0; d < 4; d++)
	{
		int a = x + dx[d], b = y + dy[d];
		if (a >= 1 && a <= n && b >= 1 && b <= m && !vis[a][b] && g[x][y] > g[a][b])
		{
			vis[a][b] = true;
			dep = max(dep, dfs(a, b) + 1);
			vis[a][b] = false;
		}
	}

	return dep;
}

int main()
{
	cin >> n >> m;

	int sum = 0;

	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			cin >> g[i][j];

	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
		{
			memset(vis, 0, (n + 1) * (m + 1) * 4);
			vis[i][j] = true;
			sum = max(sum, dfs(i, j));
			vis[i][j] = false;
		}

	cout << sum + 1 << endl;
	return 0;
}
