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

template<class T> void RngShuffle(vector<T> &V) {
  for (int k = 0; k < 32; k++) {
    for (int i = 0; i < (int)(V.size()) - 1; i++) {
      int j = (rng() % ((int)(V.size()) - i)) + i;
      swap(V[i], V[j]);
    }
  }
}

caso generate_random(int n) { // n > 3.

  vector<int> perm;

  int x = (rng() % (1<<n));
  int y = (rng()&2);
  for (int i = 1; i < n; i++) {
    y += ((1^((y>>(i - 1))&1))<<i);
  }
  int a = x, b = x^((1<<n) - 1), c = b^y, d = c^((1<<n) - 1);
  for (int i = 0; i < (1<<n); i++) {
    if (i != a && i != b && i != c && i != d) perm.push_back(i);
  }
  RngShuffle(perm);
  perm.push_back(d);
  perm.push_back(c);
  perm.push_back(b);
  perm.push_back(a);

  caso X; X.N = n;

  X.strat.resize((1<<n) - 1);

  for (int i = 1; i < (1<<n); i++) {
    int Y = perm[i - 1]^perm[i];
    for (int j = 0; j < n; j++) {
      X.strat[i - 1].push_back((Y>>j)&1);
    }
  }

  return X;
}

int main() {
  int n = 15; 

  int x = 200000>>n;

  vector<caso> X;

  for (int i = 0; i < x; i++) {
    X.push_back(generate_random(n));
  }

  cout << x << endl;
  for (auto e : X) e.print();
}
