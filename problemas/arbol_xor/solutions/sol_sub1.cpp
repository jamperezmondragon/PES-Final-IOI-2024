#include <bits/stdc++.h>
using namespace std;

int const Mxn = 2000;

vector<array<int, 2>> adj[Mxn];

int ans = 0;

void dfs(int node, int parent = -1, int cnt = 0) {
  ans += cnt;
  for (auto [child, W] : adj[node]) {
    if (child != parent) dfs(child, node, cnt^W);
  }
}

long long Encuentra_xor(int N, vector<int> u, vector<int> v, vector<int> w) {
  for (int i = 0; i < N - 1; i++) {
    adj[v[i]].push_back({u[i], w[i]});
    adj[u[i]].push_back({v[i], w[i]});
  }
  for (int i = 0; i < N; i++) {
    dfs(i);
  }
  return (ans>>1);
}
