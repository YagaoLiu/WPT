#include <iostream>
#include <string>
#include <vector>

#include "global.h"
#include "operate.h"
#include "colour.h"
#include "prefix.h"

using namespace std;

extern StringColour colx;

unsigned int parray ( double ** x, unsigned int n, unsigned int m, double z, unsigned int * P )
{
	/* compute the longest valid prefix of x */
	unsigned int lvp = 0;
	double pp = 1;
	for ( unsigned int i = 0; i < n; i++ )
	{
		pp *= maximum ( x[i], m );
		if ( pp >= 1/z )
			lvp++;
		else
			break;
	}

	vector < unsigned int > xx;
	unsigned int ul = m;
	unsigned int num_bp_lvp = 0;
	for ( unsigned int i = 0; i < lvp; i++ )
	{
		if ( colx.colour[i] != 'b' )
			xx.push_back ( getLetter( x[i], m ) );
		else
		{
			xx.push_back ( ul );
			ul ++;
			num_bp_lvp ++;
		}
	}

	if ( num_bp_lvp == 0 )
		cout << "Warning: No Black Position in Longest Valid Prefix!" << endl;

	/* construct the set of string : x'$xx */
	vector < unsigned int > :: iterator it_xx;
	it_xx = xx.begin();

	vector < vector < unsigned int > > xp;
	vector < vector < unsigned int > > fragment;			//save the fragments x' (the strings between two neighboor black position )
	vector < unsigned int > frag_start;						//save the start position of each fragment

	for ( unsigned int i = 0; i < num_bp_lvp + 1; i++ )
	{
			unsigned int frag_length;
			if ( i == 0 )
			{
				frag_length = colx.bpos[i];
				frag_start.push_back ( 0 );
			}
			else if ( i == num_bp_lvp )
			{
				frag_length = lvp - colx.bpos[i-1] - 1;
				if ( frag_length == 0 )
					frag_start.push_back ( colx.bpos[i-1] );
				else
					frag_start.push_back ( colx.bpos[i-1] + 1 );
			}
			else
			{
				frag_length = colx.bpos[i] - colx.bpos[i-1] - 1;
				frag_start.push_back ( colx.bpos[i-1] + 1 );
			}
			vector  < unsigned int > buffer;
			buffer.assign ( it_xx + frag_start[i], it_xx + frag_start[i] + frag_length );		//get x'
			fragment.push_back ( buffer );
	}
	
	/* remove the empty fragments */
	for ( unsigned int i = 0; i < fragment.size(); i++ )
	{
		if ( fragment[i].size() == 0 )
		{
			fragment.erase ( fragment.begin() + i );
			frag_start.erase ( frag_start.begin() + i );
		}
	}
	
	/* construct x'$xx */
	for ( unsigned int i = 0; i < fragment.size(); i++ )
	{
		vector < unsigned int > temp;		
		temp = fragment[i];										//we have a temporary array same as x'[i]
		temp.push_back ( ul );									//we add the unique letter ($) to the end of the temp array
		temp.insert ( temp.end(), xx.begin(), xx.end() );		//we add xx to the end of the temp array, so now it is x'$xx
		xp.push_back ( temp );									//we add this array to the set of string x'$xx
	}

	/* compute prefix table for each x'$xx */
	unsigned int ** ptset = NULL;
	ptset = new unsigned int * [ xp.size() ];
	for ( unsigned int i = 0; i < xp.size(); i++ )
	{
		unsigned int * tempPT = NULL;
		tempPT = new unsigned int [ xp[i].size() ];
		ptset[i] = new unsigned int [ lvp ];
		if ( prefix ( xp[i], tempPT ) )
		{
			for ( unsigned int j = 0; j < lvp; j++ )
				ptset[i][j] = tempPT[j + fragment[i].size() + 1];		//just save part of the prefix table, from the beginning of string xx
		}
		delete[] tempPT;
	}

	P[0] = lvp;

	for ( unsigned int i = 1; i < lvp; i++ )
		P[i] = 0;

	for ( unsigned int j = 1; j < lvp; j++ )
	{
		for ( unsigned int i = 0; i < lvp - j; i++ )
		{
			unsigned int pu = i;
			unsigned int pv = j + i;

			if ( colx.colour[pu] == 'b' || colx.colour[pv] == 'b' )
				P[j] ++;
			else
			{
				int pi = 0;
				if ( findpi ( pu, frag_start ) >= 0 )
				{
					pi = findpi ( pu, frag_start );
					if ( ptset[pi][pv] == 0 )
						break;
					P[j] += ptset[pi][pv];
					i = i + ptset[pi][pv] - 1;
				}
				else
				{
					pi = findpi ( pv, frag_start );
					if ( ptset[pi][pu] == 0 )
						break;
					P[j] += ptset[pi][pu];
					i = i + ptset[pi][pu] - 1;
				}
				if ( colx.colour[i + 1] != 'b' && colx.colour[j + i + 1] != 'b' )
					break;
			}
		}
	}

/*
	for ( unsigned int i = 1; i < lvp; i++ )
	{
		unsigned int pos_u = 0;
		unsigned int pos_v = i;
		unsigned int flag = 1;
	
		do 
		{
			if ( pos_u > lvp )
			{
				P[i] = lvp;
				flag = 0;
			}
			else if ( pos_v >= n )
			{
				flag = 0;
			}
			else
			{
				if ( colx.colour[pos_u] == 'b' || colx.colour[pos_v] == 'b' )
				{
					unsigned int span;
					span = min ( ( colx.WP[pos_u] - pos_u ) , ( colx.WP[pos_v] - pos_v ) );
					P[i] += span;
					pos_u += span;
					pos_v += span;
				}
				else
				{
					int pi = findpi ( pos_u, frag_start );
					if ( pi >= 0 )
					{
						if ( ptset[pi][pos_v] == 0 )
							flag = 0;
						else
						{
							P[i] = P[i] + ptset[pi][pos_v];
							pos_u += ptset[pi][pos_v];
							pos_v += ptset[pi][pos_v];
						}
					}
					else
					{
						if ( xx[pos_u] == xx[pos_v] )
						{
							P[i] ++;
							pos_u ++;
							pos_v ++;
						}
						else
							flag = 0;
					}
				}
			}
		}while ( flag );
	}
*/

	for ( unsigned int i = 0; i < xp.size(); i++ )
		delete[] ptset[i];
	delete[] ptset;

	return 1;
}



