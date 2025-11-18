#ifndef LINKED_LIST_TPP
#define LINKED_LIST_TPP

// This file should only be included from linked_list.h
#ifndef LINKED_LIST_H
#include "linked_list.hpp"
#endif

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <utility>

/* -------- Helper method ------------------ */
template <typename T>
typename List<T>::_Node *List<T>::node_at(size_t const index) const {
  if (index >= _size)
    throw std::out_of_range("index out of range");

  if (index < _size / 2) {
    _Node *cur = _head;
    for (std::size_t i = 0; i < index; ++i)
      cur = cur->_next();
    return cur;
  } else {
    _Node *cur = _tail;
    for (std::size_t i = _size - 1; i > index; --i)
      cur = cur->_prev();
    return cur;
  }
}

/* -------- List constructors ------------------ */
template <typename T>
List<T>::List() noexcept : _head(nullptr), _tail(nullptr), _size(0) {}

template <typename T> List<T>::List(const T arr[], size_t n) : List() {
  for (size_t i = 0; i < n; ++i)
    push_back(arr[i]);
}

template <typename T> List<T>::List(const List &other) : List() {
  for (_Node *curr = other._head; curr; curr = curr->_next())
    push_back(curr->_val());
}

template <typename T> List<T>::~List() { clear(); }

/* -------- Swap ------------------ */
template <typename T> void List<T>::swap(List<T> &other) {
  std::swap(_head, other._head);
  std::swap(_tail, other._tail);
  std::swap(_size, other._size);
}

/* -------- Iterators ------------------ */
template <typename T> typename List<T>::_Iterator List<T>::begin() {
  return _Iterator(_head);
}

template <typename T> typename List<T>::_Iterator List<T>::end() {
  return _Iterator(nullptr);
}

template <typename T> typename List<T>::_ConstIterator List<T>::cbegin() const {
  return _ConstIterator(_head);
}

template <typename T> typename List<T>::_ConstIterator List<T>::cend() const {
  return _ConstIterator(nullptr);
}

/* -------- Find ------------------ */
template <typename T> typename List<T>::_Iterator List<T>::find(T value) {
  for (_Node *cur = _head; cur; cur = cur->_next()) {
    if (cur->_val() == value)
      return _Iterator(cur);
  }
  return _Iterator(nullptr);
}

template <typename T>
typename List<T>::_ConstIterator List<T>::find(T value) const {
  for (_Node *cur = _head; cur; cur = cur->_next()) {
    if (cur->_val() == value)
      return _ConstIterator(cur);
  }
  return _ConstIterator(nullptr);
}

/* -------- Push operations ------------------ */
template <typename T> void List<T>::push_front(T const value) {
  _Node *n = new _Node(value, _head, nullptr);
  if (_head)
    _head->_setPrev(n);
  _head = n;
  if (!_tail)
    _tail = n;
  ++_size;
}

template <typename T> void List<T>::push_back(T const value) {
  _Node *n = new _Node(value, nullptr, _tail);
  if (_tail)
    _tail->_setNext(n);
  _tail = n;
  if (!_head)
    _head = n;
  ++_size;
}

template <typename T> void List<T>::push_at(size_t const index, T const value) {
  if (index > _size)
    throw std::out_of_range("push_at index out of range");
  if (index == 0) {
    push_front(value);
    return;
  }
  if (index == _size) {
    push_back(value);
    return;
  }
  _Node *cur = node_at(index);
  _Node *n = new _Node(value, cur, cur->_prev());
  cur->_prev()->_setNext(n);
  cur->_setPrev(n);
  ++_size;
}

template <typename T> void List<T>::push_it(_Iterator iterator, T const value) {
  _Node *cur = iterator._getNode();
  if (!cur) {
    push_back(value);
    return;
  }
  if (cur == _head) {
    push_front(value);
    return;
  }
  _Node *n = new _Node(value, cur, cur->_prev());
  cur->_prev()->_setNext(n);
  cur->_setPrev(n);
  ++_size;
}

template <typename T> void List<T>::push_after(T const key, T const value) {
  _Iterator it = find(key);
  if (it == end())
    throw std::runtime_error("Key not found");

  _Node *cur = it._getNode();
  if (cur == _tail) {
    push_back(value);
    return;
  }

  _Node *n = new _Node(value, cur->_next(), cur);
  cur->_next()->_setPrev(n);
  cur->_setNext(n);
  ++_size;
}

/* -------- Pop operations ------------------ */
template <typename T> void List<T>::pop_front() {
  if (!_head)
    throw std::out_of_range("pop_front on empty list");
  _Node *old = _head;
  _head = _head->_next();
  if (_head)
    _head->_setPrev(nullptr);
  else
    _tail = nullptr;
  delete old;
  --_size;
}

template <typename T> void List<T>::pop_back() {
  if (!_tail)
    throw std::out_of_range("pop_back on empty list");
  _Node *old = _tail;
  _tail = _tail->_prev();
  if (_tail)
    _tail->_setNext(nullptr);
  else
    _head = nullptr;
  delete old;
  --_size;
}

template <typename T> void List<T>::pop_at(size_t const index) {
  if (index >= _size)
    throw std::out_of_range("pop_at index out of range");
  if (index == 0) {
    pop_front();
    return;
  }
  if (index == _size - 1) {
    pop_back();
    return;
  }
  _Node *cur = node_at(index);
  cur->_prev()->_setNext(cur->_next());
  cur->_next()->_setPrev(cur->_prev());
  delete cur;
  --_size;
}

template <typename T> void List<T>::pop_it(_Iterator iterator) {
  _Node *cur = iterator._getNode();
  if (!cur)
    throw std::runtime_error("Cannot delete null iterator");

  if (cur == _head) {
    pop_front();
    return;
  }
  if (cur == _tail) {
    pop_back();
    return;
  }

  cur->_prev()->_setNext(cur->_next());
  cur->_next()->_setPrev(cur->_prev());
  delete cur;
  --_size;
}

template <typename T> void List<T>::pop_after(T const key) {
  _Iterator it = find(key);
  if (it == end())
    throw std::runtime_error("Key not found");

  _Node *cur = it._getNode();
  if (cur == _tail)
    throw std::runtime_error("No element after key");

  _Node *to_delete = cur->_next();
  if (to_delete == _tail) {
    pop_back();
  } else {
    cur->_setNext(to_delete->_next());
    to_delete->_next()->_setPrev(cur);
    delete to_delete;
    --_size;
  }
}

template <typename T> void List<T>::pop_range(_Iterator from, _Iterator to) {
  while (from != to && from != end()) {
    _Iterator next = from;
    ++next;
    pop_it(from);
    from = next;
  }
}

/* -------- Min/Max ------------------ */
template <typename T> T List<T>::min() const {
  if (empty())
    throw std::runtime_error("min() on empty list");

  T min_val = _head->_val();
  for (_Node *cur = _head->_next(); cur; cur = cur->_next()) {
    if (cur->_val() < min_val)
      min_val = cur->_val();
  }
  return min_val;
}

template <typename T> T List<T>::max() const {
  if (empty())
    throw std::runtime_error("max() on empty list");

  T max_val = _head->_val();
  for (_Node *cur = _head->_next(); cur; cur = cur->_next()) {
    if (cur->_val() > max_val)
      max_val = cur->_val();
  }
  return max_val;
}

/* -------- Empty ------------------ */
template <typename T> bool List<T>::empty() const { return _size == 0; }

/* -------- Clear ------------------ */
template <typename T> void List<T>::clear() {
  while (_head) {
    _Node *next = _head->_next();
    delete _head;
    _head = next;
  }
  _head = _tail = nullptr;
  _size = 0;
}

/* -------- Sort ------------------ */
template <typename T> void List<T>::sort() {
  if (_size <= 1)
    return;

  // Bubble sort for linked list
  bool swapped;
  do {
    swapped = false;
    _Node *cur = _head;
    while (cur && cur->_next()) {
      if (cur->_val() > cur->_next()->_val()) {
        T temp = cur->_val();
        cur->_setval(cur->_next()->_val());
        cur->_next()->_setval(temp);
        swapped = true;
      }
      cur = cur->_next();
    }
  } while (swapped);
}

/* -------- Operators ------------------ */
template <typename T> List<T> &List<T>::operator=(const List &other) {
  if (this != &other) {
    clear();
    for (_Node *cur = other._head; cur; cur = cur->_next())
      push_back(cur->_val());
  }
  return *this;
}

template <typename T> T &List<T>::operator[](size_t index) {
  if (index >= _size)
    throw std::out_of_range("operator[] index out of range");
  return node_at(index)->_val();
}

template <typename T> const T &List<T>::operator[](size_t index) const {
  if (index >= _size)
    throw std::out_of_range("operator[] index out of range");
  return node_at(index)->_val();
}

template <typename T> bool List<T>::operator==(const List &other) const {
  if (_size != other._size)
    return false;

  _Node *cur1 = _head;
  _Node *cur2 = other._head;
  while (cur1 && cur2) {
    if (cur1->_val() != cur2->_val())
      return false;
    cur1 = cur1->_next();
    cur2 = cur2->_next();
  }
  return true;
}

template <typename T> bool List<T>::operator!=(const List &other) const {
  return !(*this == other);
}

template <typename T> List<T> List<T>::operator+(const List &other) const {
  List<T> result(*this);
  for (_Node *cur = other._head; cur; cur = cur->_next())
    result.push_back(cur->_val());
  return result;
}

template <typename T> List<T> &List<T>::operator+=(const List &other) {
  for (_Node *cur = other._head; cur; cur = cur->_next())
    push_back(cur->_val());
  return *this;
}

/* -------- Stream operators ------------------ */
template <typename T>
std::ostream &operator<<(std::ostream &os, const List<T> &list) {
  os << "[";
  typename List<T>::_Node *cur = list._head;
  while (cur) {
    os << cur->_val();
    if (cur->_next())
      os << ", ";
    cur = cur->_next();
  }
  os << "]";
  return os;
}

template <typename T>
std::istream &operator>>(std::istream &is, List<T> &list) {
  T value;
  if (is >> value)
    list.push_back(value);
  return is;
}

#endif // LINKED_LIST_TPP