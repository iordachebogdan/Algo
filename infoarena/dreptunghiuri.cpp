#include <fstream>
#include <cmath>
#include <cstring>
#define DIM 405
#define infile "dreptunghiuri.in"
#define outfile "dreptunghiuri.out"
 
using namespace std;
 
ifstream f(infile);
ofstream g(outfile);
 
int n, m;
 
int Ans[DIM][DIM], radical[DIM*DIM];
 
int main () {
    f >> n >> m;
    memset(radical, -1, sizeof(radical));
    for (int i = 0; i < DIM; ++i)
        radical[i * i] = i;
    for (int H = 1; H < n; ++H)
        for (int W = 1; W < m; ++W) {
            for (int A = 1; A < H; ++A) {
                int Delta = W * W - 4 * A * (H - A);
                if (Delta < 0)
                    continue;
                int rDelta = radical[Delta];
                if (rDelta < 0)
                    continue;
                if (Delta == 0) {
                    if ((W & 1) == 0 && W / 2 >= 1)
                        ++Ans[H][W];
                }
                else {
                    if (((W + rDelta) & 1) == 0 && (W + rDelta) / 2 >= 1 && (W + rDelta) / 2 < W)
                        ++Ans[H][W];
                    if (((W - rDelta) & 1) == 0 && (W - rDelta) / 2 >= 1)
                        ++Ans[H][W];
                }
            }
            ++Ans[H][W];
        }
    long long SOL = 0;
    for (int i = 1; i < n; ++i)
        for (int j = 1; j < m; ++j)
            SOL += 1LL * Ans[i][j] * (n - i) * (m - j);
    g << SOL;
    return 0;
}