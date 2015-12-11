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

	int * xx	 = new int [m];
	double * pxx = new double [m];

	/* if not mod 0( only WPtable ), construct an integer string & a probability array for x */
	if ( m != 0 )
	{
		for ( unsigned int i = 0; i < m; i++ )
		{
			xx[i] =  alphabet.find ( x[i] ) + 1;
			pxx[i] = 1;
		}

	}

	/* seperate y into an integer string, a prob~ array for WGPs and a prob~ table for BPs */
	xy.ul = sigma + 1;
	int * yy	 = new int [n];
	double * pyy = new double [n];

	for ( unsigned int i = 0; i < n; i++ )
	{
		double max = maximum ( y[i], sigma );
		if ( max > 1 - 1/z )
		{
			yy[i]  = getLetter ( y[i], sigma ) + 1;
			pyy[i] = max;
		}
		else
		{
			vector < double > yi;
			yi.assign ( y[i], y[i] + sigma );

			yy[i]  = xy.ul;
			pyy[i] = 0;
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
	if ( xy.lvp != n )
	{
		xy.lvp --;
	}

	/* combine two string, according to the mod */
	switch ( mod )
	{
		case 0:
			/* the case we only need WP table */
			cout << "text length:" << n << endl;
			xy.str.assign ( yy, yy + n );
			xy.prob.assign ( pyy, pyy + n );
			break;
		case 1:
			/* the case solid string x is text and weighted string y is pattern */
			cout << "pattern length:" << m << "\ttext length:" << n << endl;
			xy.str.assign ( yy, yy + n );
			xy.str.insert ( xy.str.end(), xx, xx + m );
			xy.prob.assign ( pyy, pyy + n );
			xy.prob.insert ( xy.prob.end(), pxx, pxx + m );
			if ( xy.lvp == n )
				xy.lvp += m;
			break;
		case 2:
			/* the case solid string x is pattern and weighted string y is text */
			cout << "pattern length:" << m << "\ttext length:" << n << endl;
			xy.str.assign ( yy, yy + n );
			xy.str.insert ( xy.str.begin(), xx, xx + m );
			xy.prob.assign ( pyy, pyy + n );
			xy.prob.insert ( xy.prob.begin(), pxx, pxx + m );
			xy.lvp += m;
			break;
	}

	delete[] xx;
	delete[] pxx;
	delete[] yy;
	delete[] pyy;

	N = xy.str.size();

	vector < unsigned int > bpos;
	vector < unsigned int > wpos;

	bpos.reserve ( N );
	wpos.reserve ( N );

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
	xy.BP.reserve ( n );
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
	xy.WP.reserve ( n );
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
	xy.FP.reserve ( n );
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


