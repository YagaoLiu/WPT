#include <iostream>
#include <string>
#include <vector>

#include "defs.h"
#include "global.h"
#include "lcve.h"

using namespace std;

extern StringColour colx;

unsigned int LCVE ( double ** x, unsigned int n, unsigned int m, double z, unsigned int lcve, unsigned int P, Factor * u, Factor * v )
{
	unsigned int letter;
	unsigned int span;
	unsigned int old_uend;
	unsigned int old_vend;
	if ( v->end == n )
		return lcve;
	while ( compareBP ( x[u->end], x[v->end], m ) < m )
	{
		if ( lcve >= P )
			return lcve;
		if ( v->end == n - 1 )
		{
			for ( unsigned int i = 0; i < m; i++ )
			{
				double pcheck_u = u->p * x[u->end][i];
				double pcheck_v = v->p * x[v->end][i];
				if ( pcheck_u > 1/z && pcheck_v > 1/z )
				{
					lcve ++;
					return lcve;
				}
			}
			return lcve;
		}
		if ( colx.colour[u->end] == 'b' && colx.colour[v->end] == 'b' )
		{
			unsigned int num_branch;
			vector < unsigned int > branch;
			vector < unsigned int > lcve_branch;
			num_branch = branchBP ( x[u->end], x[v->end], m, &branch );
			
			Factor u_branch[num_branch];
			Factor v_branch[num_branch];

			for ( unsigned int i = 0; i < num_branch; i++ )
			{
				letter = branch[i];
				double pcheck_u = u->p * x[u->end][letter];
				double pcheck_v = v->p * x[v->end][letter];

				if ( pcheck_u < 1/z || pcheck_v < 1/z )
					lcve_branch.push_back ( lcve );
				else
				{
					/* construct two new factors for u and v */
					u_branch[i] = *u;
					v_branch[i] = *v;

					/* add the new black position in factor u & v */
					u_branch[i].bpp.push_back ( u->end );
					u_branch[i].bpset.push_back ( letter );
					u_branch[i].l ++;

					v_branch[i].bpp.push_back ( v->end );
					v_branch[i].bpset.push_back ( letter );
					v_branch[i].l ++;

					if ( ( colx.BP[u->end] - u->end ) < ( colx.BP[v->end] - v->end ) )
						span = colx.BP[u->end] - u->end;
					else
						span = colx.BP[v->end] - v->end;

					if ( span + lcve > P )
						span = P - lcve;

					u_branch[i].end = u->end + span;
					v_branch[i].end = v->end + span;

					if ( span > 1 )
					{
						u_branch[i].p = u->p * x[u->end][letter] * colx.FP[ u_branch[i].end - 1];
						v_branch[i].p = v->p * x[v->end][letter] * colx.FP[ v_branch[i].end - 1];
					}
					else
					{
						u_branch[i].p = u->p * x[u->end][letter];
						v_branch[i].p = v->p * x[v->end][letter];
					}

					lcve_branch.push_back ( lcve + span );
					lcve_branch[i] = ( LCVE( x, n, m, z, lcve_branch[i], P, &u_branch[i], &v_branch[i] ) );
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
				u->bpp.insert ( u->bpp.end(), u_branch[choose_branch].bpp.begin(), u_branch[choose_branch].bpp.end() );
				u->bpset.insert ( u->bpset.end(), u_branch[choose_branch].bpset.begin(), u_branch[choose_branch].bpset.end() );
				u->l += u_branch[choose_branch].l;
				u->p = u_branch[choose_branch].p;

				v->end = v_branch[choose_branch].end;
				v->bpp.insert ( v->bpp.end(), u_branch[choose_branch].bpp.begin(), u_branch[choose_branch].bpp.end() );
				v->bpset.insert ( v->bpset.end(), v_branch[choose_branch].bpset.begin(), v_branch[choose_branch].bpset.end() );
				v->l += v_branch[choose_branch].l;
				v->p = v_branch[choose_branch].p;
			}
			return lcve;
		}
		else
		{
			letter = compareBP ( x[u->end], x[v->end], m );
			float pcheck_u = u->p * x[u->end][letter];
			float pcheck_v = v->p * x[v->end][letter];
			if ( pcheck_u < 1/z || pcheck_v < 1/z )
			{
				break;
			}
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
						span = P - lcve;

					old_uend = u->end;
					old_vend = v->end;
					u->end += span;
					v->end += span;
					/* update the probability */
					if ( span > 1 )
					{
						u->p = u->p * x[old_uend][letter] * colx.FP[u->end - 1] / colx.FP[old_uend];
						v->p = v->p * x[old_vend][letter] * colx.FP[u->end - 1] / colx.FP[old_vend];
					}
					else
					{
						u->p = u->p * x[old_uend][letter];
						v->p = v->p * x[old_vend][letter];
					}

					lcve = lcve + span;
				}
				else if ( colx.colour[u->end] == 'b' && colx.colour[v->end] != 'b' )
				{
					/* add the new black position to factor u */
					u->bpp.push_back ( u->end );
					u->bpset.push_back ( letter );
					u->l ++;

					/* skip to the next black position */
					if ( ( colx.BP[u->end] - u->end ) < ( colx.BP[v->end] - v->end ) )
						span = colx.BP[u->end] - u->end;
					else
						span = colx.BP[v->end] - v->end;
					if ( span + lcve > P )
						span = P - lcve;

					old_uend = u->end;
					old_vend = v->end;
					u->end += span;
					v->end += span;

					/* update the probability */
					if ( span > 1 )
					{
						u->p = u->p * x[old_uend][letter] * colx.FP[u->end - 1];
						v->p = v->p * x[old_vend][letter] * colx.FP[v->end - 1] / colx.FP[old_vend];
					}
					else
					{
						u->p = u->p * x[old_uend][letter];
						v->p = v->p * x[old_vend][letter];
					}

					lcve = lcve + span;
				}
				else if ( colx.colour[u->end] != 'b' && colx.colour[v->end] == 'b' )
				{
					/* add the new black position to factor v */
					v->bpp.push_back ( v->end );
					v->bpset.push_back ( letter );
					v->l ++;

					/* skip to the next black position */
					if ( ( colx.BP[u->end] - u->end ) < ( colx.BP[v->end] - v->end ) )
						span = colx.BP[u->end] - u->end;
					else
						span = colx.BP[v->end] - v->end;
					if ( span + lcve > P )
						span = P - lcve;

					old_uend = u->end;
					old_vend = v->end;
					u->end += span;
					v->end += span;

					/* update the probability */
					if ( span > 1 )
					{
						u->p = u->p * x[old_uend][letter] * colx.FP[u->end - 1] / colx.FP[old_uend];
						v->p = v->p * x[old_vend][letter] * colx.FP[v->end - 1];
					}
					else
					{
						u->p = u->p * x[old_uend][letter];
						v->p = v->p * x[old_vend][letter];
					}

					lcve = lcve + span;
				}
			}
		}
		if ( v->end == n )
			break;
	}
	return lcve;
}

unsigned int gextension ( double ** x, unsigned int n, unsigned int m, double z,Factor * u, Factor * v )
{
	unsigned ge = 0;
	if ( v->end >= n )
		return ge;
	do
	{
		vector < unsigned int > branch;
		unsigned int match = branchBP ( x[u->end], x[v->end], m, &branch );
		if ( match == 0 )
		{
			break;
		}
		else if ( match == 1 )
		{
			u->p *= x[u->end][ branch[0] ];
			v->p *= x[v->end][ branch[0] ];
			if ( u->p < 1/z || v->p < 1/z )
			{
				break;
			}
			else
			{
				u->end ++;
				v->end ++;
				ge ++;
			}
		}
		else if ( match > 1 )
		{
			unsigned int g_branch[match];
			unsigned int max_branch = 0;
			unsigned int pick = 0;

			Factor u_branch[match];
			Factor v_branch[match];
			for ( unsigned int i = 0; i < match; i++ )
			{
				g_branch[i] = 0;
				u_branch[i] = *u;
				v_branch[i] = *v;
				u_branch[i].p *= x[u->end][ branch[i] ];
				v_branch[i].p *= x[v->end][ branch[i] ];
				if ( u_branch[i].p < 1/z || v_branch[i].p < 1/z )
				{
					g_branch[i] = 0;
				}
				else
				{
					u_branch[i].end ++;
					v_branch[i].end ++;
					g_branch[i] ++;
					g_branch[i] += gextension ( x, n, m, z, &u_branch[i], &v_branch[i] );
				}
				if ( max_branch < g_branch[i] )
				{
					max_branch = g_branch[i];
					pick = i;
				
				}
			}
			ge += max_branch;
			u->p = u_branch[pick].p;
			v->p = v_branch[pick].p;
			u->end = u_branch[pick].end;
			v->end = v_branch[pick].end;
			break;
		}
	}while ( v->end < n );
	return ge;
}

