#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;
 
int n;
string s;

int main()
{
    cin >> n >> s;

    if (s == string(n, 'F'))
    {
        cout << n << endl;
        for (int i = 0; i < n; i++)
        {
            cout << i << endl;
        }
    }
    else
    {
        int l = 0, r = n - 1;
        while (s[l] == 'F')
            l++;
        while (s[r] == 'F')
            r--;

        int low = 0, high = 0;
        auto str = s;
        for (int i = l; i <= r; i++)
        {
            if (str[i] == 'F')
            {
                if (str[i - 1] == 'S')
                    str[i] = 'E';
                else
                    str[i] = 'B';
            }
            if (i > l && str[i] == str[i - 1])
                low++;
        }

        str = s;
        for (int i = l; i <= r; i++)
        {
            if (str[i] == 'F')
                str[i] = str[i - 1];
            if (i > l && str[i] == str[i - 1])
                high++;
        }

        int ends = l + n - 1 - r, d = 2;
        if (ends)
            high += ends, d = 1;

        cout << (high - low) / d + 1 << endl;
        for (int i = low; i <= high; i += d)
            cout << i << endl;
    }
    return 0;
}