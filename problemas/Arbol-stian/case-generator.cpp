#include<bits/stdc++.h>

using namespace std;

struct RandomGen{
    mt19937_64 randomEngine;
    RandomGen(vector<uint32_t> seedSeq){
        seed(seedSeq);
    }
    RandomGen(){
        random_device rdev;
        vector<uint32_t> seedSeq;
        for(int i=0; i<4; i++){
            seedSeq.push_back(rdev());
        }
        seed(seedSeq);
    }
    void seed(vector<uint32_t> &seedSeq){
        seed_seq seq(seedSeq.begin(), seedSeq.end());
        randomEngine.seed(seq);
    }
    long long Int(long long l, long long r){
        uniform_int_distribution<long long> udist(l, r);
        return udist(randomEngine);
    }
    vector<int> IntArray(int N, int A_l, int A_r){
        vector<int> A(N);
        for(int i=0; i<N; i++){
            A[i] = Int(A_l, A_r);
        }
        return A;
    }
    vector<int> IntDistinctArray(int N, int A_l, int A_r){
        vector<int> A(N);
        set<int> S;
        for(int i=0; i<N; i++){
            do{
                A[i] = Int(A_l, A_r);
            }while(S.count(A[i]));
            S.insert(A[i]);
        }
        return A;
    }
    vector<int> IntPermutation(int N, int start){
        vector<int> A(N);
        iota(A.begin(), A.end(), start);
        shuffle(A.begin(), A.end(), randomEngine);
        return A;
    }
};
RandomGen rnd;

enum weightStrategy : unsigned{
    wNull,
    wGeneral,
    wHighComp,
    wEqual, 
    edgePrime,
    wTwoPower,
};

enum treeStrategy : unsigned{
    tNull,
    treeFile,
    treeBamboo,
    treeRandom,
    treePaste,
};

struct TreeMetadata{
    int N;
    treeStrategy strategy;
    string filename;
};

struct Tree{
    int N;
    vector<array<int, 2>> E;
};

auto treeGenerator(TreeMetadata td) -> Tree{
    Tree T;
    int N = td.N;
    T.N = N;
    if(td.strategy == treeFile){
        ifstream fin;
        fin.open(td.filename);
        int dummy;
        fin>> dummy;
        for (int i = 0; i < N - 1; i++) {
            int u, v;
            fin>> u>> v;
            T.E.push_back({u, v});
        }
        fin.close();
    }
    else if(N > 1){
        switch(td.strategy){
        case treeBamboo:{
            for(int i = 1; i < N; i++){
                T.E.push_back({i - 1, i});
            }
            break;
        }
        case treePaste:{
            if(N > 2){
                int xd = 1;
            }
            int K = rnd.Int(1, N);
            for(int i = 1; i < K; i++){
                T.E.push_back({i - 1, i});
            }
            for(int i = K; i < N;){
                int nk = rnd.Int(1, N - i);
                for(int j = 1; j < nk; j++){
                    T.E.push_back({i + j - 1, i + j});
                }
                T.E.push_back({(int)rnd.Int(0, K-1), i});
                i += nk;
            }
            break;
        }
        case treeRandom:{
            vector<int> V(N - 2);
            vector<int> freq(N + 1, 0);
            for(int i = 0; i < N - 2; i++){
                V[i] = rnd.Int(0, N-1);
                freq[V[i]]++;
            }
            priority_queue<int> pq;
            for(int i = 0; i < N; i++){
                if (!freq[i]) pq.push(i);
            }
            for(int i = 0; i < N - 2; i++){
                T.E.push_back({V[i], pq.top()});
                pq.pop();
                if (--freq[V[i]] == 0) pq.push(V[i]);
            }
            int u = pq.top();
            pq.pop();
            int v = pq.top();
            pq.pop();
            T.E.push_back({u, v});
            break;
        }
        }
    }
    vector<int> A(N);
    iota(A.begin(), A.end(), 0);
    shuffle(A.begin(), A.end(), rnd.randomEngine);
    for(auto &ed : T.E){
        if(rnd.Int(0, 1)) swap(ed[0], ed[1]);
        ed[0] = A[ed[0]];
        ed[1] = A[ed[1]];
    }
    shuffle(T.E.begin(), T.E.end(), rnd.randomEngine);
    return T;
}

struct TestMetadata {
    string casename;
    TreeMetadata tst;
    weightStrategy wst;
};

vector<int> highlyComposedNumbers = {
    45360,
    50400,
    55440,
    83160,
    110880,
    166320,
    221760,
    277200,
    332640,
    498960,
    554400,
    665280,
    720720
};
const int highCompCnt = 13;

const int MAXValue = 1e6;
vector<int> primes, primesInOrder;
vector<int> primesOfNumber[MAXValue + 1];
set<int> primeNumbers;
bitset<MAXValue + 1> primeCriba;

struct Case {
    Tree T;
    vector<long long> W;
    Case(){}
    Case(TestMetadata meta){
        if(meta.tst.strategy == tNull){
            vector<treeStrategy> tst = {treeBamboo, treeBamboo, treeRandom, treePaste, treePaste};
            meta.tst.strategy = tst[rnd.Int(0, tst.size() - 1)];
        }
        if(meta.wst == wNull){
            vector<weightStrategy> wst = {wGeneral, wGeneral, wGeneral, wHighComp, wHighComp};
            meta.wst = wst[rnd.Int(0, wst.size() - 1)];
        }
        T = treeGenerator(meta.tst);
        int N = meta.tst.N;
        W.resize(N, 1);
        if(N == 1){
            W[0] = rnd.Int(1, MAXValue);
            return;
        }
        switch(meta.wst){
        case wEqual:{
            int v = highlyComposedNumbers[rnd.Int(0, highCompCnt - 1)];
            if(rnd.Int(0, 1)){
                v = rnd.Int(1, MAXValue);
            }
            for(int i=0; i<N; i++){
                W[i] = v;
            }
            break;
        }
        case edgePrime:{
            vector<vector<int>> adj(N);
            for(auto [u, v] : T.E){
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
            function<void(int, int, set<int>&, set<int> &)> dfs = [&](int v, int p, set<int> &pPrimes, set<int> &validPrimes){
                set<int> vPrimes, nValidPrimes = primeNumbers;
                if(p == -1){
                    W[v] = highlyComposedNumbers[rnd.Int(0, highCompCnt - 1)];
                    int x = W[v];
                    for(auto y : primesOfNumber[x]){
                        vPrimes.insert(y);
                        nValidPrimes.erase(y);
                    }
                }
                else{
                    int primeWeight = *next(pPrimes.begin(), rnd.Int(0, pPrimes.size() - 1));
                    W[v] = primeWeight;
                    vPrimes.insert(primeWeight);
                    nValidPrimes.erase(primeWeight);
                    while(!validPrimes.empty()){
                        int limitValue = MAXValue / W[v];
                        if(*validPrimes.begin() > limitValue) break;
                        int l = 1, r = validPrimes.size();
                        while(l < r){
                            int mit = (l + r) >> 1;
                            int num = *next(validPrimes.begin(), mit);
                            if(num > limitValue){
                                r = mit;
                            }
                            else{
                                l = mit + 1;
                            }
                        }
                        int npr = *next(validPrimes.begin(), rnd.Int(0, sqrt(l) - 1));
                        W[v] *= npr;
                        vPrimes.insert(npr);
                        nValidPrimes.erase(npr);
                    }
                }
                for(auto u : adj[v]){
                    if(u == p) continue;
                    dfs(u, v, vPrimes, nValidPrimes);
                }
            };
            set<int> dm1, dm2;
            dfs(rnd.Int(0, N-1), -1, dm1, dm2);
            break;
        }
        case wTwoPower:{
            for(int i=0; i<N; i++){
                W[i] = 1 << rnd.Int(0, 19);
            }
            break;
        }
        case wGeneral:{
            int P = rnd.Int(1, 500);
            shuffle(primes.begin() + 63, primes.end(), rnd.randomEngine);
            sort(primes.begin(), primes.begin() + P);
            vector<int> edgesIndex(N - 1);
            iota(edgesIndex.begin(), edgesIndex.end(), 0);
            for(int i=0; i<P; i++){
                int K = rnd.Int(0, N - 2);
                shuffle(edgesIndex.begin(), edgesIndex.end(), rnd.randomEngine);
                for(int j=0; j<K; j++){
                    auto [u, v] = T.E[edgesIndex[j]];
                    if(W[u] * primes[i] <= MAXValue && W[v] * primes[i] <= MAXValue){
                        W[u] *= primes[i];
                        W[v] *= primes[i];
                    }
                }
            }
            break;
        }
        case wHighComp:{
            for(int i=0; i<N; i++){
                W[i] = highlyComposedNumbers[rnd.Int(0, highCompCnt - 1)];
            }
            break;
        }
        }
    }
    void print(string filename, bool newFile) {
        ofstream input;
        if(newFile) input.open(filename);
        else input.open(filename, ifstream::app);
        input<< T.N<< '\n';
        for(int i=0; i<T.N; i++){
            input<< W[i]<< " \n"[i == T.N-1];
        }
        for (int i=0; i<T.N-1; i++) {;
            input<< T.E[i][0]<< ' '<< T.E[i][1]<< '\n';
        }
        input.close();
    }
};

vector<int> divisors[MAXValue + 1];

struct Solution{
    long long ans;
    int GCD_TRIP(int a, int b, int c) {
        return __gcd(a, __gcd(b, c));
    }
    void dfs(int u, int p, vector<vector<pair<int, int>>> &g, vector<map<int, int>> &dp) {
        for(auto v : g[u]) {
            if(v.first == p) continue;
            dfs(v.first, u, g, dp);
            ans = max(ans, (long long) v.second);
            for(auto x : dp[v.first]) {
                ans = max(ans, (long long) GCD_TRIP(x.first, v.second, 0) * (x.second + 1));
            }
            for(auto x : dp[u]) {
                ans = max(ans, (long long) GCD_TRIP(x.first, v.second, 0) * (x.second + 1));
            }
            for(auto x : dp[u]) {
                for(auto y : dp[v.first])
                    ans = max(ans, (long long) GCD_TRIP(x.first, y.first, v.second) * (x.second + y.second + 1));
            }
            dp[u][v.second] = max(dp[u][v.second], 1);
            for(auto x : dp[v.first]) {
                int gcd = __gcd(x.first, v.second);
                dp[u][gcd] = max(dp[u][gcd], 1 + dp[v.first][x.first]);
            }
        }
    }
    Solution(Case &C){
        ans = 0;
        int N = C.T.N;
        vector<vector<pair<int, int>>> g(N);
        vector<map<int, int>> dp(N);
        for(int i = 0; i < N - 1; i++) {
			auto [u, v] = C.T.E[i];
            int w = __gcd(C.W[u], C.W[v]);
			g[u].push_back(make_pair(v, w));
			g[v].push_back(make_pair(u, w));
		}
        dfs(0, -1, g, dp);
    }
    void print(string filename, bool newFile) {
        ofstream input;
        if(newFile) input.open(filename);
        else input.open(filename, ifstream::app);
        input<< ans<< '\n';
        input.close();
    }
};

struct SubtaskMetadata{
    string subtask;
    int N;
    treeStrategy tst;
    weightStrategy wst;
};

struct Testcase{
    vector<Case> casesOfTest;
    Testcase(SubtaskMetadata meta){
        int T = rnd.Int(1, meta.N / 1000);
        vector<int> N(T, 1000);
        for(int i = 1000 * T + 1; i<=meta.N; i++){
            N[rnd.Int(0, T - 1)]++;
        }
        sort(N.begin(), N.end());
        N[0] = 1;
        for(auto n : N){
            casesOfTest.push_back(Case({meta.subtask, {n, meta.tst}, meta.wst}));
        }
    }
    Testcase(vector<TestMetadata> metadata){
        for(auto &meta : metadata){
            casesOfTest.push_back(Case(meta));
        }
    }
    void print(string filename){
        ofstream input;
        input.open(filename);
        input<< casesOfTest.size()<< '\n';
        input.close();
        for(auto C : casesOfTest){
            C.print(filename, 0);
            Solution(C).print(filename, 0);
        }
    }
};


int main(){
    for(long long i=2; i*i <= MAXValue; i++){
        if(!primeCriba[i]){
            for(long long j=i; j<=MAXValue; j+=i){
                primeCriba[j] = 1;
                primesOfNumber[j].push_back(i);
            }
        }
    }
    for(int i=2; i<=MAXValue; i++){
        if(!primeCriba[i]){
            primes.push_back(i);
            primeNumbers.insert(i);
        }
    }
    primesInOrder = primes;
    //cout<< primes.size();

    for(int i=1; i<=MAXValue; i++){
        for(int j=i; j<=MAXValue; j+=i){
            divisors[j].push_back(i);
        }
    }

    vector<TestMetadata> metadata;

    //Make samples
    //Case({"sample1", {10, treeRandom}, edgePrime}).print("cases/sample1.in", 1);

    /*
    2000
    1000
    500
    250
    125
    62
    31
    15
    7
    */

    /*
    // Subtask 1
    Testcase({"sub1", 2000, tNull, wNull}).print("cases/sub1.1.in");
    Testcase({"sub1", 2000, tNull, wNull}).print("cases/sub1.2.in");
    Testcase({
        {"sub1.3.1", {7, tNull}, wNull},
        {"sub1.3.2", {15, tNull}, wNull},
        {"sub1.3.3", {31, tNull}, wNull},
        {"sub1.3.4", {62, tNull}, wNull},
        {"sub1.3.5", {125, tNull}, wNull},
        {"sub1.3.6", {250, tNull}, wNull},
        {"sub1.3.7", {500, tNull}, wNull},
        {"sub1.3.8", {1000, tNull}, wNull},
        {"sub1.3.9", {2000, tNull}, wNull},
    }).print("cases/sub1.3.in");
   Testcase({
        {"sub1.4.1", {7, tNull}, wNull},
        {"sub1.4.2", {15, tNull}, wNull},
        {"sub1.4.3", {31, tNull}, wNull},
        {"sub1.4.4", {62, tNull}, wNull},
        {"sub1.4.5", {125, tNull}, wNull},
        {"sub1.4.6", {250, tNull}, wNull},
        {"sub1.4.7", {500, tNull}, wNull},
        {"sub1.4.8", {1000, tNull}, wNull},
        {"sub1.4.9", {2000, tNull}, wNull},
    }).print("cases/sub1.4.in");
   Testcase({
        {"sub1.5.1", {7, treeFile, "ext-trees/trees/sub1.5.1-tree.in"}, wNull},
        {"sub1.5.2", {15, treeFile, "ext-trees/trees/sub1.5.2-tree.in"}, wNull},
        {"sub1.5.3", {31, treeFile, "ext-trees/trees/sub1.5.3-tree.in"}, wNull},
        {"sub1.5.4", {62, treeFile, "ext-trees/trees/sub1.5.4-tree.in"}, wNull},
        {"sub1.5.5", {125, treeFile, "ext-trees/trees/sub1.5.5-tree.in"}, wNull},
        {"sub1.5.6", {250, treeFile, "ext-trees/trees/sub1.5.6-tree.in"}, wNull},
        {"sub1.5.7", {500, treeFile, "ext-trees/trees/sub1.5.7-tree.in"}, wNull},
        {"sub1.5.8", {1000, treeFile, "ext-trees/trees/sub1.5.8-tree.in"}, wNull},
        {"sub1.5.9", {2000, treeFile, "ext-trees/trees/sub1.5.9-tree.in"}, wNull},
    }).print("cases/sub1.5.in");
    Testcase({
        {"sub1.6.1", {7, treeFile, "ext-trees/trees/sub1.6.1-tree.in"}, wNull},
        {"sub1.6.2", {15, treeFile, "ext-trees/trees/sub1.6.2-tree.in"}, wNull},
        {"sub1.6.3", {31, treeFile, "ext-trees/trees/sub1.6.3-tree.in"}, wNull},
        {"sub1.6.4", {62, treeFile, "ext-trees/trees/sub1.6.4-tree.in"}, wNull},
        {"sub1.6.5", {125, treeFile, "ext-trees/trees/sub1.6.5-tree.in"}, wNull},
        {"sub1.6.6", {250, treeFile, "ext-trees/trees/sub1.6.6-tree.in"}, wNull},
        {"sub1.6.7", {500, treeFile, "ext-trees/trees/sub1.6.7-tree.in"}, wNull},
        {"sub1.6.8", {1000, treeFile, "ext-trees/trees/sub1.6.8-tree.in"}, wNull},
        {"sub1.6.9", {2000, treeFile, "ext-trees/trees/sub1.6.9-tree.in"}, wNull},
    }).print("cases/sub1.6.in");
    //*/

    /*
    100000
    50000
    25000
    12500
    6250
    3125
    1562
    781
    390
    195
    97
    48
    24
    12
    6
    */

    /*
    //Subtask 2
    Testcase({"sub2", 200000, tNull, wEqual}).print("cases/sub2.1.in");
    Testcase({"sub2", 200000, tNull, wEqual}).print("cases/sub2.2.in");
    Testcase({
        {"sub2.3.1", {6, tNull}, wEqual},
        {"sub2.3.2", {12, tNull}, wEqual},
        {"sub2.3.3", {24, tNull}, wEqual},
        {"sub2.3.4", {48, tNull}, wEqual},
        {"sub2.3.5", {97, tNull}, wEqual},
        {"sub2.3.6", {195, tNull}, wEqual},
        {"sub2.3.7", {390, tNull}, wEqual},
        {"sub2.3.8", {781, tNull}, wEqual},
        {"sub2.3.9", {1562, tNull}, wEqual},
        {"sub2.3.10", {3125, tNull}, wEqual},
        {"sub2.3.11", {6250, tNull}, wEqual},
        {"sub2.3.12", {12500, tNull}, wEqual},
        {"sub2.3.13", {25000, tNull}, wEqual},
        {"sub2.3.14", {50000, tNull}, wEqual},
        {"sub2.3.15", {100000, tNull}, wEqual},
    }).print("cases/sub2.3.in");
    Testcase({
        {"sub2.4.1", {6, tNull}, wEqual},
        {"sub2.4.2", {12, tNull}, wEqual},
        {"sub2.4.3", {24, tNull}, wEqual},
        {"sub2.4.4", {48, tNull}, wEqual},
        {"sub2.4.5", {97, tNull}, wEqual},
        {"sub2.4.6", {195, tNull}, wEqual},
        {"sub2.4.7", {390, tNull}, wEqual},
        {"sub2.4.8", {781, tNull}, wEqual},
        {"sub2.4.9", {1562, tNull}, wEqual},
        {"sub2.4.10", {3125, tNull}, wEqual},
        {"sub2.4.11", {6250, tNull}, wEqual},
        {"sub2.4.12", {12500, tNull}, wEqual},
        {"sub2.4.13", {25000, tNull}, wEqual},
        {"sub2.4.14", {50000, tNull}, wEqual},
        {"sub2.4.15", {100000, tNull}, wEqual},
    }).print("cases/sub2.4.in");
    Testcase({
        {"sub2.5.1", {6, treeFile, "ext-trees/trees/sub2.5.1-tree.in"}, wEqual},
        {"sub2.5.2", {12, treeFile, "ext-trees/trees/sub2.5.2-tree.in"}, wEqual},
        {"sub2.5.3", {24, treeFile, "ext-trees/trees/sub2.5.3-tree.in"}, wEqual},
        {"sub2.5.4", {48, treeFile, "ext-trees/trees/sub2.5.4-tree.in"}, wEqual},
        {"sub2.5.5", {97, treeFile, "ext-trees/trees/sub2.5.5-tree.in"}, wEqual},
        {"sub2.5.6", {195, treeFile, "ext-trees/trees/sub2.5.6-tree.in"}, wEqual},
        {"sub2.5.7", {390, treeFile, "ext-trees/trees/sub2.5.7-tree.in"}, wEqual},
        {"sub2.5.8", {781, treeFile, "ext-trees/trees/sub2.5.8-tree.in"}, wEqual},
        {"sub2.5.9", {1562, treeFile, "ext-trees/trees/sub2.5.9-tree.in"}, wEqual},
        {"sub2.5.10", {3125, treeFile, "ext-trees/trees/sub2.5.10-tree.in"}, wEqual},
        {"sub2.5.11", {6250, treeFile, "ext-trees/trees/sub2.5.11-tree.in"}, wEqual},
        {"sub2.5.12", {12500, treeFile, "ext-trees/trees/sub2.5.12-tree.in"}, wEqual},
        {"sub2.5.13", {25000, treeFile, "ext-trees/trees/sub2.5.13-tree.in"}, wEqual},
        {"sub2.5.14", {50000, treeFile, "ext-trees/trees/sub2.5.14-tree.in"}, wEqual},
        {"sub2.5.15", {100000, treeFile, "ext-trees/trees/sub2.5.15-tree.in"}, wEqual},
    }).print("cases/sub2.5.in");
    Testcase({
        {"sub2.6.1", {6, treeFile, "ext-trees/trees/sub2.6.1-tree.in"}, wEqual},
        {"sub2.6.2", {12, treeFile, "ext-trees/trees/sub2.6.2-tree.in"}, wEqual},
        {"sub2.6.3", {24, treeFile, "ext-trees/trees/sub2.6.3-tree.in"}, wEqual},
        {"sub2.6.4", {48, treeFile, "ext-trees/trees/sub2.6.4-tree.in"}, wEqual},
        {"sub2.6.5", {97, treeFile, "ext-trees/trees/sub2.6.5-tree.in"}, wEqual},
        {"sub2.6.6", {195, treeFile, "ext-trees/trees/sub2.6.6-tree.in"}, wEqual},
        {"sub2.6.7", {390, treeFile, "ext-trees/trees/sub2.6.7-tree.in"}, wEqual},
        {"sub2.6.8", {781, treeFile, "ext-trees/trees/sub2.6.8-tree.in"}, wEqual},
        {"sub2.6.9", {1562, treeFile, "ext-trees/trees/sub2.6.9-tree.in"}, wEqual},
        {"sub2.6.10", {3125, treeFile, "ext-trees/trees/sub2.6.10-tree.in"}, wEqual},
        {"sub2.6.11", {6250, treeFile, "ext-trees/trees/sub2.6.11-tree.in"}, wEqual},
        {"sub2.6.12", {12500, treeFile, "ext-trees/trees/sub2.6.12-tree.in"}, wEqual},
        {"sub2.6.13", {25000, treeFile, "ext-trees/trees/sub2.6.13-tree.in"}, wEqual},
        {"sub2.6.14", {50000, treeFile, "ext-trees/trees/sub2.6.14-tree.in"}, wEqual},
        {"sub2.6.15", {100000, treeFile, "ext-trees/trees/sub2.6.15-tree.in"}, wEqual},
    }).print("cases/sub2.6.in");
    //*/

    //Subtask 2
    Testcase({"sub3", 200000, tNull, edgePrime}).print("cases/sub3.1.in");
    /*
    Testcase({"sub3", 200000, tNull, edgePrime}).print("cases/sub3.2.in");
    Testcase({
        {"sub3.3.1", {6, tNull}, edgePrime},
        {"sub3.3.2", {12, tNull}, edgePrime},
        {"sub3.3.3", {24, tNull}, edgePrime},
        {"sub3.3.4", {48, tNull}, edgePrime},
        {"sub3.3.5", {97, tNull}, edgePrime},
        {"sub3.3.6", {195, tNull}, edgePrime},
        {"sub3.3.7", {390, tNull}, edgePrime},
        {"sub3.3.8", {781, tNull}, edgePrime},
        {"sub3.3.9", {1562, tNull}, edgePrime},
        {"sub3.3.10", {3125, tNull}, edgePrime},
        {"sub3.3.11", {6250, tNull}, edgePrime},
        {"sub3.3.12", {12500, tNull}, edgePrime},
        {"sub3.3.13", {25000, tNull}, edgePrime},
        {"sub3.3.14", {50000, tNull}, edgePrime},
        {"sub3.3.15", {100000, tNull}, edgePrime},
    }).print("cases/sub3.3.in");
    Testcase({
        {"sub3.4.1", {6, tNull}, edgePrime},
        {"sub3.4.2", {12, tNull}, edgePrime},
        {"sub3.4.3", {24, tNull}, edgePrime},
        {"sub3.4.4", {48, tNull}, edgePrime},
        {"sub3.4.5", {97, tNull}, edgePrime},
        {"sub3.4.6", {195, tNull}, edgePrime},
        {"sub3.4.7", {390, tNull}, edgePrime},
        {"sub3.4.8", {781, tNull}, edgePrime},
        {"sub3.4.9", {1562, tNull}, edgePrime},
        {"sub3.4.10", {3125, tNull}, edgePrime},
        {"sub3.4.11", {6250, tNull}, edgePrime},
        {"sub3.4.12", {12500, tNull}, edgePrime},
        {"sub3.4.13", {25000, tNull}, edgePrime},
        {"sub3.4.14", {50000, tNull}, edgePrime},
        {"sub3.4.15", {100000, tNull}, edgePrime},
    }).print("cases/sub3.4.in");
    Testcase({
        {"sub3.5.1", {6, treeFile, "ext-trees/trees/sub3.5.1-tree.in"}, edgePrime},
        {"sub3.5.2", {12, treeFile, "ext-trees/trees/sub3.5.2-tree.in"}, edgePrime},
        {"sub3.5.3", {24, treeFile, "ext-trees/trees/sub3.5.3-tree.in"}, edgePrime},
        {"sub3.5.4", {48, treeFile, "ext-trees/trees/sub3.5.4-tree.in"}, edgePrime},
        {"sub3.5.5", {97, treeFile, "ext-trees/trees/sub3.5.5-tree.in"}, edgePrime},
        {"sub3.5.6", {195, treeFile, "ext-trees/trees/sub3.5.6-tree.in"}, edgePrime},
        {"sub3.5.7", {390, treeFile, "ext-trees/trees/sub3.5.7-tree.in"}, edgePrime},
        {"sub3.5.8", {781, treeFile, "ext-trees/trees/sub3.5.8-tree.in"}, edgePrime},
        {"sub3.5.9", {1562, treeFile, "ext-trees/trees/sub3.5.9-tree.in"}, edgePrime},
        {"sub3.5.10", {3125, treeFile, "ext-trees/trees/sub3.5.10-tree.in"}, edgePrime},
        {"sub3.5.11", {6250, treeFile, "ext-trees/trees/sub3.5.11-tree.in"}, edgePrime},
        {"sub3.5.12", {12500, treeFile, "ext-trees/trees/sub3.5.12-tree.in"}, edgePrime},
        {"sub3.5.13", {25000, treeFile, "ext-trees/trees/sub3.5.13-tree.in"}, edgePrime},
        {"sub3.5.14", {50000, treeFile, "ext-trees/trees/sub3.5.14-tree.in"}, edgePrime},
        {"sub3.5.15", {100000, treeFile, "ext-trees/trees/sub3.5.15-tree.in"}, edgePrime},
    }).print("cases/sub3.5.in");
    Testcase({
        {"sub3.6.1", {6, treeFile, "ext-trees/trees/sub3.6.1-tree.in"}, edgePrime},
        {"sub3.6.2", {12, treeFile, "ext-trees/trees/sub3.6.2-tree.in"}, edgePrime},
        {"sub3.6.3", {24, treeFile, "ext-trees/trees/sub3.6.3-tree.in"}, edgePrime},
        {"sub3.6.4", {48, treeFile, "ext-trees/trees/sub3.6.4-tree.in"}, edgePrime},
        {"sub3.6.5", {97, treeFile, "ext-trees/trees/sub3.6.5-tree.in"}, edgePrime},
        {"sub3.6.6", {195, treeFile, "ext-trees/trees/sub3.6.6-tree.in"}, edgePrime},
        {"sub3.6.7", {390, treeFile, "ext-trees/trees/sub3.6.7-tree.in"}, edgePrime},
        {"sub3.6.8", {781, treeFile, "ext-trees/trees/sub3.6.8-tree.in"}, edgePrime},
        {"sub3.6.9", {1562, treeFile, "ext-trees/trees/sub3.6.9-tree.in"}, edgePrime},
        {"sub3.6.10", {3125, treeFile, "ext-trees/trees/sub3.6.10-tree.in"}, edgePrime},
        {"sub3.6.11", {6250, treeFile, "ext-trees/trees/sub3.6.11-tree.in"}, edgePrime},
        {"sub3.6.12", {12500, treeFile, "ext-trees/trees/sub3.6.12-tree.in"}, edgePrime},
        {"sub3.6.13", {25000, treeFile, "ext-trees/trees/sub3.6.13-tree.in"}, edgePrime},
        {"sub3.6.14", {50000, treeFile, "ext-trees/trees/sub3.6.14-tree.in"}, edgePrime},
        {"sub3.6.15", {100000, treeFile, "ext-trees/trees/sub3.6.15-tree.in"}, edgePrime},
    }).print("cases/sub3.6.in");
    */

    return 0;
}