// Source: https://usaco.guide/general/io

#include <bits/stdc++.h>
using namespace std;
#define ll long long int
long long int Piedras_Magicas(int N, int A, int B, int Q, vector<int> a ) 
{
	ll n=N;
	ll ah=A,b=B,q=Q;
	vector<ll>qu(q+1);
	for(int k=1;k<=q;k++)
	{
		qu[k]=a[k-1];
	}
	ll dp[q+3][n+1];
	for(int k=0;k<=q;k++)
	{
		for(int i=0;i<=n;i++)
		{
			dp[k][i]=-1;
		}
	}
	qu[0]=b;
	dp[0][ah]=0;
	for(ll k=1;k<=q;k++)
	{
		for(ll i=1;i<=n;i++)
		{
			if(dp[k-1][i]!=-1)
			{
				ll as=i,bs=qu[k-1];
				if(dp[k][as]==-1)
					dp[k][as]=dp[k-1][i]+abs(bs-qu[k]);
				dp[k][as]=min(dp[k][as],dp[k-1][i]+abs(bs-qu[k]));
				if(dp[k][bs]==-1)
					dp[k][bs]=dp[k-1][i]+abs(as-qu[k]);
				dp[k][bs]=min(dp[k][bs],dp[k-1][i]+abs(as-qu[k]));
			}
		}
	}
	ll minimo=1e18;
	for(ll k=1;k<=n;k++)
	{
		if(dp[q][k]!=-1)
			minimo=min(minimo,dp[q][k]);
	}
	return minimo;
}
