#include "alumnos.h"
#include <bits/stdc++.h>
using namespace std;

int Reto_Sebas(int N) {
    vector<int> ar;
    for (int i = 0; i < N; i++) {
        ar.push_back(valor(i));
    }
    for (int i = 0; i < N; i++) {
        if (ar[i] == ar[(i + N/2)%N]) return i;
    }
    return -1;
}
