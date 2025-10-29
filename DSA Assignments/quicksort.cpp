/*

1. [10 баллов] Требуется реализовать алгоритм Хоара сортировки массива (алгоритм
быстрой сортировки). Также, для проверки работы алгоритма требуется написать
функцию проверки отсортированного массива на упорядоченность.

Исходные данные: массив целых чисел.

2. [10 баллов] Используя тестовые массивы, сгенерированные для лабораторной № 3,
совершить хотя бы три запуска алгоритма на каждом тестовом массиве и вычислить
среднее время работы алгоритма для каждого из массивов (экономьте своё время,
реализовав вызов сортировок с помощью циклов, а не перезапуском программы или
дублированием кода). Полученные данные в виде отчёта также приложите к ответу.

В репозитории отчёт оформить в виде релиза, вместо обычного добавления.

*/

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

// --- helper integer power (non-conflicting name) ---
long long ipow(long long base, int exp) {
  if (exp <= 0) return 1;
  long long r = 1;
  while (exp--) r *= base;
  return r;
}

// checker
bool is_sorted_asc(const vector<int> &a) {
  for (size_t i = 1; i < a.size(); ++i)
    if (a[i - 1] > a[i]) return false;
  return true;
}

// deterministic dataset seed (similar to heapsort example)
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

vector<int> genRandomVector(size_t n, int lo, int hi, unsigned int seed) {
  vector<int> v;
  v.reserve(n);
  std::mt19937_64 rng(seed);
  std::uniform_int_distribution<int> dist(lo, hi);
  for (size_t i = 0; i < n; ++i) v.push_back(dist(rng));
  return v;
}

void writeVectorToTxt(const string &fname, const vector<int> &v) {
  ofstream os(fname);
  if (!os) throw runtime_error("Failed to open " + fname);
  os << v.size() << '\n';
  for (size_t i = 0; i < v.size(); ++i) {
    os << v[i] << (i + 1 == v.size() ? '\n' : ' ');
  }
}

double timeQuickOnVector(const vector<int> &vec, bool verbose = false) {
  vector<int> copy = vec;  // copy once per trial
  auto start = chrono::steady_clock::now();
  shell_sort_inplace(copy, gtype);
  auto stop = chrono::steady_clock::now();
  if (!is_sorted_asc(copy)) {
    cerr << "ERROR: shell_sort produced unsorted result for " << gapName(gtype)
         << "\n";
    // dump few elements
    for (size_t i = 0; i < min<size_t>(20, copy.size()); ++i) {
      cerr << copy[i] << (i + 1 == min<size_t>(20, copy.size()) ? '\n' : ' ');
    }
    terminate();
  }
  return chrono::duration<double, milli>(stop - start).count();
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  const vector<size_t> sizes = {10000u, 100000u, 1000000u};
  const vector<pair<int, int>> ranges = {
      {-10, 10}, {-1000, 1000}, {-100000, 100000}};
  const vector<GapType> algorithms = {GapType::Shell, GapType::Hibbard,
                                      GapType::Knuth, GapType::Sedgewick};

  // CSV header
  ofstream csv("timings_quick.csv", ios::out);
  csv << "size,lo,hi,algorithm,run1_ms,run2_ms,run3_ms,mean_ms,sorted\n";
  csv.close();

  for (size_t n : sizes) {
    for (auto [lo, hi] : ranges) {
      unsigned int seed = datasetSeed(n, lo, hi);
      cout << "Generating dataset n=" << n << " range=[" << lo << ',' << hi
           << "] seed=" << seed << " ...\n";
      string fname = "data_" + to_string(n) + "_" + to_string(lo) + "_" +
                     to_string(hi) + ".txt";

      vector<int> data = genRandomVector(n, lo, hi, seed);
      writeVectorToTxt(fname, data);

      for (GapType g : algorithms) {
        cout << "  Algorithm: " << gapName(g) << " ... " << flush;
        vector<double> runs;
        runs.reserve(3);
        for (int r = 0; r < 3; ++r) {
          double tms = timeShellOnVector(data, g);
          runs.push_back(tms);
          cout << fixed << setprecision(2) << tms << " ms"
               << (r < 2 ? ", " : "");
        }
        double mean = (runs[0] + runs[1] + runs[2]) / 3.0;
        cout << "  mean=" << fixed << setprecision(2) << mean << " ms\n";

        ofstream csv_append("timings_quick.csv", ios::app);
        csv_append << n << ',' << lo << ',' << hi << ',' << gapName(g) << ','
                   << fixed << setprecision(3) << runs[0] << ',' << runs[1]
                   << ',' << runs[2] << ',' << mean << ",1\n";
        csv_append.close();
      }
    }
  }

  cout << "All experiments complete. See timings_quick.csv and data_*.txt "
          "files.\n";
  return 0;
}