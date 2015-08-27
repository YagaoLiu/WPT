#include <iostream>
#include <string>
#include <vector>

#include "prefix.h"
#include "colour.h"
#include "global.h"
#include "parray.h"

using namespace std;

extern StringColour colx;

unsigned char test ( char a )
{
	a = a + 1;
	if ( a == 'b' )
		return 0;
	else
		return a;
}

int main ()
{
	float xx[15][4] = {
		{0,0,1,0}, 
		{0.5, 0.5, 0, 0}, 
		{0,0,1,0},
		{0,0,0.5,0.5},
		{0.5,0,0,0.5},
		{0,0,1,0},
		{1,0,0,0},
		{0,0,1,0},
		{0,1,0,0},
		{0,0,1,0},
		{0,0,0.5,0.5},
		{0,0.5,0,0.5},
		{0,0,1,0},
		{0.5,0,0,0.5},
		{0,0.99,0,0.01}
	};
	float *a[15];
	for ( int i = 0; i < 15; i++ )
		a[i] = xx[i];
	colour ( a, 15, 4, 64 );
	cout << colx.colour << endl;
	for ( int i = 0; i < 15; i++ )
		cout << colx.BP[i]<<' ';
	cout << endl;
	for ( int i = 0; i < 15; i++ )
		cout << colx.FP[i]<<' ';
	cout << endl;

	unsigned int * P;
	P = new unsigned int [15];

	parray ( a, 15, 4, 64, P );

	Factor u,v;
	u.start = 0;
	u.end = 5;
	u.l = 2;
	u.bpp.push_back ( 1 );
	u.bpp.push_back ( 3 );
	u.bpset.assign ( "ab" );
	u.p = 1;
	v = u;
	v.bpset.push_back ( 'a' );

	cout << v.bpset << endl;
	cout << u.bpset << endl;

	unsigned char letter;

	if ( test ( 'f' ) )
		letter = test ( 'f' );
	else
		letter = 'q';
	cout << letter << endl;
	
	string s1 = "aaba";
	unsigned char fff = 'b';
	s1.push_back ( fff );
	cout << s1 << endl;
	return 0;
}
