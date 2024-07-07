#include <iostream>

using namespace std;

int main()
{
    int day, afterday;
    int sum = 0;
    cin >> day >> afterday;
    int enday = afterday % 7;
    if (day + enday > 7)
    {
        sum = day + enday - 7;
    }
    else
        sum = day + enday;

    cout << sum << endl;
    return 0;
}