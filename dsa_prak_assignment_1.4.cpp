// Interation 4 - Goals:
/*
1. Implementing the Greedy Algorithm <DONE>
2. Implementing the timer for each result.
3. Implementing a new way for any Algo to solve the same matrix.

*/

#include <cstddef>
#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <time.h>
using std::cin, std::cout, std::endl;
#define MAX 1000

struct Result {
  int *path; // allocated with new int[len]
  int len;
  int cost;
  double time_ms;
  Result() : path(nullptr), len(0), cost(0), time_ms(0.0) {}
};

class CityMatrix {
private:
  int m_matrix_size;
  int m_best_cost{MAX};
  int *m_best_path{nullptr};
  int **m_distance_matrix{nullptr};

public:
  // Matrix Functions
  void generateMatrix() {
    // do not call srand here; seed once in main
    for (int i = 0; i < m_matrix_size; i++) {
      for (int j = 0; j < m_matrix_size; j++) {
        if (i == j) {
          m_distance_matrix[i][j] = 0; // diagonal = 0
        } else {
          int num = rand() % 20 + 1; // 1..20 (avoid 0 except diagonal)
          m_distance_matrix[i][j] = num;
        }
      }
    }
  }

  void printMatrix() {
    cout << "Matrix is: " << endl;
    for (int i = 0; i < m_matrix_size; i++) {
      for (int j = 0; j < m_matrix_size; j++) {
        cout << m_distance_matrix[i][j] << " ";
      }
      cout << endl;
    }
  }

  void swapRows(int pos_from, int pos_to) {
    // swap rows in matrix
    for (int j = 0; j < m_matrix_size; j++) {
      int tmp = at(pos_to, j);
      at(pos_to, j) = at(pos_from, j);
      at(pos_from, j) = tmp;
    }
  }

  void swapColumns(int pos_from, int pos_to) {
    // swap columns in matrix
    for (int i = 0; i < m_matrix_size; i++) {
      int tmp = at(i, pos_to);
      at(i, pos_to) = at(i, pos_from);
      at(i, pos_from) = tmp;
    }
  }
  void moveCityToFront(int idx) {
    swapRows(idx, 0);
    swapColumns(idx, 0);
  }

  void moveCityToBack(int idx) {
    swapRows(idx, m_matrix_size - 1);
    swapColumns(idx, m_matrix_size - 1);
  }

  int getSmallestinRowIndex(
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

  int getSmallestinRowValue(
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

  void deleteCity(int cityIndex) {
    // move the city's row and column to the last row and column. and then
    // delete that from the indexes. matrix_size--.
    moveCityToBack(cityIndex);
    delete[] m_distance_matrix[m_matrix_size - 1];
    m_matrix_size--;
  }

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

class CitySolver {
  CityMatrix *matrix = nullptr;
  int current_solver_best_cost{};
  int *current_solver_best_path = nullptr;
  int *cities = nullptr;

public:
  void printMatrix() { matrix->printMatrix(); }

  void makeMatrix(int number_of_cities) {
    matrix = new CityMatrix(number_of_cities);
  }
  void makeMatrix() { matrix = new CityMatrix(); }

  // Random Operations Methods:
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
  int calcCost(int *path, int len) {
    int sum = 0;
    for (int i = 0; i < len - 1; i++)
      sum += matrix->at(path[i], path[i + 1]);
    sum += matrix->at(path[len - 1], path[0]);
    return sum;
  }

  void permute(int *current_path, int l, int r) {
    if (l == r) {
      int cost = calcCost(current_path, r + 1);
      if (cost < current_solver_best_cost) {
        current_solver_best_cost = cost;
        // copy current_path into solver best buffer
        for (int i = 0; i < matrix->getSize(); i++)
          current_solver_best_path[i] = current_path[i];
      }
    } else {
      for (int i = l; i <= r; i++) {
        swapInts(&current_path[l], &current_path[i]);
        permute(current_path, l + 1, r);
        swapInts(&current_path[l], &current_path[i]); // backtrack
      }
    }
  }

  // Small note: to create a copy of the beginning matrix in the solver methods
  // so that they can freely manipulate it instead touching the original matrix.

  // Solver Methods:
  Result bruteForce(int startIndex) {
    Result out;
    int n = matrix->getSize();
    if (n <= 0)
      return out;

    // prepare city array
    cities = new int[n];
    for (int i = 0; i < n; i++)
      cities[i] = i;
    cout << "List of cities:";
    for (int i = 0; i < n; i++)
      cout << " " << cities[i];
    cout << endl;

    // init solver best
    current_solver_best_cost = MAX;
    delete[] current_solver_best_path; // guard
    current_solver_best_path = new int[n];

    // move chosen start to front (reorders matrix), and align cities[]
    matrix->moveCityToFront(startIndex);
    swapCityArray(cities, 0, startIndex);
    matrix->printMatrix();

    // search all permutations with city 0 fixed
    permute(cities, 1, n - 1);

    // build Result to return; allocate fresh path for caller to own
    out.len = n;
    out.cost = current_solver_best_cost;
    out.path = new int[n];
    for (int i = 0; i < n; i++)
      out.path[i] = current_solver_best_path[i];

    // clean up solver internals; returned Result owns the path
    delete[] current_solver_best_path;
    current_solver_best_path = nullptr;
    current_solver_best_cost = MAX;

    delete[] cities;
    return out;
  }

  Result solveGreedy(int startIndex) {
    int n = matrix->getSize();
    Result out; // Assuming Result has members for path and cost

    if (n <= 0) {
        return out;
    }

    // Use std::vector for safe dynamic memory management
    int *greedy_path = new int[n];
    bool *visited_cities = new bool[n];
    for (int i = 0; i < n; i++){
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
            break; //if matrix is invalid
        }
    }

    // Add cost to return to starting city to complete the cycle
    greedy_cost += matrix->at(current_city, startIndex);

    cout << endl;
    cout << "Greedy cost: " << greedy_cost << endl;

    // Build the Result object to return
    out.len = n;
    out.cost = greedy_cost;
    out.path = new int[n];
    for (int i = 0; i < n; i++)
        out.path[i] = greedy_path[i];
    
    delete [] visited_cities;
    delete [] greedy_path;

    return out;
}

};

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
  if (startIndex < 0 || startIndex >= number_of_cities) {
    cout << "Error: start index must be in [0," << number_of_cities - 1 << "]"
         << endl;
    return 1;
  }

  Result res = Cities1.solveGreedy(startIndex);

  if (res.path != nullptr) {
    cout << "Solver best cost: " << res.cost << endl;
    cout << "Solver best path: ";
    for (int i = 0; i < res.len; i++)
      cout << res.path[i] << " ";
    cout << endl;
    // let matrix keep canonical best if it's better
    delete[] res.path; // caller frees returned path
  } else {
    cout << "No result (n==0?)" << endl;
  }
  return 0;
}