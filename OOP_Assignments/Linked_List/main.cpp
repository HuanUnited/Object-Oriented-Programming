#include "./.include/linked_list.hpp"
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

  // Test string operations
  list.push_front("Start");
  assert(list[0] == "Start");
  test_passed("String push_front");

  auto it = list.find("World");
  assert(it != list.end());
  assert(*it == "World");
  test_passed("String find");

  // Test min/max with strings (lexicographic order)
  List<std::string> list2;
  list2.push_back("zebra");
  list2.push_back("apple");
  list2.push_back("mango");
  assert(list2.min() == "apple");
  assert(list2.max() == "zebra");
  test_passed("String min/max");

  // Test sort with strings
  list2.sort();
  assert(list2[0] == "apple");
  assert(list2[1] == "mango");
  assert(list2[2] == "zebra");
  test_passed("String sort");
}

// Test 18b: Char type
void test_with_chars() {
  section_header("Testing with Char Type");

  // Basic operations
  List<char> list;
  list.push_back('H');
  list.push_back('e');
  list.push_back('l');
  list.push_back('l');
  list.push_back('o');

  assert(list.size() == 5);
  assert(list[0] == 'H');
  assert(list[4] == 'o');

  std::cout << "Char list: " << list << std::endl;
  test_passed("List with chars - basic operations");

  // Array constructor with chars
  char arr[] = {'A', 'B', 'C', 'D', 'E'};
  List<char> list2(arr, 5);
  assert(list2.size() == 5);
  assert(list2[0] == 'A');
  assert(list2[4] == 'E');
  test_passed("Char array constructor");

  // Push operations
  list2.push_front('Z');
  assert(list2[0] == 'Z');
  list2.push_back('F');
  assert(list2[6] == 'F');
  test_passed("Char push operations");

  // Pop operations
  list2.pop_front();
  assert(list2[0] == 'A');
  list2.pop_back();
  assert(list2.size() == 5);
  test_passed("Char pop operations");

  // Find operation
  auto it = list2.find('C');
  assert(it != list2.end());
  assert(*it == 'C');
  test_passed("Char find");

  // Min/Max
  assert(list2.min() == 'A');
  assert(list2.max() == 'E');
  test_passed("Char min/max");

  // Sort
  char unsorted[] = {'Z', 'A', 'M', 'B', 'Y'};
  List<char> list3(unsorted, 5);
  list3.sort();
  assert(list3[0] == 'A');
  assert(list3[1] == 'B');
  assert(list3[2] == 'M');
  assert(list3[3] == 'Y');
  assert(list3[4] == 'Z');
  test_passed("Char sort");

  // Iterator operations
  List<char> list4;
  list4.push_back('X');
  list4.push_back('Y');
  list4.push_back('Z');

  std::string result = "";
  for (auto c : list4) {
    result += c;
  }
  assert(result == "XYZ");
  test_passed("Char iterator operations");

  // Comparison
  char arr1[] = {'a', 'b', 'c'};
  char arr2[] = {'a', 'b', 'c'};
  char arr3[] = {'a', 'b', 'd'};
  List<char> clist1(arr1, 3);
  List<char> clist2(arr2, 3);
  List<char> clist3(arr3, 3);

  assert(clist1 == clist2);
  assert(clist1 != clist3);
  test_passed("Char comparison operators");

  // Concatenation
  List<char> clist4 = clist1 + clist3;
  assert(clist4.size() == 6);
  assert(clist4[0] == 'a');
  assert(clist4[5] == 'd');
  test_passed("Char concatenation");

  // Copy constructor
  List<char> list5(list2);
  assert(list5.size() == list2.size());
  assert(list5 == list2);
  test_passed("Char copy constructor");

  // Assignment operator
  List<char> list6;
  list6 = list2;
  assert(list6 == list2);
  test_passed("Char assignment operator");
}

// Test 18c: Float type
void test_with_floats() {
  section_header("Testing with Float Type");

  // Basic operations
  List<float> list;
  list.push_back(3.14f);
  list.push_back(2.71f);
  list.push_back(1.41f);

  assert(list.size() == 3);
  assert(list[0] == 3.14f);
  assert(list[1] == 2.71f);
  assert(list[2] == 1.41f);

  std::cout << "Float list: " << list << std::endl;
  test_passed("List with floats - basic operations");

  // Array constructor with floats
  float arr[] = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f};
  List<float> list2(arr, 5);
  assert(list2.size() == 5);
  assert(list2[0] == 1.1f);
  assert(list2[4] == 5.5f);
  test_passed("Float array constructor");

  // Push operations
  list2.push_front(0.5f);
  assert(list2[0] == 0.5f);
  list2.push_back(6.6f);
  assert(list2[6] == 6.6f);
  test_passed("Float push operations");

  // Pop operations
  list2.pop_front();
  assert(list2[0] == 1.1f);
  list2.pop_back();
  assert(list2.size() == 5);
  test_passed("Float pop operations");

  // Push at index
  list2.push_at(2, 9.9f);
  assert(list2[2] == 9.9f);
  assert(list2.size() == 6);
  test_passed("Float push_at");

  // Pop at index
  list2.pop_at(2);
  assert(list2[2] == 3.3f);
  assert(list2.size() == 5);
  test_passed("Float pop_at");

  // Find operation
  auto it = list2.find(3.3f);
  assert(it != list2.end());
  assert(*it == 3.3f);
  test_passed("Float find");

  // Min/Max
  float min_val = list2.min();
  float max_val = list2.max();
  assert(min_val == 1.1f);
  assert(max_val == 5.5f);
  test_passed("Float min/max");

  // Sort
  float unsorted[] = {5.5f, 1.1f, 3.3f, 2.2f, 4.4f};
  List<float> list3(unsorted, 5);
  list3.sort();
  assert(list3[0] == 1.1f);
  assert(list3[1] == 2.2f);
  assert(list3[2] == 3.3f);
  assert(list3[3] == 4.4f);
  assert(list3[4] == 5.5f);
  test_passed("Float sort");

  // Iterator operations
  List<float> list4;
  list4.push_back(10.5f);
  list4.push_back(20.5f);
  list4.push_back(30.5f);

  float sum = 0.0f;
  for (auto val : list4) {
    sum += val;
  }
  assert(sum == 61.5f);
  test_passed("Float iterator operations");

  // Comparison
  float arr1[] = {1.0f, 2.0f, 3.0f};
  float arr2[] = {1.0f, 2.0f, 3.0f};
  float arr3[] = {1.0f, 2.0f, 4.0f};
  List<float> flist1(arr1, 3);
  List<float> flist2(arr2, 3);
  List<float> flist3(arr3, 3);

  assert(flist1 == flist2);
  assert(flist1 != flist3);
  test_passed("Float comparison operators");

  // Concatenation
  List<float> flist4 = flist1 + flist3;
  assert(flist4.size() == 6);
  assert(flist4[0] == 1.0f);
  assert(flist4[5] == 4.0f);
  test_passed("Float concatenation");

  // += operator
  List<float> flist5;
  flist5.push_back(1.5f);
  flist5.push_back(2.5f);
  flist5 += flist1;
  assert(flist5.size() == 5);
  assert(flist5[0] == 1.5f);
  assert(flist5[4] == 3.0f);
  test_passed("Float += operator");

  // Copy constructor
  List<float> list5(list2);
  assert(list5.size() == list2.size());
  assert(list5 == list2);
  test_passed("Float copy constructor");

  // Assignment operator
  List<float> list6;
  list6 = list2;
  assert(list6 == list2);
  test_passed("Float assignment operator");

  // Swap
  List<float> flist6;
  flist6.push_back(100.0f);
  flist6.push_back(200.0f);

  List<float> flist7;
  flist7.push_back(10.0f);
  flist7.push_back(20.0f);
  flist7.push_back(30.0f);

  flist6.swap(flist7);
  assert(flist6.size() == 3);
  assert(flist6[0] == 10.0f);
  assert(flist7.size() == 2);
  assert(flist7[0] == 100.0f);
  test_passed("Float swap");

  // Clear
  list2.clear();
  assert(list2.empty());
  assert(list2.size() == 0);
  test_passed("Float clear");

  // Push after
  List<float> list7;
  list7.push_back(1.0f);
  list7.push_back(2.0f);
  list7.push_back(3.0f);
  list7.push_after(2.0f, 2.5f);
  assert(list7.size() == 4);
  assert(list7[2] == 2.5f);
  test_passed("Float push_after");

  // Pop after
  list7.pop_after(2.0f);
  assert(list7.size() == 3);
  assert(list7[0] == 1.0f);
  assert(list7[1] == 2.0f);
  assert(list7[2] == 3.0f);
  test_passed("Float pop_after");

  // Negative floats
  List<float> list8;
  list8.push_back(-5.5f);
  list8.push_back(-1.1f);
  list8.push_back(3.3f);
  list8.push_back(-2.2f);

  assert(list8.min() == -5.5f);
  assert(list8.max() == 3.3f);
  test_passed("Float with negative values");

  list8.sort();
  assert(list8[0] == -5.5f);
  assert(list8[1] == -2.2f);
  assert(list8[2] == -1.1f);
  assert(list8[3] == 3.3f);
  test_passed("Float sort with negative values");
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
  std::cout << YELLOW << "║  Linked List Comprehensive Test Suite  ║" << RESET
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
    test_with_chars();
    test_with_floats();

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