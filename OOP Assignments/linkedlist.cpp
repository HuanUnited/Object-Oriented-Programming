/*
i want to write my own code so i'll try to understand this assignment first. The
thing we need to make is a doubly linked list.

Understanding the structure of the doubly linked list we have a node.

A node contains the value of itself, and the nodes before and after it.

A list is a collection of nodes, though not an array so we can assume that it
contains the nodes and a tool to navigate the nodes.

No know if a bunch of nodes is contained within the list or not we can assume
that we need to know the position of the head of the list, and the position of
the tail of the list.

We will also need to know how many nodes are contained within the list.

Great. Next will be navigating the list.
The simplest way to do so would be to just make seperate methods to handle the
navigation. But then there would be no tracker to know which position we're at
at any given time. We need an iterator class to be used as a pointer for
navigating the list. The iterator class should point to the position of the
current node, be able to move up and down the list, compare itself or in this
case the value that the interator points to within the list to the value of
another iterator.
1. The Node Class / Struct <I don't know if there is a difference>
*/

/*
Now that we've defined the elements of your doubly linked list class, we can get
to work on implementing the specific features of the class.

The required linked list class needs for a constructor, destructor
getting the size of the list
I/O stream
push_front push_back pop - push_at, pop_at, push_it, pop_it, pop_range

bool empty()
size
begin and end pointers
etc.. i'll write it all out as per the requirements of the assignments
*/

#include <cstddef>
#include <stdexcept>

/* -------- List class ------------------ */
template <typename T> class List {
public:
  class _Iterator;

  //- конструкторы (по умолчанию, конструктор из обычного массива, конструктор
  // копирования); - деструктор;
  List() noexcept;
  List(const T arr[], size_t n);
  List(const List &other);
  ~List();

  // - получение размера списка;
  std::size_t size() const { return _size; };

  // - обмен содержимого с другим списком (swap);
  void swap(List<T> &other);

  // - получение итераторов на начало/конец списка (методы должны называться
  // begin и end. Метод end должен возвращать итератор не на последний элемент,
  // а за позицию после него);
  _Iterator begin();
  _Iterator end();

  _Iterator cbegin() const;
  _Iterator cend() const;

  // - поиск элемента по ключу (возвращает указатель/итератор на элемент или
  // nullptr, если элемента нет в списке);
  _Iterator find(T value) const;

  // - добавление элемента (в голову, хвост, на позицию, после ключа (после
  // первого вхождения), по итератору);
  //  - удаление элемента (из головы, хвоста, позиции, по ключу (первое
  //  вхождение), по итератору);
  void push_front(T const value);
  void pop_front();

  void push_back(T const value);
  void pop_back();

  void push_at(size_t const index, T const value);
  void pop_at(size_t const index, T const value);

  void push_it(_Iterator iterator, T const value);
  void pop_it(_Iterator iterator, T const value);

  void push_after(T const value);
  void pop_after(T const value);

  // - удаление диапазона элементов с помощью итераторов;
  void pop_range(_Iterator from, _Iterator to);

  // - поиск максимального/минимального элемента;
  T min();
  T max();

  // - isEmpty() - возвращает true, если список пуст;
  bool empty();

  // - очистка списка;
  void clear();

  // - сортировка списка;
  void sort();

  // - присваивание (=);
  List<T> operator=(List &other);

  // - получение ссылки на ключ элемента ([ ]);
  T operator[](size_t index);

  // - сравнение (==, !=);
  bool operator==(List &other);
  bool operator!=(List &other);

  // - сложение (конкатенация) списков (+, +=).
  List<T> operator+(List &other);
  List<T> operator+=(List &other);

  // helper methods:

private:
  class _Node;

  _Node *node_at(size_t const index) const;

  _Node *_head;
  _Node *_tail;
  std::size_t _size;
};

/* -------- List node ------------------ */
template <typename T> class List<T>::_Node {
private:
  T _value;
  _Node *_posnext;
  _Node *_posprev;

public:
  _Node(const T &, _Node = nullptr, _Node = nullptr);

  T const _val() const { return _value; };
  void _setval(T value) { _value = value; };

  _Node *_next() const { return _posnext; };
  _Node *_prev() const { return _posprev; };

  void *_setNext(_Node const *other) { _posnext = other; };
  void *_setPrev(_Node const *other) { _posprev = other; };

  void unlink_and_delete(_Node *n);
};

/* -------- Iterator  ------------------ */
template <typename T> class List<T>::_Iterator {
public:
  _Iterator &operator++() {
    if (_pos)
      _pos = _pos->_next();
    return *this;
  };
  _Iterator operator++(int) {
    _Iterator old(*this);
    if (_pos)
      _pos = _pos->_next();
    return old;
  };

  _Iterator &operator--() {
    if (_pos)
      _pos = _pos->_prev();
    return *this;
  };
  _Iterator operator--(int) {
    _Iterator old(*this);
    if (_pos)
      _pos = _pos->_prev();
    return old;
  };

  // overload for setval, it's just an extra layer of abstraction.
  void _itSetVal(T const val) { _pos->_setval(val); };

  bool operator==(const _Iterator &other) const { return _pos == other._pos; };
  bool operator!=(const _Iterator &other) const { return _pos != other._pos; };

  T &operator*() const { return _pos->_val(); };

  _Iterator(_Node * = nullptr);

private:
  _Node *_pos;
};
/* -------- Node implementations ------------------ */
template <typename T>
typename List<T>::_Node *List<T>::_Node::node_at(size_t const index) const {
  if (index >= _size)
    throw std::out_of_range("index out of range");
  if (index < _size / 2) {
    *Node *cur = _head;
    for (std::size_t i = 0; i < index; ++i)
      cur = cur->next;
    return cur;
  } else {
    *Node *cur = _tail;
    for (std::size_t i = _size - 1; i > index; --i)
      cur = cur->prev;
    return cur;
  }
}

/* -------- List constructors ------------------ */
template <typename T> List<T>::List() noexcept {
  _head = _tail = nullptr;
  _size = 0;
};
template <typename T> List<T>::List(const T arr[], size_t n) : List() {
  for (int i = 0; i < n; ++i)
    push_back(arr[i]);
};

template <typename T> List<T>::List(const List &other) : List() {
  for (_Node *curr = other._head; curr; curr = curr->_next())
    push_back(curr->_val());
};

template <typename T> List<T>::~List() { clear(); }

/* -------- List implementations ------------------ */

template <typename T> void List<T>::swap(List<T> &other) {
  List<T> temp(*this);
  _head = other._head;
  _tail = other._tail;
  _size = other._size;

  other._head = temp._head;
  other._tail = temp._tail;
  other._size = temp._size;
  ~temp;
}

template <typename T> typename List<T>::_Iterator List<T>::begin() {
  return typename List<T>::_Iterator(_head);
}
template <typename T> typename List<T>::_Iterator List<T>::end() {
  return typename List<T>::_Iterator(nullptr);
}
template <typename T> typename List<T>::_Iterator List<T>::cbegin() const {
  return typename List<T>::_Iterator(_head);
}
template <typename T> typename List<T>::_Iterator List<T>::cend() const {
  return typename List<T>::_Iterator(nullptr);
}

template <typename T> typename List<T>::_Iterator List<T>::find(T value) const {
  for (_Node *cur = _head; cur; cur = cur->next) {
    if (cur->value == value)
      return _Iterator(cur);
  }
  return _Iterator(nullptr);
}

template <typename T> void List<T>::push_front(T const value) {
  _Node *n = new _Node(value, _head, nullptr);
  if (_head)
    _head->prev = n;
  _head = n;
  if (!_tail)
    _tail = n;
  ++_size;
};

template <typename T> void List<T>::push_back(T const value) {
  _Node *n = new _Node(value, nullptr, _tail);
  if (_tail)
    _tail->next = n;
  _tail = n;
  if (!_head)
    _head = n;
  ++_size;
};

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
  _Node *cur = node_at(index); // current node at position index
  _Node *n = new _Node(value, cur, cur->prev);
  cur->prev->next = n;
  cur->prev = n;
  ++_size;
};
template <typename T> void List<T>::push_it(_Iterator iterator) {};
template <typename T> void List<T>::push_after(T const value) {};

template <typename T> void List<T>::pop_front() {
  if (!_head)
    throw std::out_of_range("pop_front on empty list");
  _Node *old = _head;
  _head = _head->next;
  if (_head)
    _head->prev = nullptr;
  else
    _tail = nullptr;
  delete old;
  --_size;
};

template <typename T> void List<T>::pop_back() {
  if (!_tail)
    throw std::out_of_range("pop_back on empty list");
  _Node *old = _tail;
  _tail = _tail->prev;
  if (_tail)
    _tail->next = nullptr;
  else
    _head = nullptr;
  delete old;
  --_size;
};

template <typename T> void List<T>::pop_at(size_t const index) {};
template <typename T> void List<T>::pop_it(_Iterator iterator) {};
template <typename T> void List<T>::pop_after(T const value) {};
template <typename T> void List<T>::pop_range(_Iterator from, _Iterator to) {};

template <typename T> T List<T>::min() {};
template <typename T> T List<T>::max() {};

template <typename T> bool List<T>::empty() {};

template <typename T> void List<T>::clear() {};

template <typename T> void List<T>::sort() {};

// - присваивание (=);
template <typename T> List<T> List<T>::operator=(List &other) {};

// - получение ссылки на ключ элемента ([ ]);
template <typename T> T List<T>::operator[](size_t index) {};

// - сравнение (==, !=);
template <typename T> bool List<T>::operator==(List &other) {};
template <typename T> bool List<T>::operator!=(List &other) {};

// - сложение (конкатенация) списков (+, +=).
template <typename T> List<T> operator+(List<T> &other) {};
template <typename T> List<T> operator+=(List<T> &other) { return *this; };

// - ввод/вывод в консоль (потоковый);
