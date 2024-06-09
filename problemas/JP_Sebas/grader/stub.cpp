#include "alumnos.h"
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

const int INF = (int)1e9;

vector<int> test_ar;
int test_call;
int valor(int id) {
    test_call++;
    if (id >= test_ar.size()) return -1;
    return test_ar[id];
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

        int N, aux;
        vector<int> AR;


        fscanf(fin, "%d", &N);
        for (int i = 0; i < N; i++) {
            fscanf(fin, "%d", &aux);
            AR.push_back(aux);
        }


        swap(test_ar, AR);

        aux = Reto_Sebas(N);

        if (test_call > 60) aux = INF;
        fprintf(fout, "%d\n", aux);
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
