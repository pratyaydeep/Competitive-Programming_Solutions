#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <queue>
#include <cmath>
#include <algorithm>
#define EPS 1e-9

using namespace std;

const int N = 505;

struct vec;
struct pt;
struct line;

struct vec
{
	double x, y;
	vec(){}
	vec(double _x, double _y): x(_x), y(_y){}
};

struct pt
{
	double x, y;
	pt(){}
	pt(double _x, double _y): x(_x), y(_y){}
	vec operator - (const pt &rhs)
	{
		return vec(x - rhs.x, y - rhs.y);
	}
};

struct line
{
	double a, b; // expressed as y = ax + b
	line(){}
	line(double _a, double _b): a(_a), b(_b){}
};

// l is a line
inline bool above(const double &x, const double &y, const pt &l)
{
	return y > l.x * x - l.y + EPS;
}

double cross(const vec &a, const vec &b)
{
	return a.x * b.y - a.y * b.x;
}

bool pt_cmp(pt a, pt b)
{
	if (a.x != b.x)
		return a.x < b.x;
	else
		return a.y < b.y;
}

bool pt_cmp_rev(pt a, pt b)
{
	if (a.x != b.x)
		return a.x > b.x;
	else
		return a.y > b.y;
}

//bool parallel(const line &a, const line &b)
//{
//	return abs(a.a - b.a) < EPS;
//}

// negate y
// a = x, b = -y
pt intersect(const pt &a, const pt &b)
{
	double x, y;
	if (a.x == b.x)
		return pt(1e9, 1e9);
	x = (a.y - b.y) / (a.x - b.x);
	y = a.x * x - a.y;
	
	return pt(x, y);
}

int stk[N];
void monotone_half(pt p[], int &sz, char rev = 0)
{
	int i, t = -1;
	if (!rev) sort(p, p + sz, pt_cmp);
	else sort(p, p + sz, pt_cmp_rev);
	for (i = 0; i < sz; i++)
	{
		if (t < 1){ stk[++t] = i; continue; }
		while (t >= 1 && cross(p[i] - p[stk[t]], p[stk[t]] - p[stk[t-1]]) >= 0) --t;
		stk[++t] = i;
	}
	for (i = 0; i <= t; i++) p[i] = p[stk[i]];
	sz = t + 1;
}

inline int input()
{
	char c;
	for (c = getchar(); c < 48 || c > 57; c = getchar() ) ;
	int x = c - 48;
	for (c = getchar(); c > 47 && c < 58; c = getchar() ) x = x * 10 + c - 48;
	return x;
}

pt pt_l[N], pt_u[N]; int sz_l, sz_u;
double w, h;

int main()
{
//	freopen("11265.in", "r", stdin);
//	freopen("11265.txt", "w", stdout);
	int i, tt = 0, n;
	double x1, y1, x2, y2;
	double left_x, right_x;
	double ref_x, ref_y;
	int it_l, it_u;
	pt i_i, i_l, i_u;
	double dy, next_dy, x, next_x, ans;
	char st;
	pt tmp;
	while (scanf("%d", &n) == 1)
	{
		w = input(); h = input();
		sz_l = sz_u = 0;
		it_l = it_u = 0;
		st = 0;
		ans = 0.0;
		ref_x = input(); ref_y = input();
		pt_l[sz_l++] = pt(0.0, 0.0);
		pt_u[sz_u++] = pt(0.0, -h);
		left_x = 0.0;
		right_x = w;
		for (i = 0; i < n; i++)
		{
			x1 = input(); y1 = input(); x2 = input(); y2 = input();
			if (x1 == x2) // vertical line
			{
				if (x1 <= ref_x)
					left_x = max(left_x, x1);
				else
					right_x = min(right_x, x1);
			}
			else
			{
				tmp = pt((y2 - y1) / (x2 - x1), (y1 * x2 - y2 * x1) / (x1 - x2));
				if (above(ref_x, ref_y, tmp))
					pt_l[sz_l++] = tmp;
				else
					pt_u[sz_u++] = tmp;
			}
		}
		monotone_half(pt_l, sz_l, 0);
		monotone_half(pt_u, sz_u, 1);
		
		if (sz_l >= 2 && pt_l[sz_l - 1].x == pt_l[sz_l - 2].x) --sz_l;
		if (sz_u >= 2 && pt_u[sz_u - 1].x == pt_u[sz_u - 2].x) --sz_u;
		
		for ( ; it_l < sz_l - 1; it_l++) if (intersect(pt_l[it_l], pt_l[it_l + 1]).x >= left_x) break;
		for ( ; it_u < sz_u - 1; it_u++) if (intersect(pt_u[it_u], pt_u[it_u + 1]).x >= left_x) break;
		for (x = left_x; x < right_x; )
		{
			i_i = intersect(pt_l[it_l], pt_u[it_u]);
			i_l = it_l < sz_l - 1 ? intersect(pt_l[it_l], pt_l[it_l + 1]) : pt(1e9, 1e9);
			i_u = it_u < sz_u - 1 ? intersect(pt_u[it_u], pt_u[it_u + 1]) : pt(1e9, 1e9);
			if (st == 0 && i_i.x >= x && pt_u[it_u].x * x - pt_u[it_u].y - (pt_l[it_l].x * x - pt_l[it_l].y) >= 0.0) i_i = pt(-1e9, -1e9);
			if (st == 0 && i_i.x <= i_l.x && i_i.x <= i_u.x)
			{	
				next_x = max(x, i_i.x);
				next_dy = i_i.x <= x ? pt_u[it_u].x * x - pt_u[it_u].y - (pt_l[it_l].x * x - pt_l[it_l].y) : 0.0;
				st = 1;
				x = next_x; // not counting the first edge (the area between the previous edge and the first edge)
			}
			else if (i_i.x > x + EPS && i_i.x <= i_l.x && i_i.x <= i_u.x)
			{
				next_x = min(i_i.x, right_x);
				next_dy = i_i.x <= right_x ? 0.0 : pt_u[it_u].x * right_x - pt_u[it_u].y - (pt_l[it_l].x * right_x - pt_l[it_l].y);
				st = 2;
			}
			else if (i_l.x <= i_u.x)
			{
				next_x = min(i_l.x, right_x);
				next_dy = pt_u[it_u].x * next_x - pt_u[it_u].y - (pt_l[it_l].x * next_x - pt_l[it_l].y);
				it_l++;
			}
			else // if (i_u.x <= i_l.x)
			{
				next_x = min(i_u.x, right_x);
				next_dy = pt_u[it_u].x * next_x - pt_u[it_u].y - (pt_l[it_l].x * next_x - pt_l[it_l].y);
				it_u++;
			}
			
			if (st && next_dy < -EPS) break;
			
			if (st) ans += (next_dy + dy) * (next_x - x) * 0.5;
			if (st == 2) break;
			dy = next_dy;
			x = next_x;
		}
		printf("Case #%d: %.3f\n", ++tt, ans);
	}
	return 0;
}
