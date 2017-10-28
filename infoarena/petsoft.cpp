#include <fstream>
#include <algorithm>
#include <vector>
 
#define DIM 1005
#define vint vector<int>::iterator
#define infile "petsoft.in"
#define outfile "petsoft.out"
 
using namespace std;
 
ifstream fin(infile);
ofstream fout(outfile);
 
int n;
 
vector<int> L[DIM], v;
 
int dp[2][DIM];
 
int cost[DIM][DIM];
 
void solve(int node) {
 
    for (vint it = L[node].begin(); it != L[node].end(); ++it) {
 
        solve(*it);
 
    }
 
    v.clear();
 
    for (vint it = L[node].begin(); it != L[node].end(); ++it)
        v.push_back(*it);
 
    sort(v.begin(), v.end());
 
    for (int i = 0; i < v.size(); ++i) {
 
        cost[i][i] = max(dp[0][v[i]], dp[1][v[i]]);
 
    }
 
    for (int len = 2; len <= v.size(); ++len) {
 
        for (int i = 0; i + len - 1 < v.size(); ++i) {
 
            int j = i + len - 1;
 
            cost[i][j] = max(cost[i + 1][j] + (v[i] == node ? 0 : dp[1][v[i]]), cost[i][j - 1] + (v[j] == node ? 0 : dp[1][v[j]]));
 
            int temp = (len > 2 ? cost[i + 1][j - 1] : 0);
 
            cost[i][j] = max(cost[i][j], temp + (dp[0][v[i]] + dp[0][v[j]]) + v[j] - v[i]);
 
        }
 
    }
 
    dp[0][node] = cost[0][v.size() - 1];
 
    v.push_back(node);
 
    sort(v.begin(), v.end());
 
    for (int i = 0; i < v.size(); ++i) {
 
        cost[i][i] = max(dp[0][v[i]], dp[1][v[i]]);
 
        if (v[i] == node)
            cost[i][i] = 0;
 
    }
 
    for (int len = 2; len <= v.size(); ++len) {
 
        for (int i = 0; i + len - 1 < v.size(); ++i) {
 
            int j = i + len - 1;
 
            cost[i][j] = max(cost[i + 1][j] + (v[i] == node ? 0 : dp[1][v[i]]), cost[i][j - 1] + (v[j] == node ? 0 : dp[1][v[j]]));
 
            int temp = (len > 2 ? cost[i + 1][j - 1] : 0);
 
            if (v[i] == node || v[j] == node)
                temp -= dp[0][node];
 
            cost[i][j] = max(cost[i][j], temp + (dp[0][v[i]] + dp[0][v[j]]) + v[j] - v[i]);
 
        }
 
    }
 
    dp[1][node] = max(dp[0][node], cost[0][v.size() - 1]);
 
}
 
int main() {
 
    fin >> n;
 
    for (int i = 2; i <= n; ++i) {
 
        int x;
 
        fin >> x;
 
        L[x].push_back(i);
 
    }
 
    solve(1);
 
    fout << dp[0][1];
 
    return 0;
 
}