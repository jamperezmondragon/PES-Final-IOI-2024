#include <bits/stdc++.h>

using namespace std;

pair<int, int> solve(int N, int K) {
    bool vis[N];
    for (int i = 0; i < N; i++) vis[i] = false;
    vector <int> v;
    v.push_back(0);
    vis[0] = true;
    while (!v.empty()) {
        auto nd = v.back();
        v.pop_back();

        for (int i = 0; i < N; i++) {
            if (vis[i]) continue;
            int aux = query(i, nd);
            if (aux < 1) vis[i] = true;
            if (aux == 0) v.push_back(i);
        }
    }
    pair<int, int> ans = make_pair(-1, -1);

    for (int i = 0; i < N; i++)
        if (!vis[i]) ans = make_pair(1, i);
    return ans;
}
