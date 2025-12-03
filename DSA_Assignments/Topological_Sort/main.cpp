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
  vector<int> unsorted;

  for (int i = 0; i < n; i++) {
    unsorted.push_back(i);
  }
  while (!unsorted.empty()) {

    for (int row = 0; row < n; row++) {
      bool hasTrue = false;

      for (int column = 0; column < n; column++) {
        if (matrix[row][column]) {
          hasTrue = true;
          break;
        }
      }

      if (!hasTrue) {
        for (int column = 0; i < n; i++) {
          matrix[row][column] = false;
        }
        sorted.push_back(row);
        unsorted.erase(unsorted.begin() + row);
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
  // in-degree is the number of edges that points to a vertex
  for (int i = 0; i < nodes; i++) {
    if (in_degree[i] == 0) {
      zero_in_degree.push(i);
    }
  }

  // zero in degree is a list of vertexes that have an in-degree of zero.

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

// Print List:
void printList(std::vector<std::pair<int, int>> &_graph, int nodes) {
  List<List<int>> adj_list = to_adjacency_list(_graph, nodes);

  if (adj_list.empty())
    return;

  for (int i = 0; i < nodes; i++) {
    auto thing = adj_list[i];

    if (thing.empty())
      continue;

    for (auto val : thing) {
      printf("%d %d\n", i, val);
    }
  }
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

    std::cout << "=== Print List Test ===\n";
    std::cout << "=== Ordered Graph List === \n";
    printList(graph, nodes);

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

    // Test with manual graph
    std::cout << "\n=== Testing with Manual Graph ===\n";
    std::vector<std::pair<int, int>> manual_graph = {
        {0, 1}, {0, 2}, {1, 3}, {2, 3}, {2, 4}, {3, 5}, {4, 5}};
    int manual_nodes = 6;

    printList(manual_graph, manual_nodes);
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