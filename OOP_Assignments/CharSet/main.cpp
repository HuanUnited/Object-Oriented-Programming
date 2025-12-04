#include "./.include/charset.hpp"
#include "bitvector.hpp"
#include <iostream>
#include <ostream>

int main() {
  std::cout << "=== CharacterSet Testing ===" << std::endl << std::endl;

  // Test 1: Default constructor and adding elements
  std::cout << "Test 1: Default constructor and adding elements" << std::endl;
  CharacterSet set1;
  set1 += 'a';
  set1 += 'b';
  set1 += 'c';
  std::cout << "set1: " << set1 << std::endl;
  std::cout << "Cardinality: " << set1.getCardinality() << std::endl;
  std::cout << std::endl;

  // Test 2: String constructor
  std::cout << "Test 2: String constructor" << std::endl;
  CharacterSet set2("hello");
  std::cout << "set2 (from \"hello\"): " << set2 << std::endl;
  std::cout << "Cardinality: " << set2.getCardinality() << std::endl;
  std::cout << std::endl;

  // Test 3: Contains check
  std::cout << "Test 3: Contains check" << std::endl;
  std::cout << "set2 contains 'h': " << (set2.contains('h') ? "yes" : "no")
            << std::endl;
  std::cout << "set2 contains 'x': " << (set2.contains('x') ? "yes" : "no")
            << std::endl;
  std::cout << std::endl;

  // Test 4: Min/Max
  std::cout << "Test 4: Min/Max elements" << std::endl;
  std::cout << "set2 min: '" << set2.getMin() << "'" << std::endl;
  std::cout << "set2 max: '" << set2.getMax() << "'" << std::endl;
  std::cout << std::endl;

  // Test 5: Union
  std::cout << "Test 5: Union operation" << std::endl;
  CharacterSet set3("world");
  std::cout << "set2: " << set2 << std::endl;
  std::cout << "set3 (from \"world\"): " << set3 << std::endl;
  CharacterSet unionSet = set2 | set3;
  std::cout << "set2 | set3: " << unionSet << std::endl;
  std::cout << std::endl;

  // Test 6: Intersection
  std::cout << "Test 6: Intersection operation" << std::endl;
  CharacterSet intersectionSet = set2 & set3;
  std::cout << "set2 & set3: " << intersectionSet << std::endl;
  std::cout << std::endl;

  // Test 7: Difference
  std::cout << "Test 7: Difference operation" << std::endl;
  CharacterSet diffSet = set2 / set3;
  std::cout << "set2 / set3: " << diffSet << std::endl;
  std::cout << std::endl;

  // Test 8: Add and remove elements
  std::cout << "Test 8: Add and remove elements" << std::endl;
  CharacterSet set4("abc");
  std::cout << "set4: " << set4 << std::endl;
  CharacterSet set5 = set4 + 'd';
  std::cout << "set4 + 'd': " << set5 << std::endl;
  CharacterSet set6 = set5 - 'a';
  std::cout << "set5 - 'a': " << set6 << std::endl;
  std::cout << std::endl;

  // Test 9: Complement
  std::cout << "Test 9: Complement (showing first 10 elements)" << std::endl;
  CharacterSet smallSet;
  smallSet += 'a';
  smallSet += 'b';
  std::cout << "smallSet: " << smallSet << std::endl;
  CharacterSet complement = ~smallSet;
  std::cout << "Complement cardinality: " << complement.getCardinality()
            << std::endl;
  std::cout << "Does complement contain 'z': "
            << (complement.contains('z') ? "yes" : "no") << std::endl;
  std::cout << "Does complement contain 'a': "
            << (complement.contains('a') ? "yes" : "no") << std::endl;
  std::cout << std::endl;

  // Test 10: Comparison
  std::cout << "Test 10: Comparison operators" << std::endl;
  CharacterSet set7("abc");
  CharacterSet set8("abc");
  CharacterSet set9("xyz");
  std::cout << "set7: " << set7 << std::endl;
  std::cout << "set8: " << set8 << std::endl;
  std::cout << "set9: " << set9 << std::endl;
  std::cout << "set7 == set8: " << (set7 == set8 ? "yes" : "no") << std::endl;
  std::cout << "set7 == set9: " << (set7 == set9 ? "yes" : "no") << std::endl;
  std::cout << "set7 != set9: " << (set7 != set9 ? "yes" : "no") << std::endl;
  std::cout << std::endl;

  // Test 11: Copy constructor
  std::cout << "Test 11: Copy constructor" << std::endl;
  CharacterSet set10(set2);
  std::cout << "set10 (copy of set2): " << set10 << std::endl;
  std::cout << "set10 == set2: " << (set10 == set2 ? "yes" : "no") << std::endl;
  std::cout << std::endl;

  // Test 12: Print + Scan
  // BitVector printvector(8, false);
  // CharacterSet printset("abfjdishjsfd");
  std::cout << "Test 12: Print + Scan" << std::endl;
  BitVector printvector;
  CharacterSet printset;

  BitVector *ptr = &printvector;

  // 000000000
  std::cout << "Insert BitVector: ";
  ptr->scan();
  ptr->print2();
  std::cout << std::endl;

  ptr = &printset;

  // dadskadjlajdklsa
  std::cout << "Insert CharacterSet: ";
  ptr->scan();
  ptr->print2();
  std::cout << std::endl;

  std::cout << "=== All tests completed ===" << std::endl;

  return 0;
}