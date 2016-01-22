#include <iostream>
#include <string>
#include <cstring>
#include <getopt.h>

#include "defs.h"

static struct option long_options[] =
{
	{ "mode",					required_argument,	NULL,	'm' },
	{ "solid-string",			required_argument,	NULL,	's' },
	{ "weighted-string",		required_argument,	NULL,	'w' },
	{ "output-file",			required_argument,	NULL,	'o' },
	{ "cumulative-threshold",	required_argument,	NULL,	'z'	},
	{ "help",					0,					NULL,	'h' },
};

int decode_switches ( int argc, char * argv[], struct TSwitch * sw )
{
	int opt;
	char *ep;
	double val;
	int args;

	/* initialisation */
	sw -> z						=	1;
	sw -> mod					=	-1;

	args = 0;

	while ( ( opt = getopt_long ( argc, argv, "m:s:w:o:z:h", long_options, NULL ) ) != -1 )
	{
		switch ( opt )
		{
			case 'a':
				sw -> alphabet = optarg;
				args ++;
				break;
			case 'm':
				val = strtol ( optarg, &ep, 10 );
				if ( optarg == ep )
				{
					return 0;
				}
				sw -> mod = val;
				args ++;
				break;
			case 's':
				sw -> solid_str_filename = optarg;
				args ++;
				break;
			case 'w':
				sw -> weighted_str_filename = optarg;
				args ++;
				break;
			case 'o':
				sw -> output_filename = optarg;
				args ++;
				break;
			case 'z':
				val = strtod ( optarg, &ep );
				if ( optarg == ep )
				{
					return 0;
				}
				sw -> z = val;
				args ++;
				break;
			case 'h':
				return 0;
		}
	}

	if ( args < 4 )
	{
		usage();
		exit ( 1 );
	}
	else
		return ( optind );
}

void usage ( void )
{
	cout << "Usage: wpt <options>"  << endl;
	cout << "Standard (Mandatory):" << endl;
	cout << "	-m, --mode\t<int>\tchoose the model for program.\n '0' for Prefix Table of one Weighted String,\n '1' for Weighted Pattern and Solid Text matching,\n '2' for Solid Pattern and Weighted Text matching." << endl;
	cout << "	-w, --weighted-string\t<str>\tFilename for Weight String." << endl;
	cout << "	-s, --solid-string\t<str>\tFilename for Solid String." << endl;
	cout << "	-o, --output-file\t<str>\tFilename for result output." << endl;
	cout << "	-z, --cumulative-threshold\t<dbl>\tcumulative weight threshold."<<endl;
}






