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
    long long operator ()(long long l, long long r){
        return Int(l, r);
    }
};
RandomGen rnd;

enum treeStrategy : unsigned{
    tNull,
    treeRandom,
    treeBamboo,
    treeCaterpillar,
    treeKary,
    treePaste,
    treeFromFile,
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
auto TreeGenerator(TreeMetadata td, bool doShuffle, int start) -> Tree{
    Tree T;
    int N = td.N;
    T.N = N;
    if(N > 1){
        switch(td.strategy){
        case treeFromFile:{
            ifstream fin;
            fin.open(td.filename);
            fin>> T.N;
            for (int i = 0; i < N - 1; i++) {
                int u, v;
                fin>> u>> v;
                T.E.push_back({u, v});
            }
            fin.close();
            break;
        }
        case treeBamboo:{
            for(int i = 1; i < N; i++){
                T.E.push_back({i - 1, i});
            }
            break;
        }
        case treeRandom:{
            vector<int> V(N - 2);
            vector<int> freq(N + 1, 0);
            for(int i = 0; i < N - 2; i++){
                V[i] = rnd(0, N-1);
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
        case treeCaterpillar:{
            int K = rnd(1, N);
            for(int i=1; i<K; i++){
                T.E.push_back({i-1, i});
            }
            for(int i=K; i<N; i++){
                T.E.push_back({i, (int)rnd(0, K-1)});
            }
            break;
        }
        case treeKary:{
            int K = rnd(1, N);
            for(int i=1; i<N; i++){
                T.E.push_back({i, i/K});
            }
            break;
        }
        case treePaste:{
            if(N > 2){
                int xd = 1;
            }
            int K = rnd(1, N);
            for(int i = 1; i < K; i++){
                T.E.push_back({i - 1, i});
            }
            for(int i = K; i < N;){
                int nk = rnd(1, N - i);
                for(int j = 1; j < nk; j++){
                    T.E.push_back({i + j - 1, i + j});
                }
                T.E.push_back({(int)rnd(0, K-1), i});
                i += nk;
            }
            break;
        }
        }
    }
    if(doShuffle){
        vector<int> A(N);
        iota(A.begin(), A.end(), 0);
        shuffle(A.begin(), A.end(), rnd.randomEngine);
        for(auto &[u, v] : T.E){
            if(rnd(0, 1)) swap(u, u);
            u = A[u];
            v = A[v];
        }
        shuffle(T.E.begin(), T.E.end(), rnd.randomEngine);
    }
    if(start){
        for(auto &[u, v] : T.E){
            u++;
            v++;
        }
    }
    return T;
}

enum graphStrategy : unsigned{
    gNull,
    graphFromFile,
    connectedGraph,
    disconnectedGraph,
    graphForest,
    graphTree,
    graphFixedTree,
    graphAllButOne,
    randomGraph
};
struct GraphMetadata{
    int N;
    long long M;
    graphStrategy strategy;
    treeStrategy tStrategy;
};
struct Graph{
    Graph(){
    }
    Graph(const Graph &G) : N(G.N), M(G.M), E(G.E){
    }
    Graph(const Tree &T) : N(T.N), M(T.N-1), E(T.E){
    }
    int N, M;
    vector<array<int, 2>> E;
    bool isConnected;
};
struct BasicDSU{
    BasicDSU(){
    }
    BasicDSU(int n){
        build(n);
    }
    vector<int> id, sz;
    int cnt;
    void build(int N){
        id.resize(N); sz.resize(N);
        iota(id.begin(), id.end(), 0);
        fill(sz.begin(), sz.end(), 1);
        cnt = N;
    }
    int findset(int u){
        return id[u] == u ? u : id[u] = findset(id[u]);
    }
    void joinset(int u, int v){
        int pu = findset(u), pv = findset(v);
        if(pu == pv) return;
        if(sz[pu] > sz[pv]) swap(pu, pv);
        id[pu] = pv;
        sz[pv] += sz[pu];
        cnt--;
    }
};
long long MaxEdgesForN(long long N){
    return (N * (N - 1)) / 2;
}
auto GraphGenerator(GraphMetadata gd, bool doShuffle, int start) -> Graph{
    Graph G;
    int N = gd.N;
    G.N = N;
    if(N > 1){
        switch(gd.strategy){
        case graphFixedTree:{
            G = TreeGenerator({gd.N, gd.tStrategy}, 0, 0);
            break;
        }
        case connectedGraph:{
            vector<treeStrategy> tst = {treeRandom,
                                        treeBamboo,
                                        treeCaterpillar,
                                        treeKary,
                                        treePaste};
            Tree T = TreeGenerator({N, tst[rnd(0, tst.size() - 1)]}, 0, 0);
            set<pair<int, int>> edges;
            for(auto [u, v] : T.E){
                edges.insert({u, v});
                edges.insert({v, u});
            }
            int repeatedEdges = 0;
            while(G.E.size() < G.M && repeatedEdges < 100){
                int u = rnd(0, N-1), v;
                do{
                    v = rnd(0, N-1);
                }while(u == v);
                if(edges.count({u, v})){
                    repeatedEdges++;
                    continue;
                }
                repeatedEdges = 0;
                edges.insert({u, v});
                edges.insert({v, u});
                G.E.push_back({u, v});
            }
            G.isConnected = 1;
            break;
        }
        case disconnectedGraph:{
            int sz;
            if(rnd(0, 1)){
                sz = rnd(2, N/2);
            }
            else sz = rnd(2, sqrt(N));
            vector<int> K(sz, 1);
            for(int i = sz + 1; i <= N; i++){
                K[rnd(0, sz-1)]++;
            }
            int prev = 0;
            for(int i = 0; i < sz; i++){
                Graph g = GraphGenerator({K[i], MaxEdgesForN(K[i]), connectedGraph}, 0, 0);
                for(auto [u, v] : g.E){
                    G.E.push_back({u + prev, v + prev}); 
                }
                prev += K[i];
            }
            G.isConnected = 0;
            break;
        }
        case graphForest:{
            int sz;
            if(rnd(0, 1)){
                sz = rnd(2, N/2);
            }
            else sz = rnd(2, sqrt(N));
            vector<int> K(sz, 1);
            for(int i = sz + 1; i <= N; i++){
                K[rnd(0, sz-1)]++;
            }
            int prev = 0;
            vector<treeStrategy> tst = {treeRandom,
                                        treeBamboo,
                                        treeCaterpillar,
                                        treeKary,
                                        treePaste};
            for(int i = 0; i < sz; i++){
                Tree t = TreeGenerator({K[i], tst[rnd(0, tst.size()-1)]}, 0, 0);
                for(auto [u, v] : t.E){
                    G.E.push_back({u + prev, v + prev}); 
                }
                prev += K[i];
            }
            G.isConnected = 0;
            break;
        }
        case graphAllButOne:{
            G = GraphGenerator({N-1, MaxEdgesForN(N-1), connectedGraph}, 0, 0);
            G.N = N;
            G.isConnected = 0;
            break;
        }
        case graphTree:{
            vector<treeStrategy> tst = {treeRandom,
                                        treeBamboo,
                                        treeCaterpillar,
                                        treeKary,
                                        treePaste};
            Tree T = TreeGenerator({N, tst[rnd(0, tst.size() - 1)]}, 0, 0);
            G = T;
            G.isConnected = 1;
        }
        case randomGraph:{
            set<pair<int, int>> edges;
            int repeatedEdges = 0;
            while(G.E.size() < G.M && repeatedEdges < 100){
                int u = rnd(0, N-1), v;
                do{
                    v = rnd(0, N-1);
                }while(u == v);
                if(edges.count({u, v})){
                    repeatedEdges++;
                    continue;
                }
                repeatedEdges = 0;
                edges.insert({u, v});
                edges.insert({v, u});
                G.E.push_back({u, v});
                
            }
            
        }
        }
    }
    BasicDSU dsu(N);
    for(auto [u, v] : G.E){
        dsu.joinset(u, v);
    }
    G.isConnected = dsu.cnt == 1 ? 1 : 0;
    G.M = G.E.size();
    if(doShuffle){
        vector<int> A(N);
        iota(A.begin(), A.end(), 0);
        shuffle(A.begin(), A.end(), rnd.randomEngine);
        for(auto &[u, v] : G.E){
            if(rnd(0, 1)) swap(u, v);
            u = A[u];
            v = A[v];
        }
        shuffle(G.E.begin(), G.E.end(), rnd.randomEngine);
    }
    if(start){
        for(auto &[u, v] : G.E){
            u++;
            v++;
        }
    }
    return G;
}

vector<vector<int>> GraphToAdjMatrix(Graph g){
    int N = g.N;
    vector<vector<int>> adjMat(N, vector<int>(N, 0));
    for(auto [u, v] : g.E){
        adjMat[u][v] = adjMat[v][u] = 1;
    }
    return adjMat;
};

vector<vector<int>> GenerateDistMatrix(Graph g){
    int N = g.N;
    vector<vector<int>> dist(N, vector<int>(N, -1));
    vector<vector<int>> adj(N);
    for(auto [u, v] : g.E){
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for(int i = 0; i < N; i++){
        dist[i][i] = 0;
        queue<int> q;
        q.push(i);
        while(!q.empty()){
            auto v = q.front();
            q.pop();
            for(auto u : adj[v]){
                if(dist[i][u] != -1) continue;
                dist[i][u] = dist[i][v] + 1;
                q.push(u);
            }
        }
    }
    return dist;
};

struct TestMetadata {
    string casename;
    GraphMetadata gmt;
    int K;
};
struct Case{
    int N, K;
    vector<vector<int>> distMat;
    int isConnected;
    Case(){}
    Case(TestMetadata meta){
        if(meta.gmt.strategy == gNull){
            vector<graphStrategy> gst = {connectedGraph,
                                        disconnectedGraph,
                                        graphForest,
                                        graphTree,
                                        graphAllButOne,
                                        randomGraph};
            meta.gmt.strategy = gst[rnd.Int(0, gst.size() - 1)];
        }
        N = meta.gmt.N, K = meta.K;
        Graph G = GraphGenerator(meta.gmt, 1, 0);
        distMat = GenerateDistMatrix(G);
        isConnected = G.isConnected;
    }
    void print(string filename, bool newFile){
        ofstream input;
        if(newFile) input.open(filename);
        else input.open(filename, ifstream::app);
        input<< N<< ' '<< K<< '\n';
        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){
                input<< distMat[i][j]<< " \n"[j == N-1];
            }
        }
        int MAXCalls = (2 * N * N) / K;
        input<< isConnected<< ' '<< MAXCalls<< '\n';
        input.close();
    }
};

struct SubtaskMetadata{
    int subtask;
    string subname;
    graphStrategy gst;
};

struct Testcase{
    vector<Case> casesOfTest;
    Testcase(SubtaskMetadata meta){
        int T = rnd(1, 50);
        vector<int> N(T, 2);
        for(int i = 2 * T + 1; i <= 1000; i++){
            N[rnd(0, T - 1)]++;
        }
        sort(N.begin(), N.end());
        if(meta.subtask == 3){
            for(auto &n : N){
                if(n & 1){
                    n--;
                }
            }
        }
        for(int i=0; i<T; i++){
            string casename = meta.subname + "." + to_string(i + 1);
            casesOfTest.push_back(Case({casename, {N[i], MaxEdgesForN(N[i]), meta.gst}, getKForCase(meta.subtask, N[i])}));
        }
    }
    Testcase(vector<TestMetadata> metadata){
        for(auto &meta : metadata){
            casesOfTest.push_back(Case(meta));
        }
    }
    static int getKForCase(int subtask, int N){
        int K = -1;
        switch(subtask){
        case 1:
            K = N - 1;
            break;
        case 2:
            K = rnd(1, 4);
            break;
        case 3:
            K = N/2;
            break;
        case 4:
            K = rnd(1, N);
            break;
        case 5:
            K = rnd(1, N);
            break;
        }
        return K;
    }
    void print(string filename){
        ofstream input;
        input.open(filename);
        input<< casesOfTest.size()<< '\n';
        input.close();
        for(auto C : casesOfTest){
            C.print(filename, 0);
        }
    }
};


int main(){
    /*
    connectedGraph
    disconnectedGraph
    graphForest
    graphTree
    graphAllButOne
    randomGraph
    */
    // Subtask 1
    Testcase({1, "sub1", gNull}).print("cases/sub1.1.in");
    Testcase({1, "sub1", gNull}).print("cases/sub1.2.in");
    Testcase({1, "sub1", gNull}).print("cases/sub1.3.in");
    Testcase({
        {"sub1.4.1", {100, MaxEdgesForN(100), graphAllButOne}, Testcase::getKForCase(1, 100)},
        {"sub1.4.2", {100, MaxEdgesForN(100), connectedGraph}, Testcase::getKForCase(1, 100)},
        {"sub1.4.3", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(1, 100)},
        {"sub1.4.4", {100, MaxEdgesForN(100), randomGraph}, Testcase::getKForCase(1, 100)},
        {"sub1.4.5", {100, MaxEdgesForN(100), disconnectedGraph}, Testcase::getKForCase(1, 100)},
        {"sub1.4.6", {100, MaxEdgesForN(100), graphAllButOne}, Testcase::getKForCase(1, 100)},
        {"sub1.4.7", {100, MaxEdgesForN(100), connectedGraph}, Testcase::getKForCase(1, 100)},
        {"sub1.4.8", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(1, 100)},
        {"sub1.4.9", {100, MaxEdgesForN(100), randomGraph}, Testcase::getKForCase(1, 100)},
        {"sub1.4.10", {100, MaxEdgesForN(100), disconnectedGraph}, Testcase::getKForCase(1, 100)},
    }).print("cases/sub1.4.in");
    Testcase({
        {"sub1.5.1", {100, MaxEdgesForN(100), graphAllButOne}, Testcase::getKForCase(1, 100)},
        {"sub1.5.2", {100, MaxEdgesForN(100), connectedGraph}, Testcase::getKForCase(1, 100)},
        {"sub1.5.3", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(1, 100)},
        {"sub1.5.4", {100, MaxEdgesForN(100), randomGraph}, Testcase::getKForCase(1, 100)},
        {"sub1.5.5", {100, MaxEdgesForN(100), disconnectedGraph}, Testcase::getKForCase(1, 100)},
        {"sub1.5.6", {100, MaxEdgesForN(100), graphAllButOne}, Testcase::getKForCase(1, 100)},
        {"sub1.5.7", {100, MaxEdgesForN(100), connectedGraph}, Testcase::getKForCase(1, 100)},
        {"sub1.5.8", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(1, 100)},
        {"sub1.5.9", {100, MaxEdgesForN(100), randomGraph}, Testcase::getKForCase(1, 100)},
        {"sub1.5.10", {100, MaxEdgesForN(100), disconnectedGraph}, Testcase::getKForCase(1, 100)},
    }).print("cases/sub1.5.in");
    Testcase({
        {"sub1.6.1", {125, MaxEdgesForN(125), graphFixedTree, treeRandom}, Testcase::getKForCase(1, 125)},
        {"sub1.6.2", {125, MaxEdgesForN(125), graphFixedTree, treeBamboo}, Testcase::getKForCase(1, 125)},
        {"sub1.6.3", {125, MaxEdgesForN(125), graphFixedTree, treeCaterpillar}, Testcase::getKForCase(1, 125)},
        {"sub1.6.4", {125, MaxEdgesForN(125), graphFixedTree, treePaste}, Testcase::getKForCase(1, 125)},
        {"sub1.6.5", {125, MaxEdgesForN(125), graphFixedTree, treeRandom}, Testcase::getKForCase(1, 125)},
        {"sub1.6.6", {125, MaxEdgesForN(125), graphFixedTree, treeBamboo}, Testcase::getKForCase(1, 125)},
        {"sub1.6.7", {125, MaxEdgesForN(125), graphFixedTree, treeCaterpillar}, Testcase::getKForCase(1, 125)},
        {"sub1.6.8", {125, MaxEdgesForN(125), graphFixedTree, treePaste}, Testcase::getKForCase(1, 125)},
    }).print("cases/sub1.6.in");

    //Subtask 2
    Testcase({2, "sub2", gNull}).print("cases/sub2.1.in");
    Testcase({2, "sub2", gNull}).print("cases/sub2.2.in");
    Testcase({2, "sub2", gNull}).print("cases/sub2.3.in");
    Testcase({
        {"sub2.4.1", {100, MaxEdgesForN(100), graphAllButOne}, Testcase::getKForCase(2, 100)},
        {"sub2.4.2", {100, MaxEdgesForN(100), connectedGraph}, Testcase::getKForCase(2, 100)},
        {"sub2.4.3", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(2, 100)},
        {"sub2.4.4", {100, MaxEdgesForN(100), randomGraph}, Testcase::getKForCase(2, 100)},
        {"sub2.4.5", {100, MaxEdgesForN(100), disconnectedGraph}, Testcase::getKForCase(2, 100)},
        {"sub2.4.6", {100, MaxEdgesForN(100), graphAllButOne}, Testcase::getKForCase(2, 100)},
        {"sub2.4.7", {100, MaxEdgesForN(100), connectedGraph}, Testcase::getKForCase(2, 100)},
        {"sub2.4.8", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(2, 100)},
        {"sub2.4.9", {100, MaxEdgesForN(100), randomGraph}, Testcase::getKForCase(2, 100)},
        {"sub2.4.10", {100, MaxEdgesForN(100), disconnectedGraph}, Testcase::getKForCase(2, 100)},
    }).print("cases/sub2.4.in");
    Testcase({
        {"sub2.5.1", {100, MaxEdgesForN(100), graphAllButOne}, Testcase::getKForCase(2, 100)},
        {"sub2.5.2", {100, MaxEdgesForN(100), connectedGraph}, Testcase::getKForCase(2, 100)},
        {"sub2.5.3", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(2, 100)},
        {"sub2.5.4", {100, MaxEdgesForN(100), randomGraph}, Testcase::getKForCase(2, 100)},
        {"sub2.5.5", {100, MaxEdgesForN(100), disconnectedGraph}, Testcase::getKForCase(2, 100)},
        {"sub2.5.6", {100, MaxEdgesForN(100), graphAllButOne}, Testcase::getKForCase(2, 100)},
        {"sub2.5.7", {100, MaxEdgesForN(100), connectedGraph}, Testcase::getKForCase(2, 100)},
        {"sub2.5.8", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(2, 100)},
        {"sub2.5.9", {100, MaxEdgesForN(100), randomGraph}, Testcase::getKForCase(2, 100)},
        {"sub2.5.10", {100, MaxEdgesForN(100), disconnectedGraph}, Testcase::getKForCase(2, 100)},
    }).print("cases/sub2.5.in");
    Testcase({
        {"sub2.6.1", {125, MaxEdgesForN(125), graphFixedTree, treeRandom}, Testcase::getKForCase(2, 125)},
        {"sub2.6.2", {125, MaxEdgesForN(125), graphFixedTree, treeBamboo}, Testcase::getKForCase(2, 125)},
        {"sub2.6.3", {125, MaxEdgesForN(125), graphFixedTree, treeCaterpillar}, Testcase::getKForCase(2, 125)},
        {"sub2.6.4", {125, MaxEdgesForN(125), graphFixedTree, treePaste}, Testcase::getKForCase(2, 125)},
        {"sub2.6.5", {125, MaxEdgesForN(125), graphFixedTree, treeRandom}, Testcase::getKForCase(2, 125)},
        {"sub2.6.6", {125, MaxEdgesForN(125), graphFixedTree, treeBamboo}, Testcase::getKForCase(2, 125)},
        {"sub2.6.7", {125, MaxEdgesForN(125), graphFixedTree, treeCaterpillar}, Testcase::getKForCase(2, 125)},
        {"sub2.6.8", {125, MaxEdgesForN(125), graphFixedTree, treePaste}, Testcase::getKForCase(2, 125)},
    }).print("cases/sub2.6.in");

    //Subtask 3
    Testcase({3, "sub3", graphForest}).print("cases/sub3.1.in");
    Testcase({3, "sub3", graphTree}).print("cases/sub3.2.in");
    Testcase({3, "sub3", graphForest}).print("cases/sub3.3.in");
    Testcase({
        {"sub3.4.1", {100, MaxEdgesForN(100), graphForest}, Testcase::getKForCase(3, 100)},
        {"sub3.4.2", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(3, 100)},
        {"sub3.4.3", {100, MaxEdgesForN(100), graphForest}, Testcase::getKForCase(3, 100)},
        {"sub3.4.4", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(3, 100)},
        {"sub3.4.5", {100, MaxEdgesForN(100), graphForest}, Testcase::getKForCase(3, 100)},
        {"sub3.4.6", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(3, 100)},
        {"sub3.4.7", {100, MaxEdgesForN(100), graphForest}, Testcase::getKForCase(3, 100)},
        {"sub3.4.8", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(3, 100)},
        {"sub3.4.9", {100, MaxEdgesForN(100), graphForest}, Testcase::getKForCase(3, 100)},
        {"sub3.4.10", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(3, 100)},
    }).print("cases/sub3.4.in");
    Testcase({
        {"sub3.5.1", {100, MaxEdgesForN(100), graphForest}, Testcase::getKForCase(3, 100)},
        {"sub3.5.2", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(3, 100)},
        {"sub3.5.3", {100, MaxEdgesForN(100), graphForest}, Testcase::getKForCase(3, 100)},
        {"sub3.5.4", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(3, 100)},
        {"sub3.5.5", {100, MaxEdgesForN(100), graphForest}, Testcase::getKForCase(3, 100)},
        {"sub3.5.6", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(3, 100)},
        {"sub3.5.7", {100, MaxEdgesForN(100), graphForest}, Testcase::getKForCase(3, 100)},
        {"sub3.5.8", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(3, 100)},
        {"sub3.5.9", {100, MaxEdgesForN(100), graphForest}, Testcase::getKForCase(3, 100)},
        {"sub3.5.10", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(3, 100)},
    }).print("cases/sub3.5.in");
    Testcase({
        {"sub3.6.1", {124, MaxEdgesForN(124), graphFixedTree, treeRandom}, Testcase::getKForCase(3, 124)},
        {"sub3.6.2", {124, MaxEdgesForN(124), graphFixedTree, treeBamboo}, Testcase::getKForCase(3, 124)},
        {"sub3.6.3", {124, MaxEdgesForN(124), graphFixedTree, treeCaterpillar}, Testcase::getKForCase(3, 124)},
        {"sub3.6.4", {124, MaxEdgesForN(124), graphFixedTree, treePaste}, Testcase::getKForCase(3, 124)},
        {"sub3.6.5", {126, MaxEdgesForN(126), graphFixedTree, treeRandom}, Testcase::getKForCase(3, 126)},
        {"sub3.6.6", {126, MaxEdgesForN(126), graphFixedTree, treeBamboo}, Testcase::getKForCase(3, 126)},
        {"sub3.6.7", {126, MaxEdgesForN(126), graphFixedTree, treeCaterpillar}, Testcase::getKForCase(3, 126)},
        {"sub3.6.8", {126, MaxEdgesForN(126), graphFixedTree, treePaste}, Testcase::getKForCase(3, 126)},
    }).print("cases/sub3.6.in");

    //Subtask 4
    Testcase({4, "sub1", gNull}).print("cases/sub4.1.in");
    Testcase({4, "sub1", gNull}).print("cases/sub4.2.in");
    Testcase({4, "sub1", gNull}).print("cases/sub4.3.in");
    Testcase({
        {"sub4.4.1", {100, MaxEdgesForN(100), graphAllButOne}, Testcase::getKForCase(4, 100)},
        {"sub4.4.2", {100, MaxEdgesForN(100), connectedGraph}, Testcase::getKForCase(4, 100)},
        {"sub4.4.3", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(4, 100)},
        {"sub4.4.4", {100, MaxEdgesForN(100), randomGraph}, Testcase::getKForCase(4, 100)},
        {"sub4.4.5", {100, MaxEdgesForN(100), disconnectedGraph}, Testcase::getKForCase(4, 100)},
        {"sub4.4.6", {100, MaxEdgesForN(100), graphAllButOne}, Testcase::getKForCase(4, 100)},
        {"sub4.4.7", {100, MaxEdgesForN(100), connectedGraph}, Testcase::getKForCase(4, 100)},
        {"sub4.4.8", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(4, 100)},
        {"sub4.4.9", {100, MaxEdgesForN(100), randomGraph}, Testcase::getKForCase(4, 100)},
        {"sub4.4.10", {100, MaxEdgesForN(100), disconnectedGraph}, Testcase::getKForCase(4, 100)},
    }).print("cases/sub4.4.in");
    Testcase({
        {"sub4.5.1", {100, MaxEdgesForN(100), graphAllButOne}, Testcase::getKForCase(4, 100)},
        {"sub4.5.2", {100, MaxEdgesForN(100), connectedGraph}, Testcase::getKForCase(4, 100)},
        {"sub4.5.3", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(4, 100)},
        {"sub4.5.4", {100, MaxEdgesForN(100), randomGraph}, Testcase::getKForCase(4, 100)},
        {"sub4.5.5", {100, MaxEdgesForN(100), disconnectedGraph}, Testcase::getKForCase(4, 100)},
        {"sub4.5.6", {100, MaxEdgesForN(100), graphAllButOne}, Testcase::getKForCase(4, 100)},
        {"sub4.5.7", {100, MaxEdgesForN(100), connectedGraph}, Testcase::getKForCase(4, 100)},
        {"sub4.5.8", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(4, 100)},
        {"sub4.5.9", {100, MaxEdgesForN(100), randomGraph}, Testcase::getKForCase(4, 100)},
        {"sub4.5.10", {100, MaxEdgesForN(100), disconnectedGraph}, Testcase::getKForCase(4, 100)},
    }).print("cases/sub4.5.in");
    Testcase({
        {"sub4.6.1", {125, MaxEdgesForN(125), graphFixedTree, treeRandom}, Testcase::getKForCase(4, 125)},
        {"sub4.6.2", {125, MaxEdgesForN(125), graphFixedTree, treeBamboo}, Testcase::getKForCase(4, 125)},
        {"sub4.6.3", {125, MaxEdgesForN(125), graphFixedTree, treeCaterpillar}, Testcase::getKForCase(4, 125)},
        {"sub4.6.4", {125, MaxEdgesForN(125), graphFixedTree, treePaste}, Testcase::getKForCase(4, 125)},
        {"sub4.6.5", {125, MaxEdgesForN(125), graphFixedTree, treeRandom}, Testcase::getKForCase(4, 125)},
        {"sub4.6.6", {125, MaxEdgesForN(125), graphFixedTree, treeBamboo}, Testcase::getKForCase(4, 125)},
        {"sub4.6.7", {125, MaxEdgesForN(125), graphFixedTree, treeCaterpillar}, Testcase::getKForCase(4, 125)},
        {"sub4.6.8", {125, MaxEdgesForN(125), graphFixedTree, treePaste}, Testcase::getKForCase(4, 125)},
    }).print("cases/sub4.6.in");

    //Subtask 5
    Testcase({5, "sub1", gNull}).print("cases/sub5.1.in");
    Testcase({5, "sub1", gNull}).print("cases/sub5.2.in");
    Testcase({5, "sub1", gNull}).print("cases/sub5.3.in");
    Testcase({
        {"sub5.4.1", {100, MaxEdgesForN(100), graphAllButOne}, Testcase::getKForCase(5, 100)},
        {"sub5.4.2", {100, MaxEdgesForN(100), connectedGraph}, Testcase::getKForCase(5, 100)},
        {"sub5.4.3", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(5, 100)},
        {"sub5.4.4", {100, MaxEdgesForN(100), randomGraph}, Testcase::getKForCase(5, 100)},
        {"sub5.4.5", {100, MaxEdgesForN(100), disconnectedGraph}, Testcase::getKForCase(5, 100)},
        {"sub5.4.6", {100, MaxEdgesForN(100), graphAllButOne}, Testcase::getKForCase(5, 100)},
        {"sub5.4.7", {100, MaxEdgesForN(100), connectedGraph}, Testcase::getKForCase(5, 100)},
        {"sub5.4.8", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(5, 100)},
        {"sub5.4.9", {100, MaxEdgesForN(100), randomGraph}, Testcase::getKForCase(5, 100)},
        {"sub5.4.10", {100, MaxEdgesForN(100), disconnectedGraph}, Testcase::getKForCase(5, 100)},
    }).print("cases/sub5.4.in");
    Testcase({
        {"sub5.5.1", {100, MaxEdgesForN(100), graphAllButOne}, Testcase::getKForCase(5, 100)},
        {"sub5.5.2", {100, MaxEdgesForN(100), connectedGraph}, Testcase::getKForCase(5, 100)},
        {"sub5.5.3", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(5, 100)},
        {"sub5.5.4", {100, MaxEdgesForN(100), randomGraph}, Testcase::getKForCase(5, 100)},
        {"sub5.5.5", {100, MaxEdgesForN(100), disconnectedGraph}, Testcase::getKForCase(5, 100)},
        {"sub5.5.6", {100, MaxEdgesForN(100), graphAllButOne}, Testcase::getKForCase(5, 100)},
        {"sub5.5.7", {100, MaxEdgesForN(100), connectedGraph}, Testcase::getKForCase(5, 100)},
        {"sub5.5.8", {100, MaxEdgesForN(100), graphTree}, Testcase::getKForCase(5, 100)},
        {"sub5.5.9", {100, MaxEdgesForN(100), randomGraph}, Testcase::getKForCase(5, 100)},
        {"sub5.5.10", {100, MaxEdgesForN(100), disconnectedGraph}, Testcase::getKForCase(5, 100)},
    }).print("cases/sub5.5.in");
    Testcase({
        {"sub5.6.1", {125, MaxEdgesForN(125), graphFixedTree, treeRandom}, Testcase::getKForCase(5, 125)},
        {"sub5.6.2", {125, MaxEdgesForN(125), graphFixedTree, treeBamboo}, Testcase::getKForCase(5, 125)},
        {"sub5.6.3", {125, MaxEdgesForN(125), graphFixedTree, treeCaterpillar}, Testcase::getKForCase(5, 125)},
        {"sub5.6.4", {125, MaxEdgesForN(125), graphFixedTree, treePaste}, Testcase::getKForCase(5, 125)},
        {"sub5.6.5", {125, MaxEdgesForN(125), graphFixedTree, treeRandom}, Testcase::getKForCase(5, 125)},
        {"sub5.6.6", {125, MaxEdgesForN(125), graphFixedTree, treeBamboo}, Testcase::getKForCase(5, 125)},
        {"sub5.6.7", {125, MaxEdgesForN(125), graphFixedTree, treeCaterpillar}, Testcase::getKForCase(5, 125)},
        {"sub5.6.8", {125, MaxEdgesForN(125), graphFixedTree, treePaste}, Testcase::getKForCase(5, 125)},
    }).print("cases/sub5.6.in");
    return 0;
}