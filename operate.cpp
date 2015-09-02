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

unsigned int compareBP ( double * x, double * y, unsigned int m )
{
	for ( unsigned int i = 0; i < m; i++ )
		if ( x[i] > 0 && y[i] > 0 )
		{
			return i;
		}
	return m;
}

unsigned int branchBP ( double * x, double * y, unsigned int m, vector < unsigned int > *branch )
{
	unsigned int num_branch = 0;
	for ( int i = 0; i < m; i++ )
	{
		if ( x[i] > 0 && y[i] > 0 )
		{
			branch->push_back ( i );
			num_branch ++;
		}
	}
	return num_branch;
}


