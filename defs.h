#define max(a,b) ((a) > (b)) ? (a) : (b)
#define min(a,b) ((a) < (b)) ? (a) : (b)

#include <string>
#include <vector>

using namespace std;

unsigned int prefix ( vector < unsigned int > x, unsigned int * pref );
unsigned int parray ( double ** x, unsigned int n, unsigned int m, double z, unsigned int * P );
unsigned int LCVE ( double ** x, unsigned int n, unsigned int m, double z, unsigned int lcve, unsigned int P, Factor * u, Factor * v );
unsigned int wptable ( double ** x, unsigned int n, unsigned int m, double z, unsigned int * WP );

struct StringColour
{
	string colour;
	vector < unsigned int > stringxx;
	vector < unsigned int > BP;
	vector < double > FP;
	vector < unsigned int > bpos;
};

struct Factor
{	
		unsigned int start;
		unsigned int end;
		unsigned int l;
		vector < unsigned int > bpp;
		vector < unsigned int > bpset;
		float p;
};


