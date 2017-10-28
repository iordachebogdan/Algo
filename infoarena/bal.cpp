#include <fstream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <queue>
 
#define infile "bal.in"
#define outfile "bal.out"
 
std::vector<int> *edges;
 
std::vector< std::pair<int, int> > solution;
 
bool *deleted, *visited;
 
int *adjNodeCount, *frecv;
 
int main() {
 
    std::ifstream fin(infile);
    std::ofstream fout(outfile);
 
    int nodeCount, edgeCount;
 
    fin >> nodeCount >> edgeCount;
 
    nodeCount <<= 1;
 
    edges = new std::vector<int>[nodeCount + 2];
    adjNodeCount = new int[nodeCount + 3];
    frecv = new int[nodeCount + 3];
 
    for (int node = 1; node <= nodeCount; ++node)
        adjNodeCount[node] = frecv[node] = 0;
 
    for (int edgeIndex = 1; edgeIndex <= edgeCount; ++edgeIndex) {
 
        int boy, girl;
 
        fin >> boy >> girl;
 
        girl += nodeCount / 2;
 
        edges[boy].push_back(girl);
        edges[girl].push_back(boy);
 
        adjNodeCount[boy]++;
        adjNodeCount[girl]++;
 
    }
 
    for (int node = 1; node <= nodeCount; ++node) {
 
        for (std::vector<int>::iterator it = edges[node].begin(); it != edges[node].end(); ++it) {
 
            frecv[*it]++;
 
            if (frecv[*it] > 1)
                --adjNodeCount[node];
 
        }
 
        for (std::vector<int>::iterator it = edges[node].begin(); it != edges[node].end(); ++it) {
 
            frecv[*it]--;
 
        }
 
    }
 
    std::queue<int> queue;
 
    int remainingNodes = nodeCount;
 
    for (int node = 1; node <= nodeCount; ++node) {
 
        if (adjNodeCount[node] == 1) {
 
            queue.push(node);
 
        }
 
        if (adjNodeCount[node] == 0) {
 
            fout << "NU";
            return 0;
 
        }
 
    }
 
    deleted = new bool[nodeCount + 3];
    visited = new bool[nodeCount + 3];
 
    std::memset(deleted, false, nodeCount + 2);
    std::memset(visited, false, nodeCount + 2);
 
    while (!queue.empty()) {
 
        int currNode = queue.front();
 
        queue.pop();
 
        if (deleted[currNode])
            continue;
 
        int adjacentNode;
 
        for (std::vector<int>::iterator it = edges[currNode].begin(); it != edges[currNode].end(); ++it) {
 
            if (!deleted[*it]) {
 
                adjacentNode = *it;
                break;
 
            }
 
        }
 
        deleted[currNode] = deleted[adjacentNode] = true;
 
        remainingNodes -= 2;
 
        solution.push_back(std::make_pair(std::min(currNode, adjacentNode), std::max(currNode, adjacentNode) - nodeCount / 2));
 
        for (std::vector<int>::iterator it = edges[adjacentNode].begin(); it != edges[adjacentNode].end(); ++it) {
 
            if (deleted[*it] || visited[*it])
                continue;
 
            visited[*it] = true;
 
            adjNodeCount[*it]--;
 
            if (adjNodeCount[*it] == 1) {
 
                queue.push(*it);
 
            }
 
            if (adjNodeCount[*it] == 0) {
 
                fout << "NU";
                return 0;
 
            }
 
        }
 
        for (std::vector<int>::iterator it = edges[adjacentNode].begin(); it != edges[adjacentNode].end(); ++it) {
 
            visited[*it] = false;
 
        }
 
    }
 
    if (remainingNodes > 0) {
 
        fout << "NU";
        return 0;
 
    }
 
    fout << "DA\n";
 
    std::sort(solution.begin(), solution.end());
 
    for (std::vector< std::pair<int, int> >::iterator it = solution.begin(); it != solution.end(); ++it)
        fout << it->second << '\n';
 
    return 0;
 
}