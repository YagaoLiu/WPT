#include <string>
#include <vector>

using namespace std;

struct StringColour
{
	string colour;
	vector < unsigned int > BP;
	vector < float > FP;
	vector < unsigned int > bpos;
};

struct Factor
{
	
		unsigned int start;
		unsigned int end;
		unsigned int l;
		vector < unsigned int > bpp;
		string bpset;
		float p;
};


