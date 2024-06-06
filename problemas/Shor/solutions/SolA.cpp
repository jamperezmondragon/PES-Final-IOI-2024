#include <bits/stdc++.h>
using namespace std;

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
