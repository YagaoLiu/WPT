#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>

#include "prefix.h"
#include "colour.h"
#include "global.h"
#include "parray.h"
#include "wptable.h"

using namespace std;

extern StringColour colx;

int main (int argc, char **argv)
{
	if ( argc == 3 )
	{
		clock_t start, finish;
	
		unsigned int n = 0;
		unsigned int m = 4;
		double z = 1.0;
		sscanf ( argv[2], "%lf", &z );

		ifstream ReadFile;
		string tmp;
		ReadFile.open ( argv[1] );
		if ( ReadFile.fail() )
			cout << "Cannot open the file" << endl;
		else
		{
			while ( getline ( ReadFile, tmp, '\n' ) )
				n++;
		}
		ReadFile.close();

		ifstream in ( argv[1] );
		double ** x;
		x = new double * [n];
		for ( unsigned int i = 0; i < n; i++ )
			x[i] = new double [m];

		for ( unsigned int i = 0; i < n; i++ )
		{
			for ( unsigned int j = 0; j < m; j++ )
			{
				in >> x[i][j];
			}
		}
		in.close();

		start = clock();	
		if ( ! (colour ( x, n, m, z ) ) )
		{
			cout << "Error: no Black Position in String" << endl;
			return 0;
		}
		else
		{
						unsigned int * P;
						P = new unsigned int [n];
start = clock();
						parray ( x, n, m, z, P );
						finish = clock();
			double pass = ( double ) ( finish - start )/ CLOCKS_PER_SEC;
			cout << "P takes: " << pass << endl;
			unsigned int * WP = new unsigned int [n];
			
			wptable ( x, n, m, z, WP );

			finish = clock();
			double duration = ( double ) ( finish - start )/ CLOCKS_PER_SEC;
/*			ofstream result( "WPTableReport" );
			result << "string length=" << n << "		z=" << z << endl;
			result << "Number of	Black Position: " << colx.bpos.size() << "	White & Grey position: " << n - colx.bpos.size() << endl;
			result << "Elapsed time for processing is " << duration << " seconds" << endl;
			int row = 0;
			int column = 0;
			do
			{
				result << WP[row * 20 + column] << ' ';
				column++;
				if ( column == 20 )
				{
					result << endl;
					row++;
					column = 0;
				}
			}while ( row * 20 + column < n );

			result << endl;

			cout << "It takes " << duration << " seconds." << endl;
*/
			for ( unsigned int i = 0; i < n; i++ )
				delete[] x[i];
			delete[] x;

			delete[] P;
			delete[] WP;

		}
	}
	return 0;
}
