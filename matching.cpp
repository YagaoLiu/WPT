#include <iostream>
#include <vector>

#include "defs.h"

using namespace std;

extern WStr xy;

unsigned int matching ( unsigned int n, string alphabet, double z, vector < unsigned int > * Occ )
{
	unsigned int Occ_number = 0;

	unsigned int N = xy.str.size();
	int sigma = alphabet.size();

	unsigned int * WP = new unsigned int [N];

	wptable ( sigma, z, WP );

	for ( unsigned int i = n; i < N; i++ )
	{
		if ( WP[i] >= n )
		{
			Occ->push_back ( i - n );
			Occ_number ++;
		}
	}

	return Occ_number;
}




