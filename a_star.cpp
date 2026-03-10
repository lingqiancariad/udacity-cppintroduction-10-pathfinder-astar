#include <iostream>
#include <vector>
#include <queue>
#include <cmath>    // abs
#include <algorithm>
#include "a_star.hpp"

// Default grid dimensions: 4x4
int num_rows = 4;
int num_cols = 4;

// Minimal node model for this exercise
struct Node {
    int id;
    int row, col;
    int parentId = -1;

    int g = 0;   // cost-so-far
    int h = 0;   // heuristic-to-goal
    int f = 0;   // g + h

    std::vector<int> neighbors; // neighbor IDs
};

// Comparator: priority_queue returns the element with "smallest f" first
struct CompareF {
    bool operator()(const Node& a, const Node& b) const {
        return a.f > b.f; // min-heap by f
    }
};

// Manhattan distance
int heuristic(const Node& a, const Node& b) {
    return std::abs(a.row - b.row) + std::abs(a.col - b.col);
}

// Calculate id of node
int id_of(int r, int c) { return r*num_rows + c;}

// -----------------------------------------------------------------------------
// Assume: 'nodes' is already populated (ids, row/col, neighbors).
// Implement ONLY the main A* loop marked below.
// -----------------------------------------------------------------------------
int main() {
    int startId = 0;                // start node by default is 0
    int goalId  = startId + 1;       // goal node by default the last of nodes
    
    // The R"(...)" syntax defines a raw string. 
    // Everything inside the parentheses is printed exactly as typed.
    std::string intro = R"(
=====================================================
    A* PATHFINDING ALGORITHM MODULE            
=====================================================
Welcome! This program calculates the shortest path 
between two points on a 2D grid using the A* search.

ALGORITHM OVERVIEW:
- Uses the formula: f(n) = g(n) + h(n)
- g(n): Actual cost from the start to the current node.
- h(n): Estimated cost (Heuristic) to the goal.

Please prepare to enter 
1. grid dimensions 
2. start/end coordinates.
-----------------------------------------------------
)";
    std::cout << intro << std::endl;

    std::cout << ">>> Initializing A* Environment <<<\n";

    std::cout << "1. Map Size [Rows x Cols]: \n";
    std::cout << "Rows: ";
    std::cin >> num_rows;
    std::cout << "Cols: ";
    std::cin >> num_cols;

    int max_id = (num_rows * num_cols) - 1;

    std::cout << "2. Start Node ID [0 - " << max_id << "]: ";
    std::cin >> startId;

    std::cout << "3. Goal Node ID  [0 - " << max_id << "]: ";
    std::cin >> goalId;

    std::cout << "\nSetting up a " << num_rows << "x" << num_cols << " grid..." << std::endl;
    std::cout << "Pathfinding from Node " << startId << " to " << goalId << ".\n";

    std::vector<Node> nodes(num_rows*num_cols);   

    // ... populate nodes (ids 0..N-1, row/col, neighbors) before running ...
    int idex = 0;
    for (int r = 0; r < num_rows; r++) {
        for (int c = 0; c < num_cols; c++ ) {
            nodes[idex] = Node{ idex, r, c, -1, 0, 0, 0, {} };
            idex++;
        }
    }
    
    for (int r = 0; r < num_rows; r++) {
        for (int c = 0; c < num_cols; c++) {
            int id_current = id_of(r,c);
            if (r > 0) nodes[id_current].neighbors.push_back(id_of(r - 1, c));
            if (r < num_rows-1) nodes[id_current].neighbors.push_back(id_of(r + 1, c));
            if (c > 0) nodes[id_current].neighbors.push_back(id_of(r, c - 1));
            if (c < num_cols-1) nodes[id_current].neighbors.push_back(id_of(r, c + 1));
        }
    }

    // Safety: ensure there is some graph
    if (nodes.empty()) {
        std::cout << "Populate 'nodes' before running.\n";
        return 0;
    }   

    std::priority_queue<Node, std::vector<Node>, CompareF> openSet;
    std::vector<bool> closed(nodes.size(), false); // visited/closed

    // Initialize start
    nodes[startId].g = 0;
    nodes[startId].h = heuristic(nodes[startId], nodes[goalId]);
    nodes[startId].f = nodes[startId].g + nodes[startId].h;
    openSet.push(nodes[startId]);

    // ============================
    // TODO: MAIN A* SEARCH LOOP
    // ----------------------------
    // While the open set is not empty:
    //   1) Pop the node with the smallest f (call it 'current').
    //   2) If current.id == goalId:
    //        - Print "Goal reached!" and break.
    //   3) If current is already closed, continue.
    //   4) Mark current as closed.
    //   5) For each neighborId of current:
    //        - If neighbor is closed, continue.
    //        - Compute tentativeG = current.g + 1 (unit edge cost).
    //        - If tentativeG improves neighbor.g (or neighbor.g == 0 for first set):
    //             * Update neighbor.g = tentativeG
    //             * neighbor.h = heuristic(neighbor, goal)
    //             * neighbor.f = neighbor.g + neighbor.h
    //             * neighbor.parentId = current.id
    //             * Push updated neighbor into openSet
    // ============================

    // Example path reconstruction (optional to keep minimal):
    // Follow parentId from goal back to start and print the ids.

    while (!openSet.empty()) {
        // 1) Pop the node with the smallest f (call it 'current').
        Node current = openSet.top();
        openSet.pop();

        // 2)
        if (current.id == goalId) {
            std::cout <<  "Goal reached!" << std::endl;
            closed[current.id] = true;
            break;
        }
        
        // 3)
        if (closed[current.id]){
            continue;
        }
            
        // 4)
        closed[current.id] = true;
        
        // 5)
        for (int neighborId:current.neighbors ) {
            if (closed[neighborId]) {
                continue;
            }

            // Compute tentative g cost
            int tentativeG = current.g + 1;

            // If this is an improvement (or neighbor not yet initialized)
            if (nodes[neighborId].parentId == -1 || tentativeG < nodes[neighborId].g) {
                // Update neighbor
                nodes[neighborId].g = tentativeG;
                nodes[neighborId].h = heuristic(nodes[neighborId], nodes[goalId]);
                nodes[neighborId].f = nodes[neighborId].g + nodes[neighborId].h;
                nodes[neighborId].parentId = current.id;

                // Push updated neighbor into openSet
                openSet.push(nodes[neighborId]);
            }
        }
    }

    if (!closed[goalId]) {
        std::cout << "No path found.\n";
        return 0;
    }

    std::vector<int> path;
    int cur = goalId;
    while (cur != -1) {
        path.push_back(cur);
        cur = nodes[cur].parentId;
    }
    std::reverse(path.begin(), path.end());

    std::cout << "Final path: ";
    for (std::size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i] << (i + 1 < path.size() ? " " : "\n");
    }

    return 0;
}
