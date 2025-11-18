// Goals
/*
1. Implement a random array generator. <DONE>
2. Implement a counting sort function
2.1 A function that reads how many numbers the max digit has.
2.1.1 A function that finds the max digit.
2.2 A function that tells us which digit we are currenting sorting (in counting
sort)
3. Radix Sort is Counting Sort advanced.
*/

#include <cmath>
#include <cstdlib>
#include <iostream>
using std::cout, std::cin, std::endl;

class Array {
private:
  int *m_array = nullptr;
  int m_length{0};

public:
  // Array Manip Methods
  void swap(int pos1, int pos2) {
    int temp = m_array[pos2];
    m_array[pos2] = m_array[pos1];
    m_array[pos1] = temp;
  }

  void fillArray() {
    for (int i = 0; i < m_length; i++) {
      m_array[i] = rand() % 999 + 1; // in [1,999];
    }
  }

  void printArray() {
    cout << "Array:";
    for (int i = 0; i < m_length; i++)
      cout << " " << m_array[i];
    cout << endl;
  }

  int at(int x) const { return m_array[x]; }
  void setAt(int index, int value) { m_array[index] = value; }

  // Sorting Methods:
  int findMax() {
    int max = m_array[0];
    for (int i = 1; i < m_length; i++) {
      if (max < m_array[i]) {
        max = m_array[i];
      }
    }
    return max;
  }

  int findMaxLen() {
    int max_val = findMax();
    if (max_val == 0) {
      return 1;
    }
    return (int)log10(max_val) + 1;
  }

  void countSort(int exp) {
    int *output = new int[m_length];
    int count[10] = {0};

    // Store count of occurrences in count[]
    for (int i = 0; i < m_length; i++) {
      count[(m_array[i] / exp) % 10]++;
    }

    // Change count[i] so that count[i] now
    // contains actual position of this digit in output[]
    for (int i = 1; i < 10; i++) {
      count[i] += count[i - 1];
    }

    // Build the output array. We iterate from the back to ensure stability.
    for (int i = m_length - 1; i >= 0; i--) {
      int digit = (m_array[i] / exp) % 10;
      output[count[digit] - 1] = m_array[i];
      count[digit]--;
    }

    // Copy the sorted output array back to the main array.
    for (int i = 0; i < m_length; i++) {
      m_array[i] = output[i];
    }

    delete[] output;
  }

  void radixSort() {
    // Find the maximum number of digits
    int max_len = findMaxLen();
    int exp = 1;
    // Perform counting sort for each digit from LSB to MSB
    for (int i = 0; i < max_len; i++) {
      countSort(exp);
      exp *= 10;
    }
    printArray();
  }

  // Too lazy to make a copy constructor.
  Array(int length) : m_length{length} {
    if (m_length <= 0) {
      std::cout << "Error: Array length must be positive." << std::endl;
      m_length = 0;
      m_array = nullptr;
      return;
    }
    m_array = new int[m_length];
    fillArray();
    printArray();
  }

  Array() : Array(rand() % 50 + 1) {}
  ~Array() { delete[] m_array; }
};

int main() {
  srand(static_cast<unsigned int>(time(NULL))); // Seed once here.
  Array a1(10);
  a1.radixSort();
}