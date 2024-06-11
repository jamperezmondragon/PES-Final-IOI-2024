#include <bits/stdc++.h>
using namespace std;

int main() {
  vector<vector<int>> V = {{1, 1}, {1, 0}, {1, 1}};
  int n = 3;
  vector<vector<int>> C;


  for (int k = 1; k < n; k++) {
    for (int i = 0; i <= size(V); i++) {

      for (int j = 0; j < size(V); j++) {
        C.push_back({});
        for (int h = 0; h < size(V[j]); h++) {
          C.back().push_back(V[j][h]); 
        }
        for (int h = 0; h < size(V[j]); h++) {
          C.back().push_back(V[j][h]); 
        }
      }
      if (i == size(V)) continue;
      C.push_back({});
      for (int h = 0; h < size(V[i]); h++) {
        C.back().push_back(V[i][h]); 
      }
      for (int h = 0; h < size(V[i]); h++) {
        C.back().push_back(0); 
      }
    }
    V = C; C = {};
  }

  cout << 1 << endl;

  cout << (1<<n) << endl;

  cout << size(V) << endl;

  for (int i = 0; i < size(V); i++) {
    for (int j = 0; j < size(V[i]); j++) {
      cout << V[i][j] << " ";
    }
    cout << endl;
  }

}
