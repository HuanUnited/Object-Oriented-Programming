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

// --- helper swap
template<typename T> void swap_pos(T& t1, T& t2) {
    T temp = std::move(t1); // or T temp(std::move(t1));
    t1 = std::move(t2);
    t2 = std::move(temp);
}

int partition(vector<int> &arr, int first, int last) {
  int pivot = arr[last];

  int i = first;
  for (int j = first; j < last; j++) {
    if (arr[j] <= pivot) {
      swap_pos(arr[i], arr[j]);
      i++;
    }
  }
  swap_pos(arr[i], arr[last]);
  return (i);
}

// Iterative QuickSort
void quickSort(vector<int> &a, int first, int last) {
  int stack[last - first + 1];
  int top = -1;
  stack[++top] = first;
  stack[++top] = last;
  while (top >= 0) {
    last = stack[top--];
    first = stack[top--];
    int pivot_pos = partition(a, first, last);

    // If there are elements on left side of pivot, then
    // push left side to stack
    if (pivot_pos - 1 > first) {
      stack[++top] = first;
      stack[++top] = pivot_pos - 1;
    }

    // If there are elements on right side of pivot,
    // then push right side to stack
    if (pivot_pos + 1 < last) {
      stack[++top] = pivot_pos + 1;
      stack[++top] = last;
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

double timeQuickOnVector(const vector<int> &vec, bool verbose = false) {
  vector<int> copy = vec;  // copy once per trial
  auto start = chrono::steady_clock::now();
  quickSort(copy,0 , copy.size() - 1);
  auto stop = chrono::steady_clock::now();
  if (!is_sorted_asc(copy)) {
    cerr << "ERROR: quicksort produced unsorted result" << "\n";
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
      vector<double> runs;
      runs.reserve(3);
      for (int r = 0; r < 3; ++r) {
        double tms = timeQuickOnVector(data);
        runs.push_back(tms);
        cout << fixed << setprecision(2) << tms << " ms" << (r < 2 ? ", " : "");
      }
      double mean = (runs[0] + runs[1] + runs[2]) / 3.0;
      cout << "  mean=" << fixed << setprecision(2) << mean << " ms\n";

      ofstream csv_append("timings_quick.csv", ios::app);
      csv_append << n << ',' << lo << ',' << hi << ',' << ',' << fixed
                 << setprecision(3) << runs[0] << ',' << runs[1] << ','
                 << runs[2] << ',' << mean << ",1\n";
      csv_append.close();
    }
  }

  cout << "All experiments complete. See timings_quick.csv and data_*.txt "
          "files.\n";
  return 0;
}