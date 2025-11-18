#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <utility>

// Declarations for DynamicArray<T>. Implementations are pulled in at the end.
template <typename T> class DynamicArray {
public:
  using value_type = T;
  using iterator = T *;
  using const_iterator = const T *;
  using size_type = std::size_t;

  DynamicArray();
  DynamicArray(const T *arr, size_type count);
  DynamicArray(size_type rows);
  DynamicArray(const DynamicArray &other);
  DynamicArray(DynamicArray &&other) noexcept;
  ~DynamicArray();

  size_type size() const noexcept;
  size_type capacity() const noexcept;
  bool empty() const noexcept;

  iterator begin() noexcept;
  iterator end() noexcept;
  const_iterator begin() const noexcept;
  const_iterator end() const noexcept;
  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;

  T &operator[](size_type index);
  const T &operator[](size_type index) const;
  T &at(size_type index);
  const T &at(size_type index) const;

  void reserve(size_type newCapacity);
  void shrink_to_fit();
  void clear() noexcept;
  void push_back(const T &value);
  void push_back(T &&value);
  bool insert_at(size_type index, const T &value);
  iterator insert(iterator pos, const T &value);
  bool erase_at(size_type index);
  iterator erase(iterator pos);
  iterator erase(iterator first, iterator last);
  bool pop_first(const T &value);
  void pop_all(const T &value);
  int find(const T &value) const noexcept;
  void swap(DynamicArray &other) noexcept;

  void sort();
  void qsort();

  T min() const;
  T max() const;

  DynamicArray &operator=(const DynamicArray &other);
  DynamicArray &operator=(DynamicArray &&other) noexcept;
  DynamicArray &operator+=(const T &value);
  DynamicArray operator+(const T &value) const;
  DynamicArray &operator+=(const DynamicArray &other);
  DynamicArray operator+(const DynamicArray &other) const;
  bool operator==(const DynamicArray &other) const;
  bool operator!=(const DynamicArray &other) const;

  void setconsole();
  void getconsole();

  int partition(T arr[], size_type low, size_type high);
  void quickSort(T arr[], size_type low, size_type high);

private:
  T *m_arr;
  size_type m_capacity;
  size_type m_size;
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const DynamicArray<T> &a);
template <typename T>
std::istream &operator>>(std::istream &is, DynamicArray<T> &a);

#include "dynamic_array.tpp"

#endif // DYNAMIC_ARRAY_HPP
