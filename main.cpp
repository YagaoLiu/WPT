#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
		unsigned int n = 0;
		unsigned int m = 0;
		string argv2 = argv[2];
		if ( argv2 == "-d" )
			m = 4;

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
		cout << n << "	" << m << endl;
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

		for ( unsigned int i = 0; i < n; i++ )
		{
			for ( unsigned int j = 0; j < m; j++ )
			{
				cout << x[i][j] << ' ';
			}
			cout << endl;
		}
/*			double xx[10][4] = {
			{0.95,	0.05,	0,		0},
			{1,		0,		0,		0},
			{0.5,	0.5,	0,		0},
			{0,		0,		1,		0},
			{0.8,	0,		0.2,	0},
			{0,		0.05,	0,		0.95},
			{0.01,	0.01,	0.02,	0.95},
			{0,		0.75,	0.25,	0},
			{0,		0,		0.95,	0.05},
			{0.95,	0,		0,		0.05}
			};
			double *a[11];
			for ( int i = 0; i < 11; i++ )
			a[i] = xx[i];
*/		
		colour ( x, n, m, 16 );
		cout << "colour:" << colx.colour << endl;
		cout << "BP:";
		for ( int i = 0; i < n; i++ )
			cout << colx.BP[i]<<' ';
		cout << endl;
		cout << "FP:";
		for ( int i = 0; i < n; i++ )
			cout << colx.FP[i]<<' ';
		cout << endl;
		cout << "String:";
		for ( int i = 0; i < n; i++ )
			cout << colx.stringxx[i]<<' ';
		cout << endl;

		unsigned int * P;
		P = new unsigned int [n];

		parray ( x, n, m, 16, P );
		cout << "P:";
		for ( unsigned int i = 0; i < n; i++ )
			cout << P[i] <<' ';
		cout <<endl;

		unsigned int * WP = new unsigned int [n];
		wptable ( x, n, m, 16, WP );
		cout << "WP:";
		for ( int i = 0; i < n; i++ )
			cout << WP[i] << ' ';
		cout << endl;
	}

	return 0;
}
