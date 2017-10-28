#include <fstream>
#include <iomanip>
#include <algorithm>
#define DIM 105
#define INF 1000000000
#define EPS 0.0000001
#define infile "flux.in"
#define outfile "flux.out"
 
using namespace std;
 
ifstream f(infile);
ofstream g(outfile);
 
int n, m, i, j, k, l, a, b;
 
double c, Res;
 
double C[DIM][DIM], A[DIM][DIM], X[DIM];
 
int D[DIM][DIM];
 
bool ok[DIM];
 
void DFS(int nod) {
    ok[nod] = 1;
    for (int i = 1; i <= n; ++i)
    if (ok[i] == 0 && D[nod][i] != 0)
        DFS(i);
}
 
int main() {
    f >> n >> m;
    for (i = 1; i <= n; ++i)
    for (j = 1; j <= n; ++j)
        C[i][j] = INF;
    for (i = 1; i <= m; ++i) {
        f >> a >> b >> c;
        C[a][b] = std::min(C[a][b], c);
        C[b][a] = C[a][b];
        ++D[a][b]; ++D[b][a];
    }
    for (i = 2; i <= n; ++i)
    for (j = 1; j <= n; ++j) 
    if (i != j){
        A[i][j] = D[i][j];
        A[i][i] -= D[i][j];
    }
    A[n][n + 1] = -1;
    i = 2; j = 2;
    while (i <= n && j <= n) {
        for (k = i; k <= n; ++k)
        if (A[k][j] < -EPS || A[k][j] > EPS)
            break;
        if (k == n + 1) {
            ++j;
            continue;
        }
        if (k != i)
        for (l = 1; l <= n + 1; ++l) {
            double tmp = A[i][l];
            A[i][l] = A[k][l];
            A[k][l] = tmp;
        }
        for (l = j + 1; l <= n + 1; ++l)
            A[i][l] /= A[i][j];
        A[i][j] = 1;
        for (k = i + 1; k <= n; ++k) {
            for (l = j + 1; l <= n + 1; ++l)
                A[k][l] -= A[i][l] * A[k][j];
            A[k][j] = 0;
        }
        ++i; ++j;
    }
    for (i = n; i >= 2; --i)
    for (j = 2; j <= n + 1; ++j)
    if (A[i][j] < -EPS || A[i][j] > EPS) {
        X[j] = A[i][n + 1];
        for (k = j + 1; k <= n; ++k)
            X[j] -= X[k] * A[i][k];
        break;
    }
    DFS(1);
    if (!ok[n]) {
        g << "0.000";
        return 0;
    }
    Res = INF;
    for (i = 1; i <= n; ++i)
    for (j = 1; j <= n; ++j) {
        if (i == j || D[i][j] == 0)
            continue;
        double tmp = X[i] - X[j];
        if (tmp >= -EPS && tmp <= EPS)
            continue;
        if (tmp < 0)
            tmp = -tmp;
        Res = std::min(Res, C[i][j] / tmp);
    }
    g << setprecision(3) << fixed << Res;
    return 0;
}