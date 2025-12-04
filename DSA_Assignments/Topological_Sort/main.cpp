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
#include "bitvector.hpp"
#include "dynamic_array.hpp"
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

List<List<int>> to_in_list(std::vector<std::pair<int, int>> &_graph,
                           int nodes) {
  List<List<int>> out_list;
  // Initialize empty lists for each node
  for (int i = 0; i < nodes; i++) {
    out_list.push_back(List<int>());
  }

  // Add edges to adjacency list
  for (auto edge : _graph) {
    int from = edge.first;
    int to = edge.second;
    out_list[to].push_back(from);
  }
  return out_list;
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

// row = from. column = to: each pair: from -> to
void topSortMatrix(BitMatrix &matrix, vector<int> &sorted) {
  if (matrix.columns() != matrix.rows()) {
    throw std::invalid_argument("Matrix must be square");
  }

  int n = matrix.rows();

  DynamicArray<int> inA;
  for (int i = 0; i < n; i++) {
    inA.push_back(i);
  }

  // std::cout << inA << '\n';

  while (!inA.empty()) {

    for (auto column : inA) {
      bool noTrue{true};

      for (int i = 0; i < n; i++) {
        if (matrix[i].get(column) == true) {
          noTrue = false;
          break;
        };
      }

      if (noTrue) {
        sorted.push_back(column);
        matrix[column].setAll(false);
        inA.pop_first(column);
        break;

        // // Debug
        // std::cout << "--- inA --- \n" << inA << '\n';
        // std::cout << "--- Sorted --- \n";
        // for (auto i : sorted) {
        //   std::cout << " " << i;
        // }
        // std::cout << '\n';
        // std::cout << "--- Matrix --- \n" << matrix << '\n';
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

// Top Sort Linked List Lecture
void topSortListLecture(std::vector<std::pair<int, int>> &_graph, int nodes,
                        vector<int> &sorted) {
  // Clear output in case caller passed a non-empty vector
  sorted.clear();

  // Build adjacency list using LinkedList List<T>
  List<List<int>> adj_list = to_adjacency_list(_graph, nodes);

  // Calculate in-degree for each vertex
  vector<int> in_degree(nodes, 0);
  for (auto &edge : _graph) {
    in_degree[edge.second]++;
  }

  // LD: list of vertices still unprocessed (lecture's LD)
  List<int> LD;
  for (int i = 0; i < nodes; ++i) {
    LD.push_back(i);
  }

  // LDnew: list of vertices with in-degree == 0 (lecture's LDnew)
  List<int> LDnew;

  // Initial step: move all vertices with in-degree 0 from LD -> LDnew
  for (auto it = LD.begin(); it != LD.end();) {
    int v = *it;
    if (in_degree[v] == 0) {
      LDnew.push_back(v); // add to LDnew
      // advance iterator before erasing current element
      auto it_next = it;
      ++it_next;
      LD.pop_it(it); // erase the element 'it'
      it = it_next;  // continue from next
    } else {
      ++it;
    }
  }

  // Process LDnew until empty
  while (!LDnew.empty()) {
    // Pop the first element from LDnew
    auto it_ldnew = LDnew.begin();
    int u = *it_ldnew;
    LDnew.pop_it(it_ldnew);

    // Output u
    sorted.push_back(u);

    // For every neighbor v of u: decrement indegree, and if it becomes zero
    // move v from LD -> LDnew
    List<int> &neighbors = adj_list[u];
    for (auto itn = neighbors.begin(); itn != neighbors.end(); ++itn) {
      int v = *itn;
      --in_degree[v];
      if (in_degree[v] == 0) {
        // find v in LD and remove it, then add to LDnew
        for (auto it_ld = LD.begin(); it_ld != LD.end(); ++it_ld) {
          if (*it_ld == v) {
            // advance iterator before erasing
            auto it_ld_next = it_ld;
            ++it_ld_next;
            LDnew.push_back(v);
            LD.pop_it(it_ld); // remove from LD
            // we break because we've removed v
            break;
          }
        }
      }
    }
  }

  // If not all vertices were output, the graph contains a cycle.
  if (static_cast<int>(sorted.size()) != nodes) {
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
    std::cout << "\n--- Method 1: Matrix-based Topological Sort (Kahn)---\n";
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

    // Method 3: Linked List-lecture
    std::cout << "\n --- Method 3: Lecture Leader Trailer --- \n";
    vector<int> sorted_lecture;
    topSortListLecture(graph, nodes, sorted_lecture);
    print_sorted(sorted_lecture);

    bool valid_lecture = verify_topological_sort(graph, sorted_lecture);
    std::cout << "Verification: " << (valid_lecture ? "VALID" : "INVALID")
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