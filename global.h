#define max(a,b) ((a) > (b)) ? (a) : (b)
#define min(a,b) ((a) < (b)) ? (a) : (b)

#include <string>
#include <vector>

using namespace std;

struct StringColour
{
	string colour;
	vector < unsigned int > stringxx;
	vector < unsigned int > BP;
	vector < unsigned int > WP;
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


