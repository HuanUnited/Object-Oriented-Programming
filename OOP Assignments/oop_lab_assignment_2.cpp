// Implement Dynamic Array (minimal reimplementation of std::vector)

#include <bits/stdc++.h>
#include <cinttypes>
using std::cin, std::cout, std::endl;
template <typename T> class  DynamicArray {


public:


  void shrink() {
    if (m_size >= m_capacity) {
      return;
    } else {
      T *temp = new T[m_size];
      for (int i = 0; i < m_capacity; i++) {
        temp[i] = m_arr[i];
      }
      delete[] m_arr;
      m_capacity = m_size;
      m_arr = temp;
    }
  }

  void update(int capacity) {
    if (m_size >= m_capacity) {
      return;
    } else {
      T *temp = new T[capacity];
      for (int i = 0; i < m_capacity; i++) {
        temp[i] = m_arr[i];
      }
      delete[] m_arr;
      m_capacity = capacity;
      m_arr = temp;
    }
  }

  void swap_member(T x, T y) {
    int temp = y;
    y = x;
    x = temp;
  }

  // Swaps all the elements of this array and the other array.
  // Update the capacity of both arrays to the value of the bigger array
  // Swaps the elements
  // (Possibly) shrink the arrays.
  void swap(DynamicArray &other) {
    if (m_capacity > other.m_capacity) {
      other.update(m_capacity);
      other.m_size = m_size;
    } else {
      update(other.m_capacity);
      m_size = other.m_size;
    }
    for (int i = 0; i < m_capacity; i++) {
      swap_member(m_arr[i], other.m_arr[i]);
    }
  }

  // Function to add an element at the last
  void push(T data) {
    if (m_size == m_capacity) {
      T *temp = new T[2 * m_capacity];

      for (int i = 0; i < m_capacity; i++) {
        temp[i] = m_arr[i];
      }
      delete[] m_arr;
      m_capacity *= 2;
      m_arr = temp;
    }

    m_arr[m_size] = data;
    m_size++;
  }

  // function to add element at any index
  void push(T data, int index) {
    if (index == m_capacity)
      push(data);
    else
      m_arr[index] = data;
  }

  // function to extract element at any index
  T at(int index) {
    if (index < m_size)
      return m_arr[index];
    return -1;
  }

  // function to delete last element
  void pop() { m_size--; }

  // function to get size of the vector
  int getSize() { return m_size; }

  // function to get capacity of the vector
  int getCapacity() { return m_capacity; }

  // function to print array elements
  void print() {
    for (int i = 0; i < m_size; i++) {
      cout << m_arr[i] << " ";
    }
    cout << endl;
  }

  void input() {
    int size{};
    cout << "Input size of Array: ";
    cin >> size;
    cout << '\n' << "Input elements of the Array: ";
    for (int i = 0; i < size; i++) {
      int temp{};
      cin >> temp;
      push(temp);
    }
  }
};

// Driver code
int main() {
  DynamicArray<int> v;

  cout << "Size: " << v.getSize() << '\n';
  cout << "Capacity: " << v.getCapacity() << '\n';

  v.input();
  v.print();
  cout << "Size: " << v.getSize() << '\n';
  cout << "Capacity: " << v.getCapacity() << '\n';

  v.shrink();
  cout << "Size: " << v.getSize() << '\n';
  cout << "Capacity: " << v.getCapacity() << '\n';

  cout<< "Copy Constructor";
    DynamicArray<int> v2(v);
    v2.print();
    v2.push(4);
    v2.push(5);

  cout<<"Swap: "<< '\n';
    v.swap(v2);
    v.print();
    v2.print();

  return 0;
}