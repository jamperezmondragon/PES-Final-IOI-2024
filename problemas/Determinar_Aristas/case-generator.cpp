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
    treeBinaryFixed,
    treeBinary,
    treeStar,
    treePaste,
    treeLine,
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
auto TreeFromPruffer(int N, vector<int> V) -> Tree{
    Tree T;
    T.N = N;
    vector<int> freq(N + 1, 0);
    for(int i = 0; i < N - 2; i++){
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
    return T;
}
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
            for(int i = 0; i < N - 2; i++){
                V[i] = rnd(0, N-1);
            }
            T = TreeFromPruffer(N, V);
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
        case treeBinaryFixed:{
            for(int i=2; i<=N; i++){
                T.E.push_back({i - 1, (i / 2) - 1});
            }
            shuffle(T.E.begin(), T.E.end(), rnd.randomEngine);
            return T;
        }
        case treeBinary:{
            for(int i=2; i<=N; i++){
                T.E.push_back({i - 1, (i / 2) - 1});
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
        case treeLine:{
            for(int i = 1; i < N; i++){
                T.E.push_back({i - 1, i});
            }
            shuffle(T.E.begin(), T.E.end(), rnd.randomEngine);
            break;
        }
        case treeStar:{
            for(int i = 1; i < N; i++){
                T.E.push_back({0, i});
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
            if(rnd(0, 1)) swap(u, v);
            u = A[u];
            v = A[v];
        }
    }
    shuffle(T.E.begin(), T.E.end(), rnd.randomEngine);
    if(start){
        for(auto &[u, v] : T.E){
            u++;
            v++;
        }
    }
    return T;
}

vector<Tree> generateAllTrees(int size){
    vector<int> V;
    vector<Tree> allTrees;
    function<void(int)> dfs = [&](int x){
        if (x == 0) { 
            allTrees.push_back(TreeFromPruffer(size, V));
        }
        else{
            for(int i = 0; i < size; i++){
                V.push_back(i);
                dfs(x - 1);
                V.pop_back();
            }
        }
    };
    dfs(size - 2);
    return allTrees;
}

void shuffleTree(Tree &T){
    vector<int> A(T.N);
    iota(A.begin(), A.end(), 0);
    shuffle(A.begin(), A.end(), rnd.randomEngine);
    for(auto &[u, v] : T.E){
        if(rnd(0, 1)) swap(u, v);
        u = A[u];
        v = A[v];
    }
    shuffle(T.E.begin(), T.E.end(), rnd.randomEngine);
}

struct TestMetadata {
    int N;
    treeStrategy tst;
};
struct Case{
    Tree T;
    Case(){}
    Case(TestMetadata meta){
        if(meta.tst == tNull){
            vector<treeStrategy> tst = {treeRandom,
                                    treeBamboo,
                                    treeCaterpillar,
                                    treeKary,
                                    treeStar,
                                    treePaste};
            meta.tst = tst[rnd(0, tst.size() - 1)];           
        }
        T = TreeGenerator({meta.N, meta.tst}, 1, 1);
    }
    Case(const Case &c){
        T = c.T;
    }
    Case(const Tree &t){
        T = t;
    }
    void print(string filename, bool newFile){
        ofstream input;
        if(newFile) input.open(filename);
        else input.open(filename, ifstream::app);
        input<< T.N<< '\n';
        for(auto [u, v] : T.E){
            input<< u<< ' '<< v<< '\n';
        }
        input.close();
    }
};

struct SubtaskMetadata{
    treeStrategy tst;
};

struct Testcase{
    vector<Case> casesOfTest;
    Testcase(SubtaskMetadata meta){
        int T = rnd(1, 50);
        vector<int> N(T, 2);
        for(int i = 2 * T + 1; i <= 2000; i++){
            N[rnd(0, T - 1)]++;
        }
        sort(N.begin(), N.end());
        for(int i=0; i<T; i++){
            casesOfTest.push_back(Case({N[i], meta.tst}));
        }
    }
    Testcase(vector<TestMetadata> metadata){
        for(auto &meta : metadata){
            casesOfTest.push_back(Case(meta));
        }
    }
    Testcase(int x){
        auto aT = generateAllTrees(x);
        for(auto t : aT){
            shuffleTree(t);
            casesOfTest.push_back({t});
        }
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
    //Subtask 1
    Testcase(4).print("cases/sub1.1.in");

    //Subtask 2
    SubtaskMetadata smt = {tNull};
    Testcase(5).print("cases/sub2.1.in");
    Testcase(smt).print("cases/sub2.2.in");
    Testcase(smt).print("cases/sub2.3.in");
    Testcase(smt).print("cases/sub2.3.in");
    Testcase({
        {1000, treeRandom},
        {1000, treeBamboo}
    }).print("cases/sub2.4.in");
    Testcase({
        {1000, treeCaterpillar},
        {1000, treeKary}
    }).print("cases/sub2.5.in");
    Testcase({
        {1000, treeBinary},
        {1000, treeStar}
    }).print("cases/sub2.6.in");
    Testcase({
        {1000, treePaste},
    }).print("cases/sub2.7.in");

    //Subtask 3
    smt = {treeLine};
    Testcase(smt).print("cases/sub3.1.in");
    Testcase(smt).print("cases/sub3.2.in");
    Testcase({
        {2000, treeLine}
    }).print("cases/sub3.3.in");

    //Subtask 4
    smt = {treeBinaryFixed};
    Testcase(smt).print("cases/sub4.1.in");
    Testcase(smt).print("cases/sub4.2.in");
    Testcase({
        {2000, treeBinaryFixed}
    }).print("cases/sub4.3.in");

    //Subtask 5
    smt = {treeStar};
    Testcase(smt).print("cases/sub5.1.in");
    Testcase(smt).print("cases/sub5.2.in");
    Testcase({
        {2000, treeStar}
    }).print("cases/sub5.3.in");

    //Subtask 6
    smt = {tNull};
    Testcase(5).print("cases/sub6.1.in");
    Testcase(smt).print("cases/sub6.2.in");
    Testcase(smt).print("cases/sub6.3.in");
    Testcase(smt).print("cases/sub6.3.in");
    Testcase({
        {1000, treeRandom},
        {1000, treeBamboo}
    }).print("cases/sub6.4.in");
    Testcase({
        {1000, treeCaterpillar},
        {1000, treeKary}
    }).print("cases/sub6.5.in");
    Testcase({
        {1000, treeBinary},
        {1000, treeStar}
    }).print("cases/sub6.6.in");
    Testcase({
        {1000, treePaste},
    }).print("cases/sub6.7.in");
    return 0;
}