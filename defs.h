#include <string>
#include <vector>

using namespace std;

#ifndef STRINGCOLOUR_H
#define STRINGCOLOUR_H
struct StringColour
{
	string colour;
	vector < unsigned int > stringxx;
	vector < unsigned int > BP;
	vector < unsigned int > WP;
	vector < double > FP;
	vector < unsigned int > bpos;
};
#endif

#ifndef FACTOR_H
#define FACTOR_H 
struct Factor
{	
	unsigned int start;
	unsigned int end;
	unsigned int l;
	vector < unsigned int > bpp;
	vector < unsigned int > bpset;
	float p;
};
#endif

unsigned int colour ( double ** x, unsigned int n, unsigned int m, double z );
void parray ( double ** x, unsigned int n, unsigned int m, double z, unsigned int * P );
unsigned int wptable ( double ** x, unsigned int n, unsigned int m, double z, unsigned int * WP );
double maximum ( double * x, unsigned int m );
unsigned int getLetter ( double * x, unsigned int m );
unsigned int compareBP ( double * x, double * y, unsigned int m );
unsigned int branchBP ( double * x, double * y, unsigned int m, vector < unsigned int > * branch );
