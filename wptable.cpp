#include <iostream>
#include <string>
#include <vector>

#include <global.h>
#include <parray.h>
#include <lcve.h>
#include <wptable.h>

using namespace std;

extern StringColour colx;

int PrefixMap ( float ** x, unsigned int n, unsigned int m, float z, unsigned int start, float p, string BPset, map < string, int > * STable )
{
	
	unsigned int nextBP = colx.BP[start];
	unsigned int endposition;
	int newp = p;
	if ( colx.colour[start] == 'b' )
	{
		for ( int i = 0; i < m; i++ )
		{
			newp = newp * s[start][i];
			if ( newp > 1/z )
			{
				string newBPset = BPset;
				newBPset.push_back ( 'a' + i );
				PrefixMap ( x, ,n , m, z, nextBP + 1, newp, newBPset, STable );
			}
		}
	}
	if ( colx.colour[nextBP - 1] != 'b' )
		newp = colx.FP[nextBP - 1] * p;
	
	if ( newp < 1/z )
	{
		for ( int i = start; i < nextBP; i++ )
		{
			p = p * maximum ( x[i], m );
			if ( p < 1/z )
				endposition = i - 1;
		}
		STable->insert ( pair < string, int > ( BPset, endposition ) );
		return 1;
	}
	else
	{
		for ( int i = 0l i < m; i++ )
		{
			newp = newp * x[nextBP][i];
			if ( newp > 1/z )
			{
				string NewBPset = BPset;
				newBPset.push_back ( 'a' + i );
				PrefixMap ( x, n, m, z, nextBP + 1, newp, newBPset, STable );
			}
		}
	}
	return 1;
}


unsigned int wptable ( float ** x, unsigned int n, unsigned int m, float z , unsigned int * WP )
{
	/* This function is used to compute the Weighted Prefix Table */

	/* compute the longest valid preifx for WP[0] */
	WP[0] = 0;
	float p_wp0 = 1;
	for ( unsigned int i = 0; i < n && p_wp0 >= 1/z; i++ )
	{
		p_wp0 = p_wp0 *  maximum ( x[i], m );
		WP[0] ++;
	}
	WP[0] = WP[0] - 1;

	/* make maps for the black position set and the stop position */
	map < string, int > STable;
	map < string, int > :: iterator it_u = STable.begin();
	map < string, int > FTable;
	map < string, int > :: iterator it_v = FTable.begin();

	string BPset;
	PrefixMap ( x, n, m, z, 0, 1, BPset, &STable );		//compute all the maps from the beginning of string x

	unsigned int * Parray = new unsigned int [n];
	parray ( x, n, m , z, Parray );

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
		lcve_wp = LCVE ( x, n, m, z, lcve_wp, Parray, &u, &v );

		/* check the probability fail caused by grey position, and get the longest valid extension */
		unsigned int lve_u = lcve_wp;
		unsigned int lve_v = lcve_wp;
		if ( colx.colour[i] == 'b' )
			FTable.clear();			//if we get to the next black position, we clear the map table for the previous one.
		if ( u.p < 1/z )
		{
			it_u = STable.find ( u.bpset );
			if ( it_u != STbale.end() )
			{
				lve_u = lcve_wp - ( u.end - it_u->second );
				u.end = it_u->second;
			}
		}
		if ( v.p < 1/z )
		{
			it_v = FTable.find ( v.bpset );
			if ( it_v != FTable.end() )
			{
				/* if we can find the bp set of factor v in the table, we get the lve directly */
				lve_v = lcve_wp - ( v.end - it_v->second );
				v.end = it_v->second;
			}
			else
			{
				/* if we cannot find the bp set in the table, we compute the lve from the end of v, go back one by one and check the probability. And then add it to the table */
				for ( unsigned int j = v.end; j > v.bpp[v.l - 1]; j-- )
				{
					v.p = v.p / ( maximum ( x[j]. m ) );
					if ( v.p > 1/z )
					{
						lve_v = lcve_wp - ( v.end - j ) - 1;
						v.end = j - 1;
						FTable.insert ( pair <string, int > ( v.bpset, v.end ) );
					}
				}
			}
		}

		if ( i < g && lcve_wp < g - i )
			WP[i] = lcve_wp;
		else
		{
			f = i;
			g = g > i ? g: i;
			while ( ( g < n ) && ( g - f <= lcve_wp ) && compareBP ( x[g], x[g-f], m ) )
				g ++;
			WP[i] = g - f;
		}
	}

	return 1;
}










