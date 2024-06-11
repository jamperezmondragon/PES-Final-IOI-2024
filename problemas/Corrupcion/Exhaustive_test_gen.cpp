#include <bits/stdc++.h>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct caso {
  int N;
  vector<vector<int>> strat;

  bool validate() {
    if (N < 0 || N > 16) return 0;
    for (auto e : strat) if (size(e) != N) return 0;
    for (auto e : strat) for (auto x : e) if (x < 0 || x > 1) return 0;
    return 1;
  }
  void print() {
    cout << N << endl;
    cout << size(strat) << endl;
    for (auto e : strat) {
      for (auto x : e) cout << x << " ";
      cout << endl;
    }
    cout << endl;
  }
};

vector<int> V;
vector<bool> vis(8, false);
vector<caso> X;

void dfs(int n = 3, int cnt = 0) {

  if ((1<<n) == cnt) {

    caso dummy; dummy.N = n;

    dummy.strat.resize((1<<n) - 1);

    for (int i = 1; i < (1<<n); i++) {
      int Y = V[i - 1]^V[i];
      for (int j = 0; j < n; j++) {
        dummy.strat[i - 1].push_back((Y>>j)&1);
      }
    }
    X.push_back(dummy);
  }

  for (int i = 0; i < (1<<n); i++) {
    if (vis[i]) continue;
    V.push_back(i);
    vis[i] = true;
    dfs(n, cnt + 1);
    vis[i] = false;
    V.pop_back();
  }
}

int main() {
  dfs();
  cout << size(X) << endl;
  for (auto e : X) e.print();
}
