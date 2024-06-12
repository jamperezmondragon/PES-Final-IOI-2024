#include <bits/stdc++.h>
using namespace std;


namespace solution {
  int const Mxn = 2e5 + 1;
  int MN;

  // dsu
  int leader[Mxn], sz[Mxn];
  int cnt = 0;
  stack<array<int, 2>> St;

  int find(int x) {return ((leader[x] == x) ? x : find(leader[x]));}
  int unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return 0;
    if (sz[x] < sz[y]) swap(x, y);
    cnt--;
    leader[y] = x; sz[x] += sz[y];
    St.push({x, y});
    return 1;
  }
  void undo() {
    auto [x, y] = St.top(); 
    leader[y] = y; sz[x] -= sz[y];
    cnt++;
    St.pop();
  }

  // offline deletion
  vector<array<int, 2>> tree[Mxn<<1];
  int ans[Mxn];

  void add(int l, int r, array<int, 2> X) {
    for (l += MN, r += MN; l < r; l >>= 1, r >>= 1) {
      if (l&1) tree[l++].push_back(X);
      if (r&1) tree[--r].push_back(X);
    }
  }
  void dfs(int node = 1) {
    int c = 0;
    for (auto [x, y] : tree[node]) {
      c += unite(x, y);
    }
    if (node < MN) dfs(2*node), dfs(2*node + 1);
    else ans[node - MN] = cnt;
    while (c--) undo();
  }

  vector<int> El_Robot_Shor(int N, int Q, vector<int> p, vector<int> u, vector<int> v) {
    // limpiar variables globales.
    MN = Q + 1;
    for (int i = 0; i < N; i++) {
      leader[i] = i; sz[i] = 1;
    }
    for (int i = 0; i < 2*MN; i++) tree[i].clear();

    map<array<int, 2>, int> time_added;
    cnt = N;
    for (int i = 0; i < N; i++) {
      time_added[{i, p[i]}] = 0;
    }
    for (int i = 0; i < Q; i++) {
      int l, r;

      l = time_added[{u[i], p[u[i]]}], r = i + 1;
      add(l, r, {u[i], p[u[i]]});

      l = time_added[{v[i], p[v[i]]}], r = i + 1;
      add(l, r, {v[i], p[v[i]]});

      swap(p[u[i]], p[v[i]]);

      time_added[{u[i], p[u[i]]}] = i + 1;
      time_added[{v[i], p[v[i]]}] = i + 1;
    }
    for (int i = 0; i < N; i++) {
      add(time_added[{i, p[i]}], Q + 1, {i, p[i]});
    }
    dfs();
    vector<int> Ans(Q + 1);
    for (int i = 0; i <= Q; i++) {
      Ans[i] = ((N - ans[i])&1) ? -1 : (N - ans[i])/2;
    }
    return Ans;
  }
}

class testcase {
  public:
    int N, Q; 
    vector<int> a, u, v;

    bool validate() {
      if (N < 0 || N > 200000) return 0;
      if (Q < 0 || Q > 200000) return 0;
      if (size(a) != N) return 0;
      if (size(u) != Q) return 0;
      if (size(v) != Q) return 0;
      for (auto e : a) if (e > N - 1 || e < 0) return 0;
      for (auto e : u) if (e > N - 1 || e < 0) return 0;
      for (auto e : v) if (e > N - 1 || e < 0) return 0;
    }

    void print(string s) {
      ofstream coutt;
      cout.open(s);

      coutt << N << endl << Q << endl;
      for (auto e : a) coutt << e << " ";
      coutt << endl;
      for (auto e : u) coutt << e << " ";
      coutt << endl;
      for (auto e : v) coutt << e << " ";
      cout << endl;
      vector<int> dummy = solution::El_Robot_Shor(N, Q, a, u, v);
      for (auto e : dummy) coutt << e << " ";
      coutt << endl;
    }

};

vector<int> V, P = {0, 1, 2, 3, 4, 5, 6, 7};
vector<bool> vis(8, false);
vector<testcase> X;
vector<array<int, 2>> swaps;

void dfs(int n = 7, int cnt = 0) {

  if (n == cnt) {
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
      a[P[i]] = i;
    }
    for (int i = 0; i < n; i++) {
      int x = i, y = a[V[i]];
      if (x != y) swaps.push_back({x, y});
    }
    P = V;

  }

  for (int i = 0; i < n; i++) {
    if (vis[i]) continue;
    V.push_back(i);
    vis[i] = true;
    dfs(n, cnt + 1);
    vis[i] = false;
    V.pop_back();
  }
}

int main() {
  int n = 8; // subtask 2 y 4
  //int n = 5 subtask 3
  //int n = 1, 2, 3, 4 subtask 1.
  dfs(n);
  testcase X;
  X.a.resize(n); iota(X.a.begin(), X.a.end(), 0);
  X.N = n;
  X.Q = swaps.size();
  for (auto [x, y] : swaps) X.u.push_back(x), X.v.push_back(y);
  X.print("sub1.1.in"); //cambia el número de subtas y caso.
}
