#include <iostream>
#include <cstring>
#include <algorithm>
#include <cmath>

using namespace std;

const int N = 1000010, M = 20;

int n, k, t;
int q[N];
int f[N][M]; // min_val

int query(int l, int r) {
	int len = log(r - l + 1) / log(2);
	int x = f[l][len], y = f[r - (1 << len) + 1][len];
	
	int res = 0;
	if(q[x] > q[y]) res = y;
	else res = x;
	
	return res;
}

int main() {
	scanf("%d", &n);
	for(int i = 1; i <= n; i ++) scanf("%d", &q[i]);
	
	scanf("%d", &k);
	
	t = log(n) / log(2);
	
	for(int j = 0; j <= t; j ++) {
		for(int i = 1; i + (1 << j) - 1 <= n; i ++) {
			if(!j) f[i][j] = i;
			else {
				int l = f[i][j - 1], r = f[i + (1 << j - 1)][j - 1];
				if(q[l] > q[r]) f[i][j] = r;
				else f[i][j] = l;
			}
		}
	}
	
	int l, r;
	for(int i = 1; i <= n; i ++) {
		l = max(1, i - k), r = min(n, i + k);
		cout << q[query(l, r)] << " ";
	}
	
	cout << "\n";
	
	return 0;
}
