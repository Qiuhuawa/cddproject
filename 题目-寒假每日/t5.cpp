#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

typedef long long LL;

const int N = 100010;
int d,n;
int v[N],a[N];

int main(){
    scanf("%d%d", &n, &d);

    for (int i = 1; i < n; i ++ ) scanf("%d", &v[i]);
    for (int i = 1; i <= n; i ++ ) scanf("%d", &a[i]);

    LL res = 0, oil = 0, dist = 0;//res即要付的钱，oil为已有的油，dist为距离
    int price = a[1];

    for (int i = 2; i <= n; i ++ )
    {
        dist += v[i - 1];
        LL t = (dist + d - 1) / d - oil;
        res += t * price;
        oil += t;
        price = min(price, a[i]);
    }

    printf("%lld\n", res);
    return 0;
}