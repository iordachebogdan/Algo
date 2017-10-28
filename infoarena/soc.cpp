#include <fstream>
#include <algorithm>
#include <cstring>
 
#define DIM 300
#define infile "soc.in"
#define outfile "soc.out"
 
using namespace std;
 
ifstream fin(infile);
ofstream fout(outfile);
 
int n, m;
 
bool edge[DIM][DIM];
 
int E[DIM];
 
bool nodeSet[DIM], maxClique[DIM], maxIndepSet[DIM];
 
int maxCliqueCount, maxIndepSetCount;
 
void makeGraphComplement() {
 
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            if (i != j)
                edge[i][j] ^= 1;
 
}
 
void DFS(int node, bool nodeSet[], int conex[], int crt) {
 
    conex[node] = crt;
 
    for (int i = 1; i <= n; ++i) {
 
        if (!edge[node][i] || !nodeSet[i] || conex[i])
            continue;
 
        DFS(i, nodeSet, conex, crt);
 
    }
 
}
 
void solve(bool nodeSet[], bool maxClique[], bool maxIndepSet[], int &maxCliqueCount, int &maxIndepSetCount) {
 
    memset(maxClique, 0, sizeof maxClique);
    memset(maxIndepSet, 0, sizeof maxIndepSet);
 
    maxCliqueCount = maxIndepSetCount = 0;
 
    int inSetCount = 0;
 
    for (int i = 1; i <= n; ++i)
        inSetCount += nodeSet[i];
 
    if (inSetCount == 1) {
 
        for (int i = 1; i <= n; ++i) {
 
            if (nodeSet[i]) {
 
                maxClique[i] = maxIndepSet[i] = true;
 
                maxCliqueCount = maxIndepSetCount = E[i];
 
                break;
 
            }
 
        }
 
        return;
 
    }
 
    int conex[DIM], conexCount = 0;
 
    memset(conex, 0, sizeof conex);
 
    for (int i = 1; i <= n; ++i) {
 
        if (!nodeSet[i] || conex[i])
            continue;
 
        ++conexCount;
 
        DFS(i, nodeSet, conex, conexCount);
 
    }
 
    if (conexCount == 1) {
 
        makeGraphComplement();
 
        solve(nodeSet, maxIndepSet, maxClique, maxIndepSetCount, maxCliqueCount);
 
        makeGraphComplement();
 
        return;
 
    }
 
    bool newNodeSet[DIM], newMaxClique[DIM], newMaxIndepSet[DIM];
 
    int newMaxCliqueCount, newMaxIndepSetCount;
 
    for (int i = 1; i <= n; ++i) {
 
        if (!nodeSet[i] || !conex[i])
            continue;
 
        memset(newNodeSet, 0, sizeof newNodeSet);
        memset(newMaxClique, 0, sizeof newMaxClique);
        memset(newMaxIndepSet, 0, sizeof newMaxIndepSet);
 
        for (int j = i + 1; j <= n; ++j) {
 
            if (!nodeSet[j] || conex[j] != conex[i])
                continue;
 
            newNodeSet[j] = true;
 
            conex[j] = 0;
 
        }
 
        newNodeSet[i] = true;
 
        conex[i] = 0;
 
        solve(newNodeSet, newMaxClique, newMaxIndepSet, newMaxCliqueCount, newMaxIndepSetCount);
 
 
        maxIndepSetCount += newMaxIndepSetCount;
 
        for (int i = 1; i <= n; ++i)
            if (newMaxIndepSet[i])
                maxIndepSet[i] = true;
 
 
        if (newMaxCliqueCount > maxCliqueCount) {
 
            maxCliqueCount = newMaxCliqueCount;
 
            for (int i = 1; i <= n; ++i)
                maxClique[i] = newMaxClique[i];
 
        }
 
    }
 
}
 
int main() {
 
    fin >> n >> m;
 
    for (int i = 1; i <= n; ++i) {
 
        fin >> E[i];
 
        nodeSet[i] = true;
 
    }
 
    for (int i = 1; i <= m; ++i) {
 
        int x, y;
 
        fin >> x >> y;
 
        edge[x][y] = edge[y][x] = true;
 
    }
 
    solve(nodeSet, maxClique, maxIndepSet, maxCliqueCount, maxIndepSetCount);
 
    fout << maxCliqueCount << '\n';
 
    int nodesInMaxClique = 0;
 
    for (int i = 1; i <= n; ++i)
        nodesInMaxClique += maxClique[i];
 
    fout << nodesInMaxClique << '\n';
 
    for (int i = 1; i <= n; ++i)
        if (maxClique[i])
            fout << i << ' ';
 
    return 0;
 
}