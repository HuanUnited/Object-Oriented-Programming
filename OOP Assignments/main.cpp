#include "DynamicArrays_int.hpp"
#include <cstddef>
#include <iostream>

int main() {

  // DynamicArray v1{};
  // v1.push_back(1);
  // v1.push_back(2);
  // v1.push_back(52);
  // v1.push_back(64);
  // cout << "V1: " << v1 << '\n';

  // int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  // DynamicArray v2{a, 10};
  // cout << "V2: " << v2 << '\n';

  // DynamicArray v3{v2}; // copy constructor
  // cout << "V3: " << v3 << '\n';

  // DynamicArray v4{std::move(v3)}; // move constructor.
  // cout << "V4: " << v4 << '\n';
  // cout << "V3: " << v3 << '\n';

  // cout << "Console IO " << '\n';
  // v3.setconsole();
  // v3.getconsole();

  int b[] = {3,   53, 76,  76, 4,   4, 56, 545, 45, 24, 2342,
             423, 65, 546, 7,  658, 8, 5,  45,  2,  4};

  size_t sizeofArray = sizeof(b) / sizeof(b[0]);

  DynamicArray v5(b, sizeofArray);
  cout << "V5: " << v5 << '\n';

  v5.qsort();
  cout << "Sorted: " << v5 << '\n';

  DynamicArray v1{};
  v1.push_back(1);
  v1.push_back(2);
  v1.push_back(52);
  v1.push_back(64);
  cout << "V1: " << v1 << '\n';

  DynamicArray v2{};
  v2.push_back(6);
  v2.push_back(8);
  v2.push_back(7);
  v2.push_back(12);
  cout << "V2: " << v2 << '\n';

  v1 += v2;
  cout << "V1: " << v1 << '\n';

  DynamicArray v3 = v1 + v2;
  cout << "V3: " << v3 << '\n';

  DynamicArray v4{v3};

  if(v4 == v3) cout << "True" << '\n';
  if(v4 != v2) cout << "False" << '\n';

}