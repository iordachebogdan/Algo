#include <fstream>
#include <algorithm>
#define DIM 1005
#define lsb(x) (x&(-x))
#define infile "tri2.in"
#define outfile "tri2.out"
 
using namespace std;
 
ifstream f(infile);
ofstream g(outfile);
 
int n, sum, q, p1, p2, p3;
 
int px, py;
 
int D[DIM][DIM], A[DIM];
 
struct point {
    int x;
    int y;
    int xx;
    int p;
} v[DIM], w[DIM], P[4];
 
inline int modul(int a) {
    return (a > 0 ? a : -a);
}
 
void query(int p) {
    for (int i = p; i; i -= lsb(i))
        sum += A[i];
}
 
void update(int p) {
    for (int i = p; i <= n; i += lsb(i))
        ++A[i];
}
 
bool cmp1(const point &a, const point &b) {
    return a.x < b.x;
}
 
bool cmp2(const point &a, const point &b) {
    return (1LL*(a.x - px)*(b.y - py) - 1LL*(a.y - py)*(b.x - px) > 0);
}
 
bool cmp3(const point &a, const point &b) {
    return a.p < b.p;
}
 
int main() {
    f >> n;
    for (int i = 1; i <= n; ++i) {
        f >> v[i].x >> v[i].y;
        v[i].p = i;
    }
    sort(v + 1, v + n + 1, cmp1);
    for (int i = 1; i <= n; ++i)
        v[i].xx = i;
    for (int i = 1; i <= n; ++i) {
        int k = 0;
        for (int j = i + 1; j <= n; ++j)
            w[++k] = v[j];
        px = v[i].x;
        py = v[i].y;
        sort(w + 1, w + k + 1, cmp2);
        for (int j = 0; j < DIM; ++j)
            A[j] = 0;
        for (int j = 1; j <= k; ++j) {
            sum = 0;
            query(w[j].xx);
            D[v[i].p][w[j].p] = D[w[j].p][v[i].p] = sum;
            update(w[j].xx);
        }
    }
    /*for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j)
            g << D[i][j] << " ";
        g << "\n";
    }*/
    f >> q;
    sort(v + 1, v + n + 1, cmp3);
    for (int i = 1; i <= q; ++i) {
        f >> p1 >> p2 >> p3;
        P[1] = v[p1];
        P[2] = v[p2];
        P[3] = v[p3];
        sort(P + 1, P + 4, cmp1);
        int sol = D[P[1].p][P[2].p] + D[P[3].p][P[2].p] - D[P[1].p][P[3].p];
        sol = modul(sol);
        if (1LL*(P[2].x - P[1].x)*(P[3].y - P[1].y) - 1LL*(P[2].y - P[1].y)*(P[3].x - P[1].x) > 0)
            --sol;
        g << sol << "\n";
    }
    return 0;
}