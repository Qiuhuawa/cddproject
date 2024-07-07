#include <iostream>
#include <cstring>
#include <algorithm>

#define x first
#define y second

using namespace std;

typedef pair<int, int> PII;

const int N = 110;

int W, H;
int n, R;
PII NUM[N];

int main()
{
    cin >> W >> H >> n >> R;

    for (int i = 0; i < n; i++)
        cin >> NUM[i].x >> NUM[i].y;

    int end = 0;
    for (int x = 0; x <= W; x++)
        for (int y = 0; y <= H; y++)
        {
            bool flag = false;
            for (int j = 0; j < n; j++)
            {
                if ((x - NUM[j].x) * (x - NUM[j].x) +
                        (y - NUM[j].y) * (y - NUM[j].y) <=
                    R * R)
                {
                    flag = true;
                    break;
                }
            }

            if (flag)
                end++;
        }

    cout << end << endl;
    return 0;
}