#include <fstream>
#include <algorithm>
#include <cstring>
#include <vector>
 
using namespace std;
 
ifstream fin("harbingers.in");
ofstream fout("harbingers.out");
 
const int dim = 100005;
double eps = 1e-9;
typedef pair<int, int> pii;
typedef long long ll;
 
int n;
 
vector<pii> g[dim];
int s[dim], v[dim];
 
int head;
double p[dim];
int nodes[dim], d[dim];
ll sol[dim];
 
inline double getY(double x, int node) {
 
    return sol[node] - d[node] * x;
 
}
 
inline double getIntersect(int node1, int node2) {
 
    double x = (sol[node1] - sol[node2]) / (1.0 * (d[node1] - d[node2]));
    return x;
 
}
 
void dfs(int node, int parent) {
 
    int oldHead = 0, oldNode = 0;
    double oldP = 0;
 
    if (node == 1) {
 
        head = 1;
        p[1] = 0;
        nodes[1] = 1;
 
    }
    else {
 
        int l = 1, r = head, ans = 1;
        while (l <= r) {
            int m = (l + r) / 2;
            if (p[m] <= v[node] + eps)
                ans = m, l = m + 1;
            else
                r = m - 1;
        }
 
        int x = nodes[ans];
        sol[node] = sol[x] - 1LL * d[x] * v[node] + 1LL * d[node] * v[node] + s[node];
 
        l = 1, r = head, ans = head + 1;
        while (l <= r) {
            int m = (l + r) / 2;
            if (getY(p[m], nodes[m]) >= getY(p[m], node) - eps)
                ans = m, r = m - 1;
            else
                l = m + 1;
        }
 
        oldNode = nodes[ans];
        oldHead = head;
        oldP = p[ans];
 
        head = ans;
        nodes[head] = node;
        p[head] = (head == 1 ? 0 : getIntersect(nodes[head - 1], node));
 
    }
 
    for (pii adj : g[node]) {
 
        if (adj.first == parent)
            continue;
 
        d[adj.first] = d[node] + adj.second;
        dfs(adj.first, node);
 
    }
 
    if (node != 1) {
 
        nodes[head] = oldNode;
        p[head] = oldP;
        head = oldHead;
 
    }
 
}
 
int main() {
 
    fin >> n;
    for (int i = 1; i < n; ++i) {
 
        int x, y, c;
        fin >> x >> y >> c;
 
        g[x].push_back(pii(y, c));
        g[y].push_back(pii(x, c));
 
    }
 
    for (int i = 2; i <= n; ++i)
        fin >> s[i] >> v[i];
 
    dfs(1, 0);
 
    for (int i = 2; i <= n; ++i)
        fout << sol[i] << ' ';
    fout << '\n';
 
    return 0;
}