#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <ctime>

#include "defs.h"

using namespace std;

extern WStr xy;

int compare ( unsigned int a, unsigned int b, int m, pair < double, double > * p )
{
	if ( xy.str[a] > m && xy.str[b] > m )
	{
		/*both positions are Black*/
		p->first  = 0;
		p->second = 0;
		return 4;
	}
	else if ( ( xy.str[a] < m + 1 ) && ( xy.str[b] < m + 1 ) )
	{
		/*both positions are White of Greg*/
		if ( xy.str[a] == xy.str[b] )
		{
			p->first  = xy.prob[a];
			p->second = xy.prob[b];
	
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		
		if ( xy.str[a] > m )
		{
			/*only position u is Black*/
			int row = xy.str[a] - m - 1;
			int col = xy.str[b] - 1;
			if ( xy.bpt[row][col] > 0 )
			{
				p->first  = xy.bpt[row][col];
				p->second = xy.prob[b];
			
				return 2;
			}
			else
				return 0;
		}
		else
		{
			/*only position v is Black*/
			int row = xy.str[b] - m - 1;
			int col = xy.str[a] - 1;
			if ( xy.bpt[row][col] > 0 )
			{
				p->first  = xy.prob[a];
				p->second = xy.bpt[row][col];
		
				return 3;
			}
			else
				return 0;
		}
	}
}

int branchBP ( unsigned int a, unsigned int b, int m, vector < int > * branch, vector < pair < double, double > > * pro )
{
	int num_branch = 0;
	for ( int i = 0; i < m; i++ )
	{
		if ( xy.bpt[a][i] > 0 && xy.bpt[b][i] > 0 )
		{
			pair < double, double > p;
			p.first  = xy.bpt[a][i];
			p.second = xy.bpt[b][i];
			branch->push_back ( i + 1 );
			pro->push_back ( p );
			num_branch ++;
		}
	}
	return num_branch;
}

unsigned int LCVE ( unsigned int n, int m, double z, unsigned int lcve, unsigned int P, Factor * u, Factor * v )
{
	unsigned int letter;
	unsigned int span;
	unsigned int old_uend;
	unsigned int old_vend;
	pair < double, double > pro;
	if ( v->end == n )
		return lcve;

	while ( v->end < n )
	{
		int result = compare ( u->end, v->end, m, &pro );
		
		if ( result == 0 )
			return lcve;

		if ( lcve >= P )
			return lcve;
		if ( v->end == n - 1 )
		{
			if ( result != 4 )
			{
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
					double pcheck_u = u->p * xy.bpt[xy.str[u->end] - m - 1][l];
					double pcheck_v = v->p * xy.bpt[xy.str[v->end] - m - 1][l];
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
			int row_u = xy.str[u->end] - m - 1;
			int row_v = xy.str[v->end] - m - 1;
			int num_branch;
			vector < int > branch;
			vector < pair < double, double > > p_branch;
			vector < unsigned int > lcve_branch;
			
			num_branch = branchBP ( row_u, row_v, m, &branch, &p_branch );
			
			if ( num_branch == 0 )
				break;
			
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

					if ( ( xy.BP[u->end] - u->end ) < ( xy.BP[v->end] - v->end ) )
						span = xy.BP[u->end] - u->end;
					else
						span = xy.BP[v->end] - v->end;

					if ( span + lcve > P )
						span = P - lcve;

					u_branch[i].end = u->end + span;
					v_branch[i].end = v->end + span;

					if ( span > 1 )
					{
						u_branch[i].p = u->p * p_branch[i].first * xy.FP[ u_branch[i].end - 1];
						v_branch[i].p = v->p * p_branch[i].second * xy.FP[ v_branch[i].end - 1];
					}
					else
					{
						u_branch[i].p = u->p * p_branch[i].first;
						v_branch[i].p = v->p * p_branch[i].second;
					}

					lcve_branch.push_back ( lcve + span );
					lcve_branch[i] = ( LCVE( n, m, z, lcve_branch[i], P, &u_branch[i], &v_branch[i] ) );
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
					if ( ( xy.BP[u->end] - u->end ) < ( xy.BP[v->end] - v->end ) )
						span = xy.BP[u->end] - u->end;
					else
						span = xy.BP[v->end] - v->end;

					if ( span + lcve > P )
						span = P - lcve;

					old_uend = u->end;
					old_vend = v->end;
					u->end += span;
					v->end += span;
					/* update the probability */
					if ( span > 1 )
					{
						u->p = u->p * pro.first * xy.FP[u->end - 1] / xy.FP[old_uend];
						v->p = v->p * pro.second * xy.FP[u->end - 1] / xy.FP[old_vend];
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
					u->bpset.push_back ( xy.str[v->end] );
					u->l ++;

					/* skip to the next black position */
					if ( ( xy.BP[u->end] - u->end ) < ( xy.BP[v->end] - v->end ) )
						span = xy.BP[u->end] - u->end;
					else
						span = xy.BP[v->end] - v->end;
					if ( span + lcve > P )
						span = P - lcve;

					old_uend = u->end;
					old_vend = v->end;
					u->end += span;
					v->end += span;

					/* update the probability */
					if ( span > 1 )
					{
						u->p = u->p * pro.first  * xy.FP[u->end - 1];
						v->p = v->p * pro.second * xy.FP[v->end - 1] / xy.FP[old_vend];
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
					v->bpset.push_back ( xy.str[u->end] );
					v->l ++;

					/* skip to the next black position */
					if ( ( xy.BP[u->end] - u->end ) < ( xy.BP[v->end] - v->end ) )
						span = xy.BP[u->end] - u->end;
					else
						span = xy.BP[v->end] - v->end;
					if ( span + lcve > P )
						span = P - lcve;

					old_uend = u->end;
					old_vend = v->end;
					u->end += span;
					v->end += span;

					/* update the probability */
					if ( span > 1 )
					{
						u->p = u->p * pro.first * xy.FP[u->end - 1] / xy.FP[old_uend];
						v->p = v->p * pro.second * xy.FP[v->end - 1];
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

