/*
Пирамидальная Сортировка

1. [10 баллов] Требуется реализовать алгоритм пирамидальной сортировки массива.
Также, для проверки работы алгоритма требуется написать функцию проверки
отсортированного массива на упорядоченность. Исходные данные: массив целых
чисел.

2. [10 баллов] Дополнительно:
- сгенерировать 9 различных массивов, заполненных случайными значениями:
(размером 10000, 100000 и 1000000 элементов, для каждого по три диапазона
значений: -10/10, -1000/1000, -100000/100000). Обратите внимание на диапазон
значений, выдаваемых функцией rand - при необходимости замените её другим ГСПЧ.
Полученные массивы записать в файл и приложить к ответу (эти же выборки будут
затем использоваться для других лабораторных);
- совершить хотя бы три запуска алгоритма на каждом тестовом массиве и вычислить
среднее время работы алгоритма для каждого из массивов (экономьте своё время,
реализовав вызов сортировок с помощью циклов, а не перезапуском программы или
дублированием кода). Полученные данные в виде отчёта также приложите к ответу.
*/

#include <cassert>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
using std::cout, std::cin, std::endl;

class Heap {
  // Using a derived class for dynamic array purposes.
 public:
  int getLastParentNode() const { return m_size / 2 - 1; }
  int getLastParentNode(int const remaining) const { return remaining / 2 - 1; }

  int getParentNode(int const index) const { return (index - 1) / 2; }
  int getLeftChild(int const index) const { return (2 * index + 1); }
  int getRightChild(int const index) const { return (2 * index + 2); }

  size_t size() const { return m_size; }
  size_t capacity() const { return m_capacity; }
  int at(int index) const { return m_arr[index]; }

  int max(int const idx1, int const idx2) const {
    return m_arr[idx1] < m_arr[idx2] ? idx2 : idx1;
  }

  void genRandom(int minv = 1, int maxv = 20, unsigned int seed = 0) {
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<int> dist(minv, maxv);
    for (size_t i = 0; i < m_capacity; i++) {
      int v{dist(rng)};
      push_back(v);
    }
  }

  void swap(int &x, int &y) {
    int temp = std::move(x);
    x = std::move(y);
    y = std::move(temp);
  }

  // void maxHeapify(int parent, int size) {
  //   int largest{parent};
  //   int left{getLeftChild(parent)};
  //   int right{getRightChild(parent)};

  //   if (left < size && m_arr[left] > m_arr[largest]) largest = left;

  //   if (right < size && m_arr[right] > m_arr[largest]) largest = right;

  //   if (largest != parent) {
  //     swap(m_arr[parent], m_arr[largest]);
  //     maxHeapify(largest, size);
  //   }
  // }

  // void buildHeap() {
  //   for (int i = (m_size - 1) / 2; i >= 0; i--) {
  //     maxHeapify(i, m_size);
  //   }
  // }

  void maxHeapify(int start, int end) {
    int parent = start;

    while (true) {
      int largest = parent;
      int left = getLeftChild(parent);
      int right = getRightChild(parent);

      if (left <= end && m_arr[left] > m_arr[largest]) largest = left;
      if (right <= end && m_arr[right] > m_arr[largest]) largest = right;

      if (largest != parent) {
        swap(m_arr[parent], m_arr[largest]);
        parent = largest;  // continue heapifying down
      } else {
        break;  // no more swaps needed
      }
    }
  }

  // Build max heap
  void buildMaxHeap() {
    int lastParent = (m_size - 2) / 2;  // last node with children
    for (int i = lastParent; i >= 0; --i) {
      maxHeapify(i, m_size - 1);
    }
  }

  // Iterative heapsort
  void sortHeap() {
    buildMaxHeap();
    for (int end = m_size - 1; end > 0; --end) {
      swap(m_arr[0], m_arr[end]);  // move max to the end
      maxHeapify(0, end - 1);      // heapify reduced heap
    }
    assert(isSorted());
  }

  bool isSorted() {
    for (size_t i = 0; i < (int)m_size - 1; i++) {
      if (m_arr[i] > m_arr[i + 1]) {
        return false;
      }
    }
    return true;
  }

  void print() {
    std::cout << "[";
    for (size_t i = 0; i < (size_t)m_size; i++) {
      if (i) cout << ", ";
      cout << m_arr[i];
    }
    cout << "]" << '\n';
  }

  // Dynamic
  void reserve(size_t newCapacity) {
    if (newCapacity <= m_capacity) return;
    int *tmp = new int[newCapacity];
    for (size_t i = 0; i < m_size; ++i)
      tmp[i] = std::move(m_arr[i]);  // std::move takes the value from the box,
                                     // puts it into a new box.
    delete[] m_arr;                  // burns the box
    m_arr = tmp;                     // now new box is box
    m_capacity = newCapacity;
  }
  void shrink_to_fit() {
    if (m_capacity == m_size) return;
    size_t newCap =
        m_size == 0 ? 1
                    : m_size;  // case check for the capacity, if it is equal
                               // to zero then the cap is 1, else = size
    int *tmp = new int[newCap];
    for (size_t i = 0; i < m_size; ++i) tmp[i] = std::move(m_arr[i]);
    delete[] m_arr;
    m_arr = tmp;
    m_capacity = newCap;
  }
  void clear() noexcept { m_size = 0; }
  void push_back(const int &value) {
    if (m_size == m_capacity) reserve(m_capacity * 2);
    m_arr[m_size++] = value;
  }
  void push_back(int &&value) {
    if (m_size == m_capacity) reserve(m_capacity * 2);
    m_arr[m_size++] = std::move(value);
  }

  // I/O:
  void writeArrayToFile(const std::string &fname, const int *a, size_t n) {
    std::ofstream os(fname);
    os << n << '\n';
    for (size_t i = 0; i < n; ++i) os << a[i] << (i + 1 == n ? '\n' : ' ');
  }

  // Constructors:
  Heap(int cap) {
    m_size = 0;
    m_capacity = cap;
    m_arr = new int[cap];
  }

  Heap(bool rand, int cap, int minv = 1, int maxv = 20) : Heap(cap) {
    // Fills array up with random numbers.
    if (rand) {
      genRandom(minv, maxv);
    }
  }

  // Copy Constructor
  Heap(const Heap &other)
      : m_arr(new int[other.m_capacity]),
        m_capacity(other.m_capacity),
        m_size(other.m_size) {
    for (size_t i = 0; i < m_size; ++i) m_arr[i] = other.m_arr[i];
  }

  Heap(std::initializer_list<int> ilist)
      : m_arr(nullptr), m_capacity(ilist.size()), m_size(ilist.size()) {
    if (m_capacity == 0) {
      m_arr = new int[1];
      m_capacity = 1;
      m_size = 0;
    } else {
      m_arr = new int[m_capacity];
      size_t i = 0;
      for (auto val : ilist) m_arr[i++] = val;
    }
  }

 private:
  int *m_arr;
  size_t m_capacity;
  size_t m_size;
};

inline std::istream &operator>>(std::istream &is, Heap &a) {
  size_t n;
  if (!(is >> n)) return is;
  a.clear();
  if (a.capacity() < n) a.reserve(n);
  for (size_t i = 0; i < n; ++i) {
    int tmp;
    is >> tmp;
    a.push_back(tmp);
  }
  return is;
}

inline std::ostream &operator<<(std::ostream &os, const Heap &a) {
  os << "[";
  for (size_t i = 0; i < a.size(); ++i) {
    if (i) os << ", ";
    os << a.at(i);
  }
  os << "]" << '\n';
  return os;
}

Heap heapFromVector(const std::vector<int> &v) {
  Heap h(static_cast<int>(v.size()));
  for (size_t i = 0; i < v.size(); ++i) h.push_back(v[i]);
  return h;
}

// Write text file: first line n, then whitespace separated ints
void writeVectorToTxt(const std::string &fname, const std::vector<int> &v) {
  std::ofstream os(fname);
  if (!os) throw std::runtime_error("Failed to open " + fname);
  os << v.size() << '\n';
  for (size_t i = 0; i < v.size(); ++i) {
    os << v[i] << (i + 1 == v.size() ? '\n' : ' ');
  }
}

// Read text file written by writeVectorToTxt
std::vector<int> readVectorFromTxt(const std::string &fname) {
  std::ifstream is(fname);
  if (!is) throw std::runtime_error("Failed to open " + fname);
  size_t n;
  is >> n;
  std::vector<int> v;
  v.reserve(n);
  for (size_t i = 0; i < n; ++i) {
    int x;
    is >> x;
    v.push_back(x);
  }
  return v;
}

// Deterministic per-dataset seed function
unsigned int datasetSeed(size_t n, int lo, int hi) {
  uint64_t s = 1469598103934665603ull;
  s ^= n;
  s *= 1099511628211ull;
  s ^= static_cast<uint64_t>(lo + 0x9e3779b1);
  s *= 1099511628211ull;
  s ^= static_cast<uint64_t>(hi ^ 0x9e3779b1);
  s *= 1099511628211ull;
  return static_cast<unsigned int>(s & 0xFFFFFFFFu);
}

// Generate vector of ints with given seed
std::vector<int> genRandomVector(size_t n, int lo, int hi, unsigned int seed) {
  std::vector<int> v;
  v.reserve(n);
  std::mt19937_64 rng(seed);
  std::uniform_int_distribution<int> dist(lo, hi);
  for (size_t i = 0; i < n; ++i) v.push_back(dist(rng));
  return v;
}

double timeSortHeapOnVector(const std::vector<int> &vec) {
  Heap h = heapFromVector(vec);  // copy into Heap (fresh)
  auto start = std::chrono::steady_clock::now();
  h.sortHeap();
  auto stop = std::chrono::steady_clock::now();
  // verify correctness
  if (!h.isSorted()) {
    std::cerr << "ERROR: heap.sortHeap() produced unsorted result!\n";
    // Optionally dump a few elements for debugging
    for (size_t i = 0; i < std::min<size_t>(10, h.size()); ++i)
      std::cerr << h.at(i)
                << (i + 1 == std::min<size_t>(10, h.size()) ? '\n' : ' ');
    std::terminate();
  }
  return std::chrono::duration<double, std::milli>(stop - start).count();
}

int main() {
  // dataset specs
  const std::vector<size_t> sizes = {10000u, 100000u, 1000000u};
  const std::vector<std::pair<int, int>> ranges = {
      {-10, 10}, {-1000, 1000}, {-100000, 100000}};

  // create/open CSV and write header (overwrite existing file)
  std::ofstream csv("timings.csv", std::ios::out);
  csv << "size,lo,hi,run1_ms,run2_ms,run3_ms,mean_ms,sorted\n";
  csv.close();

  // Generate datasets and run experiments
  for (size_t n : sizes) {
    for (auto [lo, hi] : ranges) {
      unsigned int seed = datasetSeed(n, lo, hi);
      std::cout << "Generating dataset n=" << n << " range=[" << lo << ',' << hi
                << "] seed=" << seed << " ...\n";
      std::string fname = "data_" + std::to_string(n) + "_" +
                          std::to_string(lo) + "_" + std::to_string(hi) +
                          ".txt";

      // Generate and write dataset (only if you want to regenerate every run)
      std::vector<int> data = genRandomVector(n, lo, hi, seed);
      writeVectorToTxt(fname, data);

      // Now run three trials, always starting from the vector `data`
      std::vector<double> runs;
      runs.reserve(3);
      bool ok = true;
      for (int r = 0; r < 3; ++r) {
        std::cout << "  Run " << (r + 1) << " ... " << std::flush;
        double t = timeSortHeapOnVector(data);
        runs.push_back(t);
        std::cout << std::fixed << std::setprecision(2) << t << " ms\n";
      }
      double mean = (runs[0] + runs[1] + runs[2]) / 3.0;

      // append to CSV
      std::ofstream csv_append("timings.csv", std::ios::app);
      csv_append << n << ',' << lo << ',' << hi << ',' << std::fixed
                 << std::setprecision(3) << runs[0] << ',' << runs[1] << ','
                 << runs[2] << ',' << mean << ',' << (ok ? "1" : "0") << '\n';
      csv_append.close();
    }
  }

  std::cout
      << "All experiments complete. See timings.csv and data_*.txt files.\n";
  return 0;
}