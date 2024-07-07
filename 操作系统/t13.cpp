#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int m;
char s[105];

int main()
{
    int i, j;
    unsigned long n;
    cin >> n >> m;
    for (i = 0; i < n; i++)
    {
        cin >> s[i];
    }
    while (m--)
    {
        for (i = 0; i < n - 1; i++)
            if (s[i] > s[i + 1])
            {
                for (j = i; j < n - 1; j++)
                {
                    s[j] = s[j + 1];
                }
                break;
            }
        n--;
    }
    for (i = 0; i < n; i++)
        cout << s[i];
    return 0;
}