#include "charset.hpp"
#include <cstring>
#include <limits>
#include <stdexcept>

// Default constructor - creates empty set with capacity for all 256 char values
CharacterSet::CharacterSet() : BitVector(256, false) {}

// Constructor from C-string - adds all unique characters from string to set
CharacterSet::CharacterSet(const char *str) : BitVector(256, false) {
  if (!str) {
    throw std::invalid_argument("Null string pointer");
  }

  size_t length = std::strlen(str);
  for (size_t i = 0; i < length; ++i) {
    uint8_t index = static_cast<uint8_t>(str[i]);
    set(index, true);
  }
}

// Copy constructor
CharacterSet::CharacterSet(const CharacterSet &other) : BitVector(other) {}

// Get cardinality (number of elements in set)
uint32_t CharacterSet::getCardinality() const {
  return static_cast<uint32_t>(weight());
}

// Check if element exists in set
bool CharacterSet::contains(char element) const {
  uint8_t index = static_cast<uint8_t>(element);
  return get(index);
}

// Find maximum element in set
char CharacterSet::getMax() const {
  if (getCardinality() == 0) {
    throw std::logic_error("Set is empty");
  }

  for (int i = 255; i >= 0; --i) {
    if (get(static_cast<uint8_t>(i))) {
      return static_cast<char>(i);
    }
  }
  throw std::logic_error("Set is empty");
}

// Find minimum element in set
char CharacterSet::getMin() const {
  if (getCardinality() == 0) {
    throw std::logic_error("Set is empty");
  }

  for (uint8_t i = 0; i < 256; ++i) {
    if (get(i)) {
      return static_cast<char>(i);
    }
  }
  throw std::logic_error("Set is empty");
}

// Operator[] - non-const version
BitVector::BoolRef CharacterSet::operator[](uint8_t index) {
  return BitVector::operator[](index);
}

// Operator[] - const version
bool CharacterSet::operator[](uint8_t index) const {
  return BitVector::operator[](index);
}

// Assignment operator
CharacterSet &CharacterSet::operator=(const CharacterSet &other) {
  if (this != &other) {
    BitVector::operator=(other);
  }
  return *this;
}

// Equality comparison
bool CharacterSet::operator==(const CharacterSet &rhs) const {
  for (size_t i = 0; i < 256; ++i) {
    if (get(i) != rhs.get(i)) {
      return false;
    }
  }
  return true;
}

// Inequality comparison
bool CharacterSet::operator!=(const CharacterSet &rhs) const {
  return !(*this == rhs);
}

// Set union
CharacterSet CharacterSet::operator|(const CharacterSet &rhs) const {
  CharacterSet result;
  for (size_t i = 0; i < 256; ++i) {
    result.set(i, get(i) || rhs.get(i));
  }
  return result;
}

// Set union assignment
CharacterSet &CharacterSet::operator|=(const CharacterSet &rhs) {
  for (size_t i = 0; i < 256; ++i) {
    if (rhs.get(i)) {
      set(i, true);
    }
  }
  return *this;
}

// Set intersection
CharacterSet CharacterSet::operator&(const CharacterSet &rhs) const {
  CharacterSet result;
  for (size_t i = 0; i < 256; ++i) {
    result.set(i, get(i) && rhs.get(i));
  }
  return result;
}

// Set intersection assignment
CharacterSet &CharacterSet::operator&=(const CharacterSet &rhs) {
  for (size_t i = 0; i < 256; ++i) {
    if (!rhs.get(i)) {
      set(i, false);
    }
  }
  return *this;
}

// Set difference (elements in this but not in rhs)
CharacterSet CharacterSet::operator/(const CharacterSet &rhs) const {
  CharacterSet result;
  for (size_t i = 0; i < 256; ++i) {
    result.set(i, get(i) && !rhs.get(i));
  }
  return result;
}

// Set difference assignment
CharacterSet &CharacterSet::operator/=(const CharacterSet &rhs) {
  for (size_t i = 0; i < 256; ++i) {
    if (rhs.get(i)) {
      set(i, false);
    }
  }
  return *this;
}

// Complement (all elements not in set)
CharacterSet CharacterSet::operator~() const {
  CharacterSet result;
  for (size_t i = 0; i < 256; ++i) {
    result.set(i, !get(i));
  }
  return result;
}

// Add element to set
CharacterSet CharacterSet::operator+(char element) const {
  CharacterSet result(*this);
  uint8_t index = static_cast<uint8_t>(element);
  result.set(index, true);
  return result;
}

// Add element to set (in-place)
CharacterSet &CharacterSet::operator+=(char element) {
  uint8_t index = static_cast<uint8_t>(element);
  set(index, true);
  return *this;
}

// Remove element from set
CharacterSet CharacterSet::operator-(char element) const {
  CharacterSet result(*this);
  uint8_t index = static_cast<uint8_t>(element);
  result.set(index, false);
  return result;
}

// Remove element from set (in-place)
CharacterSet &CharacterSet::operator-=(char element) {
  uint8_t index = static_cast<uint8_t>(element);
  set(index, false);
  return *this;
}

// Virtual print method for polymorphic output
void CharacterSet::print(std::ostream &os) const {
  os << "{";
  bool first = true;
  for (uint32_t i = 0; i < 256; ++i) {
    if (get(i)) {
      if (!first) {
        os << ", ";
      }

      char ch = static_cast<char>(i);
      // Print printable characters as-is, others as numeric codes
      if (ch >= 32 && ch <= 126) {
        os << "'" << ch << "'";
      } else {
        os << static_cast<int>(i);
      }
      first = false;
    }
  }
  os << "}";
}

// Polymorphic print
void CharacterSet::print2() const {
  std::cout << "{";
  bool first = true;
  for (uint32_t i = 0; i < 256; ++i) {
    if (get(i)) {
      if (!first) {
        std::cout << ", ";
      }

      char ch = static_cast<char>(i);
      // Print printable characters as-is, others as numeric codes
      if (ch >= 32 && ch <= 126) {
        std::cout << "'" << ch << "'";
      } else {
        std::cout << static_cast<int>(i);
      }
      first = false;
    }
  }
  std::cout << "}";
}

void CharacterSet::scan() {
  CharacterSet out;
  std::cin >> out;
  *this = out;
}

// Stream output operator
std::ostream &operator<<(std::ostream &os, const CharacterSet &cs) {
  cs.print(os);
  return os;
}

// Stream input operator - reads characters until whitespace
std::istream &operator>>(std::istream &is, CharacterSet &cs) {
  std::string str;
  if (!(is >> str)) {
    return is;
  }

  cs = CharacterSet(); // Clear the set
  for (char ch : str) {
    cs += ch;
  }

  return is;
}