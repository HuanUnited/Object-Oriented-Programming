#pragma once

#include "bitvector.hpp"
#include <climits>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// Charset: a set of `char` values backed by BitVector
// Domain of allowed values is [low_u, high_u] where low_u/high_u are
// unsigned char values (0..UCHAR_MAX). Internally maps value v -> index (v -
// low_u).

class Charset : public BitVector {
public:
  // low/high are integer values in range [0, UCHAR_MAX]
  explicit Charset(int low = 0, int high = UCHAR_MAX);

  // construct from initializer list of chars (and optional domain)
  Charset(std::initializer_list<char> elems, int low = 0, int high = UCHAR_MAX);

  // copy
  Charset(const Charset &other);

  // default move/dtor/assign are fine
  Charset &operator=(const Charset &other) = default;

  ~Charset() = default;

  // polymorphic print/read
  virtual void print(std::ostream &os) const;
  virtual void read(std::istream &is);

  // stream operators (non-member friends declared below)

  // membership
  bool contains(char c) const noexcept;

  // cardinality
  size_t cardinality() const noexcept { return weight(); }

  // min/max element (throws runtime_error if empty)
  char minElement() const;
  char maxElement() const;

  // add / remove single element
  void add(char c);
  void remove(char c);

  // in-place set operations
  Charset &operator|=(const Charset &rhs);
  Charset &operator&=(const Charset &rhs);
  Charset &operator/=(const Charset &rhs); // difference (this = this \ rhs)
  Charset &operator+=(char c);             // add element
  Charset &operator-=(char c);             // remove element

  // equality
  bool equals(const Charset &other) const noexcept;

  // helpers
  int low() const noexcept { return low_u; }
  int high() const noexcept { return high_u; }
  size_t domainSize() const noexcept { return size(); }

private:
  unsigned int low_u = 0;
  unsigned int high_u = UCHAR_MAX;

  // map char to index in BitVector (0..domainSize()-1)
  inline bool inDomain(unsigned char uc) const noexcept {
    return uc >= low_u && uc <= high_u;
  }
  inline size_t indexOf(unsigned char uc) const noexcept {
    return static_cast<size_t>(uc - low_u);
  }

  // create a Charset with merged domain covering both operands
  static void merged_domain(const Charset &a, const Charset &b, int &out_low,
                            int &out_high);
};

// non-member operators & friends
std::ostream &operator<<(std::ostream &os, const Charset &cs);
std::istream &operator>>(std::istream &is, Charset &cs);

Charset operator|(const Charset &a, const Charset &b);
Charset operator&(const Charset &a, const Charset &b);
Charset operator/(const Charset &a, const Charset &b);
Charset operator~(const Charset &a);

Charset operator+(const Charset &a, char c);
Charset operator-(const Charset &a, char c);

inline bool operator==(const Charset &a, const Charset &b) {
  return a.equals(b);
}
inline bool operator!=(const Charset &a, const Charset &b) {
  return !a.equals(b);
}