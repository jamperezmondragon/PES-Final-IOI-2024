#include <bits/stdc++.h>
using namespace std;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

namespace solution {

  int const Mxn = 2e5;

  vector<array<int, 2>> adj[Mxn];

  long long int dp[2][Mxn];
  long long int ans = 0;

  void dfs(long long int bit, int node = 0, int parent = -1) {
    dp[0][node] = 1, dp[1][node] = 0;
    for (auto [child, W] : adj[node]) {
      if (child == parent) continue;
      int w = ((W>>bit)&1);
      dfs(bit, child, node);
      ans += ((dp[0][node]*dp[w^1][child])<<bit);
      ans += ((dp[1][node]*dp[w][child])<<bit);

      dp[0][node] += dp[w][child];
      dp[1][node] += dp[w^1][child];
    }
  }

  long long int Encuentra_xor(int N, vector<int> u, vector<int> v, vector<int> w) {
    ans = 0;
    for (int i = 0; i < N - 1; i++) {
      adj[v[i]].push_back({u[i], w[i]});
      adj[u[i]].push_back({v[i], w[i]});
    }
    for (int i = 0; i < 30; i++) dfs(i);
    for (int i = 0; i < N; i++) adj[i].clear();
    return ans;
  }
}

class testcase {
  public:
    int SubtaskNum;
    int N; 
    vector<int> u, v, w;

    bool validate() {
      if (N < 0 || N > 200000) return 0;
      if (SubtaskNum == 1 && N > 2000) return 0;
      if (size(u) != N - 1) return 0;
      if (size(v) != N - 1) return 0;
      if (size(w) != N - 1) return 0;
      for (auto e : u) if (e < 0 || e > N - 1) return 0;
      for (auto e : v) if (e < 0 || e > N - 1) return 0;
      for (auto e : w) if (e < 0 || (SubtaskNum == 2 && e > 1) || e > 1000000000) return 0;
      //check connectivity, dsu
      vector<int> leader(N), sz(N, 1); 
      iota(leader.begin(), leader.end(), 0);
      for (int i = 0; i < N - 1; i++) {
        int x = u[i], y = v[i];
        while (x != leader[x]) x = leader[x];
        while (y != leader[y]) y = leader[y];
        if (x == y) return 0;
        if (sz[x] < sz[y]) swap(x, y);
        leader[y] = x; sz[x] += sz[y];
      }
      return 1;
    }

    void print() {
      if (!validate()) {
        cout << "\nERRORRR\n";
        return;
      }
      cout << N << endl;
      for (auto e : u) cout << e << " ";
      cout << endl;
      for (auto e : v) cout << e << " ";
      cout << endl;
      for (auto e : w) cout << e << " ";
      cout << endl;
      cout << solution::Encuentra_xor(N, u, v, w) << endl;
    }

};

auto LineTree(int N) -> vector<array<int, 2>> { // 0-indexed
  vector<array<int, 2>> V;
  for (int i = 0; i < N - 1; i++) {
    V.push_back({i, i + 1});
  }
  return V;
}

int main() {
  cout << 11 << endl;
  for (int i = 0; i < 10; i++) {
    vector<array<int, 2>> dummy = LineTree(100);
    testcase X;
    for (auto [a, b] : dummy) X.v.push_back(a), X.u.push_back(b), X.w.push_back(rng()%(1000000000));
    X.N = 100;
    X.print();
  }
  vector<array<int, 2>> dummy = LineTree(1000);
  testcase X;
  for (auto [a, b] : dummy) X.v.push_back(a), X.u.push_back(b), X.w.push_back(rng()%(1000000000));
  X.N = 1000;
  X.print();
}
