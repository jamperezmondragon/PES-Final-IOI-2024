#include <bits/stdc++.h>
using namespace std;

long long Encuentra_xor(int N, vector<int> u, vector<int> v, vector<int> w) {
  long long int ans = 0;
  for (int k = 0; k < 30; k++) {
    int curr = 0, cnt[2] = {1, 0}; 
    for (int i = 0; i < N - 1; i++) {
      curr ^= ((w[i] >> k)&1);
      ans += (cnt[curr^1] << k);
      cnt[curr]++;
    }
  }
  return ans;
}
