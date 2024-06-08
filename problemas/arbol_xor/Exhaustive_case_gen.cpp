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

    //print testcase
    void printF(string filename) {
      ofstream file_output;
      file_output.open(filename);
      file_output << N << endl;
      for (auto e : u) file_output << e << " ";
      file_output << endl;
      for (auto e : v) file_output << e << " ";
      file_output << endl;
      for (auto e : w) file_output << e << " ";
      file_output << endl;
      file_output << solution::Encuentra_xor(N, u, v, w) << endl;
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

// Códigos de Prüfer para generar árboles aleatorios. 0-indexed.

auto Prufer(int n, vector<int> V = {}) -> vector<array<int, 2>> {
  vector<array<int, 2>> Edges;
  if (size(V) == 0) {
    V.resize(n - 2);
    for (int i = 0; i < n - 2; i++) V[i] = (rng() % n);
  }
  
  priority_queue<int> Q;
  vector<int> freq(n, 0);
  for (int i = 0; i < n - 2; i++) freq[V[i]]++;
  for (int i = 0; i < n; i++)
    if (!freq[i]) Q.push(i);
  for (int i = 0; i < n - 2; i++) {
    Edges.push_back({V[i], Q.top()});
    Q.pop();
    if (--freq[V[i]] == 0) Q.push(V[i]);
  }
  int u = Q.top();
  Q.pop();
  int v = Q.top();
  Edges.push_back({u, v});
  return Edges;
}

namespace exhaustive { 
  vector<int> V;
  vector<testcase> Casos;
  int size = 5;
  void dfs(int x = size - 2) {
    if (x == 0) {
      vector<array<int, 2>> X = Prufer(size, V); 
      vector<int> u, v, w;
      for (auto [a, b] : X) {
        u.push_back(a), v.push_back(b);
        w.push_back((rng()%(2)));
      }
      testcase Y; Y.u = u, Y.v = v, Y.w = w; Y.N = size;
      Casos.push_back(Y);
    } else {
      for (int i = 0; i < size; i++) {
        V.push_back(i);
        dfs(x - 1);
        V.pop_back();
      }
    }
  }
  auto gen(int x) -> vector<testcase> {
    size = x; Casos.clear();
    dfs();
    return Casos;
  }
}

int main() {
  cout << 5000 << endl;
  for (int mask = 0; mask < (1<<4); mask++) {
    vector<int> w;
    for (int i = 0; i < 4; i++) {
      w.push_back((mask>>i)&1);
    }
    vector<testcase> X = exhaustive::gen(5);
    for (auto &e : X) e.w = w;
    for (auto e : X) e.print();
  }
}
