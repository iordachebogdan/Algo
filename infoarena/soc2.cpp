#include <fstream>
#include <algorithm>
#include <vector>
#include <algorithm>
#include <bitset>
 
using namespace std;
 
ifstream fin("soc2.in");
ofstream fout("soc2.out");
 
const int dim = 260;
 
int n, m;
bitset<dim> g[dim];
 
int w[dim], common[dim][dim], nodes[dim];
 
struct TreeNode {
 
    bitset<dim> clique;
    bitset<dim> *sol[dim];
    int dp[dim];
    int newNode;
    vector<int> sons;
 
} tree[dim];
 
void dfs(int node) {
 
    for (int son : tree[node].sons)
        dfs(son);
 
    for (int i = 1; i <= n; ++i) {
 
        if (!tree[node].clique[i])
            continue;
 
        tree[node].sol[i] = new bitset < dim > ;
        tree[node].sol[i]->reset();
 
        int cost = w[i];
        bitset<dim> curSol;
        curSol.reset();
        curSol[i] = 1;
 
        for (int son : tree[node].sons) {
 
            int newNode = tree[son].newNode;
            if (tree[son].clique[i]) {
                cost += tree[son].dp[i] - w[i];
                curSol |= *(tree[son].sol[i]);
            }
            else {
 
                if (tree[son].dp[0] > tree[son].dp[newNode]) {
                    cost += tree[son].dp[0];
                    curSol |= *(tree[son].sol[0]);
                }
                else {
                    cost += tree[son].dp[newNode];
                    curSol |= *(tree[son].sol[newNode]);
                }
 
            }
 
        }
 
        tree[node].dp[i] = cost;
        *tree[node].sol[i] = curSol;
 
    }
 
    tree[node].sol[0] = new bitset < dim >;
    tree[node].sol[0]->reset();
 
    int cost = 0;
    bitset<dim> curSol;
    curSol.reset();
     
    for (int son : tree[node].sons) {
 
        int newNode = tree[son].newNode;
        if (tree[son].dp[0] > tree[son].dp[newNode]) {
            cost += tree[son].dp[0];
            curSol |= *(tree[son].sol[0]);
        }
        else {
            cost += tree[son].dp[newNode];
            curSol |= *(tree[son].sol[newNode]);
        }
 
    }
 
    tree[node].dp[0] = cost;
    *tree[node].sol[0] = curSol;
 
}
 
int main() {
 
    fin >> n >> m;
    for (int i = 1; i <= n; ++i)
        fin >> w[i];
 
    for (int i = 1; i <= m; ++i) {
 
        int x, y;
        fin >> x >> y;
 
        g[x][y] = g[y][x] = 1;
 
    }
 
    bitset<dim> aux;
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            aux = (g[i] & g[j]);
            common[i][j] = common[j][i] = aux.count();
        }
    }
 
    bitset<dim> vis;
    vis.reset();
    for (int step = n; step; --step) {
 
        int cur;
        for (cur = 1; cur <= n; ++cur) {
 
            if (vis[cur])
                continue;
            if (step == 1)
                break;
 
            int adjCount = 0, temp = 0;
            bool ok = true;
            for (int i = 1; i <= n; ++i) {
 
                if (vis[i] || !g[cur][i])
                    continue;
 
                if (!adjCount) {
                    adjCount = 1;
                    temp = common[i][cur];
                }
                else {
                    ++adjCount;
                    if (temp != common[i][cur]) {
                        ok = false;
                        break;
                    }
                }
 
            }
 
            if (ok && temp + 1 == adjCount)
                break;
 
        }
 
        vis[cur] = 1;
        nodes[step] = cur;
 
        for (int i = 1; i <= n; ++i)
            for (int j = i + 1; j <= n; ++j)
                if (cur != i && cur != j && !vis[i] && !vis[j] && g[i][cur] && g[j][cur])
                    common[i][j]--, common[j][i]--;
 
    }
 
    tree[1].newNode = nodes[1];
    tree[1].clique[nodes[1]] = 1;
    for (int step = 2; step <= n; ++step) {
 
        int cur = nodes[step];
        aux.reset();
 
        for (int i = 1; i < step; ++i)
            if (g[nodes[i]][cur])
                aux[nodes[i]] = 1;
 
        for (int i = 1; i < step; ++i) {
            if ((tree[i].clique & aux) == aux) {
 
                tree[i].sons.push_back(step);
                tree[step].clique = aux;
                tree[step].clique[cur] = 1;
                tree[step].newNode = cur;
                break;
 
            }
        }
 
    }
 
    dfs(1);
 
    int sol = 0, cur = 0;
    for (int i = 0; i <= n; ++i)
        if (sol < tree[1].dp[i])
            sol = tree[1].dp[i], cur = i;
 
    fout << sol << '\n';
 
    aux = *(tree[1].sol[cur]);
    int cnt = 0;
    for (int i = 1; i <= n; ++i)
        if (aux[i])
            ++cnt;
 
    fout << cnt << '\n';
    for (int i = 1; i <= n; ++i)
        if (aux[i])
            fout << i << ' ';
    fout << '\n';
 
    return 0;
 
}