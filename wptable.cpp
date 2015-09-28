#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctime>

#include "parray.h"
#include "lcve.h"
#include "wptable.h"
#include "operate.h"

using namespace std;

extern StringColour colx;

struct BPmap
{
	vector < unsigned int > BPset;
	unsigned int endposition;
};

unsigned int PrefixMap ( double ** x, unsigned int n, unsigned int m, double z, double p, unsigned int start, vector < unsigned int > BPstring, vector < BPmap > * PrefixBPmaps )
{
	BPmap root;
	unsigned int nextBP;
	double newp = p;

	if ( start != 0 )
		root.BPset = BPstring;

	if ( colx.colour[start] == 'b' )
	{
		for ( unsigned int j = 0; j < m; j++ )
		{
			newp = p * x[start][j];
			if ( newp > 1/z )
			{
				root.BPset.push_back ( j );
				PrefixMap ( x, n, m, z, p, start + 1, root.BPset, PrefixBPmaps );
			}
		}
	}
	else
	{
		nextBP = colx.BP[start];
		p = p * colx.FP[nextBP - 1];
		if ( p < 1/z )
		{
			/* cannot extent to next BP, porb invalid */
			for ( unsigned int i = start; i < nextBP; i++ )
			{
				newp *= maximum ( x[i], m );
				if ( newp < 1/z )
				{
					root.endposition = i - 1;
					PrefixBPmaps -> push_back ( root );
					return 1;
				}
			}
		}
		else if ( nextBP != n )
		{
			/* can extent to next BP */
			for ( unsigned int j = 0; j < m; j++ )
			{
				newp = p * x[nextBP][j];
				if ( newp > 1/z )
				{
					root.BPset.push_back ( j );
					PrefixMap ( x, n, m, z, newp, nextBP + 1, root.BPset, PrefixBPmaps );
				}
			}
		}
	}

	if ( PrefixBPmaps -> size() == 0 )
		return 0;
	else
		return 1;
}

unsigned int wptable ( double ** x, unsigned int n, unsigned int m, double z , unsigned int * WP )
{
	/* This function is used to compute the Weighted Prefix Table */
	clock_t Pstart, Pend, WPstart, WPend, allstart, allend;
	clock_t * Lstart = new clock_t [n];
	clock_t * Lend = new clock_t [n];
	clock_t * Gstart = new clock_t [n];
	clock_t * Gend = new clock_t [n];

	/* compute the longest valid preifx for WP[0] */
	WP[0] = 0;
	double p_wp0 = 1;
	for ( unsigned int i = 0; i < n && p_wp0 >= 1/z; i++ )
	{
		p_wp0 = p_wp0 *  maximum ( x[i], m );
		WP[0] ++;
	}
	WP[0] = WP[0] - 1;

	/* make maps for the black position set and the stop position */
	map < vector < unsigned int >, unsigned int > STable;									//the map for the
	map < vector < unsigned int >, unsigned int > :: iterator it_u = STable.begin();

	vector < BPmap > PrefixBPmaps;
	vector < unsigned int > BPstring;
	allstart = clock();
	PrefixMap ( x, n, m, z, 1, 0, BPstring, &PrefixBPmaps );

	for ( unsigned int i = 0; i < PrefixBPmaps.size(); i++ )
	{
		STable.insert ( pair < vector < unsigned int >, unsigned int > ( PrefixBPmaps[i].BPset, PrefixBPmaps[i].endposition ) );
	}

	allend = clock();
	unsigned int * Parray = new unsigned int [ WP[0] ];
	Pstart = clock();
	parray ( x, n, m , z, Parray );
	Pend = clock();
	double Ppass = ( double ) ( Pend - Pstart ) / CLOCKS_PER_SEC;
	cout << "P takes: " << Ppass << "s.\n";

	unsigned int g = 0;
	unsigned int f;

	WPstart = clock();
	for ( unsigned int i = 1; i < n; i++ )
	{
		/* construct two factor u & v, to computer the lcve between stirng x and x[i....] */
		Factor u,v;
		u.start = 0;
		u.end = 0;
		u.p = 1;
		u.l = 0;
		v.start = i;
		v.end = i;
		v.p = 1;
		v.l = 0;
		int flag = 1;
		unsigned int lcve_wp = 0;
		if ( i < g )
		{
			unsigned int limit = min ( P[i - f], g - i );
			Lstart[i] = clock();
			lcve_wp = LCVE ( x, n, m, z, lcve_wp, limit, &u, &v );
			Lend[i] = clock();
			/* check the probability fail caused by grey position, and get the longest valid extension */
			unsigned int lve_u = lcve_wp;
			unsigned int lve_v = lcve_wp;
			Gstart[i] = clock();
			if ( u.p < 1/z )
			{
				it_u = STable.find ( u.bpset );
				if ( it_u != STable.end() )
				{
					lve_u = lcve_wp - ( u.end - it_u->second );
					u.end = it_u->second;
				}
				else
				{
					cout << "Error : BPset of u not found in Table!" << endl;
				}
			}
			if ( v.p < 1/z )
			{
				unsigned int j = v.end - 1;
				for ( j; j > v.bpp[v.l - 1]; j-- )
				{
					v.p = v.p / ( maximum ( x[j], m ) );
					if ( v.p > 1/z )
					{
						lve_v = j - v.start;
						v.end = j - 1;
						break;
					}
				}
			}
			Gend[i] = clock();

			if ( lve_u != lcve_wp || lve_v != lcve_wp )
				lcve_wp = min ( lve_u, lve_v );
		
			if ( lcve_wp < g - i )
			{
				flag = 0;
				WP[i] = lcve_wp;
			}
			else
				flag = 1;
		}
		if ( flag )
		{
			f = i;
			g = max( g, i ); 
			while ( g < n )
			{
				

				
			WP[i] = g - f;
		}
	}
	WPend = clock();
	double WPpass = ( double ) ( WPend - WPstart ) / CLOCKS_PER_SEC;
	cout << "WP takes: " << WPpass << "s.\n";

	double Lpass = 0, Gpass = 0;
	for ( unsigned int i = 0; i < n; i++ )
	{
		Lpass += ( double ) ( Lend[i] - Lstart[i] ) / CLOCKS_PER_SEC;
		Gpass += ( double ) ( Gend[i] - Gstart[i] ) / CLOCKS_PER_SEC;
	}

	cout << "LCVE takes: " << Lpass << "s.\n";
	cout << "Grey check: " << Gpass << "s.\n";


	double allpass = ( double ) ( allend - allstart ) / CLOCKS_PER_SEC;
	cout << "All takes: " << allpass << "s.\n";

	delete [] Parray;

	return 1;
}

