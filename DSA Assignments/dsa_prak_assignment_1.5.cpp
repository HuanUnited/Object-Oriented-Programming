// Interation 4 - Goals:
/*
1. Implementing the Greedy Algorithm <DONE>
2. Implementing the timer for each result. <DONE>
3. Implementing a new way for any Algo to solve the same matrix. <NEXT> -
Duplicate the matrix and then delete.

Additional changes:
1. Removed current_solver_best_path/cost from City Solver, it's kind of
redundant to have an additional value saved within the solver, this can be saved
within the matrix.
2. Due to the removal of the above mentioned, the permutation method is now
encapsulated.
3. Changed time formatting value to long long.
4. Fixes to the permutation method to allow for an enclosed area.
5. Better result printing.
6. City indexing fixes for better output results.

*/

#include <cassert>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <random>
#include <stdlib.h>
#include <time.h>
using std::cin, std::cout, std::endl, std::chrono::steady_clock;
#define MAX 1000

using ll = long long;
using size_type = size_t;
static constexpr ll INF = (ll)9e15;

struct Result {
  int *path = nullptr;
  size_t path_len = 0;
  ll cost = -1;
  long long time_ms = 0;

  Result() = default;
  // move semantics
  Result(Result &&o) noexcept {
    path = o.path;
    path_len = o.path_len;
    cost = o.cost;
    time_ms = o.time_ms;
    o.path = nullptr;
    o.path_len = 0;
    o.cost = -1;
    o.time_ms = 0;
  }
  Result &operator=(Result &&o) noexcept {
    if (this != &o) {
      clear();
      path = o.path;
      path_len = o.path_len;
      cost = o.cost;
      time_ms = o.time_ms;
      o.path = nullptr;
      o.path_len = 0;
      o.cost = -1;
      o.time_ms = 0;
    }
    return *this;
  }
  // disable copy to simplify ownership
  Result(const Result &) = delete;
  Result &operator=(const Result &) = delete;

  void clear() {
    if (path)
      delete[] path;
    path = nullptr;
    path_len = 0;
    cost = -1;
    time_ms = 0;
  }

  ~Result() { clear(); }

  void setPathFromArray(const int *arr, size_t len) {
    clear();
    if (len == 0)
      return;
    path = new int[len];
    path_len = len;
    for (size_t i = 0; i < len; ++i)
      path[i] = arr[i];
  }

  void print() const {
    if (!path || path_len == 0) {
      std::cout << "No valid path\n";
      return;
    }
    std::cout << "Cost: " << cost << "  Time: " << time_ms << " ms\nPath: ";
    for (size_t i = 0; i < path_len; ++i) {
      std::cout << (path[i] + 1) << (i + 1 == path_len ? "" : " ");
    }
    std::cout << "\n";
  }
};

struct Matrix {
  ll **m_matrix = nullptr;
  size_t m_size{};

  Matrix(int n = 0) : m_size(n), m_matrix(nullptr) {
    if (n > 0)
      allocate(n);
  }

  void allocate(int n) {
    freeMatrix();
    m_size = n;
    m_matrix = new ll *[n];
    for (int i = 0; i < n; ++i)
      m_matrix[i] = new ll[n];
  }

  Matrix(const Matrix &o) {
    m_size = o.m_size;
    if (m_size > 0) {
      m_matrix = new ll *[m_size];
      for (size_t i = 0; i < m_size; ++i) {
        m_matrix[i] = new ll[m_size];
        for (size_t j = 0; j < m_size; ++j)
          m_matrix[i][j] = o.m_matrix[i][j];
      }
    } else {
      m_matrix = nullptr;
    }
  }

  ~Matrix() { freeMatrix(); }

  void freeMatrix() {
    if (!m_matrix)
      return;
    for (size_t i = 0; i < m_size; ++i)
      delete[] m_matrix[i];
    delete[] m_matrix;
    m_matrix = nullptr;
    m_size = 0;
  }

  Matrix *clone() const { return new Matrix(*this); }

  void generateRandomMatrix(int min_val, int max_val, bool symmetric = true,
                            unsigned int seed = 0) {
    assert(min_val <= max_val);
    if (m_size == 0)
      return;
    if (seed == 0)
      seed = static_cast<unsigned int>(
          std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<ll> dist(min_val, max_val);
    for (size_t i = 0; i < m_size; ++i) {
      for (size_t j = 0; j < m_size; ++j) {
        if (i == j)
          m_matrix[i][j] = 0;
        else if (symmetric && j < i)
          m_matrix[i][j] = m_matrix[j][i];
        else
          m_matrix[i][j] = dist(rng);
      }
    }
  }

  size_t size() const { return m_size; }
  ll at(size_t i, size_t j) const { return m_matrix[i][j]; }
  ll &at(size_t i, size_t j) { return m_matrix[i][j]; }

  void print() const {
    std::cout << "Matrix " << m_size << "x" << m_size << ":\n";
    for (size_t i = 0; i < m_size; ++i) {
      for (size_t j = 0; j < m_size; ++j) {
        std::cout << m_matrix[i][j] << (j + 1 == m_size ? "" : " ");
      }
      std::cout << '\n';
    }
  }

  void swapRows(int pos_from, int pos_to) {
    // swap rows in matrix
    for (int j = 0; j < m_size; j++) {
      int tmp = at(pos_to, j);
      at(pos_to, j) = at(pos_from, j);
      at(pos_from, j) = tmp;
    }
  }

  void swapColumns(int pos_from, int pos_to) {
    // swap columns in matrix
    for (int i = 0; i < m_size; i++) {
      int tmp = at(i, pos_to);
      at(i, pos_to) = at(i, pos_from);
      at(i, pos_from) = tmp;
    }
  }
  void moveCityToFront(int idx) {
    swapRows(idx, 0);
    swapColumns(idx, 0);
  }
};

void swapInts(int *x, int *y) {
  int tmp = *x;
  *x = *y;
  *y = tmp;
}

void printPath(int *path, int len) {
  for (int i = 0; i < len; i++) {
    cout << path[i] << " ";
  }
}

void swapCityArray(int *cities, int pos1, int pos2) {
  int temp = cities[pos1]; // Dereference the pointer to get the value
  cities[pos1] = cities[pos2];
  cities[pos2] = temp;
}

// Methods used by Solvers
int calcCost(int *path, int len, Matrix mat) {
  int sum = 0;
  for (int i = 0; i < len - 1; i++)
    sum += mat.at(path[i], path[i + 1]);
  sum += mat.at(path[len - 1], path[0]);
  return sum;
}

void permute(int *current_path, int l, int r, int *solver_path,
             int *solver_cost, Matrix mat) {
  if (l == r) {
    int cost = calcCost(current_path, r + 1, mat);
    if (cost < *solver_cost) {
      *solver_cost = cost; // <-- Correct: update the VALUE, not the pointer
      // copy current_path into solver best buffer
      for (int i = 0; i < mat.m_size; i++)
        solver_path[i] = current_path[i];
    }
  } else {
    for (int i = l; i <= r; i++) {
      swapInts(&current_path[l], &current_path[i]);
      permute(current_path, l + 1, r, solver_path, solver_cost, mat);
      swapInts(&current_path[l], &current_path[i]); // backtrack
    }
  }
}

// Small note: to create a copy of the beginning matrix in the solver methods
// so that they can freely manipulate it instead touching the original matrix.

// Solver Methods:

Result solveBruteForce(int startIndex, Matrix matrix) {

  Matrix mat = *matrix.clone();

  auto start = steady_clock::now();
  Result out;
  int n = mat.m_size;
  if (n <= 0)
    return out;

  // prepare city array
  int *cities = new int[n];
  for (int i = 0; i < n; i++)
    cities[i] = i;

  // init brute best path and cost
  int *brute_best_path = new int[n];
  int brute_best_cost = MAX;

  // move chosen start to front (reorders matrix), and align cities[]
  mat.moveCityToFront(startIndex);
  swapCityArray(cities, 0, startIndex);

  // search all permutations with city 0 fixed
  permute(cities, 1, n - 1, brute_best_path, &brute_best_cost, mat);

  // build Result to return; allocate fresh path for caller to own
  out.path_len = n;
  out.cost = brute_best_cost;
  out.path = new int[n];
  for (int i = 0; i < n; i++)
    out.path[i] = brute_best_path[i];

  // Time Tracking
  auto end = steady_clock::now();
  auto duration = end - start;
  out.time_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

  delete[] cities;

  return out;
}

Result solveGreedy(int startIndex, Matrix matrix) {

  Matrix mat = *matrix.clone();

  auto start = steady_clock::now();
  int n = mat.m_size;
  Result out; // Assuming Result has members for path and cost

  if (n <= 0) {
    return out;
  }

  // Greedy values init
  int *greedy_path = new int[n];
  bool *visited_cities = new bool[n];
  for (int i = 0; i < n; i++) {
    visited_cities[i] = false;
  }
  int greedy_cost = 0;
  int current_city = startIndex;

  // Start with the initial city
  greedy_path[0] = current_city;
  visited_cities[current_city] = true;

  // Loop until all cities have been visited
  for (int i = 0; i < n - 1; ++i) {
    int smallest_cost = 10000;
    int next_city = -1;

    // Find the next unvisited city with the smallest cost
    for (int j = 0; j < n; ++j) {
      // Check if city 'j' is unvisited and has a smaller cost
      if (!visited_cities[j] && mat.at(current_city, j) < smallest_cost) {
        smallest_cost = mat.at(current_city, j);
        next_city = j;
      }
    }

    // Add the next city to the path
    if (next_city != -1) {
      greedy_path[i + 1] = next_city;
      visited_cities[next_city] = true;
      greedy_cost += smallest_cost;
      current_city = next_city;
    } else {
      break; // if matrix is invalid
    }
  }

  // Add cost to return to starting city to complete the cycle
  greedy_cost += mat.at(current_city, startIndex);

  // Build the Result object to return
  out.path_len = n;
  out.cost = greedy_cost;
  out.path = new int[n];
  for (int i = 0; i < n; i++)
    out.path[i] = greedy_path[i];

  // Time Tracking
  auto end = steady_clock::now();
  auto duration = end - start;
  out.time_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

  delete[] visited_cities;
  delete[] greedy_path;

  return out;
};

// Func signature: Result f(const Matrix&, int start)
typedef Result (*SolverFunc)(const Matrix &, int);

// run solver f for each start, return heap-allocated Result[n] (moved into
// array), caller must delete[]
Result *runAllStarts_results(const Matrix &mat, SolverFunc f) {
  size_t n = mat.m_size;
  if (n == 0)
    return nullptr;
  Result *arr = new Result[n];
  for (size_t s = 0; s < n; ++s) {
    Result r = f(mat, (int)s);
    r.print();
    arr[s] = std::move(r); // move into slot
  }
  return arr;
}

int bestStartIndexFromResults(const Result *arr, size_t n) {
  int best = -1;
  ll best_cost = INF;
  for (size_t i = 0; i < n; ++i) {
    if (!arr[i].path || arr[i].path == 0)
      continue;
    if (arr[i].cost >= 0 && arr[i].cost < best_cost) {
      best_cost = arr[i].cost;
      best = (int)i;
    }
  }
  return best;
}

int main() {

  int n;
  std::cout << "Number of cities: ";
  if (!(std::cin >> n) || n <= 0) {
    std::cerr << "Invalid n\n";
    return 1;
  }

  Matrix M(n);
  M.generateRandomMatrix(1, 20, true);
  M.print();

  const int BRUTE_LIMIT = 15;
  bool allow_brute = (n <= BRUTE_LIMIT);
  if (!allow_brute)
    std::cout << "(n > " << BRUTE_LIMIT
              << " -> skipping brute-force algorithms)\n";

  // fixed list of algorithm names (C-style)
  const char *algo_names[] = {"brute", "greedy"};
  const int algo_count = 2;

  // Summary arrays (C-style)
  bool ran[algo_count];
  int bestStart[algo_count];
  Result
      bestResult[algo_count]; // stores best result per algo (moved into slots)

  for (int a = 0; a < algo_count; ++a) {
    ran[a] = false;
    bestStart[a] = -1;
  }

  // 0) brute (direct)
  if (allow_brute) {
    Result *resArr =
        runAllStarts_results(M, [](const Matrix &mat, int s) -> Result {
          return solveBruteForce(s,mat);
        });
    int best = bestStartIndexFromResults(resArr, M.size());
    if (best != -1) {
      ran[0] = true;
      bestStart[0] = best;
      bestResult[0] = std::move(resArr[best]);
      std::cout << "\nAlgorithm brute -> best start " << (best + 1) << "\n";
      bestResult[0].print();
    } else {
      ran[0] = true;
      bestStart[0] = -1;
      std::cout << "\nAlgorithm brute produced no valid tours\n";
    }
    delete[] resArr;
  } else {
    ran[0] = false;
    bestStart[0] = -1;
  }

  // 1) greedy
  {
    Result *resArr =
        runAllStarts_results(M, [](const Matrix &mat, int s) -> Result {
          return solveGreedy(s, mat);
        });
    int best = bestStartIndexFromResults(resArr, M.size());
    if (best != -1) {
      ran[1] = true;
      bestStart[1] = best;
      bestResult[1] = std::move(resArr[best]);
      std::cout << "\nAlgorithm greedy -> best start " << (best + 1) << "\n";
      bestResult[1].print();
    } else {
      ran[1] = true;
      bestStart[1] = -1;
      std::cout << "\nAlgorithm greedy produced no valid tours\n";
    }
    delete[] resArr;
  }

  // pick overall best among those that ran and produced a path
  int bestAlgo = -1;
  ll bestCostVal = INF;
  for (int a = 0; a < algo_count; ++a) {
    if (!ran[a])
      continue;
    if (bestStart[a] == -1)
      continue;
    if (bestResult[a].cost >= 0 && bestResult[a].cost < bestCostVal) {
      bestCostVal = bestResult[a].cost;
      bestAlgo = a;
    }
  }

  if (bestAlgo != -1) {
    std::cout << "\n=== OVERALL BEST ===\n";
    std::cout << "Algorithm: " << algo_names[bestAlgo] << "\n";
    std::cout << "Best start city: " << (bestStart[bestAlgo] + 1) << "\n";
    std::cout << "Score: " << bestResult[bestAlgo].cost << "\nPath: ";
    for (size_t i = 0; i < bestResult[bestAlgo].path_len; ++i)
      std::cout << (bestResult[bestAlgo].path[i] + 1)
                << (i + 1 == bestResult[bestAlgo].path_len ? "" : " ");
    std::cout << "\nTime (ms): " << bestResult[bestAlgo].time_ms << "\n";
  } else {
    std::cout << "\nNo valid results found across algorithms.\n";
  }

  // cleanup (bestResult destructors will free their paths)
  return 0;
}