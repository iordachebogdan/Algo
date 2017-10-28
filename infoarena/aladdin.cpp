#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>
#include <stack>
 
using namespace std;
 
ifstream fin("aladdin.in");
ofstream fout("aladdin.out");
 
const int dim = 1005;
 
int n, m, nodes;
int sum2x2[dim][dim], init[dim][dim];
 
inline int SetSign(int value, int sign) {
    return (sign & 1 ? -value : value);
}
 
inline int Not(int node) {
    return (node <= nodes ? node + nodes : node - nodes);
}
 
vector<int> graph[4 * dim];
 
void ResetGraph(void) {
 
    for (int i = 1; i <= 2 * nodes; ++i)
        graph[i].clear();
 
}
 
void MakeOrEdges(int node1, int node2) {
 
    graph[Not(node1)].push_back(node2);
    graph[Not(node2)].push_back(node1);
 
}
 
void BuildGraph(int matCorner) {
 
    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < m; ++j) {
            for (int rowVal : {0, 1}) {
                for (int colVal : {0, 1}) {
 
                    int matIJ = init[i][j] + SetSign(rowVal, j) + SetSign(colVal, i) + SetSign(matCorner, i + j + 1);
                    if (matIJ < 0 || matIJ > 1)
                        MakeOrEdges(colVal ? Not(j + n - 1) : j + n - 1, rowVal ? Not(i) : i);
 
                }
            }
        }
    }
 
}
 
bool noSol;
int level[4 * dim], low[4 * dim], currLevel;
stack<int> st;
set<int> inScc;
 
int v[4 * dim];
 
void DFS(int node) {
 
    level[node] = low[node] = ++currLevel;
    st.push(node);
 
    for (auto& adj : graph[node]) {
 
        if (!level[adj])
            DFS(adj);
        if (level[adj] > 0)
            low[node] = min(low[node], low[adj]);
 
    }
 
    if (low[node] == level[node]) {
 
        int curr;
        inScc.clear();
 
        do {
 
            curr = st.top();
            st.pop();
 
            level[curr] *= -1;
 
            if (inScc.find(Not(curr)) != inScc.end())
                noSol = true;
 
            inScc.insert(curr);
            if (v[curr] == -1) {
                v[curr] = 1;
                v[Not(curr)] = 0;
            }
 
 
        } while (curr != node);
 
    }
 
}
 
void Solve2SAT(void) {
 
    currLevel = 0;
    memset(level, 0, sizeof level);
    memset(low, 0, sizeof low);
    memset(v, -1, sizeof v);
    noSol = false;
 
    for (int i = 1; i <= 2 * nodes; ++i) {
        if (level[i] == 0)
            DFS(i);
        if (noSol)
            break;
    }
 
}
 
int mat[dim][dim];
 
void Solve(int matCorner) {
 
    ResetGraph();
    BuildGraph(matCorner);
    Solve2SAT();
 
    if (noSol)
        return;
 
    mat[0][0] = matCorner;
    for (int i = 1; i < m; ++i)
        mat[0][i] = v[i + n - 1];
    for (int i = 1; i < n; ++i) {
 
        mat[i][0] = v[i];
        for (int j = 1; j < m; ++j)
            mat[i][j] = init[i][j] + SetSign(v[i], j) + SetSign(v[j + n - 1], i) + SetSign(matCorner, i + j + 1);
 
    }
 
    for (int i = 0; i < n; ++i, fout << '\n')
        for (int j = 0; j < m; ++j)
            fout << mat[i][j] << ' ';
 
    exit(0);
 
}
 
int main() {
 
    fin >> n >> m;
    nodes = n + m - 2;
 
    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < m; ++j) {
            fin >> sum2x2[i][j];
            init[i][j] = sum2x2[i][j] - init[i - 1][j] - init[i][j - 1] - init[i - 1][j - 1];
        }
    }
 
    Solve(0);
    Solve(1);   
 
    fout << "-1\n";
 
    return 0;
 
}
 