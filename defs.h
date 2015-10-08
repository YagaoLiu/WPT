#include <string>
#include <vector>

using namespace std;

#define DNA		 "ACGT";

struct TSwith
{
	string	weighted_str_filename;
	string	solid_str_filename;
	string	output_filename;
	double	z;
	string	alphabet;
	int		mod;
};


#ifndef STRINGCOLOUR_H
#define STRINGCOLOUR_H
struct WStr 
{
	vector < int > str;
	vector < double > prob;
	vector < vector < double > > bpt;
	vector < unsigned int > WP;
	vector < unsigned int > BP;
	vector < double > FP;
	unsigned int ul;
	unsigned int lvp;
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
void parray ( WStr x, int m, double z, unsigned int * P );
unsigned int wptable ( double ** x, unsigned int n, unsigned int m, double z, unsigned int * WP );
double maximum ( double * x, unsigned int m );
unsigned int getLetter ( double * x, unsigned int m );
unsigned int compareBP ( double * x, double * y, unsigned int m );
unsigned int branchBP ( double * x, double * y, unsigned int m, vector < unsigned int > * branch );
unsigned int LCVE ( WStr x, unsigned int n, int m, double z, unsigned int lcve, unsigned int P, Factor *u, Factor *v );
unsigned int gextension ( WStr x, unsigned int n, int m, double z, Factor * u, Factor * v );
