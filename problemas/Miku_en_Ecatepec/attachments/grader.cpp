#include <bits/stdc++.h>

using namespace std;

int query(int a, int b);

pair<int, int> solve(int N, int K) {
    return make_pair(-1, -1);
}

// GRADER
string filename = "sub0.1.in";
int N, K, cnt, MAX, dist[100][100];
bool conectividad;

int query(int a, int b) {
    cnt++;
    if (a < 0 || a > N - 1 || b < 0 || b > N - 1) return 1;
    if (dist[a][b] > K || dist[a][b] == -1) return 1;
    if (dist[a][b] == K) return 0;
    return -1;
}

int evalua(pair<int, int> p) {
    if (cnt > MAX) return 2;
    if (p.first > N - 1 || p.second > N - 1) return 3;
    if ((p.first < 0 || p.second < 0) && p != make_pair(-1, -1)) return 3;
    if (p != make_pair(-1, -1) && dist[p.first][p.second] != -1) return 0;
    return ((p == make_pair(-1, -1)) == conectividad);
}

int main() {
    ifstream infile(filename);
    infile >> N >> K;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            infile >> dist[i][j];
    infile >> conectividad;
    MAX = (2*N*N + K - 1)/K;
    switch ((evalua(solve(N, K)))) {
        case 0:
            cout << "Respuesta erronea.";
            break;
        case 1:
            cout << "Respuesta correcta :)";
            break;
        case 2:
            cout << "Numero de preguntas excedido.";
            break;
        case 3:
            cout << "Respuesta invalida.";
            break;
    }
    return 0;
}
