#include <iostream>
#include <vector>
#include <ctime>

#include "global.h"
#include "defs.h"
#include "util.h"

#include <sdsl/rmq_support.hpp>

using namespace sdsl;
using namespace std;

extern WStr xy;

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

void parray ( int m, double z, unsigned int * P )
{
	clock_t start, finish;
	start = clock();
	unsigned int n = xy.str.size();
	vector < int > tmpxx;
	tmpxx = xy.str;
	tmpxx.insert ( tmpxx.end(), 3, 0 );

	int * xx	= &tmpxx[0];
	int * SA	= new int [n+3];
	int * iSA	= new int [n];
	int * LCP	= new int [n];

	SA[n] = SA[n+1] = SA[n+2] = 0;
	clock_t SAstart, SAfin;
	
	/* computer SA, iSA, LCParray for xx */
	SAstart = clock();
	suffixArray ( xx, SA, n, xy.ul );
	SAfin = clock();

	double SAtime = ( double ) ( SAfin - SAstart ) / CLOCKS_PER_SEC;
	cout << "SAtime: " << SAtime << endl;

	for ( unsigned int i = 0; i < n; i++ )
		iSA[ SA[i] ] = i;

	clock_t LCPs, LCPe;
	LCPs = clock();
	LCParray ( xx, n, SA, iSA, LCP );
	LCPe = clock();

	double LCPtime = ( double ) ( LCPe - LCPs ) / CLOCKS_PER_SEC;
	cout << "LCPtime: " << LCPtime << endl;
	
	/* construct data structure RMQ */
	vector < int > v( n, 0 );
	for ( unsigned int i = 0; i < n; i++ )
		v[i] = LCP[i];

	rmq_succinct_sct<> rmq ( &v );

	P[0] = xy.lvp;

	clock_t Ps, Pe;
	Ps = clock();
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
				if ( xy.str[pos_u] > m || xy.str[pos_v] > m )
				{
					span = max ( ( xy.WP[pos_u] - pos_u ), ( xy.WP[pos_v] - pos_v ) );
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
	Pe = clock();

	double Ptime = ( double ) ( Pe - Ps ) / CLOCKS_PER_SEC;
	cout << Ptime << endl;

	delete [] SA;
	delete [] iSA;
	delete [] LCP;

	finish = clock();
	double passtime = (double) ( finish - start ) / CLOCKS_PER_SEC;
}

