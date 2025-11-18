#include "linked_list.hpp"
#include <cassert>
#include <iostream>
#include <string>

// Color codes for terminal output
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

void test_passed(const std::string &test_name) {
  std::cout << GREEN << "[PASS] " << RESET << test_name << std::endl;
}

void test_failed(const std::string &test_name, const std::string &reason = "") {
  std::cout << RED << "[FAIL] " << RESET << test_name;
  if (!reason.empty()) {
    std::cout << " - " << reason;
  }
  std::cout << std::endl;
}

void section_header(const std::string &section) {
  std::cout << "\n"
            << YELLOW << "=== " << section << " ===" << RESET << std::endl;
}

// Test 1: Constructors (10 баллов - yellow)
void test_constructors() {
  section_header("Testing Constructors");

  // Default constructor
  List<int> list1;
  assert(list1.size() == 0);
  assert(list1.empty());
  test_passed("Default constructor");

  // Array constructor
  int arr[] = {1, 2, 3, 4, 5};
  List<int> list2(arr, 5);
  assert(list2.size() == 5);
  assert(list2[0] == 1);
  assert(list2[4] == 5);
  test_passed("Array constructor");

  // Copy constructor
  List<int> list3(list2);
  assert(list3.size() == 5);
  assert(list3[0] == 1);
  assert(list3 == list2);
  test_passed("Copy constructor");
}

// Test 2: Size operations (10 баллов - yellow)
void test_size_operations() {
  section_header("Testing Size Operations");

  List<int> list;
  assert(list.size() == 0);
  assert(list.empty());
  test_passed("Initial size and empty check");

  list.push_back(10);
  assert(list.size() == 1);
  assert(!list.empty());
  test_passed("Size after push_back");

  list.push_front(5);
  assert(list.size() == 2);
  test_passed("Size after push_front");

  list.clear();
  assert(list.size() == 0);
  assert(list.empty());
  test_passed("Size after clear");
}

// Test 3: Push operations (10 баллов - yellow)
void test_push_operations() {
  section_header("Testing Push Operations");

  List<int> list;

  // push_front
  list.push_front(3);
  list.push_front(2);
  list.push_front(1);
  assert(list[0] == 1);
  assert(list[1] == 2);
  assert(list[2] == 3);
  test_passed("push_front");

  // push_back
  list.push_back(4);
  list.push_back(5);
  assert(list[3] == 4);
  assert(list[4] == 5);
  test_passed("push_back");

  // push_at
  list.push_at(2, 99);
  assert(list[2] == 99);
  assert(list.size() == 6);
  test_passed("push_at");

  // push_after
  List<int> list2;
  list2.push_back(1);
  list2.push_back(2);
  list2.push_back(3);
  list2.push_after(2, 25);
  assert(list2[2] == 25);
  test_passed("push_after");
}

// Test 4: Pop operations (10 баллов - yellow)
void test_pop_operations() {
  section_header("Testing Pop Operations");

  int arr[] = {1, 2, 3, 4, 5};
  List<int> list(arr, 5);

  // pop_front
  list.pop_front();
  assert(list.size() == 4);
  assert(list[0] == 2);
  test_passed("pop_front");

  // pop_back
  list.pop_back();
  assert(list.size() == 3);
  assert(list[2] == 4);
  test_passed("pop_back");

  // pop_at
  list.pop_at(1);
  assert(list.size() == 2);
  assert(list[0] == 2);
  assert(list[1] == 4);
  test_passed("pop_at");

  // pop_after
  List<int> list2;
  list2.push_back(1);
  list2.push_back(2);
  list2.push_back(3);
  list2.pop_after(1);
  assert(list2.size() == 2);
  assert(list2[0] == 1);
  assert(list2[1] == 3);
  test_passed("pop_after");
}

// Test 5: Swap (10 баллов - yellow)
void test_swap() {
  section_header("Testing Swap");

  int arr1[] = {1, 2, 3};
  int arr2[] = {4, 5, 6, 7};
  List<int> list1(arr1, 3);
  List<int> list2(arr2, 4);

  list1.swap(list2);

  assert(list1.size() == 4);
  assert(list1[0] == 4);
  assert(list2.size() == 3);
  assert(list2[0] == 1);
  test_passed("swap");
}

// Test 6: Find (5 баллов - green)
void test_find() {
  section_header("Testing Find");

  int arr[] = {10, 20, 30, 40, 50};
  List<int> list(arr, 5);

  auto it = list.find(30);
  assert(it != list.end());
  assert(*it == 30);
  test_passed("find - element exists");

  auto it2 = list.find(999);
  assert(it2 == list.end());
  test_passed("find - element doesn't exist");
}

// Test 7: Min/Max (5 баллов - green)
void test_min_max() {
  section_header("Testing Min/Max");

  int arr[] = {5, 2, 8, 1, 9};
  List<int> list(arr, 5);

  assert(list.min() == 1);
  test_passed("min");

  assert(list.max() == 9);
  test_passed("max");
}

// Test 8: Iterators (5 баллов - green)
void test_iterators() {
  section_header("Testing Iterators");

  int arr[] = {1, 2, 3, 4, 5};
  List<int> list(arr, 5);

  // begin/end iteration
  int expected = 1;
  for (auto it = list.begin(); it != list.end(); ++it) {
    assert(*it == expected);
    expected++;
  }
  test_passed("begin/end iteration");

  // Range-based for loop
  expected = 1;
  for (auto val : list) {
    assert(val == expected);
    expected++;
  }
  test_passed("Range-based for loop");

  // Const iterators
  const List<int> const_list(arr, 5);
  expected = 1;
  for (auto it = const_list.cbegin(); it != const_list.cend(); ++it) {
    assert(*it == expected);
    expected++;
  }
  test_passed("Const iterators (cbegin/cend)");

  // Iterator increment/decrement
  auto it = list.begin();
  ++it;
  assert(*it == 2);
  it++;
  assert(*it == 3);
  --it;
  assert(*it == 2);
  it--;
  assert(*it == 1);
  test_passed("Iterator increment/decrement");
}

// Test 9: Iterator-based operations (5 баллов - green)
void test_iterator_operations() {
  section_header("Testing Iterator-based Operations");

  int arr[] = {1, 2, 3, 4, 5};
  List<int> list(arr, 5);

  // push_it
  auto it = list.begin();
  ++it; // position at 2
  list.push_it(it, 99);
  assert(list[1] == 99);
  test_passed("push_it");

  // pop_it
  it = list.find(99);
  list.pop_it(it);
  assert(list.find(99) == list.end());
  test_passed("pop_it");

  // pop_range
  List<int> list2;
  list2.push_back(1);
  list2.push_back(2);
  list2.push_back(3);
  list2.push_back(4);
  list2.push_back(5);

  auto from = list2.begin();
  ++from; // at 2
  auto to = from;
  ++to;
  ++to; // at 4
  list2.pop_range(from, to);
  assert(list2.size() == 3);
  assert(list2[0] == 1);
  assert(list2[1] == 4);
  test_passed("pop_range");
}

// Test 10: Assignment operator
void test_assignment() {
  section_header("Testing Assignment Operator");

  int arr[] = {1, 2, 3};
  List<int> list1(arr, 3);
  List<int> list2;

  list2 = list1;
  assert(list2.size() == 3);
  assert(list2[0] == 1);
  assert(list2 == list1);
  test_passed("Assignment operator");
}

// Test 11: Element access operator []
void test_subscript() {
  section_header("Testing Subscript Operator []");

  int arr[] = {10, 20, 30, 40};
  List<int> list(arr, 4);

  assert(list[0] == 10);
  assert(list[3] == 40);
  test_passed("Read access with []");

  list[1] = 99;
  assert(list[1] == 99);
  test_passed("Write access with []");

  // Test const access
  const List<int> const_list(arr, 4);
  assert(const_list[2] == 30);
  test_passed("Const access with []");
}

// Test 12: Comparison operators
void test_comparison() {
  section_header("Testing Comparison Operators");

  int arr1[] = {1, 2, 3};
  int arr2[] = {1, 2, 3};
  int arr3[] = {1, 2, 4};

  List<int> list1(arr1, 3);
  List<int> list2(arr2, 3);
  List<int> list3(arr3, 3);

  assert(list1 == list2);
  test_passed("Operator ==");

  assert(list1 != list3);
  test_passed("Operator !=");
}

// Test 13: Concatenation operators
void test_concatenation() {
  section_header("Testing Concatenation Operators");

  int arr1[] = {1, 2, 3};
  int arr2[] = {4, 5, 6};
  List<int> list1(arr1, 3);
  List<int> list2(arr2, 3);

  // operator+
  List<int> list3 = list1 + list2;
  assert(list3.size() == 6);
  assert(list3[0] == 1);
  assert(list3[3] == 4);
  assert(list3[5] == 6);
  test_passed("Operator +");

  // operator+=
  List<int> list4(arr1, 3);
  list4 += list2;
  assert(list4.size() == 6);
  assert(list4[0] == 1);
  assert(list4[5] == 6);
  test_passed("Operator +=");
}

// Test 14: Sort
void test_sort() {
  section_header("Testing Sort");

  int arr[] = {5, 2, 8, 1, 9, 3};
  List<int> list(arr, 6);

  list.sort();

  assert(list[0] == 1);
  assert(list[1] == 2);
  assert(list[2] == 3);
  assert(list[3] == 5);
  assert(list[4] == 8);
  assert(list[5] == 9);
  test_passed("sort");
}

// Test 15: Stream output
void test_stream_output() {
  section_header("Testing Stream Output");

  int arr[] = {1, 2, 3, 4, 5};
  List<int> list(arr, 5);

  std::cout << "List contents: " << list << std::endl;
  test_passed("Stream output (<<)");
}

// Test 16: Clear
void test_clear() {
  section_header("Testing Clear");

  int arr[] = {1, 2, 3, 4, 5};
  List<int> list(arr, 5);

  list.clear();
  assert(list.size() == 0);
  assert(list.empty());
  test_passed("clear");
}

// Test 17: Exception handling
void test_exceptions() {
  section_header("Testing Exception Handling");

  List<int> list;

  try {
    list.pop_front();
    test_failed("pop_front on empty list should throw");
  } catch (const std::out_of_range &) {
    test_passed("pop_front on empty list throws exception");
  }

  try {
    list.pop_back();
    test_failed("pop_back on empty list should throw");
  } catch (const std::out_of_range &) {
    test_passed("pop_back on empty list throws exception");
  }

  try {
    list[0];
    test_failed("operator[] out of range should throw");
  } catch (const std::out_of_range &) {
    test_passed("operator[] out of range throws exception");
  }

  try {
    list.min();
    test_failed("min on empty list should throw");
  } catch (const std::runtime_error &) {
    test_passed("min on empty list throws exception");
  }

  try {
    list.max();
    test_failed("max on empty list should throw");
  } catch (const std::runtime_error &) {
    test_passed("max on empty list throws exception");
  }
}

// Test 18: String type
void test_with_strings() {
  section_header("Testing with String Type");

  List<std::string> list;
  list.push_back("Hello");
  list.push_back("World");
  list.push_back("!");

  assert(list.size() == 3);
  assert(list[0] == "Hello");
  assert(list[1] == "World");
  assert(list[2] == "!");

  std::cout << "String list: " << list << std::endl;
  test_passed("List with strings");
}

// Test 19: Large list operations
void test_large_list() {
  section_header("Testing Large List");

  List<int> list;

  // Add 1000 elements
  for (int i = 0; i < 1000; ++i) {
    list.push_back(i);
  }
  assert(list.size() == 1000);
  assert(list[500] == 500);
  test_passed("Large list creation (1000 elements)");

  // Remove some elements
  for (int i = 0; i < 100; ++i) {
    list.pop_front();
  }
  assert(list.size() == 900);
  assert(list[0] == 100);
  test_passed("Large list pop operations");

  list.clear();
  assert(list.empty());
  test_passed("Large list clear");
}

// Test 20: Memory leak test (destructor)
void test_memory_management() {
  section_header("Testing Memory Management");

  // Create and destroy multiple lists
  for (int i = 0; i < 100; ++i) {
    List<int> list;
    for (int j = 0; j < 100; ++j) {
      list.push_back(j);
    }
    // List should be automatically destroyed here
  }
  test_passed("Memory management (no leaks expected)");
}

int main() {
  std::cout << "\n"
            << YELLOW << "╔════════════════════════════════════════╗" << RESET
            << std::endl;
  std::cout << YELLOW << "║  Linked List Comprehensive Test Suite ║" << RESET
            << std::endl;
  std::cout << YELLOW << "╚════════════════════════════════════════╝" << RESET
            << "\n"
            << std::endl;

  try {
    // Basic operations (10 баллов - yellow methods)
    test_constructors();
    test_size_operations();
    test_push_operations();
    test_pop_operations();
    test_swap();
    test_assignment();
    test_subscript();
    test_comparison();
    test_concatenation();
    test_clear();

    // Advanced operations (5 баллов - green methods)
    test_find();
    test_min_max();
    test_sort();

    // Iterator operations (5 баллов - no color methods)
    test_iterators();
    test_iterator_operations();

    // Stream operations
    test_stream_output();

    // Edge cases and error handling
    test_exceptions();

    // Different types
    test_with_strings();

    // Performance and memory
    test_large_list();
    test_memory_management();

    std::cout << "\n"
              << GREEN << "╔════════════════════════════════════════╗" << RESET
              << std::endl;
    std::cout << GREEN << "║     All Tests Completed Successfully!  ║" << RESET
              << std::endl;
    std::cout << GREEN << "╚════════════════════════════════════════╝" << RESET
              << "\n"
              << std::endl;

    std::cout << "\nTest Coverage:" << std::endl;
    std::cout << "✓ 10 баллов (yellow): Basic constructors and operations"
              << std::endl;
    std::cout << "✓ 5 баллов (green): Advanced search, min/max, sort"
              << std::endl;
    std::cout
        << "✓ 5 баллов (no color): Iterators and iterator-based operations"
        << std::endl;

  } catch (const std::exception &e) {
    std::cout << RED << "\nUnexpected exception: " << e.what() << RESET
              << std::endl;
    return 1;
  }

  return 0;
}