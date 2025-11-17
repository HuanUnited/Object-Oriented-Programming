/*
1. [10 баллов] Требуется реализовать алгоритм побитовой (не путать с
поразрядной) сортировки массива. Также, для проверки работы алгоритма требуется
написать функцию проверки отсортированного массива на упорядоченность.
Достаточно сортировки неотрицательных чисел.

Исходные данные: массив целых чисел.

2. [5 баллов, если сортируются только неотрицательные числа, иначе - 10 баллов]
Используя тестовые массивы, сгенерированные для лабораторной № 3 (если алгоритм
сортирует только неотрицательные числа, перед запуском к каждому значению
массива прибавьте число так, чтобы диапазоны стали неотрицательными - например,
-100/100 -> 0/200), совершить хотя бы три запуска алгоритма на каждом тестовом
массиве и вычислить среднее время работы алгоритма для каждого из массивов
(экономьте своё время, реализовав вызов сортировок с помощью циклов, а не
перезапуском программы или дублированием кода). Полученные данные в виде отчёта
также приложите к ответу.

В репозитории отчёт оформить в виде релиза, вместо обычного добавления.
*/

#include <fmt/format.h>
#include <tuple>
#include <vector>

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

#include ".include/bitmatrix.hpp"
#include "bitvector.hpp"

using std::vector, std::cout;

// --- helper swap
template <typename T> void swap_pos(T &t1, T &t2) {
  T temp = std::move(t1);
  t1 = std::move(t2);
  t2 = std::move(temp);
}

int _partition(BitMatrix &matrix, int low, int high, int col) {
  int i = low;
  int j = high;

  while (i <= j) {
    // advance i until we find a 1 (out-of-place for left)
    while (i <= j && !matrix[i][col])
      ++i; // skip zeros; zeros belong left
    // move j left until we find a 0 (out-of-place for right)
    while (i <= j && matrix[j][col])
      --j; // skip ones; ones belong right

    if (i < j) {
      swap_pos(matrix[i], matrix[j]);
      ++i;
      --j;
    }
  }

  // j is the last index with bit == 0 (could be < low)
  return j;
}

// Iterative MSD-style radix partition sort (sort by bits MSB->LSB).
void msd_radix_sort(BitMatrix &matrix) {
  int n = static_cast<int>(matrix.rows());
  if (n <= 1)
    return;

  int ncols = static_cast<int>(matrix.columns()); // number of bits
  if (ncols <= 0)
    return;

  // stack entries: low, high, col (col increases from MSB->LSB because MSB
  // index = 0)
  std::vector<std::tuple<int, int, int>> stack;
  stack.emplace_back(0, n - 1, 0); // start at MSB (index 0)

  while (!stack.empty()) {
    auto [low, high, col] = stack.back();
    stack.pop_back();

    if (low >= high)
      continue;
    if (col >= ncols)
      continue; // no more bits

    int p = _partition(matrix, low, high, col); // last idx with bit == 0

    // left: [low, p], right: [p+1, high]
    int left_size = p - low + 1;         // may be 0
    int right_size = high - (p + 1) + 1; // may be 0

    // push deeper subproblems with next less-significant bit -> col+1
    // push bigger partition first to limit stack (optional)
    if (left_size > right_size) {
      if (left_size > 1)
        stack.emplace_back(low, p, col + 1);
      if (right_size > 1)
        stack.emplace_back(p + 1, high, col + 1);
    } else {
      if (right_size > 1)
        stack.emplace_back(p + 1, high, col + 1);
      if (left_size > 1)
        stack.emplace_back(low, p, col + 1);
    }
  }
}

// checker
bool is_sorted_asc(const vector<unsigned int> &a) {
  for (size_t i = 1; i < a.size(); ++i)
    if (a[i - 1] > a[i])
      return false;
  return true;
}

// deterministic dataset seed
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

vector<unsigned int> genRandomVector(size_t n, int lo, int hi,
                                     unsigned int seed) {
  vector<unsigned int> v;
  v.reserve(n);
  std::mt19937_64 rng(seed);
  std::uniform_int_distribution<int> dist(lo, hi);
  for (size_t i = 0; i < n; ++i)
    v.push_back(dist(rng));
  return v;
}

void writeVectorToTxt(const std::string &fname, const vector<unsigned int> &v) {
  std::ofstream os(fname);
  if (!os)
    throw std::runtime_error("Failed to open " + fname);
  os << v.size() << '\n';
  for (size_t i = 0; i < v.size(); ++i) {
    os << v[i] << (i + 1 == v.size() ? '\n' : ' ');
  }
}

double timeMSD_QuickVector(const vector<unsigned int> &vec) {
  BitMatrix dupe(vec);

  auto start = std::chrono::steady_clock::now();
  msd_radix_sort(dupe);
  auto stop = std::chrono::steady_clock::now();

  vector<unsigned int> copy = dupe.to_vec();

  if (!is_sorted_asc(copy)) {
    std::cerr << "ERROR: побитовая сортикровка produced unsorted result"
              << "\n";
    // dump few elements
    for (size_t i = 0; i < std::min<size_t>(20, copy.size()); ++i) {
      std::cerr << copy[i]
                << (i + 1 == std::min<size_t>(20, copy.size()) ? '\n' : ' ');
    }
    std::terminate();
  }
  return std::chrono::duration<double, std::milli>(stop - start).count();
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  const std::vector<size_t> sizes = {10000u, 100000u, 1000000u};
  const std::vector<std::pair<unsigned int, unsigned int>> ranges = {
      {0, 20}, {0, 2000}, {0, 200000}};

  // CSV header
  std::ofstream csv("timings_msd_radix.csv", std::ios::out);
  csv << "size,lo,hi,algorithm,run1_ms,run2_ms,run3_ms,mean_ms,sorted\n";
  csv.close();

  for (size_t n : sizes) {
    for (auto [lo, hi] : ranges) {
      unsigned int seed = datasetSeed(n, lo, hi);
      cout << "Generating dataset n=" << n << " range=[" << lo << ',' << hi
           << "] seed=" << seed << " ...\n";
      std::string fname = "data_" + std::to_string(n) + "_" +
                          std::to_string(lo) + "_" + std::to_string(hi) +
                          ".txt";

      vector<unsigned int> data = genRandomVector(n, lo, hi, seed);
      writeVectorToTxt(fname, data);
      vector<double> runs;
      runs.reserve(3);
      for (int r = 0; r < 3; ++r) {
        double tms = timeMSD_QuickVector(data);
        runs.push_back(tms);
        cout << std::fixed << std::setprecision(2) << tms << " ms"
             << (r < 2 ? ", " : "");
      }
      double mean = (runs[0] + runs[1] + runs[2]) / 3.0;
      cout << "  mean=" << std::fixed << std::setprecision(2) << mean
           << " ms\n";

      std::ofstream csv_append("timings_msd_radix.csv", std::ios::app);
      csv_append << n << ';' << lo << ';' << hi << ";MSD Binary Radix;"
                 << std::fixed << std::setprecision(3) << runs[0] << ','
                 << runs[1] << ',' << runs[2] << ',' << mean << ",1\n";
      csv_append.close();
    }
  }

  cout << "All experiments complete. See timings_msd_radix.csv and data_*.txt "
          "files.\n";
  return 0;
}
