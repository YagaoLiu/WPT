#include <iostream>
#include <string>
#include <vector>

#include "prefix.h"

using namespace std;

unsigned int prefix ( vector < unsigned int > x, unsigned int * pref )
{
	unsigned int n = x.size();		//get the length of string.
	
	pref[0] = n;

	unsigned int g = 0;
	unsigned int f;

	for ( unsigned int i = 1; i < n; i++ )
	{
		if ( i < g && pref[i - f] != g - i )
			pref[i] = pref[i - f] < ( g - i ) ? pref[i - f] : ( g - i );
		else
		{
			g = g > i ? g : i;
			f = i;
			while ( g < n && x[g] == x[g - f] )
				g = g + 1;
			pref[i] = g - f;
		}
	}

	return 1;
}

