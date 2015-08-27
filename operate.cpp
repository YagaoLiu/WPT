#include <iostream>
#include <string>
#include <vector>

using namespace std;

float maximum ( float * x, unsigned int m )
{
	float max = 0;
	for ( unsigned int i = 0; i < m; i++ )
		if ( max < x[i] )
			max = x[i];
	return max;
}

char getLetter ( float *x, unsigned int m )
{
	float max = maximum ( x, m );
	for ( unsigned int i = 0; i < m; i++ )
		if ( x[i] == max )
			return 'a' + i;
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


