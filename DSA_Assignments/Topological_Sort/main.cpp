/*
Topological Sorting Implementation
- Matrix-based (20 points)
- Linked List-based (10 points)
*/

#include <functional>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "acgraph.hpp"
#include "bitmatrix.hpp"
#include "linked_list.hpp"

using std::vector;

// Convert edge list to adjacency matrix
BitMatrix to_matrix(std::vector<std::pair<int, int>> &_graph, int nodes) {
  BitMatrix out(nodes, nodes, false);
  for (auto pair : _graph) {
    out.set(pair.first, pair.second, true);
  }
  return out;
}

// Convert edge list to adjacency list representation
List<List<int>> to_adjacency_list(std::vector<std::pair<int, int>> &_graph,
                                  int nodes) {
  List<List<int>> adj_list;

  // Initialize empty lists for each node
  for (int i = 0; i < nodes; i++) {
    adj_list.push_back(List<int>());
  }

  // Add edges to adjacency list
  for (auto edge : _graph) {
    int from = edge.first;
    int to = edge.second;
    adj_list[from].push_back(to);
  }

  return adj_list;
}

/**
 * Topological Sort using Boolean Matrix (Kahn's Algorithm)
 *
 * Algorithm:
 * 1. Calculate in-degree for each vertex (count incoming edges)
 * 2. Find all vertices with in-degree 0 and add to queue
 * 3. While queue is not empty:
 *    a. Remove vertex from queue and add to result
 *    b. For each neighbor of removed vertex:
 *       - Decrease its in-degree by 1
 *       - If in-degree becomes 0, add to queue
 * 4. If result contains all vertices, return; else graph has cycle
 *
 * Time Complexity: O(V^2) where V is number of vertices
 * Space Complexity: O(V)
 */
void topSortMatrix(BitMatrix &matrix, vector<int> &sorted) {
  if (matrix.columns() != matrix.rows()) {
    throw std::invalid_argument("Matrix must be square");
  }

  int n = matrix.rows();
  vector<int> in_degree(n, 0);

  // Calculate in-degree for each vertex
  // in_degree[j] = number of edges pointing to vertex j
  for (int j = 0; j < n; j++) {
    for (int i = 0; i < n; i++) {
      if (matrix[i][j]) { // Edge from i to j
        in_degree[j]++;
      }
    }
  }

  // Queue for vertices with in-degree 0
  std::queue<int> zero_in_degree;

  // Add all vertices with in-degree 0
  for (int i = 0; i < n; i++) {
    if (in_degree[i] == 0) {
      zero_in_degree.push(i);
    }
  }

  // Process vertices
  while (!zero_in_degree.empty()) {
    int u = zero_in_degree.front();
    zero_in_degree.pop();
    sorted.push_back(u);

    // For each vertex v that u points to
    for (int v = 0; v < n; v++) {
      if (matrix[u][v]) { // Edge from u to v
        in_degree[v]--;
        if (in_degree[v] == 0) {
          zero_in_degree.push(v);
        }
      }
    }
  }

  // Check if topological sort is possible (no cycles)
  if (sorted.size() != static_cast<size_t>(n)) {
    sorted.clear();
    throw std::runtime_error(
        "Graph contains a cycle - topological sort impossible");
  }
}

/**
 * Topological Sort using Linked List (Kahn's Algorithm)
 *
 * Uses adjacency list representation with linked lists
 *
 * Algorithm:
 * 1. Build adjacency list and calculate in-degrees
 * 2. Find vertices with in-degree 0
 * 3. Process vertices in BFS manner
 * 4. Update in-degrees of neighbors
 *
 * Time Complexity: O(V + E) where V = vertices, E = edges
 * Space Complexity: O(V + E)
 */
void topSortLinkedList(std::vector<std::pair<int, int>> &_graph, int nodes,
                       vector<int> &sorted) {
  // Build adjacency list using LinkedList
  List<List<int>> adj_list = to_adjacency_list(_graph, nodes);

  // Calculate in-degree for each vertex
  vector<int> in_degree(nodes, 0);
  for (auto edge : _graph) {
    in_degree[edge.second]++;
  }

  // Queue for vertices with in-degree 0
  std::queue<int> zero_in_degree;

  // Add all vertices with in-degree 0
  for (int i = 0; i < nodes; i++) {
    if (in_degree[i] == 0) {
      zero_in_degree.push(i);
    }
  }

  // Process vertices
  while (!zero_in_degree.empty()) {
    int u = zero_in_degree.front();
    zero_in_degree.pop();
    sorted.push_back(u);

    // For each neighbor v of u
    List<int> &neighbors = adj_list[u];
    for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {
      int v = *it;
      in_degree[v]--;
      if (in_degree[v] == 0) {
        zero_in_degree.push(v);
      }
    }
  }

  // Check if topological sort is possible
  if (sorted.size() != static_cast<size_t>(nodes)) {
    sorted.clear();
    throw std::runtime_error(
        "Graph contains a cycle - topological sort impossible");
  }
}

/**
 * Alternative: DFS-based Topological Sort using Matrix
 * Uses depth-first search and a stack
 * Vertices are added to result in reverse order of completion
 */
void topSortMatrixDFS(BitMatrix &matrix, vector<int> &sorted) {
  if (matrix.columns() != matrix.rows()) {
    throw std::invalid_argument("Matrix must be square");
  }

  int n = matrix.rows();
  vector<bool> visited(n, false);
  vector<bool> rec_stack(n, false); // For cycle detection
  vector<int> finish_order;

  // DFS helper function
  std::function<void(int)> dfs = [&](int u) {
    visited[u] = true;
    rec_stack[u] = true;

    // Visit all neighbors
    for (int v = 0; v < n; v++) {
      if (matrix[u][v]) { // Edge from u to v
        if (rec_stack[v]) {
          throw std::runtime_error(
              "Graph contains a cycle - topological sort impossible");
        }
        if (!visited[v]) {
          dfs(v);
        }
      }
    }

    rec_stack[u] = false;
    finish_order.push_back(u);
  };

  // Run DFS from each unvisited vertex
  for (int i = 0; i < n; i++) {
    if (!visited[i]) {
      dfs(i);
    }
  }

  // Reverse the finish order to get topological sort
  sorted.assign(finish_order.rbegin(), finish_order.rend());
}

/**
 * Alternative: DFS-based Topological Sort using Linked List
 */
void topSortLinkedListDFS(std::vector<std::pair<int, int>> &_graph, int nodes,
                          vector<int> &sorted) {
  List<List<int>> adj_list = to_adjacency_list(_graph, nodes);

  vector<bool> visited(nodes, false);
  vector<bool> rec_stack(nodes, false);
  vector<int> finish_order;

  std::function<void(int)> dfs = [&](int u) {
    visited[u] = true;
    rec_stack[u] = true;

    List<int> &neighbors = adj_list[u];
    for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {
      int v = *it;
      if (rec_stack[v]) {
        throw std::runtime_error(
            "Graph contains a cycle - topological sort impossible");
      }
      if (!visited[v]) {
        dfs(v);
      }
    }

    rec_stack[u] = false;
    finish_order.push_back(u);
  };

  for (int i = 0; i < nodes; i++) {
    if (!visited[i]) {
      dfs(i);
    }
  }

  sorted.assign(finish_order.rbegin(), finish_order.rend());
}

// Utility: Print graph edges
void print_graph(const std::vector<std::pair<int, int>> &graph) {
  std::cout << "\nGraph edges:\n";
  for (auto edge : graph) {
    std::cout << "  " << edge.first << " -> " << edge.second << "\n";
  }
}

// Utility: Print sorted result
void print_sorted(const vector<int> &sorted) {
  std::cout << "Topological order: ";
  for (size_t i = 0; i < sorted.size(); i++) {
    std::cout << sorted[i];
    if (i < sorted.size() - 1)
      std::cout << " -> ";
  }
  std::cout << "\n";
}

// Utility: Verify topological sort
bool verify_topological_sort(const std::vector<std::pair<int, int>> &graph,
                             const vector<int> &sorted) {
  // Create position map
  vector<int> position(sorted.size());
  for (size_t i = 0; i < sorted.size(); i++) {
    position[sorted[i]] = i;
  }

  // Check each edge: for edge u->v, position[u] must be < position[v]
  for (auto edge : graph) {
    if (position[edge.first] >= position[edge.second]) {
      return false;
    }
  }
  return true;
}

int main() {
  std::cout << "=== Topological Sorting Implementation ===\n";

  try {
    // Generate random DAG
    std::vector<std::pair<int, int>> graph;
    int nodes = 0;
    graph_gen(graph, &nodes);

    std::cout << "\nGenerated DAG with " << nodes << " nodes and "
              << graph.size() << " edges\n";
    print_graph(graph);

    // Method 1: Matrix-based (Kahn's Algorithm)
    std::cout << "\n--- Method 1: Matrix-based Topological Sort (Kahn) ---\n";
    BitMatrix matrix = to_matrix(graph, nodes);
    std::cout << "Adjacency Matrix:\n" << matrix << "\n";

    vector<int> sorted_matrix;
    topSortMatrix(matrix, sorted_matrix);
    print_sorted(sorted_matrix);

    bool valid_matrix = verify_topological_sort(graph, sorted_matrix);
    std::cout << "Verification: " << (valid_matrix ? "✓ VALID" : "✗ INVALID")
              << "\n";

    // Method 2: Linked List-based (Kahn's Algorithm)
    std::cout
        << "\n--- Method 2: Linked List-based Topological Sort (Kahn) ---\n";
    vector<int> sorted_list;
    topSortLinkedList(graph, nodes, sorted_list);
    print_sorted(sorted_list);

    bool valid_list = verify_topological_sort(graph, sorted_list);
    std::cout << "Verification: " << (valid_list ? "✓ VALID" : "✗ INVALID")
              << "\n";

    // Method 3: Matrix-based DFS
    std::cout << "\n--- Method 3: Matrix-based Topological Sort (DFS) ---\n";
    vector<int> sorted_matrix_dfs;
    topSortMatrixDFS(matrix, sorted_matrix_dfs);
    print_sorted(sorted_matrix_dfs);

    bool valid_matrix_dfs = verify_topological_sort(graph, sorted_matrix_dfs);
    std::cout << "Verification: "
              << (valid_matrix_dfs ? "✓ VALID" : "✗ INVALID") << "\n";

    // Method 4: Linked List-based DFS
    std::cout
        << "\n--- Method 4: Linked List-based Topological Sort (DFS) ---\n";
    vector<int> sorted_list_dfs;
    topSortLinkedListDFS(graph, nodes, sorted_list_dfs);
    print_sorted(sorted_list_dfs);

    bool valid_list_dfs = verify_topological_sort(graph, sorted_list_dfs);
    std::cout << "Verification: " << (valid_list_dfs ? "✓ VALID" : "✗ INVALID")
              << "\n";

    // Test with manual graph
    std::cout << "\n=== Testing with Manual Graph ===\n";
    std::vector<std::pair<int, int>> manual_graph = {
        {0, 1}, {0, 2}, {1, 3}, {2, 3}, {2, 4}, {3, 5}, {4, 5}};
    int manual_nodes = 6;

    std::cout << "Manual graph (0->1, 0->2, 1->3, 2->3, 2->4, 3->5, 4->5):\n";
    print_graph(manual_graph);

    BitMatrix manual_matrix = to_matrix(manual_graph, manual_nodes);
    vector<int> manual_sorted;
    topSortMatrix(manual_matrix, manual_sorted);
    print_sorted(manual_sorted);

    bool valid_manual = verify_topological_sort(manual_graph, manual_sorted);
    std::cout << "Verification: " << (valid_manual ? "✓ VALID" : "✗ INVALID")
              << "\n";

    std::cout << "\n=== All tests completed successfully! ===\n";

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}