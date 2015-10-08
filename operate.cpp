#include <iostream>
#include <string>
#include <vector>

using namespace std;

double maximum ( double * x, unsigned int m )
{
	double max = 0;
	for ( unsigned int i = 0; i < m; i++ )
		if ( max < x[i] )
			max = x[i];
	return max;
}

unsigned int getLetter ( double *x, unsigned int m )
{
	double max = maximum ( x, m );
	for ( unsigned int i = 0; i < m; i++ )
		if ( x[i] == max )
			return i;
}

int findpi ( unsigned int a, vector < unsigned int > sp )
{
	int pi = 0;
	for ( unsigned int i = 0; i < sp.size(); i++ )
	{
		if ( sp[i] == a )
		{
			pi = i;
			return pi;
		}
	}

	return -1;
}

