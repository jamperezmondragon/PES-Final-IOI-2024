#include <bits/stdc++.h>

using namespace std;

struct nodo {
    int x, y, M;
    double P;
    nodo(){}
    nodo(int xx, int yy, int MM, double PP) {
        x = xx;
        y = yy;
        M = MM;
        P = PP;
    }
    bool operator < (const nodo &rhs) const {
        if (M != rhs.M) return (M > rhs.M);
        return (P > rhs.P);
    }
};

int ar[500][500];
bool vis[500][500];

double caso(int x) {
    if (x == 3) return 1.0;
    if (x == 4) return 1.584962500721156;
    return 0.0;
}

int check(int x, int y) {
    int ans = 0;
    if (ar[x + 1][y]) ans++;
    if (ar[x][y + 1]) ans++;
    if (ar[x - 1][y]) ans++;
    if (ar[x][y - 1]) ans++;
    return ans;
}

void tc(string prefix) {
    ifstream infile(prefix + ".in");
    if (!infile.is_open()) return;
    ofstream outfile("lol" + prefix + ".in");

    int N; infile >> N; N = 2*N + 1;
    string S[N];
    for (int i = 0; i < N; i++) infile >> S[i];
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            ar[i][j] = ((S[i][j] == '#') ? 0 : 1);

    priority_queue <nodo> pq;
    pq.push(nodo(N - 2, 1, 0, caso(check(N - 2, 1))));

    int M = 69, P = 69;

    while (!pq.empty()) {
        while (!pq.empty() && vis[pq.top().x][pq.top().y]) pq.pop();
        if (pq.empty()) break;
        auto p = pq.top();

        vis[p.x][p.y] = true;

        if (p.x == 1 && p.y == N - 2) {
            M = p.M;
            P = p.P + 1;
            break;
        }

        if (!vis[p.x + 1][p.y] && ar[p.x + 1][p.y]) pq.push(nodo(p.x + 1, p.y, p.M + 1, p.P + caso(check(p.x + 1, p.y))));
        if (!vis[p.x][p.y + 1] && ar[p.x][p.y + 1]) pq.push(nodo(p.x, p.y + 1, p.M + 1, p.P + caso(check(p.x, p.y + 1))));
        if (!vis[p.x - 1][p.y] && ar[p.x - 1][p.y]) pq.push(nodo(p.x - 1, p.y, p.M + 1, p.P + caso(check(p.x - 1, p.y))));
        if (!vis[p.x][p.y - 1] && ar[p.x][p.y - 1]) pq.push(nodo(p.x, p.y - 1, p.M + 1, p.P + caso(check(p.x, p.y - 1))));
    }

    outfile << "1\n";
    outfile << N << "\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            outfile << ar[i][j] << " ";
        outfile << "\n";
    }
    outfile << M << " " << P << "\n";
    return;
}

string num(int x) {
    string ans;
    while (x > 0) {
        ans.push_back('0' + x%10);
        x = x/10;
    }
    if (ans.size() == 1) ans.push_back('0');
    reverse(ans.begin(), ans.end());
    return ans;
}

void reset() {
    for (int i = 0; i < 500; i++)
        for (int j = 0; j < 500; j++) {
            ar[i][j] = 0;
            vis[i][j] = false;
        }
    return;
}

int main() {
    for (int i = 1; i <= 40; i++) {
        tc(num(i));
        reset();
    }
    return 0;
}
