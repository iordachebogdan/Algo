#include <cstdio>
#include <fstream>
#include <set>
#include <vector>
#include <cstring>
#include <algorithm>
#define DIM 805
#define BMAX 4000000
#define eps 0.00000001
#define vint vector<int>::iterator
#define pint vector< pair< pair<int, int>, pair<int, int> > >::iterator
#define sint set<int>::iterator
#define st first.first
#define nd first.second
#define rd second.first
#define th second.second
#define infile "poligon.in"
#define outfile "poligon.out"
 
using namespace std;
 
ifstream f(infile);
ofstream g(outfile);
 
vector< pair< pair<int, int>, pair<int, int> > > Sides;
 
vector< pair< pair<double, double>, pair<double, double> > > Segments[DIM];
 
set<int> S;
 
vector<int> X;
 
int n, m, x, y, xx, yy;
 
char buf[BMAX];
 
int buffer;
 
void gint(int &ans) {
    ans=0;
    while(buf[buffer]<'0'||buf[buffer]>'9') {
        buffer++;
        if(buffer==BMAX-1) {
            f.get(buf,BMAX,EOF);
            buffer=0;
        }
    }
    while(buf[buffer]>='0'&&buf[buffer]<='9') {
        ans=ans*10+buf[buffer++]-'0';
        if(buffer==BMAX-1) {
            f.get(buf,BMAX,EOF);
            buffer=0;
        }
    }
}
 
inline double det(const pair<double, double> &a, const pair<double, double> &b, const pair<double, double> &c) {
    return ((b.first - a.first) * (c.second - a.second) - (c.first - a.first) * (b.second - a.second));
}
 
bool cmp(const pair< pair<double, double>, pair<double, double> > &a, const pair< pair<double, double>, const pair<double, double> > &b) {
    return a.nd + a.th < b.nd + b.th;
}
 
int main () {
    //freopen(infile, "r", stdin);
    //freopen(outfile, "w", stdout);
    f.get(buf,BMAX,EOF);
    //scanf("%d%d", &n, &m);
    gint(n); gint(m);
    //scanf("%d%d", &xx, &yy);
    gint(xx); gint(yy);
    int first_x = xx;
    int first_y = yy;
    S.insert(xx);
    for (int i = 1; i < n; ++i) {
        //scanf("%d%d", &x, &y);
        gint(x); gint(y);
        S.insert(x);
        if (xx < x || (xx == x && yy < y))
            Sides.push_back( make_pair( make_pair(xx, yy), make_pair(x, y) ) );
        else
            Sides.push_back( make_pair( make_pair(x, y), make_pair(xx, yy) ) );
        xx = x;
        yy = y;
    }
    x = first_x; y = first_y;
    if (xx < x || (xx == x && yy < y))
            Sides.push_back( make_pair( make_pair(xx, yy), make_pair(x, y) ) );
        else
            Sides.push_back( make_pair( make_pair(x, y), make_pair(xx, yy) ) );
    for (sint it = S.begin(); it != S.end(); ++it)
        X.push_back(*it);
    for (pint it = Sides.begin(); it != Sides.end(); ++it)
        for (int i = 0; i < X.size() - 1; ++i) {
            if (it->st > X[i] || X[i + 1] > it->rd)
                continue;
            pair<double, double> left_end, right_end;
            left_end.first = X[i];
            right_end.first = X[i + 1];
            if (it->st != it->rd) {
                left_end.second = it->nd + (X[i] - it->st) * (it->th - it->nd) / (it->rd * 1.0 - it->st);
                right_end.second = it->nd + (X[i + 1] - it->st) * (it->th - it->nd) / (it->rd * 1.0 - it->st);
            }
            else {
                left_end.second = it->nd;
                right_end.second = it->th;
            }
            Segments[i].push_back(make_pair(left_end, right_end));
        }
    for (int i = 0; i < X.size() - 1; ++i)
        std::sort(Segments[i].begin(), Segments[i].end(), cmp);
    int SOL = 0;
    for (; m; --m) {
        //scanf("%d%d", &x, &y);
        gint(x); gint(y);
        int left = 0, right = X.size() - 1, mid = 0;
        while (left <= right) {
            mid = (left + right) / 2;
            if (x >= X[mid] && x <= X[mid+1])
                break;
            if (x < X[mid])
                right = mid - 1;
            else
                left = mid + 1;
        }
        pair<double, double> point;
        int p = mid;
        point = make_pair(x*1.0, y*1.0);
        if ( det( Segments[p].begin()->first, Segments[p].begin()->second, point ) < 0 )
            continue;
        if ( det( Segments[p].rbegin()->first, Segments[p].rbegin()->second, point ) > 0 )
            continue;
        left = 0, right = Segments[p].size() - 1, mid = 0;
        bool ok = false;
        while (left <= right) {
            mid = (left + right) / 2;
            double aux = det( Segments[p][mid].first, Segments[p][mid].second, point );
            if (-eps < aux && aux < eps) {
                ++SOL;
                ok = true;
                break;
            }
            if (aux > 0)
                left = mid + 1;
            else
                right = mid - 1;
        }
        if (!ok && !(right & 1))
            ++SOL;
    }
    //printf("%d", SOL);
    g << SOL;
    return 0;
}