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

#include <cstdint>
#include <tuple>
#include <vector>

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

using std::vector, std::cout;

// --- helper swap
template <typename T> void swap_pos(T &t1, T &t2) {
  T temp = std::move(t1);
  t1 = std::move(t2);
  t2 = std::move(temp);
}

void msb_radixsort(vector<int> &out) {

  if (out.empty())
    return;
  const uint32_t SIGN_MASK = 0x80000000u;
  int n = out.size();

  // Create a mask for negative numbers
  std::vector<uint32_t> keys(n);
  for (int i = 0; i < n; ++i) {
    keys[i] = static_cast<uint32_t>(out[i]) ^ SIGN_MASK;
  }

  std::vector<uint32_t> tmp(n);

  using triple = std::tuple<int, int, int>;

  vector<triple> stack;
  stack.emplace_back(0, n, 31);

  while (!stack.empty()) {
    auto [l, r, bit] = stack.back();
    stack.pop_back();

    int len = r - l;

    if (len <= 1 || bit < 0)
      continue;

    // mask for specific bit in number
    uint32_t mask = 1u << bit;

    int zero_count = 0;
    for (int i = l; i < r; ++i) {
      if ((keys[i] & mask) == 0)
        ++zero_count;
    }

    // TODO: EXPLANATION

    /*
    Explanation:
    zero count counts how many elements in the segment has the bit = 0;


    zpos determines the starting position of zeroes
    opos determines the starting position of ones.

    simply put, if we have an array of 6 elements, 2 has the 7th bit = 0, 4 has
    the 7th bit = 1 then zpos = 0 -> starting position of zeroes will always be
    on the left and opos = 2 -> starting position of the ones will always be
    after the zeroes.

    next we iterate through the array from left to write, and throw everything
    into temp while preserving order.

    then we create a new partition, rinse and repeat until the stack is empty
    (either when the bits end, or when then length is = 1)
    */

    int zpos = l;
    int opos = l + zero_count;
    for (int i = l; i < r; ++i) {
      if ((keys[i] & mask) == 0)
        tmp[zpos++] = keys[i];
      else
        tmp[opos++] = keys[i];
    }

    for (int i = l; i < r; ++i) {
      keys[i] = tmp[i];
    }

    int mid = l + zero_count;
    // CORRECT: push BOTH non-empty subsegments (right first to mimic recursion
    // order)
    if (mid < r)
      stack.emplace_back(mid, r, bit - 1); // ones-group
    if (l < mid)
      stack.emplace_back(l, mid, bit - 1); // zeros-group
  }

  for (int i = 0; i < n; ++i) {
    uint32_t k = keys[i] ^ SIGN_MASK;
    out[i] = static_cast<int>(k);
  }
}

// checker
bool is_sorted_asc(const vector<int> &a) {
  for (size_t i = 1; i < a.size(); ++i)
    if (a[i - 1] > a[i])
      return false;
  return true;
}

// deterministic dataset seed
int datasetSeed(size_t n, int lo, int hi) {
  uint64_t s = 1469598103934665603ull;
  s ^= n;
  s *= 1099511628211ull;
  s ^= static_cast<uint64_t>(lo + 0x9e3779b1);
  s *= 1099511628211ull;
  s ^= static_cast<uint64_t>(hi ^ 0x9e3779b1);
  s *= 1099511628211ull;
  return static_cast<int>(s & 0xFFFFFFFFu);
}

vector<int> genRandomVector(size_t n, int lo, int hi, int seed) {
  vector<int> v;
  v.reserve(n);
  std::mt19937_64 rng(seed);
  std::uniform_int_distribution<int> dist(lo, hi);
  for (size_t i = 0; i < n; ++i)
    v.push_back(dist(rng));
  return v;
}

void writeVectorToTxt(const std::string &fname, const vector<int> &v) {
  std::ofstream os(fname);
  if (!os)
    throw std::runtime_error("Failed to open " + fname);
  os << v.size() << '\n';
  for (size_t i = 0; i < v.size(); ++i) {
    os << v[i] << (i + 1 == v.size() ? '\n' : ' ');
  }
}

double timeMSD_QuickVector(const vector<int> &vec) {

  vector<int> dupe = vec;

  auto start = std::chrono::steady_clock::now();
  msb_radixsort(dupe);
  auto stop = std::chrono::steady_clock::now();

  if (!is_sorted_asc(dupe)) {
    std::cerr << "ERROR: побитовая сортикровка produced unsorted result"
              << "\n";
    // dump few elements
    for (size_t i = 0; i < std::min<size_t>(20, dupe.size()); ++i) {
      std::cerr << dupe[i]
                << (i + 1 == std::min<size_t>(20, dupe.size()) ? '\n' : ' ');
    }
    std::terminate();
  }
  return std::chrono::duration<double, std::milli>(stop - start).count();
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  const std::vector<size_t> sizes = {10000u, 100000u, 1000000u};
  const std::vector<std::pair<int, int>> ranges = {
      {-10, 10}, {-1000, 1000}, {-100000, 100000}};

  // CSV header
  std::ofstream csv("timings_msd_radix.csv", std::ios::out);
  csv << "size;lo;hi;algorithm;run1_ms;run2_ms;run3_ms;mean_ms;sorted\n";
  csv.close();

  for (size_t n : sizes) {
    for (auto [lo, hi] : ranges) {
      int seed = datasetSeed(n, lo, hi);
      cout << "Generating dataset n=" << n << " range=[" << lo << ',' << hi
           << "] seed=" << seed << " ...\n";
      std::string fname = "data_" + std::to_string(n) + "_" +
                          std::to_string(lo) + "_" + std::to_string(hi) +
                          ".txt";

      vector<int> data = genRandomVector(n, lo, hi, seed);
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
