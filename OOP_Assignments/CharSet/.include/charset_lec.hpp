
#include <climits>
#include <iostream>
#include <ostream>

#include "bitvector.hpp"

class CharSet : protected BitVector {
public:
  CharSet() : BitVector(UCHAR_MAX, false);

  CharSet(const char *mas);
  CharSet(CharSet &other);

  ~CharSet();

  virtual void print(std::ostream &os) const;
  virtual void read(std::istream &is);

  bool contains(char element);

  char min() const;
  char max() const;

  void add(const char c);
  void remove(const char c);

  bool equals(CharSet &other);

  int power() const;

  size_t cardinality() const noexcept { return weight(); }

  CharSet operator=(CharSet &other);

  CharSet operator-=(CharSet &);
  CharSet operator+=(CharSet &);
  CharSet operator/=(CharSet &);
  CharSet operator&=(CharSet &);
  CharSet operator|=(CharSet &);
};

std::ostream &operator<<(std::ostream &os, const Charset &cs);
std::istream &operator>>(std::istream &is, Charset &cs);

inline bool operator==(const Charset &a, const Charset &b) {
  return a.equals(b);
}
inline bool operator!=(const Charset &a, const Charset &b) {
  return !a.equals(b);
}

CharSet operator|(CharSet &);
CharSet operator&(CharSet &);
CharSet operator/(CharSet &);
CharSet operator~(CharSet &);

CharSet operator+(CharSet &);
CharSet operator-(CharSet &);