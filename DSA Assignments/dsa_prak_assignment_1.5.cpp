// Goals:
/*
1. Refactor Codebase,
2. Use vector for better memory management
3. Additionals

*/
// cities_refactor.cpp
#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

using std::cin, std::cout, std::endl, std::vector,std::swap, std::mt19937, std::uniform_int_distribution, std::numeric_limits;
using Clock = std::chrono::steady_clock;
static constexpr int MAX_COST = 1000000;

struct Result {
  vector<int> path;
  int cost = MAX_COST;
  long long time_ms = 0;

  void printRes() const {
    if (!path.empty()) {
      cout << "Solver best cost: " << cost << endl;
      cout << "Solver best path: ";
      for (auto v : path)
        cout << (v + 1) << " ";
      cout << endl;
      cout << "Solver time : " << time_ms << " ms" << endl;
    } else {
      cout << "No result (n==0?)" << endl;
    }
  }
};

class CityMatrix {
public:
  CityMatrix() = default;
  explicit CityMatrix(int matrix_size);

  // default copy/move/assign are fine (vectors manage memory)
  void setSize(int matrix_size);
  int getSize() const { return m_matrix_size; }

  void generateMatrix(mt19937 &rng, int minVal = 1, int maxVal = 10);
  void printMatrix() const;

  // row/col operations
  void swapRows(int pos_from, int pos_to);
  void swapColumns(int pos_from, int pos_to);
  void moveCityToFront(int idx);
  void moveCityToBack(int idx);
  void deleteCity(int cityIndex);

  // helpers
  int &at(int i, int j) { return m_distance_matrix[i][j]; }
  int at(int i, int j) const { return m_distance_matrix[i][j]; }

  void setBestPath(const vector<int> &path);
  const vector<int> &getBestPath() const { return m_best_path; }
  int getBestCost() const { return m_best_cost; }
  void acceptBestIfBetter(const Result &res);

  // small utility functions (kept short)
  int getSmallestinRowIndex(int rowIndex) const;
  int getSmallestinRowValue(int rowIndex) const;

private:
  int m_matrix_size = 0;
  int m_best_cost = MAX_COST;
  vector<int> m_best_path;
  vector<vector<int>> m_distance_matrix;
};

// CitySolver declaration (implementations below)
class CitySolver {
public:
  CitySolver() = default;

  // matrix creation: user should provide RNG (seed once in main)
  void makeMatrix(int number_of_cities, mt19937 &rng);
  void makeMatrix(mt19937 &rng); // random size like original (1..20)

  void printMatrix() const;

  // solver utilities (short declarations)
  int calcCost(const vector<int> &path) const;

  // brute force (permutation) & greedy
  Result solveBruteForce(int startIndex);
  Result solveGreedy(int startIndex);

private:
  // permutation helper (recursive) - defined outside
  void permute(vector<int> &current_path, int l, int r,
               vector<int> &best_path, int &best_cost);

  CityMatrix matrix;
};

/* ---------------------------
   CityMatrix method definitions
   --------------------------- */

CityMatrix::CityMatrix(int matrix_size) {
  setSize(matrix_size);
  // don't generate here; user should call generateMatrix with RNG for
  // deterministic seeding
}

void CityMatrix::setSize(int matrix_size) {
  m_matrix_size = matrix_size;
  m_distance_matrix.assign(m_matrix_size, vector<int>(m_matrix_size, 0));
  m_best_path.assign(m_matrix_size, -1);
  m_best_cost = MAX_COST;
}

void CityMatrix::generateMatrix(mt19937 &rng, int minVal, int maxVal) {
  if (m_matrix_size <= 0)
    return;
  uniform_int_distribution<int> dist(minVal, maxVal);
  for (int i = 0; i < m_matrix_size; ++i) {
    for (int j = 0; j < m_matrix_size; ++j) {
      if (i == j)
        m_distance_matrix[i][j] = 0;
      else
        m_distance_matrix[i][j] = dist(rng);
    }
  }
}

void CityMatrix::printMatrix() const {
  cout << "Matrix is: " << endl;
  for (int i = 0; i < m_matrix_size; ++i) {
    for (int j = 0; j < m_matrix_size; ++j)
      cout << m_distance_matrix[i][j] << " ";
    cout << '\n';
  }
}

void CityMatrix::swapRows(int pos_from, int pos_to) {
  if (pos_from == pos_to)
    return;
  swap(m_distance_matrix[pos_from], m_distance_matrix[pos_to]);
}

void CityMatrix::swapColumns(int pos_from, int pos_to) {
  if (pos_from == pos_to)
    return;
  for (int i = 0; i < m_matrix_size; ++i)
    swap(m_distance_matrix[i][pos_from], m_distance_matrix[i][pos_to]);
}

void CityMatrix::moveCityToFront(int idx) {
  if (idx <= 0 || idx >= m_matrix_size)
    return;
  swapRows(idx, 0);
  swapColumns(idx, 0);
}

void CityMatrix::moveCityToBack(int idx) {
  int last = m_matrix_size - 1;
  if (idx < 0 || idx >= m_matrix_size || idx == last)
    return;
  swapRows(idx, last);
  swapColumns(idx, last);
}

void CityMatrix::deleteCity(int cityIndex) {
  // move city to last and remove last row/col
  if (cityIndex < 0 || cityIndex >= m_matrix_size)
    return;
  moveCityToBack(cityIndex);
  int last = m_matrix_size - 1;
  m_distance_matrix.erase(m_distance_matrix.begin() + last);
  for (auto &row : m_distance_matrix)
    row.erase(row.begin() + last);
  m_matrix_size--;
  if (!m_best_path.empty()) {
    m_best_path.erase(m_best_path.begin() + last);
  }
}

void CityMatrix::setBestPath(const vector<int> &path) {
  m_best_path = path;
}

void CityMatrix::acceptBestIfBetter(const Result &res) {
  if ((int)res.path.size() != m_matrix_size)
    return;
  if (res.cost < m_best_cost) {
    m_best_cost = res.cost;
    m_best_path = res.path;
  }
}

int CityMatrix::getSmallestinRowIndex(int rowIndex) const {
  if (rowIndex < 0 || rowIndex >= m_matrix_size)
    return -1;
  int bestIdx = -1;
  int bestVal = numeric_limits<int>::max();
  for (int j = 0; j < m_matrix_size; ++j) {
    if (m_distance_matrix[rowIndex][j] < bestVal) {
      bestVal = m_distance_matrix[rowIndex][j];
      bestIdx = j;
    }
  }
  return bestIdx;
}

int CityMatrix::getSmallestinRowValue(int rowIndex) const {
  if (rowIndex < 0 || rowIndex >= m_matrix_size)
    return numeric_limits<int>::max();
  int bestVal = numeric_limits<int>::max();
  for (int j = 0; j < m_matrix_size; ++j) {
    if (m_distance_matrix[rowIndex][j] < bestVal) {
      bestVal = m_distance_matrix[rowIndex][j];
    }
  }
  return bestVal;
}

/* ---------------------------
   CitySolver method definitions
   --------------------------- */

void CitySolver::makeMatrix(int number_of_cities, mt19937 &rng) {
  matrix.setSize(number_of_cities);
  matrix.generateMatrix(rng);
}

void CitySolver::makeMatrix(mt19937 &rng) {
  uniform_int_distribution<int> dist(1, 20);
  int n = dist(rng);
  matrix.setSize(n);
  matrix.generateMatrix(rng);
}

void CitySolver::printMatrix() const { matrix.printMatrix(); }

int CitySolver::calcCost(const vector<int> &path) const {
  int n = (int)path.size();
  if (n <= 0)
    return 0;
  int sum = 0;
  for (int i = 0; i < n - 1; ++i)
    sum += matrix.at(path[i], path[i + 1]);
  sum += matrix.at(path[n - 1], path[0]);
  return sum;
}

void CitySolver::permute(vector<int> &current_path, int l, int r,
                         vector<int> &best_path, int &best_cost) {
  if (l == r) {
    int cost = calcCost(current_path);
    if (cost < best_cost) {
      best_cost = cost;
      best_path = current_path;
    }
    return;
  }
  for (int i = l; i <= r; ++i) {
    swap(current_path[l], current_path[i]);
    permute(current_path, l + 1, r, best_path, best_cost);
    swap(current_path[l], current_path[i]); //backtrack
  }
}

Result CitySolver::solveBruteForce(int startIndex) {
  auto tstart = Clock::now();
  Result out;
  int n = matrix.getSize();
  if (n <= 0)
    return out;

  vector<int> cities(n);
  for (int i = 0; i < n; ++i)
    cities[i] = i;

  // move chosen start to front (reorders matrix), and align cities[]
  matrix.moveCityToFront(startIndex);
  swap(cities[0], cities[startIndex]);

  vector<int> brute_best_path;
  int brute_best_cost = MAX_COST;

  permute(cities, 1, n - 1, brute_best_path, brute_best_cost);

  out.path = brute_best_path;
  out.cost = brute_best_cost;
  out.time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                    Clock::now() - tstart)
                    .count();
  return out;
}

Result CitySolver::solveGreedy(int startIndex) {
  auto tstart = Clock::now();
  Result out;
  int n = matrix.getSize();
  if (n <= 0)
    return out;

  vector<int> greedy_path;
  greedy_path.reserve(n);
  vector<bool> visited(n, false);

  int current = startIndex;
  greedy_path.push_back(current);
  visited[current] = true;
  int greedy_cost = 0;

  for (int step = 1; step < n; ++step) {
    int best_next = -1;
    int best_cost_here = numeric_limits<int>::max();
    for (int j = 0; j < n; ++j) {
      if (!visited[j]) {
        int c = matrix.at(current, j);
        if (c < best_cost_here) {
          best_cost_here = c;
          best_next = j;
        }
      }
    }
    if (best_next == -1)
      break;
    greedy_path.push_back(best_next);
    visited[best_next] = true;
    greedy_cost += best_cost_here;
    current = best_next;
  }

  // add cost to return to start
  greedy_cost += matrix.at(current, startIndex);

  out.path = greedy_path;
  out.cost = greedy_cost;
  out.time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                    Clock::now() - tstart)
                    .count();
  return out;
}

/* ---------------------------
   main
   --------------------------- */

int main() {
  std::random_device rd;
  mt19937 rng(rd());

  int number_of_cities{};
  cout << "Input number of cities: ";
  cin >> number_of_cities;
  cout << endl;

  if (number_of_cities <= 0) {
    cout << "Error: number of cities must be > 0" << endl;
    return 1;
  }
  if (number_of_cities > 1000) {
    cout << "Error: number of cities too large (max 1000)" << endl;
    return 1;
  }

  CitySolver solver;
  solver.makeMatrix(number_of_cities, rng);
  solver.printMatrix();

  int startIndex;
  cout << "Input starting city: ";
  cin >> startIndex;
  cout << endl;
  if (startIndex < 1 || startIndex > number_of_cities) {
    cout << "Error: start index must be in [1," << number_of_cities << "]"
         << endl;
    return 1;
  }
  startIndex -= 1;

  cout << "Greedy Results: " << endl;
  Result res1 = solver.solveGreedy(startIndex);
  res1.printRes();

  cout << "============" << endl;
  cout << "Brute Force Results: " << endl;
  Result res2 = solver.solveBruteForce(startIndex);
  res2.printRes();

  return 0;
}
