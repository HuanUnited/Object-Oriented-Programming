#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

// --- helper swap
template <typename T>
void swap_pos(T &t1, T &t2) {
  T temp = std::move(t1);
  t1 = std::move(t2);
  t2 = std::move(temp);
}

// Partition with median-of-three pivot (low..high inclusive)
int partition_median_of_three(vector<int> &arr, int low, int high) {
  // median-of-three: choose median of arr[low], arr[mid], arr[high]
  int mid = low + (high - low) / 2;  // correct positioning function
  int a = arr[low], b = arr[mid], c = arr[high];
  int pivotIndex = low;

  // basically find the median value out of the three.
  if ((a <= b && b <= c) || (c <= b && b <= a))
    pivotIndex = mid;
  else if ((b <= c && c <= a) || (a <= c && c <= b))
    pivotIndex = high;
  else
    pivotIndex = low;

  // move chosen pivot to end (high)
  swap_pos(arr[pivotIndex], arr[high]);
  int pivot = arr[high];

  int i = low;
  for (int j = low; j < high; ++j) {
    if (arr[j] <= pivot) {
      swap_pos(arr[i], arr[j]);
      ++i;
    }
  }
  swap_pos(arr[i], arr[high]);
  return i;
}

// Iterative quicksort wrapper; sorts entire vector in-place
void quickSortIterative(vector<int> &a) {
  int n = static_cast<int>(a.size());
  if (n <= 1) return;

  // vector<int> stack;
  // stack.reserve(64);
  // stack.push_back(0);  // first low stack.push_back(n - 1); // first high

  // stack of intervals: push low, high pairs
  vector<pair<int, int>> stack;
  stack.emplace_back(0, n - 1);

  while (!stack.empty()) {
    // int high = stack.back();  // high is the last element in the stack
    // stack.pop_back();         // pop last element
    // int low = stack.back();   // low is the last element in the stack
    // stack.pop_back();         // pop last element

    auto [low, high] = stack.back();
    stack.pop_back();

    if (low >= high) continue;  // bound check

    int p = partition_median_of_three(a, low, high);  // find pivot point

    int left_size = p - low;    // number of elements on the left side
    int right_size = high - p;  // number on the right side

    // push larger partition first to keep stack shallow
    if (left_size > right_size) {
      if (left_size > 1) {  // (low, p - 1)
        stack.emplace_back(low, p - 1);
        // stack.push_back(low);
        // stack.push_back(p - 1);
      }
      if (right_size > 1) {  // (p + 1, high)
        stack.emplace_back(p + 1, high);
        // stack.push_back(p + 1);
        // stack.push_back(high);
      }
    } else {
      if (right_size > 1) {  // (p + 1, high)
        stack.emplace_back(p + 1, high);
        // stack.push_back(p + 1);
        // stack.push_back(high);
      }
      if (left_size > 1) {  // (low, p - 1)
        stack.emplace_back(low, p - 1);
        // stack.push_back(low);
        // stack.push_back(p - 1);
      }
    }
  }
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
  os << "[";
  for (size_t i = 0; i < v.size(); ++i) {
    os << v[i];
    if (i < v.size() - 1) {
      os << ", ";
    }
  }
  os << "]";
  return os;
}

int main() {
  std::vector<int> array = {1243, 4234, 243, 112, 223, 55, 12, 324, 54, 767, 43,
                            22,   33,   442, 15,  73,  4,  2,  64,  6,  2};

  cout << "Original: "<< array <<'\n';

  quickSortIterative(array);

  cout << "Sorted: "<< array <<'\n';

}