#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>

using namespace std;

const int N = 1000010;

int n;
char s[N];
int l[N], r[N];
vector<int> q, w;
bool st[N];

void insert(int k)
{
    if (!st[k])
    {
        st[k] = true;
        w.push_back(k);
    }
}

void filter_dels()
{
    w.clear();
    for (int k: q)
    {
        int a = l[k], b = k, c = r[k];
        if (s[a] == s[b] && s[b] != s[c] && s[c] != '#')
        {
            insert(b);
            insert(c);
        }
        else if (s[a] != s[b] && s[b] == s[c] && s[a] != '#')
        {
            insert(a);
            insert(b);
        }
    }
}

int main()
{
    scanf("%s", s + 1);
    n = strlen(s + 1);
    s[0] = s[n + 1] = '#';

    for (int i = 1; i <= n; i ++ )
    {
        l[i] = i - 1, r[i] = i + 1;
        q.push_back(i);
    }
    r[0] = 1, l[n + 1] = n;

    while (true)
    {
        filter_dels();
        if (w.empty()) break;

        q.clear();
        for (int k: w)
        {
            int a = l[k], b = k, c = r[k];

            if (!st[a] && a && (q.empty() || a != q.back())) q.push_back(a);
            if (!st[c] && c != n + 1) q.push_back(c);

            r[a] = c, l[c] = a;
        }
    }

    if (r[0] == n + 1) puts("EMPTY");
    else
    {
        for (int i = r[0]; i != n + 1; i = r[i])
            printf("%c", s[i]);
    }

    return 0;
}
