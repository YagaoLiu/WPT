#include <iostream>
#include <string>
#include <vector>

#include "global.h"
#include "operate.h"
#include "colour.h"

using namespace std;

extern StringColour colx;

unsigned int lcve ( float ** x, unsigned int n, unsigned int m, float z, unsigned int lcve, unsigned int P, Factor * u, Factor * v )
{
	unsigned char letter;
	unsigned int compare;
	unsigned int span;
	unsigned int old_ustart;
	unsigned int old_vstart;

	while ( compareBP ( x[u->end], x[v->end], m ) )
	{

		if ( lcve == P )
			return lcve;
		if ( colx.colour[u->end] == 'b' && colx.colour[v->end] == 'b' )
		{
			unsigned int num_branch;
			vector < unsigned int > branch;
			vector < unsigned int > lcve_branch;
			num_branch = branchBP ( x[u->end], x[v->end], m, &branch );
			
			Factor new_u[num_branch];
			Factor new_v[num_branch];

			for ( int i = 0; i < num_branch; i++ )
			{
				compare = branch[i];
				letter = 'a' + compare;
				float pcheck_u = u->p * x[u->end][compare];
				float pcheck_v = v->p * x[v->end][compare];

				if ( pcheck_u < 1/z || pcheck_v < 1/z )
					lcve_branch.push_back ( lcve );
				else
				{
					/* construct two new factors for u and v */
					new_u[i] = u;
					new_v[i] = v;

					/* add the new black position in factor u & v */
					new_u[i].bpp.push_back ( u->end );
					new_u[i].bpset.push_back ( letter );
					new_u
				



			
