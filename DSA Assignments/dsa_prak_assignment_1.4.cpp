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

#include <chrono>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <time.h>
using std::cin, std::cout, std::endl, std::chrono::steady_clock;
#define MAX 1000

struct Result {
  int *path; // allocated with new int[len]
  int len;
  int cost;
  long long time_ms;
  Result() : path(nullptr), len(0), cost(0), time_ms(0.0) {}

  void printRes() {
    if (path != nullptr) {
      cout << "Solver best cost: " << cost << endl;
      cout << "Solver best path: ";
      for (int i = 0; i < len; i++)
        cout << path[i] + 1 << " ";
      cout << endl;
      cout << "Solver time : " << time_ms << " ms" << endl;
      delete[] path; // caller frees returned path
    } else {
      cout << "No result (n==0?)" << endl;
    }
  }
};

class CityMatrix {
private:
  int m_matrix_size;
  int m_best_cost{MAX};
  int *m_best_path{nullptr};
  int **m_distance_matrix{nullptr};

public:
  // Matrix Functions
  void generateMatrix();
  void printMatrix();
  void swapRows(int pos_from, int pos_to);
  void swapColumns(int pos_from, int pos_to);
  void moveCityToFront(int idx);
  void moveCityToBack(int idx);
  
  int getSmallestinRowIndex(int rowIndex);
  int getSmallestinRowValue(int rowIndex);
  void deleteCity(int cityIndex);

  // Setters & Getters
  void setSize(int matrix_size) { m_matrix_size = matrix_size; }
  int getSize() const { return m_matrix_size; }
  void setBestCost(int best_cost) { m_best_cost = best_cost; }
  int getBestCost() const { return m_best_cost; }
  void setBestPath(int *path) {
    for (int i = 0; i < m_matrix_size; i++) {
      m_best_path[i] = path[i];
    }
  }
  const int *getBestPath() const { return m_best_path; }
  void printBestPath() {
    cout << "Best path is:";
    for (int i = 0; i < m_matrix_size; i++) {
      cout << " " << m_best_path[i];
    }
    cout << endl;
  }
  inline int &at(int i, int j) { return m_distance_matrix[i][j]; }
  inline int at(int i, int j) const { return m_distance_matrix[i][j]; }

  void acceptBestIfBetter(const Result &res) {
    if (res.len != m_matrix_size)
      return; // sanity
    if (res.cost < m_best_cost) {
      m_best_cost = res.cost;
      delete[] m_best_path;
      m_best_path = new int[m_matrix_size];
      for (int i = 0; i < m_matrix_size; i++)
        m_best_path[i] = res.path[i];
    }
  }

  // Constructors & Destructor
  CityMatrix(int matrix_size) { // Constructor with size input
    m_matrix_size = matrix_size;
    m_distance_matrix = new int *[m_matrix_size];
    for (int i = 0; i < m_matrix_size; ++i) {
      m_distance_matrix[i] = new int[m_matrix_size];
    }
    m_best_path = new int[m_matrix_size];
    for (int i = 0; i < m_matrix_size; i++)
      m_best_path[i] = -1;
    generateMatrix();
  }

  CityMatrix() { // Random Matrix Constructor
    srand(time(NULL));
    m_matrix_size =
        rand() %
        20; // For not setting the size of the matrix to not exceed 20 x 20
    m_distance_matrix = new int *[m_matrix_size];
    for (int i = 0; i < m_matrix_size; ++i) {
      m_distance_matrix[i] = new int[m_matrix_size];
    }
    m_best_path = new int[m_matrix_size];
    for (int i = 0; i < m_matrix_size; i++)
      m_best_path[i] = -1;
    generateMatrix();
  }
  // Copy Constructor
  CityMatrix(const CityMatrix &other) {
    m_matrix_size = other.m_matrix_size;
    m_best_cost = other.m_best_cost;
    m_distance_matrix = nullptr;
    m_best_path = nullptr;
    if (m_matrix_size > 0) {
      m_distance_matrix = new int *[m_matrix_size];
      for (int i = 0; i < m_matrix_size; i++) {
        m_distance_matrix[i] = new int[m_matrix_size];
        for (int j = 0; j < m_matrix_size; j++) {
          m_distance_matrix[i][j] = other.m_distance_matrix[i][j];
        }
      }
      if (other.m_best_path) {
        m_best_path = new int[m_matrix_size];
        for (int i = 0; i < m_matrix_size; i++)
          m_best_path[i] = other.m_best_path[i];
      }
    }
  }

  CityMatrix &operator=(const CityMatrix &other) {
    if (this == &other)
      return *this;
    delete[] m_best_path;
    for (int i = 0; i < m_matrix_size; i++) {
      delete[] m_distance_matrix[i];
    }
    delete[] m_distance_matrix;
    m_matrix_size = other.m_matrix_size;
    m_best_cost = other.m_best_cost;
    m_distance_matrix = nullptr;
    m_best_path = nullptr;
    if (m_matrix_size > 0) {
      m_distance_matrix = new int *[m_matrix_size];
      for (int i = 0; i < m_matrix_size; i++) {
        m_distance_matrix[i] = new int[m_matrix_size];
        for (int j = 0; j < m_matrix_size; j++) {
          m_distance_matrix[i][j] = other.m_distance_matrix[i][j];
        }
      }
      if (other.m_best_path) {
        m_best_path = new int[m_matrix_size];
        for (int i = 0; i < m_matrix_size; i++)
          m_best_path[i] = other.m_best_path[i];
      }
    }
    return *this;
  }

  ~CityMatrix() {
    for (int i = 0; i < m_matrix_size; ++i)
      delete[] m_distance_matrix[i];
    delete[] m_distance_matrix;
    delete[] m_best_path;
  }
};

void CityMatrix::printMatrix() {
  cout << "Matrix is: " << endl;
  for (int i = 0; i < m_matrix_size; i++) {
    for (int j = 0; j < m_matrix_size; j++) {
      cout << m_distance_matrix[i][j] << " ";
    }
    cout << endl;
  }
}

void CityMatrix::generateMatrix() {
  // do not call srand here; seed once in main
  for (int i = 0; i < m_matrix_size; i++) {
    for (int j = 0; j < m_matrix_size; j++) {
      if (i == j) {
        m_distance_matrix[i][j] = 0; // diagonal = 0
      } else {
        int num = rand() % 10 + 1; // 1..20 (avoid 0 except diagonal)
        m_distance_matrix[i][j] = num;
      }
    }
  }
}

void CityMatrix::swapRows(int pos_from, int pos_to) {
  // swap rows in matrix
  for (int j = 0; j < m_matrix_size; j++) {
    int tmp = at(pos_to, j);
    at(pos_to, j) = at(pos_from, j);
    at(pos_from, j) = tmp;
  }
}

void CityMatrix::swapColumns(int pos_from, int pos_to) {
  // swap columns in matrix
  for (int i = 0; i < m_matrix_size; i++) {
    int tmp = at(i, pos_to);
    at(i, pos_to) = at(i, pos_from);
    at(i, pos_from) = tmp;
  }
}

void CityMatrix::moveCityToFront(int idx) {
  swapRows(idx, 0);
  swapColumns(idx, 0);
}

void CityMatrix::moveCityToBack(int idx) {
  swapRows(idx, m_matrix_size - 1);
  swapColumns(idx, m_matrix_size - 1);
}

int CityMatrix::getSmallestinRowIndex(
    int rowIndex) { // this gives us the smallest value in the row, but what
                    // we want is to get also the address.
  int columnIndex{0};
  int smallest{1000};
  for (; columnIndex < m_matrix_size; columnIndex++) {
    if (m_distance_matrix[rowIndex][columnIndex] <= smallest) {
      smallest = m_distance_matrix[rowIndex][columnIndex];
    }
  }
  return columnIndex; // This gives us the index of the smallest in the row.
  // But what if the city chosen city has already been went through?
  // Then we need to remove that row and column from the 2D matrix.
}

int CityMatrix::getSmallestinRowValue(
    int rowIndex) { // this gives us the smallest value in the row, but what
                    // we want is to get also the address.
  int columnIndex{0};
  int smallest{1000};
  for (; columnIndex < m_matrix_size; columnIndex++) {
    if (m_distance_matrix[rowIndex][columnIndex] <= smallest) {
      smallest = m_distance_matrix[rowIndex][columnIndex];
    }
  }
  return smallest;
}

  void CityMatrix::deleteCity(int cityIndex) {
    // move the city's row and column to the last row and column. and then
    // delete that from the indexes. matrix_size--.
    moveCityToBack(cityIndex);
    delete[] m_distance_matrix[m_matrix_size - 1];
    m_matrix_size--;
  }

class CitySolver {
  CityMatrix *matrix = nullptr;

public:
  void printMatrix() { matrix->printMatrix(); }
  void makeMatrix(int number_of_cities) {
    matrix = new CityMatrix(number_of_cities);
  }
  void makeMatrix() { matrix = new CityMatrix(); }

  // Random Operations Methods:
  void swapInts(int *x, int *y);
  void printPath(int *path, int len);
  void swapCityArray(int *cities, int pos1, int pos2);

  // Methods used by Solvers
  int calcCost(int *path, int len);
  void permute(int *current_path, int l, int r, int *solver_path,
               int *solver_cost);

  // Solver Methods:
  Result solveBruteForce(int startIndex);
  Result solveGreedy(int startIndex);
};

void CitySolver::swapInts(int *x, int *y) {
  int tmp = *x;
  *x = *y;
  *y = tmp;
}

void CitySolver::printPath(int *path, int len) {
  for (int i = 0; i < len; i++) {
    cout << path[i] << " ";
  }
}

void CitySolver::swapCityArray(int *cities, int pos1, int pos2) {
  int temp = cities[pos1]; // Dereference the pointer to get the value
  cities[pos1] = cities[pos2];
  cities[pos2] = temp;
}

int CitySolver::calcCost(int *path, int len) {
  int sum = 0;
  for (int i = 0; i < len - 1; i++)
    sum += matrix->at(path[i], path[i + 1]);
  sum += matrix->at(path[len - 1], path[0]);
  return sum;
}

void CitySolver::permute(int *current_path, int l, int r, int *solver_path,
                         int *solver_cost) {
  if (l == r) {
    int cost = calcCost(current_path, r + 1);
    if (cost < *solver_cost) {
      *solver_cost = cost; // <-- Correct: update the VALUE, not the pointer
      // copy current_path into solver best buffer
      for (int i = 0; i < matrix->getSize(); i++)
        solver_path[i] = current_path[i];
    }
  } else {
    for (int i = l; i <= r; i++) {
      swapInts(&current_path[l], &current_path[i]);
      permute(current_path, l + 1, r, solver_path, solver_cost);
      swapInts(&current_path[l], &current_path[i]); // backtrack
    }
  }
}

// Solver Methods:
Result CitySolver::solveBruteForce(int startIndex) {
  auto start = steady_clock::now();
  Result out;
  int n = matrix->getSize();
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
  matrix->moveCityToFront(startIndex);
  swapCityArray(cities, 0, startIndex);

  // search all permutations with city 0 fixed
  permute(cities, 1, n - 1, brute_best_path, &brute_best_cost);

  // build Result to return; allocate fresh path for caller to own
  out.len = n;
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

  Result CitySolver::solveGreedy(int startIndex) {
    auto start = steady_clock::now();
    int n = matrix->getSize();
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
        if (!visited_cities[j] && matrix->at(current_city, j) < smallest_cost) {
          smallest_cost = matrix->at(current_city, j);
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
    greedy_cost += matrix->at(current_city, startIndex);

    // Build the Result object to return
    out.len = n;
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
  }

  int main() {
    srand((unsigned)time(NULL)); // seed once here

    int number_of_cities{};
    cout << "Input number of cities: ";
    cin >> number_of_cities;
    cout << endl;

    if (number_of_cities <= 0) {
      cout << "Error: number of cities must be > 0" << endl;
      return 1;
    }
    if (number_of_cities > 1000) { // guard against too large
      cout << "Error: number of cities too large (max 1000)" << endl;
      return 1;
    }

    CitySolver Cities1;

    Cities1.makeMatrix(number_of_cities);

    Cities1.printMatrix();

    int startIndex;
    cout << "Input starting city: ";
    cin >> startIndex;
    cout << endl;
    if (startIndex < 1 || startIndex >= number_of_cities) {
      cout << "Error: start index must be in [1," << number_of_cities << "]"
           << endl;
      return 1;
    }
    startIndex -= 1;
    cout << "Greedy Results: " << endl;
    Result res1 = Cities1.solveGreedy(startIndex);
    res1.printRes();

    cout << "============" << endl;
    cout << "Brute Force Results: " << endl;
    Result res2 = Cities1.solveBruteForce(startIndex);
    res2.printRes();

    return 0;
  }