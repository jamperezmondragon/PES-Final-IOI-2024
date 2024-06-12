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
	ll dp[q+3][n+3][n+3];
	for(int k=0;k<=q;k++)
	{
		for(int i=0;i<=n;i++)
		{
			for(int j=0;j<=n;j++)
			{
				dp[k][i][j]=-1;
			}
		}
	}
	dp[0][ah][b]=0;
	for(ll k=1;k<=q;k++)
	{
		for(ll i=1;i<=n;i++)
		{
			for(ll j=1;j<=n;j++)
			{
				if(dp[k-1][i][j]!=-1)
				{
					if(dp[k][qu[k]][j]==-1)
						dp[k][qu[k]][j]=dp[k-1][i][j]+abs(i-qu[k]);
					dp[k][qu[k]][j]=min(dp[k][qu[k]][j],dp[k-1][i][j]+abs(i-qu[k]));
					if(dp[k][i][qu[k]]==-1)
						dp[k][i][qu[k]]=dp[k-1][i][j]+abs(j-qu[k]);
					dp[k][i][qu[k]]=min(dp[k-1][i][j]+abs(j-qu[k]),dp[k][i][qu[k]]);
				}
			}
		}
	}
	ll minimo=1e18;
	for(int k=1;k<=n;k++)
	{
		for(int i=1;i<=n;i++)
		{
			if(dp[q][k][i]!=-1)
				minimo=min(minimo,dp[q][k][i]);
		}
	}
	return minimo;
}
