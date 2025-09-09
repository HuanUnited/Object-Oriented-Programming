// Iteration number 3, names of assignments now changed to DSA for betterclarity.
// Overhauled the entire codebase in order to appeal for OOP.
/*
Goals: <Finished>
Creating a class for matrix generation and manipulation
Creating a class wrapper for finding the most optimal path through the array.
*/

// Changes made (Iteration 3):
/*
1. Refactored solver–matrix relationship:
   - Solver now owns a CityMatrix* instead of referencing an external object.
   - Added makeMatrix() methods for creating matrices inside the solver.

2. Added input validation:
   - Guard for invalid number of cities (<=0 or too large).
   - Guard for invalid starting city index.

3. Matrix initialization:
   - Best path initialized to -1 in constructors to indicate “unsolved” state.

4. Starting city handling:
   - Always reorder matrix with moveCityToFront(startIndex).
   - Swap cities array to align with matrix order.

5. Debugging support:
   - Print cities array before permutations.
   - Print matrix after reordering.

6. Encapsulation in solver:
   - Added printMatrix() method.
   - Added swapCityArray() method.

7. Memory safety:
   - Guard delete[] before re-allocating solver best path.
   - Explicit caller responsibility for freeing Result.path.

8. Improved error messages for clarity.
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

public:
void printMatrix(){
    matrix->printMatrix();
}

 void makeMatrix(int number_of_cities){
    matrix = new CityMatrix(number_of_cities);
 }
 void makeMatrix(){
    matrix = new CityMatrix();
 }

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

  // Solver Methods:
  Result bruteForce(int startIndex) {
    Result out;
    int n = matrix->getSize();
    if (n <= 0)
      return out;

    // prepare city array
    int *cities = new int[n];
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

  Result res = Cities1.bruteForce(startIndex); // example start city = 0

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