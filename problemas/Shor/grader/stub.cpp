#include "shor.h"
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

        int N[T], Q[T], aux;
        vector<int> P[T], U[T], V[T], ANS[T], xxx;

        for (int t = 0; t < T; t++) {
            fscanf(fin, "%d", &N[t]);
            fscanf(fin, "%d", &Q[t]);
            for (int i = 0; i < N[t]; i++) {
                fscanf(fin, "%d", &aux);
                P[t].push_back(aux);
            }
            for (int i = 0; i < Q[t]; i++) {
                fscanf(fin, "%d", &aux);
                U[t].push_back(aux);
            }
            for (int i = 0; i < Q[t]; i++) {
                fscanf(fin, "%d", &aux);
                V[t].push_back(aux);
            }
            for (int i = 0; i < Q[t] + 1; i++) {
                fscanf(fin, "%d", &aux);
                ANS[t].push_back(aux);
            }
        }

        int ans = 1, ansp = 2;

        for (int t = 0; t < T; t++) {
            xxx = El_Robot_Shor(N[t], Q[t], P[t], U[t], V[t]);
            for (int i = 0; i < Q[t] + 1; i++)
                if (xxx[i] != ANS[t][i]) {
                    if (ANS[t][i] == -1 || xxx[i] == -1) ansp = 0;
                    ans = 0;
                }
        }
        if (ans == 0) ans = ansp;
        fprintf(fout, "%d\n", ans);
        fflush(fout);
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
