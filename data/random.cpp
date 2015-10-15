#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <stdlib.h>

using namespace std;

int main ()
{
	srand ( time ( NULL ) );
	char dat[10][20];
	strcpy ( dat[0], "10k1" );
	strcpy ( dat[1], "10k2" );
	strcpy ( dat[2], "10k3" );
	strcpy ( dat[3], "10k4" );
	strcpy ( dat[4], "10k5" );
	strcpy ( dat[5], "10k6" );
	strcpy ( dat[6], "10k7" );
	strcpy ( dat[7], "10k8" );
	strcpy ( dat[8], "10k9" );
	strcpy ( dat[9], "10k0" );
	for ( int aaaaa = 1; aaaaa < 11; aaaaa++ )
	{	
		int n = 10000;
		cout << n<< endl;
		double z = 256;
		double ** c;
		c = new double * [n];
		for ( int k = 0; k < n; k++ )
			c[k] = new double [4];
		int b = n * 0.3;
		int w = n - b;

		int num_w = 0;							//counter for w&g position
		int num_b = 0;							//counter for black position
		int i = 0;
		while( i < n )
		{
			int colour = rand() % 2;
			if ( colour == 0 && num_w < w )				//we add a white or grey position
			{
				double max = ( ( double ) ( rand() % 1000 ) ) / ( z * 1000.0 );
				int pos = rand() % 4;
				c[i][pos] = 1 - max;
				int a[3];
				double sum = 0.0;
				for ( int j = 0; j < 3; j++ )
				{
					a[j] = rand() % 1000;
					sum += a[j];
				}
				double b[3];
				for ( int j = 0; j < 3; j++ )
				{
					b[j] = ( a[j] / sum ) * max;
				}

				int q = 0;
				for ( int j = 0; j < 4; j++ )
				{
					if ( j != pos )
					{
						c[i][j] = b[q];
						q++;
					}
				}
				num_w ++;
				i++;
			}
			if ( colour == 1 && num_b < b )				//we add a black position
			{
				double max = 0.0;
				do
				{
					int a[4];
					double sum = 0.0;
					for ( int j = 0; j < 4; j++ )
					{
						a[j] = rand() % 1000;
						sum += a[j];
					}
					double b[4];
					for ( int j = 0; j < 4; j++ )
					{
						b[j] = a[j] / sum;
						if ( max < b[j] )
							max = b[j];
					}
					for ( int j = 0; j < 4; j++ )
						c[i][j] = b[j];
				}while ( max > 1 - 1/z );
				num_b ++;	
				i++;
			}
		}
		ofstream data ( dat[aaaaa-1] );
		for ( int j = 0; j < n; j++ )
		{
			for ( int i = 0; i < 4; i++ )
				data << c[j][i] << '\t' << '\t';
			data << '\n';
		}
		data.close();

		cout << " ------------------------------"<<endl;
	}
	return 0;
}