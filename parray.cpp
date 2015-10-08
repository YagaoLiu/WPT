#include <iostream>
#include <vector>

#include "global.h"
#include "defs.h"
#include "util.h"

#include <sdsl/rmq_support.hpp>

using namespace sdsl;
using namespace std;

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

void parray ( WStr x, int m, double z, unsigned int * P )
{
	unsigned int n = x.str.size();

	int * xx	= &x.str[0];
	int * SA	= new int [n+3];
	int * iSA	= new int [n];
	int * LCP	= new int [n];

	xx[n] = xx[n+1] = xx[n+2] = SA[n] = SA[n+1] = SA[n+2] = 0;
	
	/* computer SA, iSA, LCParray for xx */
	suffixArray ( xx, SA, n, x.ul );

	for ( unsigned int i = 0; i < n; i++ )
		iSA[ SA[i] ] = i;

	LCParray ( xx, n, SA, iSA, LCP );
	
	/* construct data structure RMQ */
	vector < int > v( n, 0 );
	for ( unsigned int i = 0; i < n; i++ )
		v[i] = LCP[i];

	rmq_succinct_sct<> rmq ( &v );

	P[0] = x.lvp;

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
			else if ( pos_u >= P[0] )
				flag = 0;
			else
			{
				if ( x.str[pos_u] > m || x.str[pos_v] > m )
				{
					span = max ( ( x.WP[pos_u] - pos_u ), ( x.WP[pos_v] - pos_v ) );
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
			}
		}while ( flag );
	}

	delete [] SA;
	delete [] iSA;
	delete [] LCP;
}

