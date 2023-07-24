#include <iostream>
#include <vector>
#include <unordered_map>
#include<queue>
#include <limits.h>

using namespace std;



// Function to perform BFS and calculate the number of shortest paths and dependencies for a single source node
void bfs(const unordered_map<int, vector<int>>& graph, int source, vector<int>& numShortestPaths, vector<double>& dependency) {
    queue<int> q;
    vector<int> distance(graph.size(), -1);
    vector<int> numPaths(graph.size(), 0);

    q.push(source);
    distance[source] = 0;
    numPaths[source] = 1;

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (int neighbor : graph.at(node)) {
            if (distance[neighbor] == -1) {
                distance[neighbor] = distance[node] + 1;
                q.push(neighbor);
            }

            if (distance[neighbor] == distance[node] + 1) {
                // Increment the number of shortest paths passing through the current node
                numPaths[neighbor] += numPaths[node];
            }
        }
    }

    vector<double> delta(graph.size(), 0.0);
    for (int i = graph.size() - 1; i >= 0; --i) {
        int node = i;
        for (int neighbor : graph.at(node)) {
            if (distance[node] + 1 == distance[neighbor]) {
                delta[node] += (static_cast<double>(numPaths[node]) / numPaths[neighbor]) * (1 + delta[neighbor]);
            }
        }

        if (node != source) {
            dependency[node] += delta[node];
        }
    }

    numShortestPaths = numPaths;
}


// Function to perform BFS and calculate the sum of distances for a single source node
int bfs2(const unordered_map<int, vector<int>>& graph, int source) {
    queue<int> q;
    vector<int> distance(graph.size(), INT_MAX);

    q.push(source);
    distance[source] = 0;

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (int neighbor : graph.at(node)) {
            if (distance[neighbor] == INT_MAX) {
                distance[neighbor] = distance[node] + 1;
                q.push(neighbor);
            }
        }
    }

    int sumOfDistances = 0;
    for (int dist : distance) {
        if (dist != INT_MAX) {
            sumOfDistances += dist;
        }
    }

    return sumOfDistances;
}




// Function to calculate Degree Centrality of nodes in the graph
vector<double> calculateDegreeCentrality(const unordered_map<int, vector<int>>& graph) {
    vector<double> centrality(graph.size(), 0.0);

    for (const auto& entry : graph) {
        centrality[entry.first] = static_cast<double>(entry.second.size());
    }

    return centrality;
}

vector<double> calculateBetweennessCentrality(const unordered_map<int, vector<int>>& graph) {
    vector<double> betweennessCentrality(graph.size(), 0.0);

    for (const auto& entry : graph) {
        int source = entry.first;
        vector<int> numShortestPaths(graph.size(), 0);
        vector<double> dependency(graph.size(), 0.0);

        // Perform BFS from the current node to find the number of shortest paths and dependencies
        bfs(graph, source, numShortestPaths, dependency);

        // Update the betweenness centrality for all nodes based on the results of BFS
        for (int i = 0; i < graph.size(); ++i) {
            if (i != source) {
                betweennessCentrality[i] += dependency[i];
            }
        }
    }

    return betweennessCentrality;
}


// Function to calculate Closeness Centrality of nodes in the graph
vector<double> calculateClosenessCentrality(const unordered_map<int, vector<int>>& graph) {
    vector<double> closenessCentrality(graph.size(), 0.0);

    for (const auto& entry : graph) {
        int source = entry.first;
        int sumOfDistances = bfs2(graph, source);

        // Calculate the Closeness Centrality for the current node based on the sum of distances
        if (sumOfDistances > 0 && sumOfDistances != INT_MAX) {
            closenessCentrality[source] = static_cast<double>(graph.size() - 1) / sumOfDistances;
        } else {
            closenessCentrality[source] = 0.0; // If the node is not reachable from the source, set closeness centrality to 0
        }
    }

    return closenessCentrality;
}
int main() {
    int numNodes, numEdges;
    cout << "Enter the number of nodes and edges in the social network: ";
    cin >> numNodes >> numEdges;

    if (numNodes <= 0 || numEdges <= 0) {
        cout << "Invalid input. The number of nodes and edges must be positive integers." << endl;
        return 1;
    }

    unordered_map<int, vector<int>> graph;

    cout << "Enter the edges (node1 node2) in the social network:" << endl;
    for (int i = 0; i < numEdges; ++i) {
        int node1, node2;
        cin >> node1 >> node2;
        graph[node1].push_back(node2);
        graph[node2].push_back(node1); // Assuming an undirected graph
    }

    // Calculate Degree Centrality
    vector<double> degreeCentrality = calculateDegreeCentrality(graph);

    // Calculate Betweenness Centrality
     vector<double> betweennessCentrality = calculateBetweennessCentrality(graph);

    // // Calculate Closeness Centrality
     vector<double> closenessCentrality = calculateClosenessCentrality(graph);

    // Find the most influential user (highest degree centrality)
    double maxDegreeCentrality = 0.0;
    int mostInfluentialUserByDegree = -1;
    for (int i = 0; i < numNodes; ++i) {
        if (degreeCentrality[i] > maxDegreeCentrality) {
            maxDegreeCentrality = degreeCentrality[i];
            mostInfluentialUserByDegree = i;
        }
    }

    // Find the most influential user (highest betweenness centrality)
    double maxBetweennessCentrality = 0.0;
    int mostInfluentialUserByBetweenness = -1;
    for (int i = 0; i < numNodes; ++i) {
        if (betweennessCentrality[i] > maxBetweennessCentrality) {
            maxBetweennessCentrality = betweennessCentrality[i];
            mostInfluentialUserByBetweenness = i;
        }
    }

    // Find the most influential user (highest closeness centrality)
    double maxClosenessCentrality = 0.0;
    int mostInfluentialUserByCloseness = -1;
    for (int i = 0; i < numNodes; ++i) {
        if (closenessCentrality[i] > maxClosenessCentrality) {
            maxClosenessCentrality = closenessCentrality[i];
            mostInfluentialUserByCloseness = i;
        }
    }

    if (mostInfluentialUserByDegree != -1) {
        cout << "The most influential user by Degree Centrality is: User " << mostInfluentialUserByDegree << endl;
        cout << "Degree Centrality: " << maxDegreeCentrality << endl;
    } else {
        cout << "No influential user found by Degree Centrality." << endl;
    }

    if (mostInfluentialUserByBetweenness != -1) {
        cout << "The most influential user by Betweenness Centrality is: User " << mostInfluentialUserByBetweenness << endl;
        cout << "Betweenness Centrality: " << maxBetweennessCentrality << endl;
    } else {
        cout << "No influential user found by Betweenness Centrality." << endl;
    }

    if (mostInfluentialUserByCloseness != -1) {
        cout << "The most influential user by Closeness Centrality is: User " << mostInfluentialUserByCloseness << endl;
        cout << "Closeness Centrality: " << maxClosenessCentrality << endl;
    } else {
        cout << "No influential user found by Closeness Centrality." << endl;
    }

    return 0;
}
//1 2
// 1 3
// 2 3
// 4 5