#include <bits/stdc++.h>

using namespace std;

const int MX = 505;
const int dx[] = {1, -1, 0, 0};
const int dy[] = {0, 0, 1, -1};
int ady[MX][MX][4], dis[MX][MX];

void agrega_hor (int r1, int c1, int c2) {
	if (c1 > c2)
		swap(c1, c2);
	
	ady[r1 - 1][c1][0]++;
	ady[r1][c1][1]++;
}

void agrega_ver (int r1, int r2, int c1) {
	if (r1 > r2)
		swap(r1, r2);
	
	ady[r1][c1 - 1][2]++;
	ady[r1][c1][3]++;
}

int Flamante_Koala (int K, int N, vector<int> r1, vector<int> c1, vector<int> r2, vector<int> c2) {
	for (int i = 0; i <= K; i++)
		for (int j = 0; j <= K; j++) {
			dis[i][j] = 1e9;
			
			for (int l = 0; l < 4; l++) {
				int x = i + dx[l];
				int y = j + dy[l];
				ady[i][j][l] = (0 <= x && x <= K) && (0 <= y && y <= K) ? 0 : -1;
			}
		}
	 
	for (int i = 0; i < N; i++) {
		if (r1[i] == r2[i]) {
			agrega_hor(r1[i], c1[i], c2[i]);
			agrega_hor(K - r1[i] + 1, K - c1[i] + 1, K - c2[i] + 1);
		} else {
			agrega_ver(r1[i], r2[i], c1[i]);
			agrega_ver(K - r1[i] + 1, K - r2[i] + 1, K - c1[i] + 1);
		}
	}
	
	dis[0][0] = 0;
	
	using iii = array<int, 3>;
	priority_queue<iii, vector<iii>, greater<iii>> pq;
	pq.push({0, 0, 0});
	
	while (pq.size()) {
		auto [d, i, j] = pq.top();
		pq.pop();
		
		if (dis[i][j] < d)
			continue;
		
		for (int l = 0; l < 4; l++) {
			if (ady[i][j][l] == -1)
				continue;
			
			int w = d + ady[i][j][l];
			int x = i + dx[l];
			int y = j + dy[l];
			
			if (dis[x][y] > w)
				pq.push({dis[x][y] = w, x, y});
		}
	}
	
	return N - dis[K / 2][K / 2];
}
