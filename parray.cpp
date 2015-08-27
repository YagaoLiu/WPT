#include <iostream>
#include <string>

#include "global.h"
#include "operate.h"
#include "colour.h"
#include "prefix.h"

using namespace std;

extern StringColour colx;

unsigned int parray ( float ** x, unsigned int n, unsigned int m, float z, unsigned int * P )
{
	/* construct a string xx, using unique letter for each black position. */
	string xx;
	char ul = '!';
	for ( unsigned int i = 0; i < n; i++ )
	{
		if ( colx.colour[i] == 'b' )
		{
			xx.push_back ( ul );
			ul ++;
		}
		else
		{
			xx.push_back ( getLetter ( x[i], m ) );
		}
	}

	/* construct the set of string : x'$xx */
	vector < string > xp;
	vector < string > fragment;			//save the fragments x' (the strings between two neighboor black position )
	vector < unsigned int > frag_start;			//save the start position of each fragment
	for ( unsigned int i = 0; i < colx.bpos.size() + 1; i++ )
	{
			unsigned int frag_length;
			if ( i == 0 )
			{
				frag_length = colx.bpos[i];
				frag_start.push_back ( 0 );
			}
			else if ( i == colx.bpos.size() )
			{
				frag_length = xx.length() - colx.bpos[i-1] - 1;
				if ( frag_length == 0 )
					frag_start.push_back ( colx.bpos[i] );
				else
					frag_start.push_back ( colx.bpos[i-1] + 1 );
			}
			else
			{
				frag_length = colx.bpos[i] - colx.bpos[i-1] - 1;
				frag_start.push_back ( colx.bpos[i-1] + 1 );
			}
			string buffer;
			buffer.assign ( xx, frag_start[i], frag_length );		//get x'
			fragment.push_back ( buffer );
	}
	/* remove the empty fragments */
	for ( unsigned int i = 0; i < fragment.size(); i++ )
	{
		if ( fragment[i].length() == 0 )
		{
			fragment.erase ( fragment.begin() + i );
			frag_start.erase ( frag_start.begin() + i );
		}
	}
	
	/* construct x'$xx */
	for ( unsigned int i = 0; i < fragment.size(); i++ )
		xp.push_back ( fragment[i] + ul + xx );

	/* compute prefix table for each x'$xx */
	unsigned int ** ptset = NULL;

	ptset = new unsigned int * [ xp.size() ];
	for ( unsigned int i = 0; i < xp.size(); i++ )
	{
		unsigned int * tempPT = NULL;
		tempPT = new unsigned int [ xp[i].length() ];
		ptset[i] = new unsigned int [ n ];
		if ( prefix ( xp[i], tempPT ) )
		{
			for ( unsigned int j = 0; j < n; j++ )
				ptset[i][j] = tempPT[j + fragment[i].length() + 1];		//just save the prefix table from the beginning of string xx part.
		}
		delete[] tempPT;
	}
	
	unsigned int lvp = 0;
	float max_prob = 1;
	for ( unsigned int i = 0; i < n && max_prob >= 1/z ; i++ )
	{
		max_prob = max_prob * maximum ( x[i], m );
		if ( max_prob >= 1/z )
			lvp ++;
	}

	P[0] = lvp;

	for ( unsigned int j = 1; j < n; j++ )
	{
		for ( unsigned int i = 0; i < n - j; i++ )
		{
			unsigned int pos_u = i;
			unsigned int pos_v = j + i;
			if ( colx.colour[pos_u] == 'b' || colx.colour[pos_v] == 'b' )
				P[j] ++;
			else
			{
				int pi = 0;
				if ( findpi ( pos_u, frag_start ) >= 0 )
				{
					pi = findpi ( pos_u, frag_start );
					if ( ptset[pi][pos_v] == 0 )
						break;
					P[j] = P[j] + ptset[pi][pos_v];
					i = i + ptset[pi][pos_v] - 1;
				}
				else
				{
					pi = findpi ( pos_v, frag_start );
					if ( ptset[pi][pos_u] == 0 )
						break;
					P[j] = P[j] + ptset[pi][pos_u];
					i = i + ptset[pi][pos_u] - 1;
				}
				if ( colx.colour[i + 1] != 'b' && colx.colour[j + i + 1] != 'b' )
					break;
			}
		}
	}

	for ( unsigned int i = 0; i < xp.size(); i++ )
		delete[] ptset[i];
	delete[] ptset;

	return 1;
}



