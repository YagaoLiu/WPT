#include <iostream>
#include <vector>
#include <string>

#include "defs.h"

using namespace std;

WStr xystr;

unsigned int matching ( unsigned char * x, unsigned int m, double ** y, unsigned int n, double z, string alphabet, int mod )
{
	int sigma = alphabet.size();
	vector < int > xx;
	for ( unsigned int i = 0; i < m; i++ )
	{
		xx.push_back ( alphabet.find ( x[i] ) + 1 );
	}

	vector < double > pxx ( m, 1 );

	xystr.ul = sigma + 1;

	for ( unsigned int i = 0; i < n; i++ )
	{
		double max = maximum ( y[i], sigma );
		if ( max > 1 - 1/z )
		{
			xystr.str.push_back ( getLetter ( y[i], sigma ) + 1 );
			xystr.prob.push_back ( max );
		}
		else
		{
			vector < double > yi;
			yi.assign ( y[i], y[i] + sigma );

			xystr.str.push_back ( xystr.ul );
			xystr.prob.push_back ( 0 );
			xystr.bpt.push_back ( yi );
			xystr.ul ++;
		}
	}

	double pp = 1;
	for ( unsigned int i = 0; i < n && pp >= 1/z; i++, xystr.lvp++ )
	{
		pp *= maximum ( y[i], sigma );
	}
	xystr.lvp --;

	switch ( mod )
	{
		case 1:
			/* the case solid string x is pattern and weighted string y is text */
			xystr.str.insert ( xystr.str.begin(), xx.begin(), xx.end() );
			xystr.prob.insert ( xystr.prob.begin(), pxx.begin(), pxx.end () );
			xystr.lvp += m;
			break;
		case 2:
			/* the case solid string x is text and weighted string y is pattern */
			xystr.str.insert ( xystr.str.end(), xx.begin(), xx.end() );
			xystr.prob.insert ( xystr.prob.end(), pxx.begin(), pxx.end() );
			if ( xystr.lvp == n )
				xystr.lvp += m;
			break;
	}

	unsigned int N = xystr.str.size();

	vector < unsigned int > bpos;
	vector < unsigned int > wpos;

	for ( unsigned int i = 0; i < N; i++ )
	{
		if ( xystr.str[i] > sigma )
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
				xystr.BP.push_back ( bpos[k] );
			else
			{
				k ++;
				if ( k == bpos.size() )
				{
					xystr.BP.push_back ( N );
					k = bpos.size() - 1;
				}
				else
					xystr.BP.push_back ( bpos[k] );
			}
		}
	}

	/* Computing WP array */
	k = 0;
	for ( unsigned int i = 0; i < N; i++ )
	{
		if ( i < wpos[k] )
			xystr.WP.push_back ( wpos[k] );
		else
		{
			k ++;
			if ( k == wpos.size() )
			{
				xystr.WP.push_back ( N );
				k = wpos.size() - 1;
			}
			else
				xystr.WP.push_back ( wpos[k] );
		}
	}

	/* Computing FP array */
	double fp = 1;
	for ( unsigned int i = 0; i < N; i++ )
	{
		if ( xystr.str[i] > sigma )
		{
			xystr.FP.push_back ( 0 );
			fp = 1;
		}
		else
		{
			fp *= xystr.prob[i];
			xystr.FP.push_back ( fp );
		}
	}

	unsigned int num_match = 0;
	return num_match;
}




			





