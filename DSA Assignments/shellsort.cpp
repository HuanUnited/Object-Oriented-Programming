/*

1. [10 баллов] Требуется реализовать алгоритм Шелла для сортировки массива.
Также, для проверки работы алгоритма требуется написать функцию проверки
отсортированного массива на упорядоченность.

Исходные данные: массив целых чисел.

2. [10 баллов] Дополнительно:
- выбрать три различных алгоритма выбора длины промежутков;
- Используя тестовые массивы, сгенерированные для лабораторной работы № 3,
совершить хотя бы по три запуска каждого алгоритма выбора диапазона на каждом
тестовом массиве и вычислить среднее время работы алгоритма для каждого из
массивов (экономьте своё время, реализовав вызов сортировок с помощью циклов, а
не перезапуском программы или дублированием кода). Полученные данные в виде
отчёта также приложите к ответу.

В репозитории отчёт и файлы с данными оформить в виде релиза, вместо обычного
добавления.

*/

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
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

// --- gap types ---
enum class GapType { Shell = 0, Hibbard = 1, Knuth = 2, Sedgewick = 3 };

string gapName(GapType g) {
  switch (g) {
    case GapType::Shell:
      return "Shell";
    case GapType::Hibbard:
      return "Hibbard";
    case GapType::Knuth:
      return "Knuth";
    case GapType::Sedgewick:
      return "Sedgewick";
    default:
      return "Unknown";
  }
}

// Produce gaps (descending order, each >= 1 and < n)
vector<int> make_gaps(size_t n, GapType gtype) {
  vector<int> gaps;
  if (n <= 1) return gaps;

  switch (gtype) {
    case GapType::Shell: {
      // classic Shell: n/2, n/4, ... 1 (stop when gap >= 1)
      for (size_t gap = n / 2; gap >= 1; gap /= 2) {
        gaps.push_back(static_cast<int>(gap));
        if (gap == 1) break;  // avoid infinite loop when gap==1
      }
      break;
    }
    case GapType::Hibbard: {
      // Hibbard: 1, 3, 7, 15, ... = 2^k - 1. We need descending < n.
      vector<int> tmp;
      for (int k = 1;; ++k) {
        long long val = ipow(2, k) - 1;  // step value
        if (val >= (long long)n)
          break;  // if step is larger or equal to n then break
        tmp.push_back(static_cast<int>(val));
        // safety
        if (k > 60) break;
      }

      // reverse to descending
      for (auto it = tmp.rbegin(); it != tmp.rend(); ++it) gaps.push_back(*it);
      break;
    }
    case GapType::Knuth: {
      // Knuth: 1, 4, 13, 40, ... = (3^k - 1)/2
      vector<int> tmp;
      for (int k = 1;; ++k) {
        long long val = (ipow(3, k) - 1) / 2;
        if (val >= (long long)n) break;
        tmp.push_back(static_cast<int>(val));
        if (k > 40) break;
      }

      // reverse to descending
      for (auto it = tmp.rbegin(); it != tmp.rend(); ++it) gaps.push_back(*it);
      break;
    }

    // Sedgewick 1982
    case GapType::Sedgewick: {
      vector<int> tmp;
      for (int k = 1;; ++k) {
        long long val = ipow(4, k) + (3 * ipow(2, k - 1)) + 1;
        if (val >= (long long)n) break;
        tmp.push_back(static_cast<int>(val));
        if (k > 40) break;
      }
      // reverse to descending
      for (auto it = tmp.rbegin(); it != tmp.rend(); ++it) gaps.push_back(*it);
      gaps.push_back(1);
      break;
    }
  }

  // ensure uniqueness and sanity (no zero gaps)
  gaps.erase(remove_if(gaps.begin(), gaps.end(), [](int x) { return x <= 0; }),
             gaps.end());
  if (gaps.empty()) gaps.push_back(1);
  return gaps;
}

// Shell (gapped insertion) sort in-place
void shell_sort_inplace(vector<int> &a, GapType gtype) {
  size_t n = a.size();
  vector<int> gaps = make_gaps(n, gtype);

  // optional: print gaps once (comment out to reduce console clutter)
  cout << gapName(gtype) << " gaps: [";
  for (size_t i = 0; i < gaps.size(); ++i) {
    if (i) cout << ", ";
    cout << gaps[i];
  }
  cout << "]\n";

  for (int gap : gaps) {
    // gapped insertion sort
    for (size_t i = gap; i < n; ++i) {
      int tmp = a[i];
      size_t j = i;
      while (j >= (size_t)gap && a[j - gap] > tmp) {
        a[j] = a[j - gap];
        j -= gap;
      }
      a[j] = tmp;
    }
  }
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

double timeShellOnVector(const vector<int> &vec, GapType gtype,
                         bool verbose = false) {
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
  ofstream csv("timings_shell.csv", ios::out);
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

        ofstream csv_append("timings_shell.csv", ios::app);
        csv_append << n << ',' << lo << ',' << hi << ',' << gapName(g) << ','
                   << fixed << setprecision(3) << runs[0] << ',' << runs[1]
                   << ',' << runs[2] << ',' << mean << ",1\n";
        csv_append.close();
      }
    }
  }

  cout << "All experiments complete. See timings_shell.csv and data_*.txt "
          "files.\n";
  return 0;
}