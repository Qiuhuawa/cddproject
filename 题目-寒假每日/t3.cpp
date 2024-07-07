#include <iostream>
#include <cstring>
#include <algorithm>

#define x first
#define y second

using namespace std;

typedef long long LL;

typedef pair<int, int> PII;

const int N = 100010;

int n, m;
PII w[N], q[N];

bool check(int mid)
{
    int cnt = 0;
    for (int i = 0; i < n; i ++ )
    {
        int L = w[i].x, S = w[i].y;
        if (S <= mid)
        {
            int t = mid - S;
            int l = max(1, L - t), r = min((LL)m, (LL)L + t);
            q[cnt ++ ] = {l, r};
        }
    }
    sort(q, q + cnt);

    int st = -1, ed = -1;
    for (int i = 0; i < cnt; i ++ )
        if (q[i].x <= ed + 1) ed = max(ed, q[i].y);
        else st = q[i].x, ed = q[i].y;

    return st == 1 && ed == m;
}

int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n; i ++ ) scanf("%d%d", &w[i].x, &w[i].y);

    int l = 0, r = 2e9;
    while (l < r)
    {
        int mid = (LL)l + r >> 1;
        if (check(mid)) r = mid;
        else l = mid + 1;
    }

    printf("%d\n", r);
    return 0;
}