#include<bits/stdc++.h>
#include "jnhen.h"

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
};
RandomGen rd;

enum treeStrategy : unsigned{
    treeRandom,
    treeLikeStar,
    treeLikeBamboo,
    treePrim,
    treeKruskal,
    treeBamboo,
    treeStar,
    treeCaterpillar,
    treeBinary,
    treeKAry,
};

void generateATree(string filename, treeStrategy strategy, int N){
    auto t = Tree();
    switch(strategy){
    case treeRandom:{
        t = Tree::random(N);
        break;
    }
    case treeLikeStar:{
        //likely a star
        t = Tree::randomPrim(N, -1000000);
        break;
    }
    case treeLikeBamboo:{
        //likely a bamboo
        t = Tree::randomPrim(N, 1000000);
        break;
    }
    case treePrim:{
        t = Tree::randomPrim(N);
        break;
    }
    case treeKruskal:{
        t = Tree::randomKruskal(N);
    }
    case treeBamboo:{
        t = Tree::bamboo(N);
    }
    case treeStar:{
        t = Tree::star(N);
    }
    case treeCaterpillar:{
        t = Tree::caterpillar(N, rd.Int(1, N));
    }
    case treeBinary:{
        t = Tree::binary(N);
    }
    case treeKAry:{
        t = Tree::kary(N, rd.Int(1, N));
    }
    default:{
        t = Tree::random(N);
    }
    }

    ofstream input;
    input.open(filename);
    input<< t.printN();
    input.close();
}

int main(){
    /*
    generateATree("trees/sub2.4-tree.in", treeCaterpillar, 50000);
    generateATree("trees/sub2.5-tree.in", treeLikeBamboo, 100000);
    generateATree("trees/sub2.6-tree.in", treeRandom, 200000);
    //*/
    /*
    generateATree("trees/sub1.5.1-tree.in", treeRandom, 7);
    generateATree("trees/sub1.5.2-tree.in", treeCaterpillar, 15);
    generateATree("trees/sub1.5.3-tree.in", treeLikeBamboo, 31);
    generateATree("trees/sub1.5.4-tree.in", treeRandom, 62);
    generateATree("trees/sub1.5.5-tree.in", treeCaterpillar, 125);
    generateATree("trees/sub1.5.6-tree.in", treeLikeBamboo, 250);
    generateATree("trees/sub1.5.7-tree.in", treeRandom, 500);
    generateATree("trees/sub1.5.8-tree.in", treeCaterpillar, 1000);
    generateATree("trees/sub1.5.9-tree.in", treeLikeBamboo, 2000);

    generateATree("trees/sub1.6.1-tree.in", treeRandom, 7);
    generateATree("trees/sub1.6.2-tree.in", treeCaterpillar, 15);
    generateATree("trees/sub1.6.3-tree.in", treeLikeBamboo, 31);
    generateATree("trees/sub1.6.4-tree.in", treeRandom, 62);
    generateATree("trees/sub1.6.5-tree.in", treeCaterpillar, 125);
    generateATree("trees/sub1.6.6-tree.in", treeLikeBamboo, 250);
    generateATree("trees/sub1.6.7-tree.in", treeRandom, 500);
    generateATree("trees/sub1.6.8-tree.in", treeCaterpillar, 1000);
    generateATree("trees/sub1.6.9-tree.in", treeLikeBamboo, 2000);
    */

    /*
    generateATree("trees/sub2.5.1-tree.in", treeRandom, 6);
    generateATree("trees/sub2.5.2-tree.in", treeCaterpillar, 12);
    generateATree("trees/sub2.5.3-tree.in", treeLikeBamboo, 24);
    generateATree("trees/sub2.5.4-tree.in", treeRandom, 48);
    generateATree("trees/sub2.5.5-tree.in", treeCaterpillar, 97);
    generateATree("trees/sub2.5.6-tree.in", treeLikeBamboo, 195);
    generateATree("trees/sub2.5.7-tree.in", treeRandom, 390);
    generateATree("trees/sub2.5.8-tree.in", treeCaterpillar, 781);
    generateATree("trees/sub2.5.9-tree.in", treeLikeBamboo, 1562);
    generateATree("trees/sub2.5.10-tree.in", treeRandom, 3125);
    generateATree("trees/sub2.5.11-tree.in", treeCaterpillar, 6250);
    generateATree("trees/sub2.5.12-tree.in", treeLikeBamboo, 12500);
    generateATree("trees/sub2.5.13-tree.in", treeRandom, 25000);
    generateATree("trees/sub2.5.14-tree.in", treeCaterpillar, 50000);
    generateATree("trees/sub2.5.15-tree.in", treeLikeBamboo, 100000);
    
    generateATree("trees/sub2.6.1-tree.in", treeRandom, 6);
    generateATree("trees/sub2.6.2-tree.in", treeCaterpillar, 12);
    generateATree("trees/sub2.6.3-tree.in", treeLikeBamboo, 24);
    generateATree("trees/sub2.6.4-tree.in", treeRandom, 48);
    generateATree("trees/sub2.6.5-tree.in", treeCaterpillar, 97);
    generateATree("trees/sub2.6.6-tree.in", treeLikeBamboo, 195);
    generateATree("trees/sub2.6.7-tree.in", treeRandom, 390);
    generateATree("trees/sub2.6.8-tree.in", treeCaterpillar, 781);
    generateATree("trees/sub2.6.9-tree.in", treeLikeBamboo, 1562);
    generateATree("trees/sub2.6.10-tree.in", treeRandom, 3125);
    generateATree("trees/sub2.6.11-tree.in", treeCaterpillar, 6250);
    generateATree("trees/sub2.6.12-tree.in", treeLikeBamboo, 12500);
    generateATree("trees/sub2.6.13-tree.in", treeRandom, 25000);
    generateATree("trees/sub2.6.14-tree.in", treeCaterpillar, 50000);
    generateATree("trees/sub2.6.15-tree.in", treeLikeBamboo, 100000);
    */

    generateATree("trees/sub3.5.1-tree.in", treeRandom, 6);
    generateATree("trees/sub3.5.2-tree.in", treeCaterpillar, 12);
    generateATree("trees/sub3.5.3-tree.in", treeLikeBamboo, 24);
    generateATree("trees/sub3.5.4-tree.in", treeRandom, 48);
    generateATree("trees/sub3.5.5-tree.in", treeCaterpillar, 97);
    generateATree("trees/sub3.5.6-tree.in", treeLikeBamboo, 195);
    generateATree("trees/sub3.5.7-tree.in", treeRandom, 390);
    generateATree("trees/sub3.5.8-tree.in", treeCaterpillar, 781);
    generateATree("trees/sub3.5.9-tree.in", treeLikeBamboo, 1562);
    generateATree("trees/sub3.5.10-tree.in", treeRandom, 3125);
    generateATree("trees/sub3.5.11-tree.in", treeCaterpillar, 6250);
    generateATree("trees/sub3.5.12-tree.in", treeLikeBamboo, 12500);
    generateATree("trees/sub3.5.13-tree.in", treeRandom, 25000);
    generateATree("trees/sub3.5.14-tree.in", treeCaterpillar, 50000);
    generateATree("trees/sub3.5.15-tree.in", treeLikeBamboo, 100000);
    
    generateATree("trees/sub3.6.1-tree.in", treeRandom, 6);
    generateATree("trees/sub3.6.2-tree.in", treeCaterpillar, 12);
    generateATree("trees/sub3.6.3-tree.in", treeLikeBamboo, 24);
    generateATree("trees/sub3.6.4-tree.in", treeRandom, 48);
    generateATree("trees/sub3.6.5-tree.in", treeCaterpillar, 97);
    generateATree("trees/sub3.6.6-tree.in", treeLikeBamboo, 195);
    generateATree("trees/sub3.6.7-tree.in", treeRandom, 390);
    generateATree("trees/sub3.6.8-tree.in", treeCaterpillar, 781);
    generateATree("trees/sub3.6.9-tree.in", treeLikeBamboo, 1562);
    generateATree("trees/sub3.6.10-tree.in", treeRandom, 3125);
    generateATree("trees/sub3.6.11-tree.in", treeCaterpillar, 6250);
    generateATree("trees/sub3.6.12-tree.in", treeLikeBamboo, 12500);
    generateATree("trees/sub3.6.13-tree.in", treeRandom, 25000);
    generateATree("trees/sub3.6.14-tree.in", treeCaterpillar, 50000);
    generateATree("trees/sub3.6.15-tree.in", treeLikeBamboo, 100000);
    
    return 0;
}