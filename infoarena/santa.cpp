#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <stack>
 
using namespace std;
 
ifstream fin("santa.in");
ofstream fout("santa.out");
 
const int dim = 45005;
 
int n, m, elves, santa, maxDamage;
vector<int> g[dim];
 
bool mustVisit[dim], vis[dim];
 
void dfsMustVisit(int node) {
 
    if (node == santa)
        return;
 
    vis[node] = true;
    for (auto& adj : g[node]) {
 
        if (vis[adj])
            continue;
         
        dfsMustVisit(adj);
        mustVisit[node] |= mustVisit[adj];
 
    }
 
}
 
vector<int> critics;
vector< vector<int> > biconex;
stack<int> _stack;
int level[dim], low[dim];
 
void dfsBiconex(int node, int parent) {
 
    _stack.push(node);
    vis[node] = true;
    level[node] = low[node] = level[parent] + 1;
 
    for (auto& adj : g[node]) {
 
        if (adj == parent)
            continue;
        if (vis[adj]) {
            low[node] = min(low[node], level[adj]);
            continue;
        }
 
        dfsBiconex(adj, node);
        low[node] = min(low[node], low[adj]);
 
        if (level[node] <= low[adj]) {
 
            if (!mustVisit[adj]) {
                int x;
                do {
                    x = _stack.top();
                    _stack.pop();
                } while (x != adj);
                continue;
            }
 
            biconex.push_back(vector<int>(0));
            critics.push_back(node);
 
            int x;
            do {
                x = _stack.top();
                _stack.pop();
                biconex.back().push_back(x);
            } while (x != adj);
 
            biconex.back().push_back(node);
 
        }
 
    }
 
}
 
bool find(vector<int>& v, int val) {
 
    for (auto& it : v)
        if (it == val)
            return true;
    return false;
 
}
 
void noChance(void) {
 
    fout << "No chance\n";
    exit(0);
 
}
 
vector<int> path;
 
bool back(int k, int count, int cur, int finish) {
 
    vis[cur] = true;
    if (k) path.push_back(cur);
 
    if (k + 1 == count) {
         
        if (finish == 0)
            return true; //final node doesn't matter
        if (finish == cur)
            return true; //final node reached
 
    }
    else {
 
        for (auto& it : g[cur])
            if (!vis[it] && !(it == finish && k + 2 != count) && back(k + 1, count, it, finish))
                return true;
 
    }
 
    vis[cur] = false;
    if (k) path.pop_back();
    return false;
 
}
 
void solveBiconex(int biconexIndex, int start, int finish) {
 
    for (auto& it : biconex[biconexIndex])
        vis[it] = false;
 
    auto ok = back(0, (int)biconex[biconexIndex].size(), start, finish);
    if (!ok)
        noChance();
 
}
 
int main() {
 
    fin >> n >> m;
    for (int i = 1; i <= m; ++i) {
 
        int x, y; fin >> x >> y;
        g[x].push_back(y);
        g[y].push_back(x);
 
    }
 
    fin >> elves >> santa >> maxDamage;
 
    mustVisit[santa] = true;
    dfsMustVisit(elves);
    if (!mustVisit[elves])
        noChance();
 
    memset(vis, false, sizeof vis);
    critics.push_back(santa);
    dfsBiconex(elves, 0);
 
    if (!find(biconex[0], maxDamage)) {
 
        for (int i = 0, j = biconex.size() - 1; i < j; ++i, --j)
            swap(biconex[i], biconex[j]);
        for (int i = 0, j = critics.size() - 1; i < j; ++i, --j)
            swap(critics[i], critics[j]);
    }
 
    if (!find(biconex[0], maxDamage))
        noChance();
 
    critics[0] = maxDamage; //path starts from here
    critics.back() = 0; //path can finish anywhere
 
    path.push_back(maxDamage);
    for (int i = 0; i < (int)biconex.size(); ++i)
        solveBiconex(i, critics[i], critics[i + 1]);
 
    fout << path.size() << '\n';
    for (auto& it : path)
        fout << it << ' ';
    fout << '\n';
 
    return 0;
 
}