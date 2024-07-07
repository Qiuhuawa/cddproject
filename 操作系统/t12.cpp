#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main()
{
    double L;
    cin >> L;
    int sum = 0;
    while (L > 1)
    {
        L = L / 2;
        sum++;
    }
    cout << sum << endl;
    return 0;
}