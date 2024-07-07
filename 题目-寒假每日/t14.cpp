#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

const int N = 1000010;

char s[N], t[N];

int main()
{
    int T;
    scanf("%d", &T);

    while (T -- )
    {
        scanf("%s%s", t, s);
        int n = strlen(s);

        int res = 0;
        for (int i = 0; i < n; i ++ )
            if (s[i] != t[i])
            {
                if (!i || i == n - 1 || s[i] == s[i - 1] || s[i] == s[i + 1])
                {
                    res = -1;
                    break;
                }
                res ++ ;
                s[i] = t[i];
            }

        printf("%d\n", res);
    }

    return 0;
}
