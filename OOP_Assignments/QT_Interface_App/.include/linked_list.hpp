#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <cstddef>
#include <iostream>
#include <stdexcept>

template <typename T> class List {
public:
  class _Iterator;
  class _ConstIterator;

  // Constructors and destructor
  List() noexcept;
  List(const T arr[], size_t n);
  List(const List &other);
  ~List();

  // Size
  std::size_t size() const { return _size; }

  // Swap
  void swap(List<T> &other);

  // Iterators
  _Iterator begin();
  _Iterator end();
  _ConstIterator cbegin() const;
  _ConstIterator cend() const;

  // Find
  _Iterator find(T value);
  _ConstIterator find(T value) const;

  // Push operations
  void push_front(T const value);
  void push_back(T const value);
  void push_at(size_t const index, T const value);
  void push_it(_Iterator iterator, T const value);
  void push_after(T const key, T const value);

  // Pop operations
  void pop_front();
  void pop_back();
  void pop_at(size_t const index);
  void pop_it(_Iterator iterator);
  void pop_after(T const key);
  void pop_range(_Iterator from, _Iterator to);

  // Min/Max
  T min() const;
  T max() const;

  // Empty check
  bool empty() const;

  // Clear
  void clear();

  // Sort
  void sort();

  // Operators
  List<T> &operator=(const List &other);
  T &operator[](size_t index);
  const T &operator[](size_t index) const;
  bool operator==(const List &other) const;
  bool operator!=(const List &other) const;
  List<T> operator+(const List &other) const;
  List<T> &operator+=(const List &other);

  // Stream operators
  template <typename U>
  friend std::ostream &operator<<(std::ostream &os, const List<U> &list);

  template <typename U>
  friend std::istream &operator>>(std::istream &is, List<U> &list);

private:
  class _Node;

  _Node *node_at(size_t const index) const;

  _Node *_head;
  _Node *_tail;
  std::size_t _size;
};

/* -------- List node ------------------ */
template <typename T> class List<T>::_Node {
public:
  T _value;
  _Node *_posnext;
  _Node *_posprev;

  _Node(const T &val, _Node *next = nullptr, _Node *prev = nullptr)
      : _value(val), _posnext(next), _posprev(prev) {}

  T &_val() { return _value; }
  const T &_val() const { return _value; }
  void _setval(T value) { _value = value; }

  _Node *_next() const { return _posnext; }
  _Node *_prev() const { return _posprev; }

  void _setNext(_Node *other) { _posnext = other; }
  void _setPrev(_Node *other) { _posprev = other; }
};

/* -------- Iterator  ------------------ */
template <typename T> class List<T>::_Iterator {
public:
  _Iterator(_Node *node = nullptr) : _pos(node) {}

  _Iterator &operator++() {
    if (_pos)
      _pos = _pos->_next();
    return *this;
  }

  _Iterator operator++(int) {
    _Iterator old(*this);
    if (_pos)
      _pos = _pos->_next();
    return old;
  }

  _Iterator &operator--() {
    if (_pos)
      _pos = _pos->_prev();
    return *this;
  }

  _Iterator operator--(int) {
    _Iterator old(*this);
    if (_pos)
      _pos = _pos->_prev();
    return old;
  }

  void _itSetVal(T const val) {
    if (_pos)
      _pos->_setval(val);
  }

  bool operator==(const _Iterator &other) const { return _pos == other._pos; }
  bool operator!=(const _Iterator &other) const { return _pos != other._pos; }

  T &operator*() const { return _pos->_val(); }
  T *operator->() const { return &(_pos->_val()); }

  _Node *_getNode() const { return _pos; }

private:
  _Node *_pos;
};

/* -------- Const Iterator  ------------------ */
template <typename T> class List<T>::_ConstIterator {
public:
  _ConstIterator(_Node *node = nullptr) : _pos(node) {}

  _ConstIterator &operator++() {
    if (_pos)
      _pos = _pos->_next();
    return *this;
  }

  _ConstIterator operator++(int) {
    _ConstIterator old(*this);
    if (_pos)
      _pos = _pos->_next();
    return old;
  }

  _ConstIterator &operator--() {
    if (_pos)
      _pos = _pos->_prev();
    return *this;
  }

  _ConstIterator operator--(int) {
    _ConstIterator old(*this);
    if (_pos)
      _pos = _pos->_prev();
    return old;
  }

  bool operator==(const _ConstIterator &other) const {
    return _pos == other._pos;
  }
  bool operator!=(const _ConstIterator &other) const {
    return _pos != other._pos;
  }

  const T &operator*() const { return _pos->_val(); }
  const T *operator->() const { return &(_pos->_val()); }

private:
  _Node *_pos;
};

#include "linked_list.tpp"

#endif // LINKED_LIST_H