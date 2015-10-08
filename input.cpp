#include <string>
#include <cstring>
#include <getopt.h>

#include "defs.h"

static struct option long_options[] =
{
	{ "alphabet",			required_argument,	NULL,	'a'	},
	{ "mode",				required_argument,	NULL,	'd' },
	{ "solid-string",		required_argument,	NULL,	's' },
	{ "weighted-string",	required_argument,	NULL,	'w' },
	{ "output-file",		required_argument,	NULL,	'o' },
	{ "help",				0,					NULL,	'h' },
};

int decode_switches ( int argc, char * argv[], struct TSwitch * sw )
{
	int opt;

	/* initialisation */
	sw -> z						=	1;
	sw -> mod					=	-1;

	while ( ( opt = getopt_long ( argc, argv, "a:d:s:w:o:h", long_options, NULL ) ) != -1 )
	{
		switch ( opt )
		{
			case 'a':
				sw -> alphabet = optarg;



