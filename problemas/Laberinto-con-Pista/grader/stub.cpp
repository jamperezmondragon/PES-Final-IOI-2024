#include "Laberinto.h"
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

int test_cnt, test_x, test_y;
vector<vector<int>> LL;

vector<int> estado() {
    vector<int> v = {0, 0, 0, 0};
    if (LL[test_y + 1][test_x]) v[0] = 1;
    if (LL[test_y][test_x + 1]) v[1] = 1;
    if (LL[test_y - 1][test_x]) v[2] = 1;
    if (LL[test_y][test_x - 1]) v[3] = 1;
    return v;
}

vector<int> mover(int x) {
    vector<int> v = estado();
    if (x == -1 || x == 4) return v;
    test_cnt++;
    if (x == 0 && v[x] == 1) test_y++;
    if (x == 1 && v[x] == 1) test_x++;
    if (x == 2 && v[x] == 1) test_y--;
    if (x == 3 && v[x] == 1) test_x--;
    return estado();
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

        int T;
        fscanf(fin, "%d", &T);

        int N[T], aux;
        vector<vector<int>> L[T];
        vector<int> PIS[T];

        for (int t = 0; t < T; t++) {
            fscanf(fin, "%d", &N[t]);

            L[t] = vector<vector<int>>(N[t]);
            for (int i = 0; i < N[t]; i++) {
                for (int j = 0; j < N[t]; j++) {
                    fscanf(fin, "%d", &aux);
                    L[t][i].push_back(aux);
                }
            }
        }

        for (int t = 0; t < T; t++)
            PIS[t] = Pista(N[t], L[t]);

        for (int t = 0; t < T; t++) {
            int sz = 0;
            if (!PIS[t].empty()) sz = PIS[t].size();
            fprintf(fout, "%d\n", sz);
            fflush(fout);
            for (int i = 0; i < sz; i++)
                fprintf(fout, "%d ", PIS[t][i]);
            fprintf(fout, "\n");
            fflush(fout);
        }
	}
    else if (string(argv[3]) == "1") { // segunda llamada, para problemas de comunicacion

        int T;
        fscanf(fin, "%d", &T);

        int N[T], P_SZ[T], aux;
        vector<int> PIS[T];
        vector<vector<int>> L[T];

        for (int t = 0; t < T; t++) {
            fscanf(fin, "%d", &P_SZ[t]);
            for (int i = 0; i < P_SZ[t]; i++) {
                fscanf(fin, "%d", &aux);
                PIS[t].push_back(aux);
            }
        }

        for (int t = 0; t < T; t++) {
            fscanf(fin, "%d", &N[t]);

            L[t] = vector<vector<int>>(N[t]);
            for (int i = 0; i < N[t]; i++) {
                for (int j = 0; j < N[t]; j++) {
                    fscanf(fin, "%d", &aux);
                    L[t][i].push_back(aux);
                }
            }
        }

        for (int t = 0; t < T; t++) {
            test_cnt = 0;
            test_x = 1;
            test_y = N[t] - 2;
            swap(LL, L[t]);
            Juego(PIS[t]);
            if (test_x == N[t] - 2 && test_y == 1) fprintf(fout, "%d\n", test_cnt);
            else fprintf(fout, "-1\n");
            fflush(fout);
        }
	}
	else {
		fprintf(stderr, "Error del evaluador.\n");
		fprintf(stderr, "Evaluador desconocido del tipo %s\n", argv[3]);
		exit(0);
	}
	return 0;

}
