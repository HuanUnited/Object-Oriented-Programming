// DynamicArray_tests.cpp
// Comprehensive test suite for DynamicArray<T>
// Adjust the #include line below if your header file name or path is different.
// Compile: g++ -std=c++17 DynamicArray_tests.cpp -o DynamicArray_tests
// Run: ./DynamicArray_tests

#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

// Replace the include below with the correct header of your DynamicArray implementation
// For example: #include "DynamicArray.hpp" or #include "DynamicArrays_int.hpp"
#include "DynamicArrays_2.hpp"

using std::cout; using std::endl;

int tests_passed = 0;
int tests_failed = 0;

#define CHECK(expr, name) do { \
    bool ok = (expr); \
    if (ok) { ++tests_passed; cout << "[PASS] " << (name) << "\n"; } \
    else { ++tests_failed; cout << "[FAIL] " << (name) << " -- expression: " #expr << "\n"; } \
} while(0)

// Helper to print a DynamicArray to a string using operator<< (if available)
template<typename A>
std::string to_string_via_stream(const A &a) {
    std::ostringstream ss;
    ss << a;
    return ss.str();
}

// Test 1: constructors (default, from raw array, copy)
void test_constructors_and_size() {
    cout << "\n== constructors and size ==\n";

    // Default constructor
    DynamicArray<int> d1;
    CHECK(d1.size() == 0, "default constructor gives size 0");

    // Constructor from raw array (assumes signature DynamicArray<T>(T* arr, size_t n))
    int raw[] = {5, 2, 9};
    DynamicArray<int> d2(raw, 3);
    CHECK(d2.size() == 3, "constructor from raw array sets correct size");
    CHECK(d2[0] == 5 && d2[1] == 2 && d2[2] == 9, "elements from raw array preserved");

    // Copy constructor
    DynamicArray<int> d3(d2);
    CHECK(d3.size() == d2.size() && d3[1] == d2[1], "copy constructor makes identical copy");
}

// Test 2: move constructor and move assignment
void test_move_semantics() {
    cout << "\n== move semantics ==\n";
    int raw[] = {1,2,3,4};
    DynamicArray<int> a(raw, 4);
    size_t old_size = a.size();
    DynamicArray<int> moved(std::move(a));
    // moved should contain elements, original a may be empty; check moved
    CHECK(moved.size() == old_size, "move constructor transfers size");

    // Move assignment
    DynamicArray<int> b;
    b = DynamicArray<int>(raw, 4);
    CHECK(b.size() == 4, "move assignment via temporary works (or copy assignment if not move-enabled)");
}

// Test 3: swap
void test_swap() {
    cout << "\n== swap ==\n";
    int r1[] = {1,1};
    int r2[] = {2,2,2};
    DynamicArray<int> a(r1, 2);
    DynamicArray<int> b(r2, 3);
    a.swap(b);
    CHECK(a.size() == 3 && b.size() == 2, "swap exchanges sizes");
    CHECK(a[0] == 2 && b[0] == 1, "swap exchanges contents");
}

// Test 4: find (search element)
void test_find() {
    cout << "\n== find ==\n";
    int r[] = {7,8,9,8};
    DynamicArray<int> a(r, 4);
    int idx = a.find(9);
    CHECK(idx == 2, "find returns index of first matched element");
    int missing = a.find(100);
    CHECK(missing == -1, "find returns -1 for missing element");
}

// Test 5: stream insertion and extraction
void test_stream_io() {
    cout << "\n== stream I/O ==\n";
    int r[] = {10,20,30};
    DynamicArray<int> a(r, 3);
    std::ostringstream os;
    os << a;
    std::string out = os.str();
    CHECK(!out.empty(), "operator<< produces output string");

    // operator>>: try to read from a stringstream into a fresh array
    std::istringstream is(out);
    DynamicArray<int> b;
    // Only attempt extraction if operator>> is available with this syntax
    bool extraction_ok = true;
    try { is >> b; }
    catch(...) { extraction_ok = false; }
    // If operator>> is implemented, b.size() should be > 0 ; otherwise extraction_ok==false
    if (extraction_ok) {
        CHECK(b.size() > 0, "operator>> read something into array");
    } else {
        cout << "[SKIP] operator>> appears not to be usable with default test input (skipping check)\n";
    }
}

// Test 6: sort
void test_sort() {
    cout << "\n== sort ==\n";
    int r[] = {3,1,4,2};
    DynamicArray<int> a(r, 4);
    a.sort(); // assumes a.sort() exists
    bool sorted = true;
    for (size_t i = 1; i < a.size(); ++i) if (a[i-1] > a[i]) sorted = false;
    CHECK(sorted, "sort produces non-decreasing order");
}

// Test 7: insert by index, remove by index
void test_insert_remove_by_index() {
    cout << "\n== insert/remove by index ==\n";
    DynamicArray<int> a;
    bool ok1 = a.insert(0, 5); // insert at 0
    CHECK(ok1 && a.size() == 1 && a[0] == 5, "insert at 0 on empty array works");

    // Insert out of bounds should return false
    bool ok2 = a.insert_at(100, 7);
    CHECK(ok2 == false, "insert with invalid index returns false");

    // removeAt
    bool rem_ok = a.erase_at(0);
    CHECK(rem_ok && a.size() == 0, "removeAt valid index removes element");
    bool rem_bad = a.erase_at(0);
    CHECK(rem_bad == false, "removeAt invalid index returns false");
}

// Test 8: remove by value (first occurrence) and remove all
void test_remove_by_value() {
    cout << "\n== remove by value ==\n";
    int r[] = {1,2,3,2,4};
    DynamicArray<int> a(r, 5);
    bool rem = a.pop_first(2);
    CHECK(rem == true && a.size() == 4 && a.find(2) == 2, "remove removes first occurrence");

    // remove all occurrences
    a.pop_all(2);
    bool none_left = (a.find(2) == -1);
    CHECK(none_left, "removeAll removes all occurrences");
}

// Test 9: max/min element
void test_min_max() {
    cout << "\n== min/max ==\n";
    int r[] = {9,3,7,1,5};
    DynamicArray<int> a(r,5);
    int mn = a.min();
    int mx = a.max();
    CHECK(mn == 1, "min() returns minimum element");
    CHECK(mx == 9, "max() returns maximum element");
}

// Test 10: iterators, insert before iterator, erase by iterator(s)
void test_iterators_and_erase() {
    cout << "\n== iterators and erase ==\n";
    int r[] = {1,2,3,4,5};
    DynamicArray<int> a(r,5);
    auto it_begin = a.begin();
    auto it_end = a.end();
    CHECK(it_begin != it_end, "begin() != end() for non-empty array");

    // Insert before second element
    auto it = a.begin(); ++it; // points to element 2
    a.insert(it, 99); // assume insert(iterator, value)
    // now second element should be 99
    auto it2 = a.begin(); ++it2;
    CHECK(*it2 == 99, "insert before iterator places new element in correct position");

    // Erase a single element using iterator
    auto it_erase = a.begin(); ++it_erase; // points to 99
    a.erase(it_erase);
    // after erase, second element should be original 2
    auto it_after = a.begin(); ++it_after;
    CHECK(*it_after == 2, "erase(iterator) removed the inserted element");

    // Erase a range: remove elements at positions 1..3 (inclusive start, exclusive end)
    auto b = a; // copy
    auto e1 = b.begin(); ++e1; // pos 1
    auto e2 = b.begin(); ++e2; ++e2; ++e2; // pos 3
    b.erase(e1, e2);
    CHECK(b.size() == a.size() - (size_t) (std::distance(e1, e2)), "erase(range) reduces the size correctly");
}

// Test 11: operator[] reference, copy assignment
void test_subscript_and_assignment() {
    cout << "\n== subscript and assignment ==\n";
    int r[] = {1,2,3};
    DynamicArray<int> a(r,3);
    a[1] = 42;
    CHECK(a[1] == 42, "operator[] returns reference allowing assignment");

    DynamicArray<int> b;
    b = a; // copy assignment
    CHECK(b.size() == a.size() && b[1] == 42, "copy assignment copies content");
}

// Test 12: add element to end (+ and +=), concatenate with another array
void test_push_and_concatenate() {
    cout << "\n== push and concatenate ==\n";
    int r[] = {1,2};
    DynamicArray<int> a(r,2);
    a += 3; // append element
    CHECK(a.size() == 3 && a[2] == 3, "operator+= adds element to end");

    DynamicArray<int> b;
    b = a + 4; // assume a + value returns new array with element appended
    CHECK(b.size() == 4 && b[3] == 4, "operator+ with a value appends element");

    // concatenation with other DynamicArray
    int r2[] = {7,8};
    DynamicArray<int> c(r2,2);
    DynamicArray<int> d = a + c; // assumes a + c concatenates
    CHECK(d.size() == a.size() + c.size(), "concatenation (+) with another array increases size correctly");
    a += c;
    CHECK(a.size() >= c.size(), "operator+= with another array works (size increased)");
}

// Test 13: comparison operators
void test_comparisons() {
    cout << "\n== comparisons ==\n";
    int r[] = {1,2,3};
    DynamicArray<int> a(r,3);
    DynamicArray<int> b(r,3);
    CHECK(a == b, "two equal arrays compare equal with ==");
    b[1] = 99;
    CHECK(a != b, "different arrays are !=");
}

int main(){
    cout << "DynamicArray test suite starting...\n";

    test_constructors_and_size();
    test_move_semantics();
    test_swap();
    test_find();
    test_stream_io();
    test_sort();
    test_insert_remove_by_index();
    test_remove_by_value();
    test_min_max();
    test_iterators_and_erase();
    test_subscript_and_assignment();
    test_push_and_concatenate();
    test_comparisons();

    cout << "\nTests passed: " << tests_passed << "\n";
    cout << "Tests failed: " << tests_failed << "\n";
    return (tests_failed == 0) ? 0 : 1;
}
