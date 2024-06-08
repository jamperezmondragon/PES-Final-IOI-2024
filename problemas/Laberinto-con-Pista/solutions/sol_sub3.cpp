#include "Laberinto.h" 
#include <bits/stdc++.h> 
using namespace std; 
  
using ll = long long; 
using pll = pair<ll,ll>; 
  
#define MP make_pair 
#define f first 
#define s second 
  
const int INF = (int)1e8; 
  
// las posiciones de inicio y fin son: [n - 2][1] -> [1][n - 2] 
  
// mover: 
// 0, abajo:     [x][y] -> [x + 1][y] 
// 1, derecha:   [x][y] -> [x][y + 1] 
// 2, arriba:    [x][y] -> [x - 1][y] 
// 3, izquierda: [x][y] -> [x][y - 1] 
  
vector<int> Pista(int N, vector<vector<int>> L) { 
    vector<vector<int>> vis(N, vector<int>(N, INF)); 
    vis[N - 2][1] = 0; 
    queue<pll> qq; 
    qq.push(MP(N - 2, 1)); 
    while (!qq.empty()) { 
        auto p = qq.front(); 
        qq.pop(); 
        if (vis[p.f][p.s + 1] == INF && L[p.f][p.s + 1]) { 
            vis[p.f][p.s + 1] = vis[p.f][p.s] + 1; 
            qq.push(MP(p.f, p.s + 1)); 
        } 
        if (vis[p.f + 1][p.s] == INF && L[p.f + 1][p.s]) { 
            vis[p.f + 1][p.s] = vis[p.f][p.s] + 1; 
            qq.push(MP(p.f + 1, p.s)); 
        } 
        if (vis[p.f][p.s - 1] == INF && L[p.f][p.s - 1]) { 
            vis[p.f][p.s - 1] = vis[p.f][p.s] + 1; 
            qq.push(MP(p.f, p.s - 1)); 
        } 
        if (vis[p.f - 1][p.s] == INF && L[p.f - 1][p.s]) { 
            vis[p.f - 1][p.s] = vis[p.f][p.s] + 1; 
            qq.push(MP(p.f - 1, p.s)); 
        } 
    } 
  
    vector<int> P; 
    int x = 1, y = N - 2; 
    while (x != N - 2 || y != 1) { 
        if (vis[x - 1][y] < vis[x][y]) { 
            P.push_back(0); 
            P.push_back(0); 
            x--; 
            continue; 
        } 
        if (vis[x + 1][y] < vis[x][y]) { 
            P.push_back(1); 
            P.push_back(0); 
            x++; 
            continue; 
        } 
        if (vis[x][y - 1] < vis[x][y]) { 
            P.push_back(0); 
            P.push_back(1); 
            y--; 
            continue; 
        } 
        if (vis[x][y + 1] < vis[x][y]) { 
            P.push_back(1); 
            P.push_back(1); 
            y++; 
            continue; 
        } 
    } 
    reverse(P.begin(), P.end()); 
    return P; 
} 
  
void Juego(vector<int> P) { 
    mover(-1); 
    int mov = 0; 
    int sz = 0; 
    if (!P.empty()) sz = P.size(); 
    for (int i = 0; i < sz; i += 2) { 
        mov = P[i] + 2*P[i + 1]; 
        mover(mov); 
    } 
    mover(4); 
    return; 
} 
