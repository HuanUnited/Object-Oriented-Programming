#pragma once

#include "bitvector.hpp"
#include <cstdint>
#include <iostream>

class CharacterSet : public BitVector {
public:
  // Constructors
  CharacterSet();
  CharacterSet(const char *str);
  CharacterSet(const CharacterSet &other);

  // Destructor
  ~CharacterSet() override = default;

  // Get cardinality (number of elements in set)
  uint32_t getCardinality() const;

  // Check if element exists in set
  bool contains(char element) const;

  // Find max/min element
  char getMax() const;
  char getMin() const;

  // Operator[] overloads
  BoolRef operator[](uint8_t index);
  bool operator[](uint8_t index) const;

  // Assignment operator
  CharacterSet &operator=(const CharacterSet &other);

  // Comparison operators
  bool operator==(const CharacterSet &rhs) const;
  bool operator!=(const CharacterSet &rhs) const;

  // Set union operators
  CharacterSet operator|(const CharacterSet &rhs) const;
  CharacterSet &operator|=(const CharacterSet &rhs);

  // Set intersection operators
  CharacterSet operator&(const CharacterSet &rhs) const;
  CharacterSet &operator&=(const CharacterSet &rhs);

  // Set difference operators
  CharacterSet operator/(const CharacterSet &rhs) const;
  CharacterSet &operator/=(const CharacterSet &rhs);

  // Complement operator
  CharacterSet operator~() const;

  // Add element operators
  CharacterSet operator+(char element) const;
  CharacterSet &operator+=(char element);

  // Remove element operators
  CharacterSet operator-(char element) const;
  CharacterSet &operator-=(char element);

  // Virtual print method for polymorphic output
  virtual void print(std::ostream &os) const;
};

// Stream operators (non-member)
std::ostream &operator<<(std::ostream &os, const CharacterSet &cs);
std::istream &operator>>(std::istream &is, CharacterSet &cs);