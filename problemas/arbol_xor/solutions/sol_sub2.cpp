#include <bits/stdc++.h>
using namespace std;

int const Mxn = 2e5;

vector<array<int, 2>> adj[Mxn];

int dp[2][Mxn];
long long int ans = 0;

void dfs(int node = 0, int parent = -1) {
  dp[0][node] = 1, dp[1][node] = 0;
  int an = 0;
  for (auto [child, W] : adj[node]) {
    if (child == parent) continue;
    dfs(child, node);
    ans += dp[0][node]*dp[W^1][child];
    ans += dp[1][node]*dp[W][child];

    dp[0][node] += dp[W][child];
    dp[1][node] += dp[W^1][child];
  }
}

long long Encuentra_xor(int N, vector<int> u, vector<int> v, vector<int> w) {
  for (int i = 0; i < N - 1; i++) {
    adj[v[i]].push_back({u[i], w[i]});
    adj[u[i]].push_back({v[i], w[i]});
  }
  dfs();
  return ans;
}
