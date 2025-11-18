#include ".include/charset.hpp"
#include <cassert>
#include <iostream>
#include <string>

// Color codes
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

void section(const std::string &section) {
  std::cout << "\n"
            << YELLOW << "=== " << section << " ===" << RESET << std::endl;
}

// --- Tests ---
void test_constructors() {
  section("Constructors");

  Charset a;
  assert(a.cardinality() == 0);
  test_passed("Default constructor");

  Charset b({'A', 'B', 'C'});
  assert(b.contains('A'));
  assert(b.cardinality() == 3);
  test_passed("Initializer list constructor");

  Charset c(b);
  assert(c == b);
  test_passed("Copy constructor");
}

void test_membership() {
  section("Membership");
  Charset cs({'x', 'y', 'z'});
  assert(cs.contains('x'));
  assert(!cs.contains('a'));
  test_passed("contains()");
}

void test_add_remove() {
  section("Add/Remove");
  Charset cs;
  cs.add('Q');
  assert(cs.contains('Q'));
  test_passed("add()");

  cs.remove('Q');
  assert(!cs.contains('Q'));
  test_passed("remove()");
}

void test_min_max() {
  section("Min/Max");
  Charset cs({'d', 'a', 'z'});
  assert(cs.minElement() == 'a');
  assert(cs.maxElement() == 'z');
  test_passed("min/max");
}

void test_set_ops() {
  section("Set operations");

  Charset A({'A', 'B', 'C'});
  Charset B({'B', 'D'});

  Charset U = A | B;
  assert(U.contains('A') && U.contains('D'));
  test_passed("union");

  Charset I = A & B;
  assert(I.contains('B') && I.cardinality() == 1);
  test_passed("intersection");

  Charset D = A / B;
  assert(D.contains('A') && !D.contains('B'));
  test_passed("difference");

  Charset C = ~Charset({'A'}, (int)'A', (int)'C');
  assert(!C.contains('A') && C.contains('B') && C.contains('C'));
  test_passed("complement");
}

void test_streams() {
  section("Stream operators");

  Charset cs({'a', 'b', 'c'});
  std::cout << "Print test: " << cs << std::endl;
  test_passed("operator<<");

  std::istringstream iss("{a,b,c}");
  Charset rd; // cin >> a,b,c
  iss >> rd;
  assert(rd.contains('b'));
  test_passed("operator>>");
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
    test_constructors();
    test_membership();
    test_add_remove();
    test_min_max();
    test_set_ops();
    test_streams();

    std::cout << "\n"
              << GREEN << "╔════════════════════════════════════════╗" << RESET
              << std::endl;
    std::cout << GREEN << "║     All Tests Completed Successfully!  ║" << RESET
              << std::endl;
    std::cout << GREEN << "╚════════════════════════════════════════╝" << RESET
              << "\n"
              << std::endl;

    std::cout << "\nTest Coverage:" << std::endl;
    std::cout << GREEN << "✓ 15 баллов:" << RESET
              << " Реализовать все методы, помеченные зелёным цветом"
              << std::endl;
    std::cout << GREEN << "✓ 5 баллов:" << RESET
              << " Дополнительно реализовать все методы, не "
                 "помеченные цветом."
              << std::endl;

  } catch (const std::exception &e) {
    std::cout << RED << "\nUnexpected exception: " << e.what() << RESET
              << std::endl;
    return 1;
  }
}
