// Implement Dynamic Array (minimal reimplementation of std::vector)

#include <bits/stdc++.h>
using std::cin, std::cout, std::endl;
template <typename T> class DynamicArray {
  // arr is the integer pointer
  // which stores the address of our vector
  T *arr;

  // capacity is the total storage
  // capacity of the vector
  int capacity;

  // size is the number of elements
  // currently present in the vector
  int size;

public:
  DynamicArray() {
    arr = new T[1];
    capacity = 1;
    size = 0;
  }
  ~DynamicArray() { delete[] arr; }

  // Function to add an element at the last
  void push(T data) {
    if (size == capacity) {
      T *temp = new T[2 * capacity];

      for (int i = 0; i < capacity; i++) {
        temp[i] = arr[i];
      }
      delete[] arr;
      capacity *= 2;
      arr = temp;
    }

    arr[size] = data;
    size++;
  }

  // function to add element at any index
  void push(T data, int index) {
    if (index == capacity)
      push(data);
    else
      arr[index] = data;
  }

  // function to extract element at any index
  T at(int index) {
    if (index < size)
      return arr[index];
    return -1;
  }

  // function to delete last element
  void pop() { size--; }

  // function to get size of the vector
  int getSize() { return size; }

  // function to get capacity of the vector
  int getCapacity() { return capacity; }

  // function to print array elements
  void print() {
    for (int i = 0; i < size; i++) {
      cout << arr[i] << " ";
    }
    cout << endl;
  }
};

// Driver code
int main() {
  DynamicArray<int> v;
  DynamicArray<char> v1;
  v.push(10);
  v.push(20);
  v.push(30);
  v.push(40);
  v.push(50);
  v1.push(71);
  v1.push(72);
  v1.push(73);
  v1.push(74);

  cout << "Vector size : " << v.getSize() << endl;
  cout << "Vector capacity : " << v.getCapacity() << endl;

  cout << "Vector elements : ";
  v.print();

  v.push(100, 1);

  cout << "\nAfter updating 1st index" << endl;

  cout << "Vector elements of type int : " << endl;
  v.print();
  // This was possible because we used templates
  cout << "Vector elements of type char : " << endl;
  v1.print();
  cout << "Element at 1st index of type int: " << v.at(1) << endl;
  cout << "Element at 1st index of type char: " << v1.at(1) << endl;

  v.pop();
  v1.pop();

  cout << "\nAfter deleting last element" << endl;

  cout << "Vector size of type int: " << v.getSize() << endl;
  cout << "Vector size of type char: " << v1.getSize() << endl;
  cout << "Vector capacity of type int : " << v.getCapacity() << endl;
  cout << "Vector capacity of type char : " << v1.getCapacity() << endl;

  cout << "Vector elements of type int: ";
  v.print();
  cout << "Vector elements of type char: ";
  v1.print();

  return 0;
}