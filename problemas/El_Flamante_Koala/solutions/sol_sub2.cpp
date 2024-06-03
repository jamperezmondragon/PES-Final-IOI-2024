#include <bits/stdc++.h>

using namespace std;

int color[4][4];

int Flamante_Koala (int K, int N, vector<int> r1, vector<int> c1, vector<int> r2, vector<int> c2) {
	int res = 0;
	
	for (int mk = 0; mk < (1 << (K * K)); mk++) {
		for (int i = 0; i < K * K; i++)
			color[i / K][i % K] = (mk >> i) & 1;
		
		bool valido = 1;
		for (int i = 0; i < K; i++)
			for (int j = 0; j < K; j++)
				valido &= color[i][j] != color[K - i - 1][K - j - 1];
		
		if (!valido)
			continue;
		
		int tam = N;
		for (int i = 0; i < N; i++)
			tam -= color[r1[i] - 1][c1[i] - 1] != color[r2[i] - 1][c2[i] - 1];
		res = max(res, tam);
	}
	
	return res;
}
