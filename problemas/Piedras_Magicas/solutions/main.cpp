#include<bits/stdc++.h>

using namespace std;

int N, M, A, B, X[200009];
long long ansMi, ansPl, ans, lzy[800009], mi[800009], pl[800009];

void Build (int nod, int st, int dr, int pos)
{
    lzy[nod] = 0;
    if (st == dr)
    {
        int dp = st - pos;
        if (dp < 0) dp = -dp;
        pl[nod] = dp + st, mi[nod] = dp - st;
        return ;
    }
    int mij = (st + dr) >> 1, f1 = nod << 1, f2 = f1 | 1;
    Build (f1, st, mij, pos);
    Build (f2, mij + 1, dr, pos);
    pl[nod] = min (pl[f1], pl[f2]);
    mi[nod] = min (mi[f1], mi[f2]);
}

void split (int nod, int f1, int f2)
{
    if (lzy[nod] == 0) return ;
    lzy[f1] += lzy[nod], lzy[f2] += lzy[nod];
    pl[f1] += lzy[nod], pl[f2] += lzy[nod];
    mi[f1] += lzy[nod], mi[f2] += lzy[nod];
    lzy[nod] = 0;
}

void Update (int nod, int st, int dr, int x, int y, int val)
{
    if (x <= st && dr <= y)
    {
        lzy[nod] += val, mi[nod] += val, pl[nod] += val;
        return ;
    }
    int mij = (st + dr) >> 1, f1 = nod << 1, f2 = f1 | 1;
    split (nod, f1, f2);
    if (x <= mij) Update (f1, st, mij, x, y, val);
    if (mij < y) Update (f2, mij + 1, dr, x, y, val);
    pl[nod] = min (pl[f1], pl[f2]);
    mi[nod] = min (mi[f1], mi[f2]);
}

void FixVal (int nod, int st, int dr, int pos, long long val)
{
    if (st == dr)
    {
        long long dp = pl[nod] - st;
        if (dp < val) return ;
        dp = val;
        pl[nod] = dp + st, mi[nod] = dp - st;
        return ;
    }
    int mij = (st + dr) >> 1, f1 = nod << 1, f2 = f1 | 1;
    split (nod, f1, f2);
    if (pos <= mij) FixVal (f1, st, mij, pos, val);
    else FixVal (f2, mij + 1, dr, pos, val);
    pl[nod] = min (pl[f1], pl[f2]);
    mi[nod] = min (mi[f1], mi[f2]);
}

void Query (int nod, int st, int dr, int x, int y)
{
    if (x <= st && dr <= y)
    {
        if (x == st) ansMi = mi[nod], ansPl = pl[nod];
        else ansMi = min (ansMi, mi[nod]), ansPl = min (ansPl, pl[nod]);
        return ;
    }
    int mij = (st + dr) >> 1, f1 = nod << 1, f2 = f1 | 1;
    split (nod, f1, f2);
    if (x <= mij) Query (f1, st, mij, x, y);
    if (mij < y) Query (f2, mij + 1, dr, x, y);
}

void getAns (int nod, int st, int dr)
{
    if (st == dr)
    {
        long long curr = pl[nod] - st;
        //printf ("%lld ", curr);
        if (curr < ans)
            ans = curr;
        return ;
    }
    int mij = (st + dr) >> 1, f1 = nod << 1, f2 = f1 | 1;
    split (nod, f1, f2);
    getAns (f1, st, mij);
    getAns (f2, mij + 1, dr);
}

void solve ()
{
    //getAns (1, 1, N), printf ("\n");
    for (int i=1; i<=M; i++)
    {
        int df = X[i] - X[i - 1];
        if (df < 0) df = -df;

        Query (1, 1, N, 1, X[i]);
        long long curr = ansMi + X[i];
        Query (1, 1, N, X[i], N);
        curr = min (curr, ansPl - X[i]);

        Update (1, 1, N, 1, N, df);
        FixVal (1, 1, N, X[i - 1], curr);
//        getAns (1, 1, N), printf ("\n");
        //getAns (1, 1, N), printf ("\n");
    }
    //exit (0);
    getAns (1, 1, N);
    //printf ("\n");
}

int main ()
{
//freopen ("input", "r", stdin);
//freopen ("output", "w", stdout);
int te;
scanf("%d",&te);
while(te--)
{
	scanf ("%d %d %d %d", &N, &M, &A, &B), ans = 1LL << 60;
	for (int i=1; i<=M; i++)
		scanf ("%d", &X[i]);
	X[0] = A, Build (1, 1, N, B);
	solve ();
	X[0] = B, Build (1, 1, N, A);
	solve ();
	printf ("%lld\n", ans);
	for(int k=0;k<=N*4;k++)
	{
		lzy[k]=0,mi[k]=0,pl[k]=0;
		if(k<=N)
        {
            X[k]=0;
        }
	}
}
return 0;
}
