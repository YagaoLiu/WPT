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
	string alphabet = DNA;
	unsigned int sigma = alphabet.size();
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
	ofstream result;

	clock_t start;
	clock_t finish;

	unsigned int k;

	/* Decodes the arguments */
	k = decode_switches ( argc, argv, &sw );

	/* Check the arguments */
	if ( k < 5 )
	{
		usage();
		return 1;
	}
	else
	{
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
		{
			wsfile = sw.weighted_str_filename;
		}

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
		double temp;
		long int num = 0;
		unsigned int column = alphabet.size();
		while ( !weighted.eof() )
		{
			weighted >> temp;
			num ++;
		}
		unsigned int row = num / column;
		y = new double * [row];
		for ( unsigned int i = 0; i < row; i++ )
			y[i] = new double [column];
		weighted.clear();
		weighted.seekg( 0, ios::beg );
		for ( unsigned int i = 0; i < row; i++ )
		{
			for ( unsigned int j = 0; j < column; j++ )
			{
				weighted >> y[i][j];
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
	
	start = clock();
	if ( mod == 0 )
	{
		string empty;
		if ( ! ( preparation ( empty, y, n, z, alphabet, mod ) ) )
		{
			return 0;
		}
		else
		{
			for ( unsigned int i = 0; i < n; i++ )
				delete[] y[i];
			delete[] y;
			unsigned int * WP = new unsigned int [n];
			wptable ( sigma, z, WP );
			
			finish = clock();
			double passtime = (	double ) ( finish - start ) / CLOCKS_PER_SEC;
			cout << "Elapsed time is " << passtime << endl;
#if 1			
			/*print*/
			result.open ( output );
			result << "Weighted Prefix Table:\n";
			for ( unsigned int i = 0; i < n; i++ )
			{
				result << WP[i] << '\n';
			}
			result.close();
#endif	
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
			for ( unsigned int i = 0; i < n; i++ )
				delete[] y[i];
			delete[] y;
	
			vector < unsigned int > Occ;
			unsigned int Occ_number;
			if ( mod == 1 )
				Occ_number = matching ( n, alphabet, z, &Occ );
			if ( mod == 2 )
				Occ_number = matching ( m, alphabet, z, &Occ );

			cout << "Number of Occurrances:" << Occ_number << endl;
	
			finish = clock();
			double passtime = (	double ) ( finish - start ) / CLOCKS_PER_SEC;
			cout << "Elapsed time is " << passtime << endl;
#if 1
			/*print result*/
			result.open ( output );
			result << "The positions of occurrances:\n";
			if ( Occ_number == 0 )
			{
				cout <<  ( Occ_number == 0 ) << endl;
				result << "No occurrance.\n";
			}
			else
			{
				for ( unsigned int i = 0; i < Occ_number; i++ )
					result << Occ[i] << '\n';
			}
			result.close();
#endif
			ofstream result;
			result.open ( "wpt.dat", ios::app );
			if ( mod == 1 )
				result << n << '\t' << passtime << '\n';
			if ( mod == 2 )
				result << m << '\t' << passtime << '\n';
			result.close();

		}
	}


	return 0;
}
