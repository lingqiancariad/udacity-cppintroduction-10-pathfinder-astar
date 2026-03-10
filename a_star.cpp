#include <iostream>
#include <vector>
#include <queue>
#include <cmath>    // abs
#include <algorithm>

#define NUM_NODES   16
#define NUM_ROWS    4
#define NUM_COLS    4

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

int id_of(int r, int c) { return r*NUM_ROWS + c;}

// -----------------------------------------------------------------------------
// Assume: 'nodes' is already populated (ids, row/col, neighbors).
// Implement ONLY the main A* loop marked below.
// -----------------------------------------------------------------------------
int main() {
    std::vector<Node> nodes(NUM_NODES);   

    // ... populate nodes (ids 0..N-1, row/col, neighbors) before running ...
    int idex = 0;
    for (int r = 0; r < NUM_ROWS; r++) {
        for (int c = 0; c < NUM_COLS; c++ ) {
            nodes[idex] = Node{ idex, r, c, -1, 0, 0, 0, {} };
            idex++;
        }
    }

    

    for (int r = 0; r < NUM_ROWS; r++) {
        for (int c = 0; c < NUM_COLS; c++) {
            int id_current = id_of(r,c);
            if (r > 0) nodes[id_current].neighbors.push_back(id_of(r - 1, c));
            if (r < NUM_ROWS-1) nodes[id_current].neighbors.push_back(id_of(r + 1, c));
            if (c > 0) nodes[id_current].neighbors.push_back(id_of(r, c - 1));
            if (c < NUM_COLS-1) nodes[id_current].neighbors.push_back(id_of(r, c + 1));
        }
    }

    // Safety: ensure there is some graph
    if (nodes.empty()) {
        std::cout << "Populate 'nodes' before running.\n";
        return 0;
    }

    // -----------------------------------------------------------------

   

    int startId = 0;
    int goalId  = static_cast<int>(nodes.size()) - 1;

    std::cout << "Give the Start ID of the node: \n";
    std::cin >> startId;
    std::cout << "Give the Goal ID of the node: \n";
    std::cin >> goalId;

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
