#include <iostream>
#include <string>
#include <vector>

#include "colour.h"
#include "operate.h"
#include "global.h"

using namespace std;

StringColour colx;

unsigned int colour ( double ** x, unsigned int n, unsigned int m, double z )
{
	/**Colouring**/
	for ( unsigned int i = 0; i < n; i++ )
	{
		double max = maximum ( x[i], m );
		if ( max == 1 )
			colx.colour.push_back ('w');
		else if ( max > 1 - 1/z )
		{
			colx.colour.push_back ('g');
			for ( unsigned int j = 0; j < m; j++ )
				if ( x[i][j] < 1/z )
					x[i][j] = 0;
		}
		else
		{
			colx.colour.push_back ('b');
			colx.bpos.push_back ( i );
			for ( unsigned int j = 0; j < m; j++ )
				if ( x[i][j] < 1/z )
					x[i][j] = 0;
		}
	}
	
	if ( colx.bpos.size() == 0 )
	{
		return 0;
	}

	/**Computing BP array**/
	unsigned int k = 0;
	if ( colx.bpos.size() > 0 )
	{
		for ( unsigned int i = 0; i < n; i++ )
		{
			if ( i < colx.bpos[k] )
				colx.BP.push_back ( colx.bpos[k] );
			else
			{
				k ++;
				if ( k == colx.bpos.size() )
				{
					colx.BP.push_back( n );
					k = colx.bpos.size() - 1;
				}
				else
					colx.BP.push_back( colx.bpos[k] );
			}
		}
	}
	
	/**Computing FP array**/
	double fp = 1;
	for ( unsigned int i = 0; i < n; i++ )
	{
		if ( colx.colour[i] == 'b' )
		{
			colx.FP.push_back ( 0 );
			fp = 1;
		}
		else
		{
			fp = fp * maximum ( x[i], m );
			colx.FP.push_back ( fp );
		}
	}
	
	return 1;
}

