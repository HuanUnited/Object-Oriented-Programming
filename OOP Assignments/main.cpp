#include "DynamicArrays_int.hpp"
#include <iostream>
using std::cout;
using std::endl;

void printArray(const DynamicArray &arr, const std::string &name) {
  cout << name << " = " << arr << ", size = " << arr.size() << endl;
}

int main() {
  cout << "=== DynamicArray Tests ===" << endl;

  // --- Constructors ---
  cout << "\n-- Constructors --" << endl;
  DynamicArray a;
  printArray(a, "a (default)");

  int raw[] = {1, 2, 3};
  DynamicArray b(raw, 3);
  printArray(b, "b (from raw array)");

  DynamicArray c(b);
  printArray(c, "c (copy of b)");

  DynamicArray d(std::move(c));
  printArray(d, "d (moved from c)");
  printArray(c, "c (after move)");

  DynamicArray inited{1, 2, 4, 6, 8, 8};
  printArray(inited, "initializer constructor");

  // --- Push back / operator+= ---
  cout << "\n-- Push Back / += --" << endl;
  printArray(a, "a before push_back(5)");
  a.push_back(5);
  printArray(a, "a after push_back(5)");

  printArray(a, "a before += 10");
  a += 10;
  printArray(a, "a after += 10");

  printArray(a, "a before creating e = a + 20");
  DynamicArray e = a + 20;
  printArray(e, "e after a + 20");

  // --- Insert / Erase ---
  cout << "\n-- Insert / Erase --" << endl;
  printArray(b, "b before insert_at(1,5)");
  b.insert_at(1, 5);
  printArray(b, "b after insert_at(1,5)");

  printArray(b, "b before erase_at(1)");
  b.erase_at(1);
  printArray(b, "b after erase_at(1)");

  printArray(b, "b before erase(begin())");
  auto it = b.begin();
  b.erase(it);
  printArray(b, "b after erase(begin())");

  // --- Erase range ---
  cout << "\n-- Erase Range --" << endl;
  DynamicArray f{1, 2, 3, 4, 5};
  printArray(f, "f initial");
  printArray(f, "f before erase range [1,4)");
  f.erase(f.begin() + 1, f.begin() + 4);
  printArray(f, "f after erase range [1,4)");

  // --- Pop first / all ---
  cout << "\n-- Pop First / All --" << endl;
  DynamicArray g{1, 2, 1, 3};
  printArray(g, "g initial");

  printArray(g, "g before pop_first(1)");
  g.pop_first(1);
  printArray(g, "g after pop_first(1)");

  printArray(g, "g before pop_all(1)");
  g.pop_all(1);
  printArray(g, "g after pop_all(1)");

  // --- Find, min, max ---
  cout << "\n-- Find / Min / Max --" << endl;
  DynamicArray h{4, 2, 7, 1};
  printArray(h, "h");
  cout << "h.find(7) = " << h.find(7) << endl;
  cout << "h.find(42) = " << h.find(42) << endl;
  cout << "h.min() = " << h.min() << ", h.max() = " << h.max() << endl;

  // --- Swap ---
  cout << "\n-- Swap --" << endl;
  DynamicArray i{1, 2};
  DynamicArray j{10, 20};
  printArray(i, "i before swap");
  printArray(j, "j before swap");
  i.swap(j);
  printArray(i, "i after swap");
  printArray(j, "j after swap");

  // --- Sorting ---
  cout << "\n-- Sorting --" << endl;
  int array[] = {1, 2,   32, 332, 534, 5,  6,   5476, 45, 756, 7,
                 4, 124, 12, 312, 4,   46, 546, 455,  2,  432, 34};
  DynamicArray k{array, 21};
  printArray(k, "k before sort");
  k.qsort();
  printArray(k, "k after sort");

  // --- Insert before iterator ---
  cout << "\n-- Insert Before Iterator --" << endl;
  DynamicArray l{1, 3};
  printArray(l, "l before insert before iterator");
  auto lit = l.begin() + 1;
  l.insert(lit, 2);
  printArray(l, "l after insert before iterator");

  // --- Stream I/O ---
  cout << "\n-- Stream I/O --" << endl;
  DynamicArray m;
  printArray(m, "m before stream input");
  cin >> m;
  printArray(m, "m after stream input");

  cout << "\n=== All tests done ===" << endl;
  return 0;
}
