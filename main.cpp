#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//#include <stdio.h>

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

		if ( ! (colour ( x, n, m, z ) ) )
		{
			cout << "Error: no Black Position in String" << endl;
			return 0;
		}
		else
		{
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

			parray ( x, n, m, z, P );
			cout << "P:";
			for ( unsigned int i = 0; i < n; i++ )
				cout << P[i] <<' ';
			cout <<endl;

			unsigned int * WP = new unsigned int [n];
			wptable ( x, n, m, z, WP );
			cout << "WP:";
			for ( int i = 0; i < n; i++ )
				cout << WP[i] << ' ';
			cout << endl;

			for ( unsigned int i = 0; i < n; i++ )
				delete[] x[i];
			delete[] x;

			delete[] P;
			delete[] WP;
		}
	}

	return 0;
}
