/*

Dynamic Arrays int

*/
#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <initializer_list>
using std::cin, std::cout, std::endl;

class DynamicArray {
public:
  using iterator = int *;
  using const_iterator = const int *;
  using size_type = std::size_t; // Better than unsigned int

  // --- constructors / destructor ---

  // Default Constructor
  DynamicArray() : m_arr(new int[1]), m_capacity(1), m_size(0) {}
  // Array Constructor
  DynamicArray(const int *arr, size_type count)
      : m_arr(nullptr), m_capacity(0), m_size(0) {
    if (count == 0) {
      m_arr = new int[1];
      m_capacity = 1;
      m_size = 0;
      return;
    }
    m_arr = new int[count];
    m_capacity = count;
    m_size = count;
    for (size_type i = 0; i < count; ++i)
      m_arr[i] = arr[i];
  }
  // Copy Constructor
  DynamicArray(const DynamicArray &other)
      : m_arr(new int[other.m_capacity]), m_capacity(other.m_capacity),
        m_size(other.m_size) {
    for (size_type i = 0; i < m_size; ++i)
      m_arr[i] = other.m_arr[i];
  }
  // Move Constructor
  DynamicArray(DynamicArray &&other) noexcept
      : m_arr(other.m_arr), m_capacity(other.m_capacity), m_size(other.m_size) {
    other.m_arr = new int[1];
    other.m_capacity = 1;
    other.m_size = 0;
  }

  // --- Initializer list constructor ---
  DynamicArray(std::initializer_list<int> ilist)
      : m_arr(nullptr), m_capacity(ilist.size()), m_size(ilist.size()) {
    if (m_capacity == 0) {
      m_arr = new int[1];
      m_capacity = 1;
      m_size = 0;
    } else {
      m_arr = new int[m_capacity];
      size_type i = 0;
      for (auto val : ilist)
        m_arr[i++] = val;
    }
  }

  ~DynamicArray() { delete[] m_arr; }

  // --- capacity/info ---
  size_type size() const noexcept { return m_size; }
  size_type capacity() const noexcept { return m_capacity; }
  bool empty() const noexcept { return m_size == 0; }

  // --- iterators ---
  iterator begin() noexcept { return m_arr; }
  iterator end() noexcept { return m_arr + m_size; }
  const_iterator begin() const noexcept { return m_arr; }
  const_iterator end() const noexcept { return m_arr + m_size; }
  const_iterator cbegin() const noexcept { return m_arr; }
  const_iterator cend() const noexcept { return m_arr + m_size; }

  // --- element access ---
  // operator[] no bounds check (like std::vector)
  int &operator[](size_type index) { return m_arr[index]; }
  const int &operator[](size_type index) const { return m_arr[index]; }
  // at with bounds check
  int &at(size_type index) {
    if (index >= m_size)
      throw std::out_of_range("DynamicArray::at: index out of range");
    return m_arr[index];
  }
  const int &at(size_type index) const {
    if (index >= m_size)
      throw std::out_of_range("DynamicArray::at: index out of range");
    return m_arr[index];
  }

  // --- modifiers / memory management ---
  void reserve(size_type newCapacity) {
    if (newCapacity <= m_capacity)
      return;
    int *tmp = new int[newCapacity];
    for (size_type i = 0; i < m_size; ++i)
      tmp[i] = std::move(m_arr[i]); // std::move takes the value from the box,
                                    // puts it into a new box.
    delete[] m_arr;                 // burns the box
    m_arr = tmp;                    // now new box is box
    m_capacity = newCapacity;
  }

  void shrink_to_fit() {
    if (m_capacity == m_size)
      return;
    size_type newCap =
        m_size == 0 ? 1 : m_size; // case check for the capacity, if it is equal
                                  // to zero then the cap is 1, else = size
    int *tmp = new int[newCap];
    for (size_type i = 0; i < m_size; ++i)
      tmp[i] = std::move(m_arr[i]);
    delete[] m_arr;
    m_arr = tmp;
    m_capacity = newCap;
  }

  void clear() noexcept { m_size = 0; }

  void push_back(const int &value) {
    if (m_size == m_capacity)
      reserve(m_capacity * 2);
    m_arr[m_size++] = value;
  }
  void push_back(int &&value) {
    if (m_size == m_capacity)
      reserve(m_capacity * 2);
    m_arr[m_size++] = std::move(value);
  }

  // insert by index: returns true on success, false if index invalid
  bool insert_at(size_type index, const int &value) {
    if (index > m_size)
      return false; // allow insert at end (index == size)
    if (m_size == m_capacity)
      reserve(m_capacity * 2);
    // shift right
    for (size_type i = m_size; i > index; --i)
      m_arr[i] = std::move(m_arr[i - 1]);
    m_arr[index] = value;
    ++m_size;
    return true;
  }

  // insert before iterator
  iterator insert(iterator pos, const int &value) {
    size_type idx = pos - m_arr;
    if (idx > m_size)
      idx = m_size; // clamp - stops from inserting past boundaries
    insert_at(idx, value);
    return m_arr + idx;
  }

  // erase by index; returns true if erased
  bool erase_at(size_type index) {
    if (index >= m_size)
      return false;
    for (size_type i = index; i + 1 < m_size; ++i)
      m_arr[i] = std::move(m_arr[i + 1]);
    --m_size;
    return true;
  }

  // erase single element at iterator; returns iterator to next element
  iterator erase(iterator pos) {
    size_type idx = pos - m_arr;
    if (idx >= m_size)
      throw std::out_of_range("erase: iterator out of range");
    erase_at(idx);
    return m_arr + idx;
  }

  // erase range [first, last) and return iterator to next element
  iterator erase(iterator first, iterator last) {
    if (first < begin() || last < begin() || first > end() || last > end() ||
        first > last) {
      throw std::out_of_range("erase: invalid range");
    }
    size_type start = first - m_arr;
    size_type finish = last - m_arr;
    size_type count = finish - start;
    for (size_type i = start; i + count < m_size; ++i)
      m_arr[i] = std::move(m_arr[i + count]);
    m_size -= count;
    return m_arr + start;
  }

  // pop first occurrence by value — returns true if something was removed
  bool pop_first(const int &value) {
    int idx = find(value);
    if (idx == -1)
      return false;
    erase_at(
        static_cast<size_type>(idx)); // static_cast is a type casting operator
                                      // / 1 type -> another: int to size_t
    return true;
  }

  // remove all occurrences
  void pop_all(const int &value) {
    size_type write = 0;
    for (size_type read = 0; read < m_size; ++read) {
      if (!(m_arr[read] == value)) {
        m_arr[write++] = std::move(m_arr[read]);
      }
    }
    m_size = write;
  }

  // find — returns index of first occurrence or -1
  int find(const int &value) const noexcept {
    for (size_type i = 0; i < m_size; ++i)
      if (m_arr[i] == value)
        return static_cast<int>(i);
    return -1;
  }

  // swap contents (member version)

  void swap(DynamicArray &other) noexcept {
    int *temp_arr = m_arr;
    size_type temp_size = m_size;
    size_type temp_cap = m_capacity;

    m_arr = other.m_arr;
    other.m_arr = temp_arr;
    m_size = other.m_size;
    other.m_size = temp_size;
    m_capacity = other.m_capacity;
    other.m_capacity = temp_cap;
  }

  // sorting
  void sort() {
    if (m_size <= 1)
      return;
    std::sort(m_arr, m_arr + m_size);
  }

  void qsort() {
    if (m_size <= 1)
      return;
    quickSort(m_arr, 0, m_size);
  }

  // min / max — throw if empty
  int min() const {
    if (m_size == 0)
      throw std::runtime_error("min(): empty array");
    int best = m_arr[0];
    for (size_type i = 1; i < m_size; ++i)
      if (m_arr[i] < best)
        best = m_arr[i];
    return best;
  }
  int max() const {
    if (m_size == 0)
      throw std::runtime_error("max(): empty array");
    int best = m_arr[0];
    for (size_type i = 1; i < m_size; ++i)
      if (m_arr[i] > best)
        best = m_arr[i];
    return best;
  }

  // --- assignments & concatenation ---
  DynamicArray &operator=(const DynamicArray &other) {
    if (this == &other)
      return *this;
    int *newBuf = new int[other.m_capacity];
    for (size_type i = 0; i < other.m_size; ++i)
      newBuf[i] = other.m_arr[i];
    delete[] m_arr;
    m_arr = newBuf;
    m_capacity = other.m_capacity;
    m_size = other.m_size;
    return *this;
  }

  DynamicArray &operator=(DynamicArray &&other) noexcept {
    if (this != &other) {
      delete[] m_arr;
      m_arr = other.m_arr;
      m_capacity = other.m_capacity;
      m_size = other.m_size;
      other.m_arr = new int[1];
      other.m_capacity = 1;
      other.m_size = 0;
    }
    return *this;
  }

  DynamicArray &operator+=(const int &value) { // Push one element
    push_back(value);
    return *this;
  }

  DynamicArray operator+(const int &value) const { // Push to new array
    DynamicArray tmp(*this);
    tmp.push_back(value);
    return tmp;
  }

  DynamicArray &operator+=(const DynamicArray &other) { // Concatenation
    if (other.m_size == 0)
      return *this;
    if (m_size + other.m_size > m_capacity)
      reserve(std::max(m_capacity * 2, m_size + other.m_size));
    for (size_type i = 0; i < other.m_size; ++i)
      m_arr[m_size + i] = other.m_arr[i];
    m_size += other.m_size;
    return *this;
  }

  DynamicArray
  operator+(const DynamicArray &other) const { // Concatenation to New Array
    DynamicArray tmp(*this);
    tmp += other;
    return tmp;
  }

  bool operator==(const DynamicArray &other) const { // Comparison
    if (m_size != other.m_size)
      return false;
    for (size_type i = 0; i < m_size; ++i)
      if (!(m_arr[i] == other.m_arr[i]))
        return false;
    return true;
  }
  bool operator!=(const DynamicArray &other) const { return !(*this == other); }

  // I/O methods:

  void setconsole() {
    cout << "Input number of elements: ";
    size_type capacity;
    cin >> capacity;
    int temp{};
    cout << '\n' << "Input values: ";
    for (size_type i = 0; i < capacity; i++) {
      cin >> temp;
      push_back(temp);
    }
  };

  void getconsole() {
    cout << "[";
    for (size_type i = 0; i < m_size; i++) {
      if (i)
        cout << ", ";
      cout << m_arr[i];
    }
    cout << "]" << '\n';
  };

  // Quick Sort implementation.
  int partition(int arr[], size_type low, size_type high) {
    using std::swap;
    // Initialize pivot to be the first element
    int p = arr[low];
    size_type i = low;
    size_type j = high;

    while (i < j) {

      // Find the first element greater than
      // the pivot (from starting)
      while (arr[i] <= p && i <= high - 1) {
        i++;
      }

      // Find the first element smaller than
      // the pivot (from last)
      while (arr[j] > p && j >= low + 1) {
        j--;
      }
      if (i < j) {
        std::swap(arr[i], arr[j]);
      }
    }
    std::swap(arr[low], arr[j]);
    return j;
  }

  void quickSort(int arr[], size_type low, size_type high) {
    if (low < high) {
      // call partition function to find Partition Index
      int pi = partition(arr, low, high);

      // Recursively call quickSort() for left and right
      // half based on Partition Index
      quickSort(arr, low, pi - 1);
      quickSort(arr, pi + 1, high);
    }
  }

private:
  int *m_arr;
  size_type m_capacity; // std::size_t
  size_type m_size;
};

// streaming: output and input (simple) // for example cout << a;
inline std::ostream &operator<<(std::ostream &os, const DynamicArray &a) {
  os << "[";
  for (size_t i = 0; i < a.size(); ++i) {
    if (i)
      os << ", ";
    os << a.at(i);
  }
  os << "]";
  return os;
}

// simple input: read n then n elements
std::istream &operator>>(std::istream &is, DynamicArray &a) {
  size_t n;
  if (!(is >> n))
    return is;
  a.clear();
  if (a.capacity() < n)
    a.reserve(n);
  for (size_t i = 0; i < n; ++i) {
    int tmp;
    is >> tmp;
    a.push_back(tmp);
  }
  return is;
}