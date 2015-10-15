#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>

#include "defs.h"
#include "global.h"

using namespace std;

int main (int argc, char **argv)
{
	TSwitch sw;
	string alphabet;
	unsigned int sigma;
	int mod;
	string wsfile;
	string ssfile;
	string output;
	double z;
	string x;				//solid string
	unsigned int m;			//length of x
	double ** y;			//weighted string
	unsigned int n;			//length of y
	WStr xystr;

	clock_t start;
	clock_t finish;

	unsigned int k;

	/* Decodes the arguments */
	k = decode_switches ( argc, argv, &sw );

	/* Check the arguments */
	if ( k < 7 )
	{
		usage();
		return 1;
	}
	else
	{
		if ( sw.alphabet.compare ( "DNA" ) == 0 )
		{
			alphabet = DNA;
			sigma = alphabet.size();
		}
		else
		{
			cout << "Error: Only support DNA alphabet up to now!" << endl;
			return 0;
		}

		mod = sw.mod;
		if ( sw.mod > 2 )
		{	
			cout << "Error: The model (-m) should be only '0', '1' or '2'!" << endl;
			return 0;
		}
		else
		{
			mod = sw.mod;
		}

		if ( sw.weighted_str_filename.size() == 0 )
		{
			cout << "Error: No weighted string input!" << endl;
			return 0;
		}
		else
			wsfile = sw.weighted_str_filename;

		if ( sw.solid_str_filename.size() == 0 && mod != 0 )
		{
			cout << "Error: Need one more solid string for matching!" << endl;
			return 0;
		}
		else
		{
			if ( mod != 0 )
				ssfile = sw.solid_str_filename;
		}

		if ( sw.output_filename.size() == 0 )
		{
			if ( mod == 0 )
				output = "WPTableReport";
			else
				output = "MatchingReport";		
		}
		else
		{
			output = sw.output_filename;
		}

		z = sw.z;
	}

	/* read input Weighted String */
	ifstream weighted ( wsfile );
	if ( weighted.fail() )
	{
		cout << "Error: Cannot open the weighted string file!" << endl;
		return 0;
	}
	else
	{
		vector < double > temparray;
		double temp;
		unsigned int column = alphabet.size();
		while ( !weighted.eof() )
		{
			weighted >> temp;
			temparray.push_back ( temp );
		}
		unsigned int row = temparray.size() / column;
		y = new double * [row];
		for ( unsigned int i = 0; i < row; i++ )
			y[i] = new double [column];

		for ( unsigned int i = 0; i < row; i++ )
		{
			for ( unsigned int j = 0; j < column; j++ )
			{
				y[i][j] = temparray[ j + i * column ];
			}
		}
		n = row;

		weighted.close();
	}


	/* read input Solid String */
	if ( mod != 0 )
	{
		ifstream solid ( ssfile );
		if ( solid.fail() )
		{
			cout << "Error: Cannot open the solid string file!" << endl;
			return 0;
		}
		else
		{
			getline ( solid, x );
			m = x.size();
		}
		solid.close();
	}

	if ( mod == 0 )
	{
		string empty;
		start = clock();
		if ( ! ( preparation ( empty, y, n, z, alphabet, mod ) ) )
		{
			return 0;
		}
		else
		{
			finish = clock();
			double passtime = (	double ) ( finish - start ) / CLOCKS_PER_SEC;
			cout << "preparation time is " << passtime << endl;
			unsigned int * WP = new unsigned int [n];
			wptable ( sigma, z, WP );
			/*print*/
			
			ofstream result ( output );
			result << "string length=" << n << "\t z=" << z << endl;
			result << "Prefix Table for this Weighted String: " << endl;
			int outrow = 0;
			int outcol = 0;
			do
			{
				result << WP[outrow * 20 + outcol] << ' ';
				outcol++;
				if ( outcol == 20 )
				{
					result << endl;
					outrow ++;
					outcol = 0;
				}
			}while ( outrow * 20 + outcol < n );
			result << endl;
			result.close();
		
		}
	}
	else
	{
		if ( ! ( preparation ( x, y, n, z, alphabet, mod ) ) )
		{
			return 0;
		}
		else
		{
			vector < unsigned int > Occ;
			unsigned int Occ_number;
			Occ_number = matching ( m, alphabet, z, &Occ );
			/*print result*/
			ofstream result ( output );
			result << "The number of occurrances is " << Occ_number << endl;
			result << "The positions of each occurrances:" << endl;
			for ( unsigned int i = 0; i < Occ_number; i++ )
				result << "Occur at position " << Occ[i] << endl;
			result.close();
		}
	}


	return 0;
}
