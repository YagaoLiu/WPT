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
		if ( ! ( colour( x, n, m, z ) ) )
		{
			cout << "Error: no Black Position in String" << endl;
			return 0;
		}
		else
		{
			unsigned int * WP = new unsigned int [n];
			
			wptable ( x, n, m, z, WP );

			finish = clock();
			double duration = ( double ) ( finish - start )/ CLOCKS_PER_SEC;
			
			cout << "Elapsed time for processing is " << duration << " seconds" << endl;
			cout << "The result is writing into the file 'WPTableReport'. It will take some time." << endl;
			
			/* write the report file */
			ofstream result( "WPTableReport" );
			result << "string length=" << n << "		z=" << z << endl;
			result << "Number of	Black Position: " << colx.bpos.size() << "	White & Grey position: " << n - colx.bpos.size() << endl;
			result << "Elapsed time for processing is " << duration << " seconds" << endl << endl;
			result << "Prefix Table for this Weighted String: " << endl;
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
			result.close();

			for ( unsigned int i = 0; i < n; i++ )
				delete[] x[i];
			delete[] x;

			delete[] WP;

		}
	}
	return 0;
}
