#include <iostream>
#include <vector>
#include <string>

#include "defs.h"

using namespace std;

WStr xy;

unsigned int preparation ( string x, double ** y, unsigned int n, double z, string alphabet, int mod )
{
	int sigma = alphabet.size();			
	unsigned int m = x.size();
	unsigned int N = m + n;					

	vector < int > xx;
	vector < double > pxx;
	/* if not mod 0( only WPtable ), construct an integer string & a probability array for x */
	if ( m != 0 )
	{
		for ( unsigned int i = 0; i < m; i++ )
		{
			xx.push_back ( alphabet.find ( x[i] ) + 1 );
			pxx.push_back ( 1 );
		}

	}

	/* seperate y into an integer string, a prob~ array for WGPs and a prob~ table for BPs */
	xy.ul = sigma + 1;

	for ( unsigned int i = 0; i < n; i++ )
	{
		double max = maximum ( y[i], sigma );
		if ( max > 1 - 1/z )
		{
			xy.str.push_back ( getLetter ( y[i], sigma ) + 1 );
			xy.prob.push_back ( max );
		}
		else
		{
			vector < double > yi;
			yi.assign ( y[i], y[i] + sigma );

			xy.str.push_back ( xy.ul );
			xy.prob.push_back ( 0 );
			xy.bpt.push_back ( yi );
			xy.ul ++;
		}
	}

	/* compute the longest valid prefix of weighted string y */
	double pp = 1;
	xy.lvp = 0;
	for ( unsigned int i = 0; i < n && pp >= 1/z; i++, xy.lvp++ )
	{
		pp *= maximum ( y[i], sigma );
	}
	xy.lvp --;

	/* combine two string, according to the mod */
	switch ( mod )
	{
		case 0:
			/* the case we only need WP table */
			break;
		case 1:
			/* the case solid string x is pattern and weighted string y is text */
			xy.str.insert ( xy.str.begin(), xx.begin(), xx.end() );
			xy.prob.insert ( xy.prob.begin(), pxx.begin(), pxx.end () );
			xy.lvp += m;
			break;
		case 2:
			/* the case solid string x is text and weighted string y is pattern */
			xy.str.insert ( xy.str.end(), xx.begin(), xx.end() );
			xy.prob.insert ( xy.prob.end(), pxx.begin(), pxx.end() );
			if ( xy.lvp == n )
				xy.lvp += m;
			break;
	}

	N = xy.str.size();

	vector < unsigned int > bpos;
	vector < unsigned int > wpos;

	for ( unsigned int i = 0; i < N; i++ )
	{
		if ( xy.str[i] > sigma )
		{
			bpos.push_back ( i );
		}
		else 
		{
			wpos.push_back ( i );
		}
	}

	if ( bpos.size() == 0 )
	{
		cout << "No Black Positions in The Weighted String" << endl;
		return 0;
	}

	/* Computing BP array */
	unsigned int k = 0;
	if ( bpos.size() > 0 )
	{
		for ( unsigned int i = 0; i < N; i++ )
		{
			if ( i < bpos[k] )
				xy.BP.push_back ( bpos[k] );
			else
			{
				k ++;
				if ( k == bpos.size() )
				{
					xy.BP.push_back ( N );
					k = bpos.size() - 1;
				}
				else
					xy.BP.push_back ( bpos[k] );
			}
		}
	}

	/* Computing WP array */
	k = 0;
	for ( unsigned int i = 0; i < N; i++ )
	{
		if ( i < wpos[k] )
			xy.WP.push_back ( wpos[k] );
		else
		{
			k ++;
			if ( k == wpos.size() )
			{
				xy.WP.push_back ( N );
				k = wpos.size() - 1;
			}
			else
				xy.WP.push_back ( wpos[k] );
		}
	}

	/* Computing FP array */
	double fp = 1;
	for ( unsigned int i = 0; i < N; i++ )
	{
		if ( xy.str[i] > sigma )
		{
			xy.FP.push_back ( 0 );
			fp = 1;
		}
		else
		{
			fp *= xy.prob[i];
			xy.FP.push_back ( fp );
		}
	}

	return 1;
}


