#include <iostream>
#include <vector>

#include "global.h"
#include "defs.h"
#include "util.h"

#include <sdsl/rmq_support.hpp>

using namespace sdsl;
using namespace std;

extern StringColour colx;

void suffixArray ( int * s, int * SA, int n, int K );

unsigned int LCParray ( int  * text, unsigned int n, int * SA, int * ISA, int * LCP )
{										
	int i=0, j=0;

	LCP[0] = 0;
	for ( i = 0; i < n; i++ ) // compute LCP[ISA[i]]
		if ( ISA[i] != 0 ) 
		{
			if ( i == 0) j = 0;
			else j = (LCP[ISA[i-1]] >= 2) ? LCP[ISA[i-1]]-1 : 0;
			while ( text[i+j] == text[SA[ISA[i]-1]+j] )
				j++;
			LCP[ISA[i]] = j;
		}

	return ( 1 );
}

void parray ( double ** x, unsigned int n, unsigned int m, double z, unsigned int * P )
{
	int * xx	= new int [n+3];
	int * SA	= new int [n+3];
	int * iSA	= new int [n];
	int * LCP	= new int [n];

	xx[n] = xx[n+1] = xx[n+2] = SA[n] = SA[n+1] = SA[n+2] = 0;

	/* construct string xx with unique letter for BP */
	int ul = m + 1;
	for ( unsigned int i = 0; i < n; i++ )
	{
		if ( colx.colour[i] != 'b' )
			xx[i] = getLetter ( x[i], m ) + 1;
		else
		{
			xx[i] = ul;
			ul++;
		}
	}

	/* computer SA, iSA, LCParray for xx */
	suffixArray ( xx, SA, n, ul );

	for ( unsigned int i = 0; i < n; i++ )
		iSA[ SA[i] ] = i;

	LCParray ( xx, n, SA, iSA, LCP );
	
	/* construct data structure RMQ */
	vector < int > v( n, 0 );
	for ( unsigned int i = 0; i < n; i++ )
		v[i] = LCP[i];

	rmq_succinct_sct<> rmq ( &v );

	/* compute the longest valid prefix */
	unsigned int lvp = 0;
	double pp = 1;
	for ( unsigned int i = 0; i < n && pp >= 1/z; i++, lvp++ )
	{
		pp *= maximum ( x[i], m );
	}
	lvp --;

	P[0] = lvp;

	for ( unsigned int i = 1; i < n; i++ )
	{
		unsigned int pos_u = 0;
		unsigned int pos_v = i;
		unsigned int flag  = 1;
		unsigned int span  = 0;

		P[i] = 0;

		do
		{
			if ( pos_v >= n )
				flag = 0;
			else if ( pos_u >= lvp )
				flag = 0;
			else
			{
				if ( colx.colour[pos_u] == 'b' || colx.colour[pos_v] == 'b' )
				{
					span = min ( ( colx.WP[pos_u] - pos_u ), ( colx.WP[pos_v] - pos_v ) );
					P[i] += span;
					pos_u += span;
					pos_v += span;
				}
				else
				{
					unsigned int l = min ( iSA[pos_u], iSA[pos_v] );
					unsigned int r = max ( iSA[pos_u], iSA[pos_v] );
					span = LCP[ rmq ( l + 1, r ) ];
					if ( span == 0 )
						flag = 0;
					else
					{
						pos_u += span;
						pos_v += span;
						P[i] += span;
					}
				}
				if ( P[i] > lvp )
					P[i] = lvp;
			}
		}while ( flag );
	}



	delete [] xx;
	delete [] SA;
	delete [] iSA;
	delete [] LCP;
}

