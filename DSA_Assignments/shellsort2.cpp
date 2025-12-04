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
  if (exp <= 0)
    return 1;
  long long r = 1;
  while (exp--)
    r *= base;
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
  if (n <= 1)
    return gaps;

  switch (gtype) {
  case GapType::Shell: {
    // classic Shell: n/2, n/4, ... 1 (stop when gap >= 1)
    for (size_t gap = n / 2; gap >= 1; gap /= 2) {
      gaps.push_back(static_cast<int>(gap));
      if (gap == 1)
        break; // avoid infinite loop when gap==1
    }
    break;
  }
  case GapType::Hibbard: {
    // Hibbard: 1, 3, 7, 15, ... = 2^k - 1. We need descending < n.
    vector<int> tmp;
    for (int k = 1;; ++k) {
      long long val = ipow(2, k) - 1; // step value
      if (val >= (long long)n)
        break; // if step is larger or equal to n then break
      tmp.push_back(static_cast<int>(val));
      // safety
      if (k > 60)
        break;
    }

    // reverse to descending
    for (auto it = tmp.rbegin(); it != tmp.rend(); ++it)
      gaps.push_back(*it);
    break;
  }
  case GapType::Knuth: {
    // Knuth: 1, 4, 13, 40, ... = (3^k - 1)/2
    vector<int> tmp;
    for (int k = 1;; ++k) {
      long long val = (ipow(3, k) - 1) / 2;
      if (val >= (long long)n)
        break;
      tmp.push_back(static_cast<int>(val));
      if (k > 40)
        break;
    }

    // reverse to descending
    for (auto it = tmp.rbegin(); it != tmp.rend(); ++it)
      gaps.push_back(*it);
    break;
  }

  // Sedgewick 1982
  case GapType::Sedgewick: {
    vector<int> tmp;
    for (int k = 1;; ++k) {
      long long val = ipow(4, k) + (3 * ipow(2, k - 1)) + 1;
      if (val >= (long long)n)
        break;
      tmp.push_back(static_cast<int>(val));
      if (k > 40)
        break;
    }
    // reverse to descending
    for (auto it = tmp.rbegin(); it != tmp.rend(); ++it)
      gaps.push_back(*it);
    gaps.push_back(1);
    break;
  }
  }

  // ensure uniqueness and sanity (no zero gaps)
  gaps.erase(remove_if(gaps.begin(), gaps.end(), [](int x) { return x <= 0; }),
             gaps.end());
  if (gaps.empty())
    gaps.push_back(1);
  return gaps;
}

// Simple Insertion Sort with comparison counting
std::pair<long long, long long> insertion_sort_inplace(vector<int> &a) {
  long long comparisons = 0;
  long long swaps = 0;
  size_t n = a.size();

  for (size_t i = 1; i < n; ++i) {
    int tmp = a[i];
    size_t j = i;
    while (j > 0) {
      comparisons++; // count the comparison
      if (a[j - 1] > tmp) {
        a[j] = a[j - 1];
        // count the swaps
        swaps++;
        j--;
      } else {
        break;
      }
    }
    a[j] = tmp;
  }

  return {comparisons, swaps};
}

// Shell (gapped insertion) sort in-place with comparison counting
std::pair<long long, long long>
shell_sort_inplace(vector<int> &a, GapType gtype, bool print_gaps = false) {
  long long comparisons = 0;
  long long swaps = 0;
  size_t n = a.size();
  vector<int> gaps = make_gaps(n, gtype);

  // optional: print gaps once
  if (print_gaps) {
    cout << gapName(gtype) << " gaps: [";
    for (size_t i = 0; i < gaps.size(); ++i) {
      if (i)
        cout << ", ";
      cout << gaps[i];
    }
    cout << "]\n";
  }

  for (int gap : gaps) {
    // gapped insertion sort
    for (size_t i = gap; i < n; ++i) {
      int tmp = a[i];
      size_t j = i;
      while (j >= (size_t)gap) {
        comparisons++; // count the comparison
        if (a[j - gap] > tmp) {
          a[j] = a[j - gap];
          // count the swaps:
          swaps++;
          j -= gap;
        } else {
          break;
        }
      }
      a[j] = tmp;
    }
  }

  return {comparisons, swaps};
}

// checker
bool is_sorted_asc(const vector<int> &a) {
  for (size_t i = 1; i < a.size(); ++i)
    if (a[i - 1] > a[i])
      return false;
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
  for (size_t i = 0; i < n; ++i)
    v.push_back(dist(rng));
  return v;
}

void writeVectorToTxt(const string &fname, const vector<int> &v) {
  ofstream os(fname);
  if (!os)
    throw runtime_error("Failed to open " + fname);
  os << v.size() << '\n';
  for (size_t i = 0; i < v.size(); ++i) {
    os << v[i] << (i + 1 == v.size() ? '\n' : ' ');
  }
}

struct SortResult {
  double time_ms;
  long long comparisons;
  long long swaps;
};

SortResult timeShellOnVector(const vector<int> &vec, GapType gtype) {
  vector<int> copy = vec;
  auto start = chrono::steady_clock::now();
  auto [comps, swaps] = shell_sort_inplace(copy, gtype, false);
  auto stop = chrono::steady_clock::now();

  if (!is_sorted_asc(copy)) {
    cerr << "ERROR: shell_sort produced unsorted result for " << gapName(gtype)
         << "\n";
    terminate();
  }

  SortResult result;
  result.time_ms = chrono::duration<double, milli>(stop - start).count();
  result.comparisons = comps;
  result.swaps = swaps;
  return result;
}

SortResult timeInsertionOnVector(const vector<int> &vec) {
  vector<int> copy = vec;
  auto start = chrono::steady_clock::now();
  auto [comps, swaps] = insertion_sort_inplace(copy);
  auto stop = chrono::steady_clock::now();

  if (!is_sorted_asc(copy)) {
    cerr << "ERROR: insertion_sort produced unsorted result\n";
    terminate();
  }

  SortResult result;
  result.time_ms = chrono::duration<double, milli>(stop - start).count();
  result.comparisons = comps;
  result.swaps = swaps;
  return result;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  //!!! Maximum size for insertion sort (skip if array is larger)
  const size_t INSERTION_SORT_MAX_SIZE = 100000u;

  const vector<size_t> sizes = {10000u, 100000u, 1000000u};
  const vector<pair<int, int>> ranges = {
      {-10, 10}, {-1000, 1000}, {-100000, 100000}};
  const vector<GapType> algorithms = {GapType::Shell, GapType::Hibbard,
                                      GapType::Knuth, GapType::Sedgewick};

  // CSV header
  ofstream csv("timings_shell.csv", ios::out);
  csv << "size,lo,hi,algorithm,run1_ms,run2_ms,run3_ms,mean_ms,run1_comps,run2_"
         "comps,run3_comps,mean_comps,run1_swaps,run2_swaps,run3_swaps,mean_"
         "swaps,sorted\n";
  csv.close();

  for (size_t n : sizes) {
    for (auto [lo, hi] : ranges) {
      unsigned int seed = datasetSeed(n, lo, hi);
      string fname = "data_" + to_string(n) + "_" + to_string(lo) + "_" +
                     to_string(hi) + ".txt";
      cout << "\n========================================\n";
      cout << "Dataset: n=" << n << " range=[" << lo << ',' << hi
           << "] seed=" << seed << "\n";
      cout << "Generating data file: " << fname << "...\n";
      cout << "========================================\n";

      vector<int> data = genRandomVector(n, lo, hi, seed);
      writeVectorToTxt(fname, data);

      // Check if we should run insertion sort
      bool run_insertion = (n <= INSERTION_SORT_MAX_SIZE);
      double ins_mean_time = 0.0;
      double ins_mean_comps = 0.0;
      double ins_mean_swaps = 0.0;

      if (run_insertion) {
        // Run Insertion Sort
        cout << "\n  Algorithm: Insertion Sort\n";
        vector<SortResult> insertion_runs;
        insertion_runs.reserve(3);
        for (int r = 0; r < 3; ++r) {
          cout << "    Run " << (r + 1) << ": sorting..." << flush;
          SortResult res = timeInsertionOnVector(data);
          insertion_runs.push_back(res);
          cout << "\r    Run " << (r + 1) << ": " << fixed << setprecision(2)
               << res.time_ms << " ms (" << res.comparisons << " comps, "
               << res.swaps << " swaps)\n";
        }
        ins_mean_time = (insertion_runs[0].time_ms + insertion_runs[1].time_ms +
                         insertion_runs[2].time_ms) /
                        3.0;
        ins_mean_comps =
            (insertion_runs[0].comparisons + insertion_runs[1].comparisons +
             insertion_runs[2].comparisons) /
            3.0;
        ins_mean_swaps = (insertion_runs[0].swaps + insertion_runs[1].swaps +
                          insertion_runs[2].swaps) /
                         3.0;
        cout << "    >> Mean: " << fixed << setprecision(2) << ins_mean_time
             << " ms (" << fixed << setprecision(0) << ins_mean_comps
             << " comps, " << fixed << setprecision(0) << ins_mean_swaps
             << " swaps)\n";

        ofstream csv_append("timings_shell.csv", ios::app);
        csv_append << n << ',' << lo << ',' << hi << ",InsertionSort," << fixed
                   << setprecision(3) << insertion_runs[0].time_ms << ','
                   << insertion_runs[1].time_ms << ','
                   << insertion_runs[2].time_ms << ',' << ins_mean_time << ','
                   << insertion_runs[0].comparisons << ','
                   << insertion_runs[1].comparisons << ','
                   << insertion_runs[2].comparisons << ','
                   << (long long)ins_mean_comps << ','
                   << insertion_runs[0].swaps << ',' << insertion_runs[1].swaps
                   << ',' << insertion_runs[2].swaps << ','
                   << (long long)ins_mean_swaps << ",1\n";
        csv_append.close();
      } else {
        cout << "\n  Algorithm: Insertion Sort\n";
        cout << "    >> SKIPPED (array size " << n << " > "
             << INSERTION_SORT_MAX_SIZE << " limit)\n";
      }

      // Run Shell Sort variants
      for (GapType g : algorithms) {
        cout << "\n  Algorithm: " << gapName(g) << "\n";
        vector<SortResult> runs;
        runs.reserve(3);
        for (int r = 0; r < 3; ++r) {
          cout << "    Run " << (r + 1) << ": sorting..." << flush;
          SortResult res = timeShellOnVector(data, g);
          runs.push_back(res);
          cout << "\r    Run " << (r + 1) << ": " << fixed << setprecision(2)
               << res.time_ms << " ms (" << res.comparisons << " comps, "
               << res.swaps << " swaps)\n";
        }
        double mean_time =
            (runs[0].time_ms + runs[1].time_ms + runs[2].time_ms) / 3.0;
        double mean_comps =
            (runs[0].comparisons + runs[1].comparisons + runs[2].comparisons) /
            3.0;
        double means_swaps =
            (runs[0].swaps + runs[1].swaps + runs[2].swaps) / 3.0;
        cout << "    >> Mean: " << fixed << setprecision(2) << mean_time
             << " ms (" << fixed << setprecision(0) << mean_comps << " comps, "
             << fixed << setprecision(0) << means_swaps << " swaps) | ";

        // Calculate speedup vs insertion sort
        double speedup = ins_mean_time / mean_time;
        double comp_ratio = ins_mean_comps / mean_comps;
        double swap_ratio = ins_mean_swaps / means_swaps;
        cout << "Speedup: " << fixed << setprecision(2) << speedup << "x time, "
             << comp_ratio << "x comps, " << swap_ratio << "x less swaps\n";

        ofstream csv_append2("timings_shell.csv", ios::app);
        csv_append2 << n << ',' << lo << ',' << hi << ',' << gapName(g) << ','
                    << fixed << setprecision(3) << runs[0].time_ms << ','
                    << runs[1].time_ms << ',' << runs[2].time_ms << ','
                    << mean_time << ',' << runs[0].comparisons << ','
                    << runs[1].comparisons << ',' << runs[2].comparisons << ','
                    << (long long)mean_comps << ',' << runs[0].swaps << ','
                    << runs[1].swaps << ',' << runs[2].swaps << ','
                    << means_swaps << ",1\n";
        csv_append2.close();
      }
    }
  }

  cout << "\n========================================\n";
  cout << "All experiments complete!\n";
  cout << "Results saved to: timings_shell.csv\n";
  cout << "Data files: data_*.txt\n";
  cout << "========================================\n";
  return 0;
}