#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctime>

#include "defs.h"
#include "global.h"

using namespace std;

struct BPmap
{
	vector < unsigned int > BPset;
	unsigned int endposition;
};

extern WStr xy;
#if 0
unsigned int PrefixMap ( unsigned int n, int m, double z, double p, unsigned int start, vector < unsigned int > BPstring, vector < BPmap > * PrefixBPmaps )
{
	BPmap root;
	unsigned int nextBP;
	double newp = p;

	if ( start != 0 )
		root.BPset = BPstring;

	if ( xy.str[start] > m )
	{
		for ( unsigned int j = 0; j < m; j++ )
		{
			newp = p * xy.bpt[ xy.str[start] - m - 1 ][j];
			if ( newp > 1/z )
			{
				root.BPset.push_back ( j );
				PrefixMap ( n, m, z, p, start + 1, root.BPset, PrefixBPmaps );
			}
		}
	}
	else
	{
		nextBP = xy.BP[start];
		p = p * xy.FP[nextBP - 1];
		if ( p < 1/z )
		{
			/* cannot extent to next BP, probability invalid */
			for ( unsigned int i = start; i < nextBP; i++ )
			{
				newp *= xy.prob[i];
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
				newp = p * xy.bpt[ xy.str[nextBP] - m - 1 ][j];
				if ( newp > 1/z )
				{
					root.BPset.push_back ( j );
					PrefixMap ( n, m, z, newp, nextBP + 1, root.BPset, PrefixBPmaps );
				}
			}
		}
	}

	if ( PrefixBPmaps -> size() == 0 )
		return 0;
	else
		return 1;
}
#endif
unsigned int wptable ( int m, double z , unsigned int * WP )
{
	/* This function is used to compute the Weighted Prefix Table */
	unsigned int n = xy.str.size();

	/* compute P array */
	unsigned int * Parray = new unsigned int [ n ];
	parray ( m, z, Parray );
	
	/* WP[0] is the longest valid prefix */
	WP[0] = xy.lvp; 
#if 0
	/* make maps for the black position set and the stop position */
	map < vector < unsigned int >, unsigned int > STable;					
	map < vector < unsigned int >, unsigned int > :: iterator it_u = STable.begin();

	vector < BPmap > PrefixBPmaps;
	vector < unsigned int > BPstring;
	if ( PrefixMap ( n, m, z, 1, 0, BPstring, &PrefixBPmaps ) )
	{
		for ( unsigned int i = 0; i < PrefixBPmaps.size(); i++ )
		{
			STable.insert ( pair < vector < unsigned int >, unsigned int > ( PrefixBPmaps[i].BPset, PrefixBPmaps[i].endposition ) );
		}
	}
#endif
	/* compute WP table */
	unsigned int g = 0;
	unsigned int f;
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
		lcve_wp = LCVE ( n, m, z, lcve_wp, Parray[i], &u, &v );
		/* check the probability fail caused by grey position, and get the longest valid extension */
		unsigned int lve_u = lcve_wp;
		unsigned int lve_v = lcve_wp;
#if 0
		if ( u.p < 1/z )
		{
			it_u = STable.find ( u.bpset );
			if ( it_u != STable.end() )
			{
				lve_u = lcve_wp - ( u.end - it_u->second );
				u.end = it_u->second;
			}
		}
		if ( v.p < 1/z )
		{
			unsigned int j = v.end - 1;
			for ( j; j > v.bpp[v.l - 1]; j-- )
			{
				v.p = v.p / xy.prob[j];
				if ( v.p > 1/z )
				{
					lve_v = j - v.start;
					v.end = j - 1;
					break;
				}
			}
		}

		if ( lve_u != lcve_wp || lve_v != lcve_wp )
			lcve_wp = min ( lve_u, lve_v );
#endif	
		WP[i] = lcve_wp;
	}

	delete [] Parray;

	return 1;
}

