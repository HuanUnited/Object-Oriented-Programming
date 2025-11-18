#include "charset.hpp"
#include <algorithm>
#include <cctype>
#include <sstream>

// --- Constructors ---
Charset::Charset(int low, int high) {
  if (low < 0)
    low = 0;
  if (high > UCHAR_MAX)
    high = UCHAR_MAX;
  if (low > high)
    throw std::invalid_argument("Invalid domain bounds");
  low_u = static_cast<unsigned int>(low);
  high_u = static_cast<unsigned int>(high);
  size_t domain = static_cast<size_t>(high_u - low_u + 1);
  // initialize BitVector with domain bits, all false
  BitVector::operator=(BitVector(domain, false));
}

Charset::Charset(std::initializer_list<char> elems, int low, int high)
    : Charset(low, high) {
  for (char c : elems)
    add(c);
}

Charset::Charset(const Charset &other)
    : BitVector(other), low_u(other.low_u), high_u(other.high_u) {}

// --- Printing / Reading ---
void Charset::print(std::ostream &os) const {
  os << '{';
  bool first = true;
  for (unsigned int v = low_u; v <= high_u; ++v) {
    if (contains(static_cast<char>(v))) {
      if (!first)
        os << ',';
      // print printable characters as themselves, otherwise print numeric value
      unsigned char uc = static_cast<unsigned char>(v);
      if (std::isprint(uc)) {
        os << static_cast<char>(uc);
      } else {
        os << static_cast<int>(uc);
      }
      first = false;
    }
  }
  os << '}';
}

void Charset::read(std::istream &is) {
  // Accept formats like: {a,b,c} or {65,66}
  std::string s;
  if (!(is >> s))
    return;
  if (s.size() < 2 || s.front() != '{' || s.back() != '}') {
    // attempt to read remainder until '}'
    std::string rest;
    std::getline(is, rest);
    s += rest;
  }
  // remove braces
  if (s.front() != '{')
    throw std::runtime_error("Bad input");
  if (s.back() != '}')
    throw std::runtime_error("Bad input");
  std::string inside = s.substr(1, s.size() - 2);
  // split by commas
  std::istringstream ss(inside);
  std::string token;
  // clear current set
  BitVector::setAll(false);
  while (std::getline(ss, token, ',')) {
    if (token.empty())
      continue;
    // trim
    size_t p1 = token.find_first_not_of(" \t\n\r");
    size_t p2 = token.find_last_not_of(" \t\n\r");
    if (p1 == std::string::npos)
      continue;
    std::string t = token.substr(p1, p2 - p1 + 1);
    // if single printable char, take it; otherwise parse int
    if (t.size() == 1) {
      add(t[0]);
    } else {
      int val = std::stoi(t);
      if (val < 0 || val > UCHAR_MAX)
        throw std::out_of_range("Value out of range");
      add(static_cast<char>(val));
    }
  }
}

// --- membership ---
bool Charset::contains(char c) const noexcept {
  unsigned char uc = static_cast<unsigned char>(c);
  if (!inDomain(uc))
    return false;
  return BitVector::get(indexOf(uc));
}

// --- min / max element ---
char Charset::minElement() const {
  for (unsigned int v = low_u; v <= high_u; ++v)
    if (contains(static_cast<char>(v)))
      return static_cast<char>(v);
  throw std::runtime_error("Set is empty");
}

char Charset::maxElement() const {
  for (int v = static_cast<int>(high_u); v >= static_cast<int>(low_u); --v)
    if (contains(static_cast<char>(v)))
      return static_cast<char>(v);
  throw std::runtime_error("Set is empty");
}

// --- add / remove ---
void Charset::add(char c) {
  unsigned char uc = static_cast<unsigned char>(c);
  if (!inDomain(uc))
    throw std::out_of_range("Element outside domain");
  BitVector::set(indexOf(uc), true);
}

void Charset::remove(char c) {
  unsigned char uc = static_cast<unsigned char>(c);
  if (!inDomain(uc))
    return; // removing outside domain is a no-op
  BitVector::set(indexOf(uc), false);
}

// --- merged domain helper ---
void Charset::merged_domain(const Charset &a, const Charset &b, int &out_low,
                            int &out_high) {
  out_low = static_cast<int>(std::min(a.low_u, b.low_u));
  out_high = static_cast<int>(std::max(a.high_u, b.high_u));
}

// --- in-place set operations ---
Charset &Charset::operator|=(const Charset &rhs) {
  int ml, mh;
  merged_domain(*this, rhs, ml, mh);
  Charset out(ml, mh);
  for (unsigned int v = out.low_u; v <= out.high_u; ++v) {
    bool present =
        ((v >= low_u && v <= high_u && contains(static_cast<char>(v))) ||
         (v >= rhs.low_u && v <= rhs.high_u &&
          rhs.contains(static_cast<char>(v))));
    if (present)
      out.add(static_cast<char>(v));
  }
  *this = out;
  return *this;
}

Charset &Charset::operator&=(const Charset &rhs) {
  int ml, mh;
  merged_domain(*this, rhs, ml, mh);
  Charset out(ml, mh);
  for (unsigned int v = out.low_u; v <= out.high_u; ++v) {
    bool present =
        (v >= low_u && v <= high_u && contains(static_cast<char>(v))) &&
        (v >= rhs.low_u && v <= rhs.high_u &&
         rhs.contains(static_cast<char>(v)));
    if (present)
      out.add(static_cast<char>(v));
  }
  *this = out;
  return *this;
}

Charset &Charset::operator/=(const Charset &rhs) {
  // this = this \ rhs
  int ml, mh;
  merged_domain(*this, rhs, ml, mh);
  Charset out(ml, mh);
  for (unsigned int v = out.low_u; v <= out.high_u; ++v) {
    bool present =
        (v >= low_u && v <= high_u && contains(static_cast<char>(v))) &&
        !(v >= rhs.low_u && v <= rhs.high_u &&
          rhs.contains(static_cast<char>(v)));
    if (present)
      out.add(static_cast<char>(v));
  }
  *this = out;
  return *this;
}

Charset &Charset::operator+=(char c) {
  add(c);
  return *this;
}
Charset &Charset::operator-=(char c) {
  remove(c);
  return *this;
}

// --- equality ---
bool Charset::equals(const Charset &other) const noexcept {
  int ml, mh;
  merged_domain(*this, other, ml, mh);
  for (int v = ml; v <= mh; ++v) {
    bool a_has =
        (v >= static_cast<int>(low_u) && v <= static_cast<int>(high_u)) &&
        contains(static_cast<char>(v));
    bool b_has = (v >= static_cast<int>(other.low_u) &&
                  v <= static_cast<int>(other.high_u)) &&
                 other.contains(static_cast<char>(v));
    if (a_has != b_has)
      return false;
  }
  return true;
}

// --- non-member operators ---
Charset operator|(const Charset &a, const Charset &b) {
  Charset out(std::min(a.low(), b.low()), std::max(a.high(), b.high()));
  for (int v = out.low(); v <= out.high(); ++v) {
    if ((v >= a.low() && v <= a.high() && a.contains(static_cast<char>(v))) ||
        (v >= b.low() && v <= b.high() && b.contains(static_cast<char>(v)))) {
      out.add(static_cast<char>(v));
    }
  }
  return out;
}

Charset operator&(const Charset &a, const Charset &b) {
  Charset out(std::min(a.low(), b.low()), std::max(a.high(), b.high()));
  for (int v = out.low(); v <= out.high(); ++v) {
    if ((v >= a.low() && v <= a.high() && a.contains(static_cast<char>(v))) &&
        (v >= b.low() && v <= b.high() && b.contains(static_cast<char>(v)))) {
      out.add(static_cast<char>(v));
    }
  }
  return out;
}

Charset operator/(const Charset &a, const Charset &b) {
  Charset out(std::min(a.low(), b.low()), std::max(a.high(), b.high()));
  for (int v = out.low(); v <= out.high(); ++v) {
    if ((v >= a.low() && v <= a.high() && a.contains(static_cast<char>(v))) &&
        !(v >= b.low() && v <= b.high() && b.contains(static_cast<char>(v)))) {
      out.add(static_cast<char>(v));
    }
  }
  return out;
}

Charset operator~(const Charset &a) {
  Charset out(a.low(), a.high());
  for (int v = out.low(); v <= out.high(); ++v) {
    if (!a.contains(static_cast<char>(v)))
      out.add(static_cast<char>(v));
  }
  return out;
}

Charset operator+(const Charset &a, char c) {
  Charset out = a;
  out.add(c);
  return out;
}
Charset operator-(const Charset &a, char c) {
  Charset out = a;
  out.remove(c);
  return out;
}

std::ostream &operator<<(std::ostream &os, const Charset &cs) {
  cs.print(os);
  return os;
}

std::istream &operator>>(std::istream &is, Charset &cs) {
  cs.read(is);
  return is;
}
