# A* Search Path Planner

This repository contains a C++ implementation of the **A* Search algorithm**, developed as part of the Udacity C++ Nanodegree. The program reads a grid map, processes a starting point and an ending point, and calculates the most efficient path while avoiding obstacles.

## 🚀 Overview

The A* algorithm is one of the most popular techniques used in pathfinding and graph traversals. It improves upon Dijkstra's algorithm by using a **heuristic** to guide the search, making it significantly more efficient for coordinate-based navigation.

The core logic follows the formula:


$$f(n) = g(n) + h(n)$$

Where:

* **$g(n)$**: The cost from the start node to the current node $n$.
* **$h(n)$**: The estimated "heuristic" cost from node $n$ to the goal (Manhattan distance).
* **$f(n)$**: The total estimated cost of the path through node $n$.

## 💻 Getting Started

### Prerequisites

* A C++ compiler (GCC/G++ or Clang)

### Compilation

To compile the project, run the following command in your terminal:

```bash
g++ -std=c++17 a_star.cpp -o pathfinder

```

### Execution

Run the compiled executable:

```bash
./pathfinder

```

## 🏁 Example Output
<img width="680" height="527" alt="image" src="https://github.com/user-attachments/assets/53b49cd3-e560-4b22-b28f-e141756ac68e" />

