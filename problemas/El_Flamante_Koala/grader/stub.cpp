#include "koala.h"
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

        int N[T], K[T], aux;
        vector<int> R1[T], C1[T], R2[T], C2[T];

        for (int t = 0; t < T; t++) {
            fscanf(fin, "%d", &K[t]);
            fscanf(fin, "%d", &N[t]);

            for (int i = 0; i < N[t]; i++) {
                fscanf(fin, "%d", &aux);
                R1[t].push_back(aux);
            }

            for (int i = 0; i < N[t]; i++) {
                fscanf(fin, "%d", &aux);
                C1[t].push_back(aux);
            }

            for (int i = 0; i < N[t]; i++) {
                fscanf(fin, "%d", &aux);
                R2[t].push_back(aux);
            }

            for (int i = 0; i < N[t]; i++) {
                fscanf(fin, "%d", &aux);
                C2[t].push_back(aux);
            }
        }

        for (int t = 0; t < T; t++) {
            aux = Flamante_Koala(K[t], N[t], R1[t], C1[t], R2[t], C2[t]);
            fprintf(fout, "%d\n", aux);
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
