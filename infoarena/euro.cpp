#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
 
using namespace std;
 
ifstream fin("euro.in");
ofstream fout("euro.out");
 
const int dim = 40005;
 
long long n, t;
long long a[dim], dp[dim];
 
vector< pair<long long, int> > v;
 
int main() {
 
    fin >> n >> t;
     
    long long sum = 0;
    for (int i = 1; i <= n; ++i) {
 
        fin >> a[i];
        sum += a[i];
 
        if (sum < 0) {
 
            v.push_back(make_pair(sum, i));
            sum = 0;
 
        }
 
    }
 
    if (!v.size() || v[v.size() - 1].second != n)
        v.push_back(make_pair(sum, (int)n));
 
    for (int i = 0; i < (int)v.size(); ++i) {
 
        sum = v[i].first;
        int day = v[i].second;
 
        dp[i + 1] = dp[i] + sum * day - t;
 
        for (int j = i - 1; j >= 0 && 1LL * (i - j)*(i - j) <= t + 100; --j) {
 
            sum += v[j].first;
            dp[i + 1] = max(dp[i + 1], dp[j] + sum * day - t);
 
        }
 
    }
 
    fout << dp[v.size()] << '\n';
 
    return 0;
 
}