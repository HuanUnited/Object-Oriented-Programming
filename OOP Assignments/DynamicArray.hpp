#include <cstdlib>
#include <iostream>

#define MAX 1000000

using std::cin, std::cout, std::endl;

// Other Functions:
void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

int partition(int arr[], int low, int high) {
  // Initialize pivot to be the first element
  int p = arr[low];
  int i = low;
  int j = high;

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
      swap(&arr[i], &arr[j]);
    }
  }
  swap(&arr[low], &arr[j]);
  return j;
}

void quickSort(int arr[], int low, int high) {
  if (low < high) {
    // call partition function to find Partition Index
    int pi = partition(arr, low, high);

    // Recursively call quickSort() for left and right
    // half based on Partition Index
    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
  }
}

template <typename T> class DynamicArray {
private:
  T *m_arr;
  int m_capacity;
  int m_size;

public:
  // Setters&Getters
  T at(int const index) const;
  void set(int const index, int const value);
  int size() const;
  int capacity() const;

  T *begin() const;
  T *end() const;

  // Methods:
  void swap(DynamicArray &other);
  T find(int const index) const;
  void sort();
  void push(T const value);

  void popat(int const index);
  void pop(T const value);
  void popall(T const value);
  void popit(T *const iterator);
  void popit(T *const iterator1, T *const iterator2);

  void setconsole();
  void getconsole();
  void min() const;
  void max() const;

  // Operators:
  T& operator[](int index);
  DynamicArray<T>& operator=(const DynamicArray<T>& other);
  DynamicArray<T>& operator=(DynamicArray<T>&& other) noexcept;
  DynamicArray<T>& operator+=(const T& value); // Push one element
  DynamicArray<T>& operator+=(const DynamicArray<T>& other); // Concatenation
  DynamicArray<T> operator+(const T& value) const; // Push to new array
  DynamicArray<T> operator+(const DynamicArray<T>& other) const; //Concatenation to New Array
  bool operator==(const DynamicArray<T>& other) const; // Comparison
  bool operator!=(const DynamicArray<T>& other) const;

  // Constructors + Destructors:
  DynamicArray();
  DynamicArray(T *arr, T size);
  DynamicArray(const DynamicArray &other); // Copy Constructor
  DynamicArray(DynamicArray&& other) noexcept; // Move Constructor
  ~DynamicArray();
};

// -------------------------------
// Method Definitions (outside)
// -------------------------------

// Constructor
template <typename T> DynamicArray<T>::DynamicArray() {
    m_arr = new T[1];
    m_capacity = 1;
    m_size = 0;
}

// Constructor with array
template <typename T> DynamicArray<T>::DynamicArray(T *arr, T size) {
  m_arr = new T[size];
  for (int i = 0; i < size; i++) {
    m_arr[i] = arr[i];
}
  m_capacity = size;
  m_size = size;
}

// Copy constructor
template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray &other)
    : m_capacity{other.m_capacity}, m_size{other.m_size} {
  m_arr = new T[m_size];
  for (int i = 0; i < m_size; i++) {
    m_arr[i] = other.m_arr[i];
}
}

// Move constructor
template <typename T>
DynamicArray<T>::DynamicArray(DynamicArray&& other) noexcept
        : m_arr(other.m_arr),
          m_capacity(other.m_capacity),
          m_size(other.m_size) {
        // Leave 'other' in a valid but empty state
  other.m_arr = nullptr;
  other.m_capacity = 0;
  other.m_size = 0;
}


// Destructor
template <typename T> DynamicArray<T>::~DynamicArray() { delete[] m_arr; }

// at() method
template <typename T> T DynamicArray<T>::at(int const index) const {
  if (index >= m_capacity)
    return false;
  else
    return m_arr[index];
}
// set()
template <typename T>
void DynamicArray<T>::set(int const index, int const value) {
  if (index >= m_capacity)
    return;
  else
    m_arr[index] = value;
}
// swap()
template <typename T> void DynamicArray<T>::swap(DynamicArray &other) {
  std::swap(m_capacity,other.m_capacity);
  std::swap(m_size,other.m_size);
  std::swap(m_arr, other.m_arr);
};
template <typename T> int DynamicArray<T>::size() const { return m_size; };
template <typename T> int DynamicArray<T>::capacity() const {
  return m_capacity;
};

template <typename T> void DynamicArray<T>::setconsole() {
  cout << "Input number of elements: ";
  int capacity;
  cin >> capacity;
  cout << '\n' << "Input values: ";
  for (int i = 0; i < capacity; i++) {
    T value{};
    cin >> value;
    push(value);
  }
};
template <typename T> void DynamicArray<T>::getconsole() {
  cout << "Elements:";
  for (int i = 0; i <= m_size; i++) {
    cout << " " << m_arr[i];
  }
  cout << '\n'
       << "Size " << m_size << '\n'
       << "Capacity: " << m_capacity << '\n';
};

template <typename T> T DynamicArray<T>::find(int const value) const {
  for (int i = 0; i <= m_size; i++) {
    if (m_arr[i] == value) {
      return i;
    } else
      return -1;
  }
};

template <typename T> void DynamicArray<T>::sort() { quickSort(m_arr); };

template <typename T> void DynamicArray<T>::push(T const value) {
  if (m_size == m_capacity) {
    T *temp = new T[2 * m_capacity];

    for (int i = 0; i < m_capacity; i++) {
      temp[i] = m_arr[i];
    }
    delete[] m_arr;
    m_capacity *= 2;
    m_arr = temp;
  }

  m_arr[m_size] = value;
  m_size++;
}

template <typename T> void DynamicArray<T>::popat(int const index) {
  // Basically shifts left
  if (index > m_size || index == -1) {
    return;
  }

  for (int k = index; k <= m_size; k++) {
    m_arr[k] = m_arr[k + 1];
    m_size--;
  }
};
template <typename T> void DynamicArray<T>::pop(T const value) {
  popat(find(value));
};
template <typename T> void DynamicArray<T>::popall(T const value) {
  for (int i = 0; i <= m_size; i++) {
    if (m_arr[i] == value) {
      popat(i);
    }
  }
};
template <typename T> void DynamicArray<T>::min() const {
  T min{MAX};
  for (int i = 0; i <= m_size; i++) {
    if (m_arr[i] < min) {
      min = m_arr[i];
    }
  }
  return min;
}
template <typename T> void DynamicArray<T>::max() const {
  T max{-MAX};
  for (int i = 0; i <= m_size; i++) {
    if (m_arr[i] > max) {
      max = m_arr[i];
    }
  }
  return max;
}

template <typename T> T *DynamicArray<T>::begin() const { return m_arr; };
template <typename T> T *DynamicArray<T>::end() const {
  return m_arr + m_size;
};

template <typename T> void DynamicArray<T>::popit(T *const iterator) {
  // Bound check
  if (iterator < begin() || iterator >= end()) {
    throw std::out_of_range("erase: iterator out of range");
  }

  int index = iterator - m_arr; // get index from iterator
  for (int i = index; i < m_size - 1; ++i) {
    m_arr[i] = m_arr[i + 1]; // shift left
  }
  --m_size;
};

template <typename T>
void DynamicArray<T>::popit(T *const iterator1, T *const iterator2) {
  // Bound Check
  if (iterator1 < begin() || iterator1 > end() || iterator2 < begin() ||
      iterator2 > end() || iterator1 > iterator2) {
    throw std::out_of_range("erase: invalid range");
  }

  int start = iterator1 - m_arr;
  int finish = iterator2 - m_arr;
  int count = finish - start; // number of elements to remove

  for (int i = start; i < m_size - count; ++i) {
    m_arr[i] = m_arr[i + count]; // shift left
  }
  m_size -= count;
};

// Operators:
template <typename T>
T& DynamicArray<T>::operator[](int index) {
  // optional: add bounds check here
  return m_arr[index];
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& other) {
    if (this == &other) return *this; // self-assignment check

  delete[] m_arr; // free old memory

  m_capacity = other.m_capacity;
  m_size = other.m_size;
  m_arr = new T[m_capacity];
  for (int i = 0; i < m_size; ++i) {
    m_arr[i] = other.m_arr[i];
  }
  return *this;
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator+=(const T& value) {
  push(value);
  return *this;
}

template <typename T>
DynamicArray<T> DynamicArray<T>::operator+(const T& value) const {
  DynamicArray<T> result(*this);
  result += value;
  return result;
}


template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray<T>&& other) noexcept {
  if (this != &other) {
    delete[] m_arr;

    m_arr = other.m_arr;
    m_size = other.m_size;
    m_capacity = other.m_capacity;

    other.m_arr = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
  }
  return *this;
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator+=(const DynamicArray<T>& other) {
  for (int i = 0; i < other.m_size; ++i) {
    *this += other.m_arr[i]; // reuse single-element +=
  }
  return *this;
}

template <typename T>
bool DynamicArray<T>::operator==(const DynamicArray<T>& other) const {
    if (m_size != other.m_size) return false;
  for (int i = 0; i < m_size; ++i) {
        if (m_arr[i] != other.m_arr[i]) return false;
  }
  return true;
}

template <typename T>
bool DynamicArray<T>::operator!=(const DynamicArray<T>& other) const {
  return !(*this == other);
}

