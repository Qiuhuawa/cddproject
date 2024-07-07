#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

int main()
{
    string s;
    cin >> s;

    int res = 0;
    for (int i = 0; i + 1 < s.size(); i++)
    {
        char a = s[i], b = s[i + 1];
        if (a == b || a == '?' || b == '?')
        {
            res++;
            i++;
        }
    }

    cout << res << endl;
    return 0;
}