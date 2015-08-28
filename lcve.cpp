#include <iostream>
#include <string>
#include <vector>

#include "global.h"
#include "operate.h"
#include "colour.h"

using namespace std;

extern StringColour colx;

unsigned int LCVE ( float ** x, unsigned int n, unsigned int m, float z, unsigned int lcve, unsigned int P, Factor * u, Factor * v )
{
	unsigned char letter;
	unsigned int compare;
	unsigned int span;
	unsigned int old_uend;
	unsigned int old_vend;

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
			
			Factor u_branch[num_branch];
			Factor v_branch[num_branch];

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
					u_branch[i] = u;
					v_branch[i] = v;

					/* add the new black position in factor u & v */
					u_bracnh[i].bpp.push_back ( u->end );
					u_branch[i].bpset.push_back ( letter );
					u_branch[i].l ++;

					v_branch[i].bpp.push_back ( v->end );
					v_branch[i].bpset.push_back ( letter );
					v_branch[i].l ++;

					if ( ( colx.BP[u->end] - u->end ) < ( colx.BP[v->end] - v->end ) )
						span = colx.BP[u->end] - u->end;
					else
						span = colx.BP[v->end] - v->end;

					if ( span > 1 )
					{
						u_branch[i].p = u->p * x[u->end][letter] * colx.FP[ new_u[i].end - 1];
						v_branch[i].p = v->p * x[v->end][letter] * colx.FP[ new_v[i].end - 1];
					}
					else
					{
						u_branch[i].p = u->p * x[u->end][letter];
						v_branch[i].p = v->p * x[v->end][letter];
					}

					lcve = lcve + span;
					lcve_branch[i] = LCVE( x, n, m, z, lcve, P, &u_branch[i], &v_branch[i] );
				}
			}

			/* find the longest extension branch */
			unsigned int choose_lcve = 0;
			unsigned int choose_branch;

			for ( unsigned int i = 0; i < num_branch; i++ )
			{
				if ( choose_lcve < lcve_branch[i] )
				{
					choose_lcve = lcve_branch[i];
					choose_branch = i;
				}
			}

			if ( lcve < choose_lcve )
			{
				lcve = choose_lcve;

				/* update the two factor u & v */
				u->end = u_branch[choose_branch].end;
				u->bpp.insert ( u->bpp.end(), u_branch[choose_branch].begin(), u_branch[choose_branch].end() );
				u->bpset += u_branch[choose_branch].bpset;
				u->l += u_branch[choose_branch].l;
				u->p = u_branch[choose_branch].p;

				v->end = v_branch[choose_branch].end;
				v->bpp.insert ( v->bpp.end(), u_branch[choose_branch].begin(), u_branch[choose_branch].end() );
				v->bpset += v_branch[choose_branch].bpset;
				v->l += v_branch[choose_branch].l;
				v->p = v_branch[choose_branch].p;
			}
		}
		else
		{
			compare = compareBP ( x[u->end], x[v->end], m ) - 1;
			letter = 'a' + compare;
			float pcheck_u = u->p * x[u->end][compare];
			float pcheck_v = v->p * x[v->end][compare];
			if ( pcheck_u < 1/z || pcheck_v < 1/z )
				break;
			else
			{
				if ( colx.colour[u->end] != 'b' && colx.colour[v->end] != 'b' )
				{
					/* skip to the next black position */
					if ( ( colx.BP[u->end] - u->end ) < ( colx.BP[v->end] - v->end ) )
						span = colx.BP[u->end] - u->end;
					else
						span = colx.BP[v->end] - v->end;
					if ( span + lcve > P )
						span = p - lcve;

					old_uend = u->end;
					old_vend = v->end;
					u->end += span;
					v->end += span;

					/* update the probability */
					if ( span > 1 )
					{
						u->p = u->p * x[old_uend][compare] * colx.FP[u->end - 1] / colx.FP[old_uend];
						v->p = v->p * x[old_vend][compare] * colx.FP[u->end - 1] / colx.FP[old_vend];
					}
					else
					{
						u->p = u->p * x[old_uend][compare];
						v->p = v->p * x[old_vend][compare];
					}

					lcve = lcve + span;
				}
				else if ( colx.colour[u->end] == 'b' && colx.colour[v->end] != 'b' )
				{
					/* add the new black position to factor u */
					u->bpp.push_back ( u->end );
					u->bpset.push_back ( letter );
				
					/* skip to the next black position */
					if ( ( colx.BP[u->end] - u->end ) < ( colx.BP[v->end] - v->end ) )
						span = colx.BP[u->end] - u->end;
					else
						span = colx.BP[v->end] - v->end;
					if ( span + lcve > P )
						span = p - lcve;

					old_uend = u->end;
					old_vend = v->end;
					u->end += span;
					v->end += span;

					/* update the probability */
					if ( span > 1 )
					{
						u->p = u->p * x[old_uend][compare] * colx.FP[u->end - 1];
						v->p = v->p * x[old_vend][compare] * colx.FP[v->end - 1] / colx.FP[old_vend];
					}
					else
					{
						u->p = u->p * x[old_uend][compare];
						v->p = v->p * x[old_vend][compare];
					}

					lcve = lcve + span;
				}
				else if ( colx.colour[u->end] != 'b' && colx.colour[v->end] == 'b' )
				{
					/* add the new black position to factor v */
					v->bpp.push_back ( v->end );
					v->bpset.push_back ( letter );

					/* skip to the next black position */
					if ( ( colx.BP[u->end] - u->end ) < ( colx.BP[v->end] - v->end ) )
						span = colx.BP[u->end] - u->end;
					else
						span = colx.BP[v->end] - v->end;
					if ( span + lcve > P )
						span = p - lcve;

					old_uend = u->end;
					old_vend = v->end;
					u->end += span;
					v->end += span;

					/* update the probability */
					if ( span > 1 )
					{
						u->p = u->p * x[old_uend][compare] * colx.FP[u->end - 1] / colx.FP[old_uend];
						v->p = v->p * x[old_vend][compare] * colx.FP[v->end - 1];
					}
					else
					{
						u->p = u->p * x[old_uend][compare];
						v->p = v->p * x[old_vend][compare];
					}

					lcve = lcve + span;
				}
			}
		}
	}
	
	return lcve;

			
