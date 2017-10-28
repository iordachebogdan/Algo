#include <fstream>
#include <algorithm>
 
#define DIM 50
#define MOD 3210121
#define VMAX 11000
#define infile "cowfood.in"
#define outfile "cowfood.out"
 
using namespace std;
 
ifstream fin(infile);
ofstream fout(outfile);
 
int k, s, n, sol, ok;
 
int a[DIM][DIM], b[DIM][DIM], dp[VMAX][DIM], D[VMAX];
 
void back(int x, int cnt) {
 
    if (x == n + 1) {
 
        int sum = 0;
 
        for (int i = 1; i <= k; ++i)
            sum += b[x][i];
 
        if (sum > s || !ok)
            return;
 
        int aux = s - sum;
 
        if (!cnt)
            sol -= D[aux];
        else
            sol += D[aux];
 
        sol += MOD;
 
        sol %= MOD;
 
        return;
 
    }
 
    for (int i = 1; i <= k; ++i)
        b[x + 1][i] = b[x][i];
 
    back(x + 1, cnt);
 
    ok++;
     
    for (int i = 1; i <= k; ++i)
        b[x + 1][i] = max(b[x][i], a[x][i]);
 
    back(x + 1, (cnt ^ 1));
 
    ok--;
 
}
 
int main() {
 
    fin >> k >> s >> n;
 
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= k; ++j)
            fin >> a[i][j];
 
    //dp[i][j] -> impart suma i in j partitii.
 
    for (int j = 1; j <= k + 1; ++j)
        dp[0][j] = 1;
 
    for (int i = 1; i <= s; ++i) {
 
        dp[i][1] = 1;
 
        for (int j = 1; j <= k + 1; ++j) {
 
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
 
            dp[i][j] %= MOD;
 
        }
 
    }
 
    D[0] = 1;
 
    for (int i = 1; i <= s; ++i)
        D[i] = dp[i][k + 1];
 
    back(1, 0);
 
    fout << (2*MOD + D[s] - k * s % MOD - 1 - sol) % MOD;
 
    return 0;
 
}