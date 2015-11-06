#include <string>
#include <vector>

using namespace std;

#define DNA		 "ACGT";

#ifndef TSWITCH_H
#define TSWITCH_H
struct TSwitch
{
	string	weighted_str_filename;
	string	solid_str_filename;
	string	output_filename;
	double	z;
	string	alphabet;
	int		mod;
};
#endif

#ifndef WSTR_H
#define WSTR_H
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

int decode_switches ( int argc, char * argv[], struct TSwitch * sw );
void usage ( void );
unsigned int preparation ( string x, double ** y, unsigned int n, double z, string alphabet, int mod );
//unsigned int colour ( double ** x, unsigned int n, unsigned int m, double z );
unsigned int matching ( unsigned int n, string alphabet, double z, vector < unsigned int > * Occ );
void parray ( int m, double z, unsigned int * P );
unsigned int wptable ( int m, double z, unsigned int * WP );
double maximum ( double * x, unsigned int m );
unsigned int getLetter ( double * x, unsigned int m );
//unsigned int compareBP ( double * x, double * y, unsigned int m );
//unsigned int branchBP ( double * x, double * y, unsigned int m, vector < unsigned int > * branch );
unsigned int LCVE ( unsigned int n, int m, double z, unsigned int lcve, unsigned int P, Factor *u, Factor *v );
unsigned int gextension ( unsigned int n, int m, double z, Factor * u, Factor * v, int *k );
