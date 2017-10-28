#include <fstream>
#include <cstring>
#include <vector>
#include <queue>
 
#define infile "drumuri2.in"
#define outfile "drumuri2.out"
 
//keeps flow-capacity data of every edge
class FlowCapacityManager {
 
private:
 
    int capacity[205][205];
    int flow[205][205];
 
public:
 
    //deletes all saved/junk data
    void initialize() {
 
        memset(capacity, 0, sizeof capacity);
        memset(flow, 0, sizeof flow);
 
    }
 
    //sets the capacity value on an edge
    void setCapacity(int outNode, int inNode, int capacityValue) {
 
        capacity[outNode][inNode] = capacityValue;
 
    }
 
    //sets the flow value on an edge
    void setFlow(int outNode, int inNode, int flowValue) {
 
        flow[outNode][inNode] = flowValue;
 
    }
 
    //returns the capacity of a specified edge
    int getCapacity(int outNode, int inNode) {
 
        return capacity[outNode][inNode];
 
    }
 
    //returns the flow of a specified edge
    int getFlow(int outNode, int inNode) {
 
        return flow[outNode][inNode];
 
    }
 
    //increases the flow on an edge
    void increaseFlow(int outNode, int inNode) {
 
        flow[outNode][inNode]++;
 
    }
 
    //decreases the flow on an edge
    void decreaseFlow(int outNode, int inNode) {
 
        flow[outNode][inNode]--;
 
    }
 
};
 
//keeps data showing if a node is visited or not
class VisitManager{
 
private:
 
    bool visitManager[205];
 
public:
 
    //sets all nodes as unvisited
    void initialize() {
 
        memset(visitManager, false, sizeof visitManager);
 
    }
 
    //shows if a node is visited or not
    bool isVisited(int node) {
 
        return visitManager[node];
 
    }
 
    //marks a node as visited
    void visit(int node) {
 
        visitManager[node] = true;
 
    }
 
};
 
//contains data of a testcase
//used by Drumuri2 class
class Test {
 
private:
 
    int nodeCount;
    int edgeCount;
    std::vector<int> *edges, *realEdges;
 
public:
 
    Test(int nodeCount, int edgeCount) {
 
        this->nodeCount = nodeCount;
        this->edgeCount = edgeCount;
 
        //edges[x] and realEdges[x] contains all adjacent nodes whith x
        this->edges = new std::vector<int>[2 * nodeCount + 5];
        this->realEdges = new std::vector<int>[nodeCount + 5];
 
    }
 
    //returns the number of nodes
    int getNodeCount() {
 
        return nodeCount;
 
    }
 
    //returns the number of edges
    int getEdgeCount() {
 
        return edgeCount;
 
    }
 
    //adds a real edge
    void addRealEdge(int outNode, int inNode) {
 
        realEdges[outNode].push_back(inNode);
 
    }
 
    //adds an edge
    void addEdge(int outNode, int inNode) {
 
        edges[outNode * 2 + 1].push_back(inNode * 2);
        edges[inNode * 2].push_back(outNode * 2 + 1);
 
        //the nodes are doubles x*2 is the in-path of the node and x*2+1 is the out-path
 
    }
 
    //adds a source edge
    void addSourceEdge(int source, int node) {
 
        edges[source].push_back(node);
        edges[node].push_back(source);
 
    }
 
    //adds a destination edge
    void addDestinationEdge(int node, int destination) {
 
        edges[destination].push_back(node);
        edges[node].push_back(destination);
 
    }
 
    //returns an array with all adjacent nodes with currentNode
    int *getAdjacentNodes(int currentNode) {
 
        if (edges[currentNode].empty()) {
 
            return NULL;
 
        }
        else {
 
            return &edges[currentNode][0];
 
        }
 
    }
 
    //return the number of adjacent nodes with currentNode
    int getAdjacentNodesCount(int currentNode) {
 
        return edges[currentNode].size();
 
    }
 
    //returns an array with all real adjacent nodes with currentNode
    int *getRealAdjacentNodes(int currentNode) {
 
        if (realEdges[currentNode].empty()) {
 
            return NULL;
 
        }
        else {
 
            return &realEdges[currentNode][0];
 
        }
 
    }
 
    //return the number of real adjacent nodes with currentNode
    int getRealAdjacentNodesCount(int currentNode) {
 
        return realEdges[currentNode].size();
 
    }
 
};
 
 
class Drumuri2 {
 
private:
 
    std::vector<int> solutions;
    Test* currentTest;
    VisitManager visitManager;
    FlowCapacityManager flowCapacityManager;
 
    //sets a basic flow graph configuration
    void setBasicFlowConfig() {
 
        int nodeCount = currentTest->getNodeCount() * 2 + 3;
 
        int realNodeCount = currentTest->getNodeCount();
 
        int source = nodeCount - 1, destination = nodeCount;
 
        flowCapacityManager.initialize();
 
        for (int nodeIndex = 1; nodeIndex <= realNodeCount; ++nodeIndex) {
 
            int *adjacentNodes = currentTest->getRealAdjacentNodes(nodeIndex);
 
            int adjacentNodeCount = currentTest->getRealAdjacentNodesCount(nodeIndex);
 
            for (int adjacentNodeIndex = 0; adjacentNodeIndex < adjacentNodeCount; ++adjacentNodeIndex) {
 
                flowCapacityManager.setCapacity(nodeIndex * 2 + 1, adjacentNodes[adjacentNodeIndex] * 2, realNodeCount);
 
            }
 
        }
 
        for (int nodeIndex = 1; nodeIndex <= realNodeCount; ++nodeIndex) {
 
            flowCapacityManager.setCapacity(nodeIndex * 2, nodeIndex * 2 + 1, realNodeCount);
 
            flowCapacityManager.setCapacity(source, nodeIndex * 2, realNodeCount);
            flowCapacityManager.setFlow(source, nodeIndex * 2, 1);
            flowCapacityManager.setFlow(nodeIndex * 2, source, -1);
 
            //add the source-node edge
            currentTest->addSourceEdge(source, nodeIndex * 2);
 
            flowCapacityManager.setCapacity(nodeIndex * 2 + 1, destination, realNodeCount);
            flowCapacityManager.setFlow(nodeIndex * 2 + 1, destination, 1);
            flowCapacityManager.setFlow(destination, nodeIndex * 2 + 1, -1);
 
            //add the node-destination edge
            currentTest->addDestinationEdge(nodeIndex * 2 + 1, destination);
 
        }
 
    }
 
    //Tries to pump back the flow from destination to source
    bool reduceFlow() {
 
        int nodeCount = currentTest->getNodeCount() * 2 + 3;
 
        int realNodeCount = currentTest->getNodeCount();
 
        int source = nodeCount - 1, destination = nodeCount;
 
        std::queue<int> que;
 
        que.push(destination);
 
        visitManager.initialize();
 
        visitManager.visit(destination);
 
        int parent[205];
 
        memset(parent, 0, sizeof parent);
 
        //do a BFS of the Graph
        for (; !que.empty(); que.pop()) {
 
            int currentNode = que.front();
 
            int *adjacentNodes = currentTest->getAdjacentNodes(currentNode);
 
            int adjacentNodeCount = currentTest->getAdjacentNodesCount(currentNode);
 
            //checks all adjacent nodes
            for (int adjacentNodeIndex = 0; adjacentNodeIndex < adjacentNodeCount; ++adjacentNodeIndex) {
 
                int adjacentNode = adjacentNodes[adjacentNodeIndex];
 
                //verifis the status of the adjacent node and check if the edge is saturated
                if (!visitManager.isVisited(adjacentNode) && flowCapacityManager.getCapacity(currentNode, adjacentNode) > flowCapacityManager.getFlow(currentNode, adjacentNode)) {
 
                    //adds the node to the queue and save the node we came from
                    parent[adjacentNode] = currentNode;
 
                    que.push(adjacentNode);
 
                    //makes sure this node won't be checked again
                    visitManager.visit(adjacentNode);
 
                }
 
            }
 
        }
 
        //we couldn't find a new path
        if (!visitManager.isVisited(source)) {
 
            return false;
 
        }
 
        //increase the flow on the edges of the path and decrease it on the back-edges
        for (int currentNode = source; parent[currentNode]; currentNode = parent[currentNode]) {
 
            flowCapacityManager.increaseFlow(parent[currentNode], currentNode);
            flowCapacityManager.decreaseFlow(currentNode, parent[currentNode]);
 
        }
 
        return true;
 
    }
 
    //Finds the minimum flow such that every node is visited
    int calculateMinimumFlow() {
 
        setBasicFlowConfig();
 
        int minFlow;
 
        //reduce the flow quantity as much as possible
        for (minFlow = currentTest->getNodeCount(); reduceFlow(); --minFlow);
 
        return minFlow;
 
    }
 
public:
 
    //loads the test/tests from a specified path
    std::vector<Test*> loadTests(std::string path) {
 
        std::ifstream file(path);
        std::vector<Test*> tests;
 
        int nodeCount, edgeCount;
 
        int testIndex = -1;
 
        //reads the number of nodes and edges, respectively
        while (file >> nodeCount >> edgeCount) {
 
            //a new test instance
            ++testIndex;
 
            //creates a test instance
            tests.push_back(new Test(nodeCount, edgeCount));
 
            for (int edgeIndex = 1; edgeIndex <= edgeCount; ++edgeIndex) {
 
                int inNode, outNode;
 
                file >> outNode >> inNode;
 
                tests[testIndex]->addRealEdge(outNode, inNode);
 
                tests[testIndex]->addEdge(outNode, inNode);
 
            }
 
            for (int nodeIndex = 1; nodeIndex <= nodeCount; ++nodeIndex)
                tests[testIndex]->addEdge(nodeIndex, nodeIndex);
 
        }
 
        return tests;
 
    }
 
    //solve a specified test
    void solveTestCase(Test *test) {
 
        currentTest = test;
 
        int solution = calculateMinimumFlow();
 
        solutions.push_back(solution);
 
    }
 
    //solve an array of tests
    void solve(std::vector<Test*> tests) {
 
        for (Test *test : tests) {
 
            solveTestCase(test);
 
        }
 
    }
 
    //write the solutions to a specified path
    void writeSolution(std::string path) {
 
        std::ofstream file(path);
 
        for (int solution : solutions)
            file << solution << '\n';
 
    }
 
};
 
 
int main() {
 
    //creates an instance of Drumuri2 class
    Drumuri2 drumuri2;
 
    //gets the tests
    std::vector<Test*> tests = drumuri2.loadTests(infile);
 
    //solves the tests
    drumuri2.solve(tests);
 
    //writes the solution
    drumuri2.writeSolution(outfile);
 
    return 0;
 
}