#include <fstream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <deque>
 
using namespace std;
 
struct Pirate {
 
    int val;
    int cnt;
 
};
 
deque<Pirate> deq;
 
vector<int> a;
 
int main() {
 
    ifstream fin("caraibe.in");
    ofstream fout("caraibe.out");
 
    int n;
    fin >> n;
 
    a.resize(n + 1);
    for (int i = 1; i <= n; ++i)
        fin >> a[i];
 
    long long sol = 10000000000LL;
    deq.push_back({ 0, 1 });
 
    for (int i = n - 2; i; --i) {
 
        int cnt0 = (n - i - 1) - a[i];
 
        while (deq.size() > 1 && deq.back().cnt <= cnt0) {
 
            cnt0 -= deq.back().cnt;
            sol += 1LL * (deq.back().val - deq.front().val) * deq.back().cnt;
 
            deq.pop_back();
 
        }
 
        deq.back().cnt -= cnt0;
        sol += 1LL * (deq.back().val - deq.front().val) * cnt0;
 
        Pirate *aux = new Pirate;
        aux->val = deq.front().val - 1;
        aux->cnt = (n - i) - a[i];
 
        deq.push_front(*aux);
        sol -= a[i];
 
    }
 
    fout << sol << '\n';
 
    return 0;
 
}