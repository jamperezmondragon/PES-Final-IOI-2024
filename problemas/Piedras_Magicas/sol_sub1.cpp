#include <bits/stdc++.h>
using namespace std;

long long int Piedras_Magicas(int N, int A, int B, int Q, vector<int> a) {
  int ans = Q*N;
  for (int mask = 0; mask < (1<<Q); mask++) {
    int curr[2] = {A, B}, cost = 0;
    for (int i = 0; i < Q; i++) {
      cost += abs(curr[(mask>>i)&1] - a[i]);
      curr[(mask>>i)&1] = a[i];
    }
    ans = min(ans, cost);
  }
  return ans;
}
