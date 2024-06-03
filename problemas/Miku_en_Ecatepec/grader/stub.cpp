#include "miku.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <ctime>
#include <cassert>
#include <complex>
#include <string>
#include <cstring>
#include <chrono>
#include <random>
#include <bitset>
#include <array>
#include <climits>
#include <csignal>
#include <cstdarg>
using namespace std;

static FILE *fin;
static FILE *fout;

const int MAXN = 100;
const int MAXT = 50;

int T, Tx;
int N[MAXT], K[MAXT], CNT[MAXT], dist[MAXT][MAXN][MAXN];
int conectividad[MAXT], U[MAXT], V[MAXT];
pair<int, int> ans[MAXT];

int llamada(int u, int v) {
    CNT[Tx]--;
    if (u < 0 || u > N[Tx] - 1 || v < 0 || v > N[Tx] - 1) return 1;
    if (dist[Tx][u][v] > K[Tx] || dist[Tx][u][v] == -1) return 1;
    if (dist[Tx][u][v] == K[Tx]) return 0;
    return -1;
}

int main(int argc, char **argv) {

    /********************** TEMPLATE **********************/

	if (argc != 4) { // para problemas de comunicacion
		fprintf(stderr, "Error del evaluador.\n");
		fprintf(stderr, "El numero de argumentos no es 4\n");
		exit(0);
	}

	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "a");

    /********************** TEMPLATE **********************/

	if (string(argv[3]) == "0") { // primera llamada, unica llamada para interactivos

        fscanf(fin, "%d", &T);
        for (int t = 0; t < T; t++) {
            fscanf(fin, "%d", &N[t]);
            fscanf(fin, "%d", &K[t]);

            for (int i = 0; i < N[t]; i++)
                for (int j = 0; j < N[t]; j++)
                    fscanf(fin, "%d", &dist[t][i][j]);

            fscanf(fin, "%d", &conectividad[t]);
            fscanf(fin, "%d", &CNT[t]);

            Tx = t;
            ans[t] = Miku_en_Ecatepec(N[t], K[t]);
        }

        for (int t = 0; t < T; t++) {
            fprintf(fout, "%d ", ans[t].first);
            fprintf(fout, "%d ", ans[t].second);
            fprintf(fout, "%d ", CNT[t]);
            fflush(fout);
        }
	}
    else if (string(argv[3]) == "1") { // segunda llamada, para problemas de comunicacion
        exit(0);
	}
	else {
		fprintf(stderr, "Error del evaluador.\n");
		fprintf(stderr, "Evaluador desconocido del tipo %s\n", argv[3]);
		exit(0);
	}
	return 0;

}
