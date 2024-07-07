#include <stdio.h>
#include <iostream>

using namespace std;

int num[30] = {99, 22, 51, 63, 72, 61, 20, 88, 40, 21, 63, 30, 11, 18, 99, 12, 93, 16, 7, 53, 64, 9, 28, 84, 34, 96, 52, 82, 51, 77};

int main()
{
    int count = 0;
    for (int i = 0; i < 30; i++)
    {
        int sum = 0;
        for (int j = i + 1; j < 30; j++)
        {
            // if(num[j] == num[i]){
            //   continue;
            //}
            sum = num[i] * num[j];
            if (sum > 2022)
            {
                count++;
            }
        }
    }
    cout <<count<< endl;
}