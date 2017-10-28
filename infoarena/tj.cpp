#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
 
#define infile "tj.in"
#define outfile "tj.out"
 
//saves the data of a test case
class Test {
 
private:
 
    int nodeCount;
    int edgeCount;
 
    int degree[260];
 
    std::vector<int> adjacentNodes;
 
    bool edges[260][260];
 
    std::vector<int> nodeList;
 
public:
 
    //the Test constructor
    Test(int nodeCount, int edgeCount) {
 
        this->nodeCount = nodeCount;
        this->edgeCount = edgeCount;
 
        memset(edges, false, sizeof edges);
        memset(degree, 0, sizeof edges);
 
        nodeList.resize(nodeCount + 5);
 
        //at first in nodeList we have all the nodes
        for (int nodeIndex = 1; nodeIndex <= nodeCount; ++nodeIndex) {
 
            nodeList[nodeIndex] = nodeIndex;
 
        }
 
    }
 
    //returns the number of nodes
    int getNodeCount() {
 
        return nodeCount;
 
    }
 
    //returns the number of edges
    int getEdgeCount() {
 
        return edgeCount;
 
    }
 
    //adds an undirected edge between 2 nodes
    void addEdge(int firstNode, int secondNode) {
 
        edges[firstNode][secondNode] = edges[secondNode][firstNode] = true;
 
    }
 
    //returns true if there is an edge between the 2 nodes or false otherwise
    bool checkEdge(int firstNode, int secondNode) {
 
        return edges[firstNode][secondNode];
 
    }
 
    //increases the degree of a node
    void increaseDegree(int node) {
 
        degree[node]++;
 
    }
     
    //returns the degree of a node
    int getDegree(int node) {
 
        return degree[node];
 
    }
 
    //decreases the degree of a node
    void decreaseDegree(int node) {
 
        degree[node]--;
 
    }
 
    //returns the node at a specified position in the nodeList
    int getNode(int nodeIndex) {
 
        return nodeList[nodeIndex];
 
    }
 
    //returns an array with all the adjacent nodes with node
    int *getAdjacentNodes(int node) {
 
        adjacentNodes.clear();
 
        for (int index = 1; index <= nodeCount; ++index) {
 
            //if index is adjacent with node we put it in the vector
            if (edges[node][nodeList[index]]) {
 
                adjacentNodes.push_back(nodeList[index]);
 
            }
 
        }
 
        if (adjacentNodes.empty()) {
 
            return NULL;
 
        }
        else {
 
            return &adjacentNodes[0];
 
        }
 
    }
 
    int getAdjacentNodeCount(int node) {
 
        int adjacentNodeCount = 0;
 
        for (int index = 1; index <= nodeCount; ++index) {
 
            //if index is adjacent with node we put it in the vector
            if (edges[node][nodeList[index]]) {
 
                ++adjacentNodeCount;
 
            }
 
        }
 
        return adjacentNodeCount;
 
    }
 
    //deletes a node from the list and updates the edges
    void deleteNode(int nodeToDelete) {
 
        //deletes the node drom the list
        for (std::vector<int>::iterator nodeIterator = nodeList.begin(); nodeIterator != nodeList.end(); ++nodeIterator) {
 
            if (*nodeIterator == nodeToDelete) {
 
                nodeList.erase(nodeIterator);
 
                break;
 
            }
 
        }
 
        //deletes the edges formed with the node that's being deleted
        for (int index = 1; index <= nodeCount; ++index) {
 
            if (edges[nodeList[index]][nodeToDelete]) {
 
                edges[nodeList[index]][nodeToDelete] = edges[nodeToDelete][nodeList[index]] = false;
 
                --edgeCount;
 
            }
 
        }
 
        --nodeCount;
 
    }
 
};
 
//solves the problem "tj"
class TomJerry {
 
private:
 
    Test *currentTest;
 
    std::vector<bool> solutions;
 
    int commonAdjacentNodes[260][260];
 
    //solves a test case
    bool solveCurrentTest() {
 
        int nodeCount = currentTest->getNodeCount();
 
        memset(commonAdjacentNodes, 0, sizeof commonAdjacentNodes);
 
        //calculates the number of common adjacent nodes between firstNode and secondNode
        for (int firstNodeIndex = 1; firstNodeIndex <= nodeCount; ++firstNodeIndex) {
 
            for (int secondNodeIndex = firstNodeIndex + 1; secondNodeIndex <= nodeCount; ++secondNodeIndex) {
 
                if (firstNodeIndex == secondNodeIndex)
                    continue;
 
                int firstNode = currentTest->getNode(firstNodeIndex);
 
                int secondNode = currentTest->getNode(secondNodeIndex);
 
                if (!currentTest->checkEdge(firstNode, secondNode))
                    continue;
 
                int adjacentNodeCount = currentTest->getAdjacentNodeCount(firstNode);
 
                int *adjacentNodes = currentTest->getAdjacentNodes(firstNode);
 
                for (int index = 0; index < adjacentNodeCount; ++index) {
 
                    if (currentTest->checkEdge(adjacentNodes[index], secondNode)) {
 
                        commonAdjacentNodes[firstNode][secondNode]++;
                        commonAdjacentNodes[secondNode][firstNode]++;
 
                    }
 
                }
 
            }
 
        }
 
        while (nodeCount >= 2) {
 
            int nodeToDelete = 0;
 
            for (int currentNodeIndex = 1; currentNodeIndex <= nodeCount && !nodeToDelete; ++currentNodeIndex) {
 
                int currentNode = currentTest->getNode(currentNodeIndex);
 
                int *adjacentNodes = currentTest->getAdjacentNodes(currentNode);
 
                int adjacentNodeCount = currentTest->getAdjacentNodeCount(currentNode);
 
                for (int index = 0; index < adjacentNodeCount; ++index) {
 
                    int adjacentNode = adjacentNodes[index];
 
                    if (currentTest->getDegree(currentNode) - 1 == commonAdjacentNodes[currentNode][adjacentNode]) {
 
                        nodeToDelete = currentNode;
 
                        break;
 
                    }
 
                }
 
            }
 
            if (!nodeToDelete) {
 
                return false;
 
            }
 
            int *adjacentNodes = currentTest->getAdjacentNodes(nodeToDelete);
 
            int adjacentNodeCount = currentTest->getAdjacentNodeCount(nodeToDelete);
 
            for (int index = 0; index < adjacentNodeCount; ++index) {
 
                int adjacentNode = adjacentNodes[index];
 
                currentTest->decreaseDegree(adjacentNode);
 
            }
 
            for (int index1 = 0; index1 < adjacentNodeCount; ++index1) {
 
                for (int index2 = index1 + 1; index2 < adjacentNodeCount; ++index2) {
 
                    int adjacentNode1 = adjacentNodes[index1];
                    int adjacentNode2 = adjacentNodes[index2];
 
                    if (currentTest->checkEdge(adjacentNode1, adjacentNode2)) {
 
                        commonAdjacentNodes[adjacentNode1][adjacentNode2]--;
                        commonAdjacentNodes[adjacentNode2][adjacentNode1]--;
 
                    }
 
                }
 
            }
 
            nodeCount--;
 
            currentTest->deleteNode(nodeToDelete);
 
        }
 
        return true;
    }
 
public:
 
    //loads the tests from a specified path
    std::vector<Test*> loadTests(std::string path) {
 
        std::ifstream file(path);
 
        std::vector<Test*> tests;
 
        int testCount;
 
        file >> testCount;
 
        while (testCount--) {
 
            int nodeCount, edgeCount;
 
            file >> nodeCount >> edgeCount;
 
            tests.push_back(new Test(nodeCount, edgeCount));
 
            for (int edgeIndex = 1; edgeIndex <= edgeCount; ++edgeIndex) {
 
                int firstNode, secondNode;
 
                file >> firstNode >> secondNode;
 
                tests.back()->addEdge(firstNode, secondNode);
 
                tests.back()->increaseDegree(firstNode);
                tests.back()->increaseDegree(secondNode);
 
            }
 
        }
 
        return tests;
     
    }
 
    void solve(std::vector<Test*> tests) {
 
        for (Test *test : tests) {
 
            currentTest = test;
 
            bool resultOfCurrentTest = solveCurrentTest();
 
            if (resultOfCurrentTest == true) {
 
                solutions.push_back(true);
 
            }
            else {
 
                solutions.push_back(false);
 
            }
 
        }
 
    }
 
    //writes the solutions to a specified path
    void writeSolutions(std::string path) {
 
        std::ofstream file(path);
 
        for (bool solution : solutions)
            file << (solution ? "DA" : "NU") << '\n';
 
    }
 
};
 
 
int main() {
 
    //creates an instance of the TomJerry class
    TomJerry tomJerry;
 
    //loads the tests
    std::vector<Test*> tests = tomJerry.loadTests(infile);
 
    //solves the problem
    tomJerry.solve(tests);
 
    //writes the solutions
    tomJerry.writeSolutions(outfile);
 
    return 0;
 
}
 