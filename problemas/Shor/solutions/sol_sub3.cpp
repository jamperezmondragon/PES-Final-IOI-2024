#include <bits/stdc++.h>
using namespace std;

vector<int> El_Robot_Shor(int N, int Q, vector<int> p, vector<int> u, vector<int> v) {
  vector<int> ans(Q + 1, N);
  for (int i = 0; i <= Q; i++) {
    vector<bool> vis(N, false);
    for (int j = 0; j < N; j++) {
      if (vis[j]) continue;
      ans[i]--;
      for (int k = p[j]; j != k; k = p[k]) vis[k] = true;
    }
    if (i != Q) swap(p[u[i]], p[v[i]]);
    ans[i] = ((ans[i]&1) ? -1 : (ans[i]>>1));
  }
  return ans;
}
