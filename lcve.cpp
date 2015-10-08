#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include "defs.h"
#include "global.h"

using namespace std;

int compare ( WStr x, unsigned int a, unsigned int b, int m, pair < double, double > * p )
{
	if ( x.str[a] > m && x.str[b] > m )
	{
		/*both positions are Black*/
		p->first  = 0;
		p->second = 0;
		return 4;
	}
	else if ( ( x.str[a] < m + 1 ) && ( x.str[b] < m + 1 ) )
	{
		/*both positions are White of Greg*/
		if ( x.str[a] == x.str[b] )
		{
			p->first  = x.prob[a];
			p->second = x.prob[b];
			return 1;
		}
		else
			return 0;
	}
	else
	{
		if ( x.str[a] > m )
		{
			/*only position u is Black*/
			int row = x.str[a] - m - 1;
			int col = x.str[b] - 1;
			if ( x.bpt[row][col] > 0 )
			{
				p->first  = x.bpt[row][col];
				p->second = x.prob[b];
				return 2;
			}
			else
				return 0;
		}
		else
		{
			/*only position v is Black*/
			int row = x.str[b] - m - 1;
			int col = x.str[a] - 1;
			if ( x.bpt[row][col] > 0 )
			{
				p->first  = x.prob[a];
				p->second = x.bpt[row][col];
				return 3;
			}
			else
				return 0;
		}
	}
}

int branchBP ( WStr x, unsigned int a, unsigned int b, int m, vector < int > * branch, vector < pair < double, double > > * pro )
{
	int num_branch = 0;
	for ( int i = 0; i < m; i++ )
	{
		if ( x.bpt[a][i] > 0 && x.bpt[b][i] > 0 )
		{
			pair < double, double > p;
			p.first  = x.bpt[a][i];
			p.second = x.bpt[b][i];
			branch->push_back ( i + 1 );
			pro->push_back ( p );
			num_branch ++;
		}
	}
	return num_branch;
}

unsigned int LCVE ( WStr x, unsigned int n, int m, double z, unsigned int lcve, unsigned int P, Factor * u, Factor * v )
{
	unsigned int letter;
	unsigned int span;
	unsigned int old_uend;
	unsigned int old_vend;
	pair < double, double > pro;
	if ( v->end == n )
		return lcve;
	while ( compare ( x, u->end, v->end, m, &pro ) )
	{
		int result = compare ( x, u->end, v->end, m, &pro );

		if ( lcve >= P )
			return lcve;
		if ( v->end == n - 1 )
		{
			if ( result != 4 )
			{
				lcve ++;
				double pcheck_u = u->p * pro.first;
				double pcheck_v = v->p * pro.second;
				if ( pcheck_u > 1/z && pcheck_v > 1/z )
				{
					lcve ++;				
					return lcve;
				}
				else
					return lcve;
			}
			else
			{
				for ( int l = 0; l < m; l++ )
				{
					double pcheck_u = u->p * x.bpt[x.str[u->end] - m - 1][l];
					double pcheck_v = v->p * x.bpt[x.str[v->end] - m - 1][l];
					if ( pcheck_u > 1/z && pcheck_v > 1/z )
					{
						lcve ++;
						return lcve;
					}
				}
				return lcve;
			}
		}

		if ( result == 4 )
		{
			int row_u = x.str[u->end] - m - 1;
			int row_v = x.str[v->end] - m - 1;
			int num_branch;
			vector < int > branch;
			vector < pair < double, double > > p_branch;
			vector < unsigned int > lcve_branch;
			num_branch = branchBP ( x, row_u, row_v, m, &branch, &p_branch );
			
			Factor u_branch[num_branch];
			Factor v_branch[num_branch];

			for ( unsigned int i = 0; i < num_branch; i++ )
			{
				letter = branch[i];
				double pcheck_u = u->p * p_branch[i].first;
				double pcheck_v = v->p * p_branch[i].second;

				if ( pcheck_u < 1/z || pcheck_v < 1/z )
					lcve_branch.push_back ( lcve );
				else
				{
					/* construct two new factors for u and v */
					u_branch[i] = * u;
					v_branch[i] = * v;

					/* add the new black position in factor u & v */
					u_branch[i].bpp.push_back ( u->end );
					u_branch[i].bpset.push_back ( letter );
					u_branch[i].l ++;

					v_branch[i].bpp.push_back ( v->end );
					v_branch[i].bpset.push_back ( letter );
					v_branch[i].l ++;

					if ( ( x.BP[u->end] - u->end ) < ( x.BP[v->end] - v->end ) )
						span = x.BP[u->end] - u->end;
					else
						span = x.BP[v->end] - v->end;

					if ( span + lcve > P )
						span = P - lcve;

					u_branch[i].end = u->end + span;
					v_branch[i].end = v->end + span;

					if ( span > 1 )
					{
						u_branch[i].p = u->p * p_branch[i].first * x.FP[ u_branch[i].end - 1];
						v_branch[i].p = v->p * p_branch[i].second * x.FP[ v_branch[i].end - 1];
					}
					else
					{
						u_branch[i].p = u->p * p_branch[i].first;
						v_branch[i].p = v->p * p_branch[i].second;
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
			float pcheck_u = u->p * pro.first;
			float pcheck_v = v->p * pro.second;
			if ( pcheck_u < 1/z || pcheck_v < 1/z )
			{
				break;
			}
			else
			{
				if ( result == 1 )
				{

					/* jump to the next black position */
					if ( ( x.BP[u->end] - u->end ) < ( x.BP[v->end] - v->end ) )
						span = x.BP[u->end] - u->end;
					else
						span = x.BP[v->end] - v->end;

					if ( span + lcve > P )
						span = P - lcve;

					old_uend = u->end;
					old_vend = v->end;
					u->end += span;
					v->end += span;
					/* update the probability */
					if ( span > 1 )
					{
						u->p = u->p * pro.first * x.FP[u->end - 1] / x.FP[old_uend];
						v->p = v->p * pro.second * x.FP[u->end - 1] / x.FP[old_vend];
					}
					else
					{
						u->p = u->p * pro.first;
						v->p = v->p * pro.second;
					}

					lcve = lcve + span;
				}
				else if ( result == 2 )
				{
					/* add the new black position to factor u */
					u->bpp.push_back ( u->end );
					u->bpset.push_back ( x.str[v->end] );
					u->l ++;

					/* skip to the next black position */
					if ( ( x.BP[u->end] - u->end ) < ( x.BP[v->end] - v->end ) )
						span = x.BP[u->end] - u->end;
					else
						span = x.BP[v->end] - v->end;
					if ( span + lcve > P )
						span = P - lcve;

					old_uend = u->end;
					old_vend = v->end;
					u->end += span;
					v->end += span;

					/* update the probability */
					if ( span > 1 )
					{
						u->p = u->p * pro.first  * x.FP[u->end - 1];
						v->p = v->p * pro.second * x.FP[v->end - 1] / x.FP[old_vend];
					}
					else
					{
						u->p = u->p * pro.first;
						v->p = v->p * pro.second;
					}

					lcve = lcve + span;
				}
				else if ( result == 3 )
				{
					/* add the new black position to factor v */
					v->bpp.push_back ( v->end );
					v->bpset.push_back ( x.str[u->end] );
					v->l ++;

					/* skip to the next black position */
					if ( ( x.BP[u->end] - u->end ) < ( x.BP[v->end] - v->end ) )
						span = x.BP[u->end] - u->end;
					else
						span = x.BP[v->end] - v->end;
					if ( span + lcve > P )
						span = P - lcve;

					old_uend = u->end;
					old_vend = v->end;
					u->end += span;
					v->end += span;

					/* update the probability */
					if ( span > 1 )
					{
						u->p = u->p * pro.first * x.FP[u->end - 1] / x.FP[old_uend];
						v->p = v->p * pro.second * x.FP[v->end - 1];
					}
					else
					{
						u->p = u->p * pro.first;
						v->p = v->p * pro.second;
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

unsigned int gextension ( WStr x, unsigned int n, unsigned int m, double z,Factor * u, Factor * v )
{
	unsigned ge = 0;
	pair < double, double > pro;
	if ( v->end >= n )
		return ge;
	do
	{
		vector < unsigned int > branch;
		int match = compare ( x, u->end, v->end, m, &pro );
		if ( match == 0 )
		{
			break;
		}
		else if ( match != 4 )
		{
			u->p *= pro.first;
			v->p *= pro.second;
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
		else if ( match == 4 )
		{
			int row_u = x.str[u->end] - m - 1;
			int row_v = x.str[v->end] - m - 1;

			vector < int > branch;
			vector < pair < double, double > > p_branch;

			int num_branch = branchBP ( x, row_u, row_v, m, &branch, &p_branch );
			unsigned int g_branch[num_branch];
			unsigned int max_branch = 0;
			unsigned int pick = 0;

			Factor u_branch[num_branch];
			Factor v_branch[num_branch];
			for ( unsigned int i = 0; i < match; i++ )
			{
				g_branch[i] = 0;
				u_branch[i] = * u;
				v_branch[i] = * v;
				u_branch[i].p *= p_branch[i].first;
				v_branch[i].p *= p_branch[i].second;
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

