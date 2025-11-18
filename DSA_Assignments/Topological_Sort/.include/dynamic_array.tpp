#ifndef DYNAMIC_ARRAY_TPP
#define DYNAMIC_ARRAY_TPP

#ifndef DYNAMIC_ARRAY_HPP
#include "dynamic_array.hpp"
#endif

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <utility>

template <typename T>
DynamicArray<T>::DynamicArray() : m_arr(new T[1]), m_capacity(1), m_size(0) {}

template <typename T>
DynamicArray<T>::DynamicArray(const T *arr, size_type count)
    : m_arr(nullptr), m_capacity(0), m_size(0) {
  if (count == 0) {
    m_arr = new T[1];
    m_capacity = 1;
    m_size = 0;
    return;
  }
  m_arr = new T[count];
  m_capacity = count;
  m_size = count;
  for (size_type i = 0; i < count; ++i)
    m_arr[i] = arr[i];
}

template <typename T>
DynamicArray<T>::DynamicArray(size_type rows)
    : m_arr(nullptr), m_capacity(rows), m_size(rows) {
  if (rows == 0) {
    m_arr = new T[1];
    m_capacity = 1;
    m_size = 0;
    return;
  }
  m_arr = new T[rows];
  m_capacity = rows;
  m_size = rows;
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray &other)
    : m_arr(new T[other.m_capacity]), m_capacity(other.m_capacity),
      m_size(other.m_size) {
  for (size_type i = 0; i < m_size; ++i)
    m_arr[i] = other.m_arr[i];
}

template <typename T>
DynamicArray<T>::DynamicArray(DynamicArray &&other) noexcept
    : m_arr(other.m_arr), m_capacity(other.m_capacity), m_size(other.m_size) {
  other.m_arr = new T[1];
  other.m_capacity = 1;
  other.m_size = 0;
}

template <typename T> DynamicArray<T>::~DynamicArray() { delete[] m_arr; }

// --- capacity/info ---
template <typename T>
typename DynamicArray<T>::size_type DynamicArray<T>::size() const noexcept {
  return m_size;
}
template <typename T>
typename DynamicArray<T>::size_type DynamicArray<T>::capacity() const noexcept {
  return m_capacity;
}
template <typename T> bool DynamicArray<T>::empty() const noexcept {
  return m_size == 0;
}

// --- iterators ---
template <typename T>
typename DynamicArray<T>::iterator DynamicArray<T>::begin() noexcept {
  return m_arr;
}
template <typename T>
typename DynamicArray<T>::iterator DynamicArray<T>::end() noexcept {
  return m_arr + m_size;
}
template <typename T>
typename DynamicArray<T>::const_iterator
DynamicArray<T>::begin() const noexcept {
  return m_arr;
}
template <typename T>
typename DynamicArray<T>::const_iterator DynamicArray<T>::end() const noexcept {
  return m_arr + m_size;
}
template <typename T>
typename DynamicArray<T>::const_iterator
DynamicArray<T>::cbegin() const noexcept {
  return m_arr;
}
template <typename T>
typename DynamicArray<T>::const_iterator
DynamicArray<T>::cend() const noexcept {
  return m_arr + m_size;
}

// --- element access ---
template <typename T> T &DynamicArray<T>::operator[](size_type index) {
  return m_arr[index];
}
template <typename T>
const T &DynamicArray<T>::operator[](size_type index) const {
  return m_arr[index];
}
template <typename T> T &DynamicArray<T>::at(size_type index) {
  if (index >= m_size)
    throw std::out_of_range("DynamicArray::at: index out of range");
  return m_arr[index];
}
template <typename T> const T &DynamicArray<T>::at(size_type index) const {
  if (index >= m_size)
    throw std::out_of_range("DynamicArray::at: index out of range");
  return m_arr[index];
}

// --- modifiers / memory management ---
template <typename T> void DynamicArray<T>::reserve(size_type newCapacity) {
  if (newCapacity <= m_capacity)
    return;
  T *tmp = new T[newCapacity];
  for (size_type i = 0; i < m_size; ++i)
    tmp[i] = std::move(m_arr[i]);
  delete[] m_arr;
  m_arr = tmp;
  m_capacity = newCapacity;
}

template <typename T> void DynamicArray<T>::shrink_to_fit() {
  if (m_capacity == m_size)
    return;
  size_type newCap = m_size == 0 ? 1 : m_size;
  T *tmp = new T[newCap];
  for (size_type i = 0; i < m_size; ++i)
    tmp[i] = std::move(m_arr[i]);
  delete[] m_arr;
  m_arr = tmp;
  m_capacity = newCap;
}

template <typename T> void DynamicArray<T>::clear() noexcept { m_size = 0; }

template <typename T> void DynamicArray<T>::push_back(const T &value) {
  if (m_size == m_capacity)
    reserve(m_capacity * 2);
  m_arr[m_size++] = value;
}

template <typename T> void DynamicArray<T>::push_back(T &&value) {
  if (m_size == m_capacity)
    reserve(m_capacity * 2);
  m_arr[m_size++] = std::move(value);
}

template <typename T>
bool DynamicArray<T>::insert_at(size_type index, const T &value) {
  if (index > m_size)
    return false;
  if (m_size == m_capacity)
    reserve(m_capacity * 2);
  for (size_type i = m_size; i > index; --i)
    m_arr[i] = std::move(m_arr[i - 1]);
  m_arr[index] = value;
  ++m_size;
  return true;
}

template <typename T>
typename DynamicArray<T>::iterator DynamicArray<T>::insert(iterator pos,
                                                           const T &value) {
  size_type idx = pos - m_arr;
  if (idx > m_size)
    idx = m_size;
  insert_at(idx, value);
  return m_arr + idx;
}

template <typename T> bool DynamicArray<T>::erase_at(size_type index) {
  if (index >= m_size)
    return false;
  for (size_type i = index; i + 1 < m_size; ++i)
    m_arr[i] = std::move(m_arr[i + 1]);
  --m_size;
  return true;
}

template <typename T>
typename DynamicArray<T>::iterator DynamicArray<T>::erase(iterator pos) {
  size_type idx = pos - m_arr;
  if (idx >= m_size)
    throw std::out_of_range("erase: iterator out of range");
  erase_at(idx);
  return m_arr + idx;
}

template <typename T>
typename DynamicArray<T>::iterator DynamicArray<T>::erase(iterator first,
                                                          iterator last) {
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

template <typename T> bool DynamicArray<T>::pop_first(const T &value) {
  int idx = find(value);
  if (idx == -1)
    return false;
  erase_at(static_cast<size_type>(idx));
  return true;
}

template <typename T> void DynamicArray<T>::pop_all(const T &value) {
  size_type write = 0;
  for (size_type read = 0; read < m_size; ++read) {
    if (!(m_arr[read] == value)) {
      m_arr[write++] = std::move(m_arr[read]);
    }
  }
  m_size = write;
}

template <typename T> int DynamicArray<T>::find(const T &value) const noexcept {
  for (size_type i = 0; i < m_size; ++i)
    if (m_arr[i] == value)
      return static_cast<int>(i);
  return -1;
}

template <typename T> void DynamicArray<T>::swap(DynamicArray &other) noexcept {
  using std::swap;
  swap(m_arr, other.m_arr);
  swap(m_size, other.m_size);
  swap(m_capacity, other.m_capacity);
}

template <typename T> void DynamicArray<T>::sort() {
  if (m_size <= 1)
    return;
  std::sort(m_arr, m_arr + m_size);
}

template <typename T> void DynamicArray<T>::qsort() {
  if (m_size <= 1)
    return;
  // NOTE: original code used m_size as 'high', keep original behavior:
  quickSort(m_arr, 0, m_size);
}

template <typename T> T DynamicArray<T>::min() const {
  if (m_size == 0)
    throw std::runtime_error("min(): empty array");
  T best = m_arr[0];
  for (size_type i = 1; i < m_size; ++i)
    if (m_arr[i] < best)
      best = m_arr[i];
  return best;
}

template <typename T> T DynamicArray<T>::max() const {
  if (m_size == 0)
    throw std::runtime_error("max(): empty array");
  T best = m_arr[0];
  for (size_type i = 1; i < m_size; ++i)
    if (m_arr[i] > best)
      best = m_arr[i];
  return best;
}

template <typename T>
DynamicArray<T> &DynamicArray<T>::operator=(const DynamicArray &other) {
  if (this == &other)
    return *this;
  T *newBuf = new T[other.m_capacity];
  for (size_type i = 0; i < other.m_size; ++i)
    newBuf[i] = other.m_arr[i];
  delete[] m_arr;
  m_arr = newBuf;
  m_capacity = other.m_capacity;
  m_size = other.m_size;
  return *this;
}

template <typename T>
DynamicArray<T> &DynamicArray<T>::operator=(DynamicArray &&other) noexcept {
  if (this != &other) {
    delete[] m_arr;
    m_arr = other.m_arr;
    m_capacity = other.m_capacity;
    m_size = other.m_size;
    other.m_arr = nullptr;
    other.m_capacity = 0;
    other.m_size = 0;
  }
  return *this;
}

template <typename T>
DynamicArray<T> &DynamicArray<T>::operator+=(const T &value) {
  push_back(value);
  return *this;
}

template <typename T>
DynamicArray<T> DynamicArray<T>::operator+(const T &value) const {
  DynamicArray tmp(*this);
  tmp.push_back(value);
  return tmp;
}

template <typename T>
DynamicArray<T> &DynamicArray<T>::operator+=(const DynamicArray &other) {
  if (other.m_size == 0)
    return *this;
  if (m_size + other.m_size > m_capacity)
    reserve(std::max(m_capacity * 2, m_size + other.m_size));
  for (size_type i = 0; i < other.m_size; ++i)
    m_arr[m_size + i] = other.m_arr[i];
  m_size += other.m_size;
  return *this;
}

template <typename T>
DynamicArray<T> DynamicArray<T>::operator+(const DynamicArray &other) const {
  DynamicArray tmp(*this);
  tmp += other;
  return tmp;
}

template <typename T>
bool DynamicArray<T>::operator==(const DynamicArray &other) const {
  if (m_size != other.m_size)
    return false;
  for (size_type i = 0; i < m_size; ++i)
    if (!(m_arr[i] == other.m_arr[i]))
      return false;
  return true;
}

template <typename T>
bool DynamicArray<T>::operator!=(const DynamicArray &other) const {
  return !(*this == other);
}

// I/O helpers
template <typename T> void DynamicArray<T>::setconsole() {
  std::cout << "Input number of elements: ";
  size_type capacity;
  std::cin >> capacity;
  T temp{};
  std::cout << '\n' << "Input values: ";
  for (size_type i = 0; i < capacity; i++) {
    std::cin >> temp;
    push_back(temp);
  }
}

template <typename T> void DynamicArray<T>::getconsole() {
  std::cout << "Elements:";
  for (size_type i = 0; i < m_size; i++) {
    std::cout << " " << m_arr[i];
  }
  std::cout << '\n'
            << "Size " << m_size << '\n'
            << "Capacity: " << m_capacity << '\n';
}

// Quick Sort implementation.
template <typename T>
int DynamicArray<T>::partition(T arr[], size_type low, size_type high) {
  using std::swap;
  T p = arr[low];
  size_type i = low;
  size_type j = high;

  while (i < j) {
    while (arr[i] <= p && i <= high - 1) {
      i++;
    }
    while (j >= low + 1 && arr[j] > p) {
      j--;
    }
    if (i < j) {
      swap(arr[i], arr[j]);
    }
  }
  swap(arr[low], arr[j]);
  return static_cast<int>(j);
}

template <typename T>
void DynamicArray<T>::quickSort(T arr[], size_type low, size_type high) {
  if (low < high) {
    int pi = partition(arr, low, high);
    quickSort(arr, low, static_cast<size_type>(pi) - 1);
    quickSort(arr, static_cast<size_type>(pi) + 1, high);
  }
}

// --- streaming operators ---
template <typename T>
std::ostream &operator<<(std::ostream &os, const DynamicArray<T> &a) {
  os << "[";
  for (size_t i = 0; i < a.size(); ++i) {
    if (i)
      os << ", ";
    os << a.at(i);
  }
  os << "]";
  return os;
}

template <typename T>
std::istream &operator>>(std::istream &is, DynamicArray<T> &a) {
  size_t n;
  if (!(is >> n))
    return is;
  a.clear();
  if (a.capacity() < n)
    a.reserve(n);
  for (size_t i = 0; i < n; ++i) {
    T tmp;
    is >> tmp;
    a.push_back(tmp);
  }
  return is;
}

#endif // DYNAMIC_ARRAY_HPP