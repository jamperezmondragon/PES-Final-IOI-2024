#include "alumnos.h"
#include <bits/stdc++.h>
using namespace std;
int Reto_Sebas(int N) {
   if ((n / 2) % 2) {
		return -1;
	}
    if (n <= 60) {
		for (int i = 0; i < n / 2; i++)
			if (valor(i) == valor(i+n/2))
            {
				return i;
			}
		return -1;
	}
	int i = 0, j = n / 2, rep = 20;
	int a = valor(i);
	int b = valor(j);
	if (a == b) {
		return a;
	}
	int f = a < b;
	while (rep--) {
		int m = (i+j)/2;
		int a = valor(m);
		int b = valor(m+n/2);

		if (a == b) {
			return m;
		}

		if ((a < b) == f) i = m;
		else j = m;
	}
	return -1;
}
