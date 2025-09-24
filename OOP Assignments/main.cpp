#include "DynamicArrays_2.hpp"
#include <iostream>

int main() {

  DynamicArray<int> v1{};
  v1.push_back(1);
  v1.push_back(2);
  v1.push_back(52);
  v1.push_back(64);
  cout << "V1: " << v1 << '\n';

  int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  DynamicArray<int> v2{a, 10};
  cout << "V2: " << v2 << '\n';

  DynamicArray<int> v3{v2}; // copy constructor
  cout << "V3: " << v3 << '\n';

  DynamicArray<int> v4{std::move(v3)}; // move constructor.
  cout << "V4: " << v4 << '\n';
  cout << "V3: " << v3 << '\n';


  cout << "Console IO " << '\n';
  v3.setconsole();
  v3.getconsole();
}