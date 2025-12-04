#include "bitvector.hpp"

#include <algorithm> // std::max, std::fill_n
#include <bit>
#include <bitset>
#include <climits>
#include <cstddef>
#include <cstring> // std::memcpy, std::strlen
#include <iostream>
#include <stdexcept>
#include <string>

// -- static helper --
size_t BitVector::bytes_for_bits(size_t bits) {
  return (bits + BITS_PER_BYTE - 1) / BITS_PER_BYTE;
}

// -- last_byte_mask --
byte_t BitVector::last_byte_mask() const {
  size_t rem = nbits % BITS_PER_BYTE;
  if (rem == 0)
    return static_cast<byte_t>(~byte_t(0));
  return static_cast<byte_t>((1u << rem) - 1u);
}

// -- check_index --
void BitVector::check_index(size_t i) const {
  if (i >= nbits)
    throw std::out_of_range("Index");
}

// -- coord --
pair BitVector::coord(size_t i) const {
  size_t b = i / BITS_PER_BYTE;
  size_t off = i % BITS_PER_BYTE;
  return {b, off};
}

// -- constructors / assignment --

BitVector::BitVector(size_t size, bool value) : nbits(size) {
  size_t nbytes = bytes_for_bits(size);
  if (nbytes) {
    data = std::make_unique<byte_t[]>(nbytes);
    std::fill_n(data.get(), nbytes, value ? byte_t(0xFF) : byte_t(0x00));
    if (value && (nbits % BITS_PER_BYTE != 0))
      data[nbytes - 1] &= last_byte_mask();
  }
}

BitVector::BitVector(const char *bitstr) {
  if (!bitstr)
    throw std::invalid_argument("Null string");
  nbits = std::strlen(bitstr);
  size_t nbytes = bytes_for_bits(nbits);
  if (nbytes) {
    data = std::make_unique<byte_t[]>(nbytes);
    std::fill_n(data.get(), nbytes, byte_t(0x00));
    for (size_t i = 0; i < nbits; ++i) {
      char c = bitstr[i];
      if (c != '0' && c != '1')
        throw std::invalid_argument("Bit string must be '0' or '1'");
      if (c == '1')
        data[i / BITS_PER_BYTE] |=
            static_cast<byte_t>(1u << (i % BITS_PER_BYTE));
    }
  }
}

BitVector::BitVector(const BitVector &other) : nbits(other.nbits) {
  size_t nbytes = bytes_for_bits(nbits);
  if (nbytes) {
    data = std::make_unique<byte_t[]>(nbytes);
    std::memcpy(data.get(), other.data.get(), nbytes);
  }
}

BitVector &BitVector::operator=(const BitVector &other) {
  if (this == &other)
    return *this;
  size_t nbytes = bytes_for_bits(other.nbits);
  if (nbytes) {
    auto tmp = std::unique_ptr<byte_t[]>(new byte_t[nbytes]);
    std::memcpy(tmp.get(), other.data.get(), nbytes);
    data.swap(tmp);
  } else {
    data.reset();
  }
  nbits = other.nbits;
  return *this;
}

// -- swap --
void BitVector::swap(BitVector &other) noexcept {
  data.swap(other.data);
  std::swap(nbits, other.nbits);
}
void swap(BitVector &a, BitVector &b) noexcept { a.swap(b); }

// -- size --
size_t BitVector::size() const noexcept { return nbits; }

// -- BoolRef definitions --
BitVector::BoolRef::BoolRef(BitVector &parent, size_t i) : bv(parent), idx(i) {}

BitVector::BoolRef &BitVector::BoolRef::operator=(bool val) {
  bv.set(idx, val);
  return *this;
}

BitVector::BoolRef &BitVector::BoolRef::operator=(const BoolRef &other) {
  return (*this = static_cast<bool>(other));
}

BitVector::BoolRef::operator bool() const { return bv.get(idx); }

BitVector::BoolRef &BitVector::BoolRef::flip() {
  bv.flip(idx);
  return *this;
}

// -- value --
int BitVector::value() const {
  int val = 0;
  for (size_t i = 0; i < nbits; ++i) {
    if (get(i))
      val |= (1 << (nbits - 1 - i));
  }
  return val;
}

// -- get / set / flip --
bool BitVector::get(size_t i) const {
  check_index(i);
  pair position = coord(i);
  return (data[position.first] >> position.second) & 1u;
}

void BitVector::set(size_t i, bool value) {
  check_index(i);
  pair b = coord(i);
  byte_t mask = static_cast<byte_t>(1u << b.second);
  if (value)
    data[b.first] |= mask;
  else
    data[b.first] &= static_cast<byte_t>(~mask);
}

void BitVector::flip(size_t i) {
  check_index(i);
  pair b = coord(i);
  data[b.first] ^= static_cast<byte_t>(1u << b.second);
}

void BitVector::flipAll() {
  size_t nbytes = bytes_for_bits(nbits);
  for (size_t i = 0; i < nbytes; ++i)
    data[i] = static_cast<byte_t>(~data[i]);
  if (nbytes && (nbits % BITS_PER_BYTE != 0))
    data[nbytes - 1] &= last_byte_mask();
}

// -- ranges and setAll --
void BitVector::setRange(size_t i, size_t k, bool value) {
  if (k == 0)
    return;
  if (i + k > nbits)
    throw std::out_of_range("Range out of bounds");
  for (size_t pos = i; pos < i + k; ++pos)
    set(pos, value);
}

void BitVector::setAll(bool value) {
  size_t nbytes = bytes_for_bits(nbits);
  if (nbytes == 0)
    return;
  for (size_t pos = 0; pos < nbits; ++pos)
    set(pos, value);
  if (value && (nbits % BITS_PER_BYTE != 0))
    data[nbytes - 1] &= last_byte_mask();
}

// -- weight --
size_t BitVector::weight() const {
  size_t cnt = 0;
  for (size_t i = 0; i < nbits; ++i) {
    if (get(i))
      ++cnt;
  }
  return cnt;
}

// -- operator[] --
BitVector::BoolRef BitVector::operator[](size_t i) {
  check_index(i);
  return BoolRef(*this, i);
}

bool BitVector::operator[](size_t i) const { return get(i); }

// -- comparison --
bool BitVector::operator==(BitVector &rhs) const {
  if (nbits != rhs.nbits) {
    return false;
  }
  for (size_t i = 0; i < nbits; i++) {
    if (get(i) != rhs.get(i))
      return false;
  }
  return true;
}

// -- bitwise ops --
BitVector BitVector::operator&(const BitVector &rhs) const {
  BitVector out(*this);
  size_t nbytes = bytes_for_bits(std::max(nbits, rhs.nbits));
  for (size_t i = 0; i < nbytes; ++i)
    out.data[i] &= rhs.data[i];
  if (nbytes && (std::max(nbits, rhs.nbits) % BITS_PER_BYTE != 0))
    out.data[nbytes - 1] &= out.last_byte_mask();
  return out;
}
BitVector &BitVector::operator&=(const BitVector &rhs) {
  *this = *this & rhs;
  return *this;
}

BitVector BitVector::operator|(const BitVector &rhs) const {
  BitVector out(*this);
  size_t nbytes = bytes_for_bits(std::max(nbits, rhs.nbits));
  for (size_t i = 0; i < nbytes; ++i)
    out.data[i] |= rhs.data[i];
  if (nbytes && (std::max(nbits, rhs.nbits) % BITS_PER_BYTE != 0))
    out.data[nbytes - 1] &= out.last_byte_mask();
  return out;
}
BitVector &BitVector::operator|=(const BitVector &rhs) {
  *this = *this | rhs;
  return *this;
}

BitVector BitVector::operator^(const BitVector &rhs) const {
  BitVector out(*this);
  size_t nbytes = bytes_for_bits(std::max(nbits, rhs.nbits));
  for (size_t i = 0; i < nbytes; ++i)
    out.data[i] ^= rhs.data[i];
  if (nbytes && (std::max(nbits, rhs.nbits) % BITS_PER_BYTE != 0))
    out.data[nbytes - 1] &= out.last_byte_mask();
  return out;
}
BitVector &BitVector::operator^=(const BitVector &rhs) {
  *this = *this ^ rhs;
  return *this;
}

// bitwise NOT
BitVector BitVector::operator~() const {
  BitVector out(*this);
  size_t nbytes = bytes_for_bits(nbits);
  for (size_t i = 0; i < nbytes; ++i)
    out.data[i] = static_cast<byte_t>(~out.data[i]);
  if (nbytes && (nbits % BITS_PER_BYTE != 0))
    out.data[nbytes - 1] &= out.last_byte_mask();
  return out;
}

// shifts
BitVector BitVector::operator<<(const size_t off) const {
  if (off >= nbits)
    return BitVector(nbits, false);

  BitVector out(nbits, false);
  for (size_t i = 0; i < nbits - off; ++i) {
    if (get(i + off)) {
      out.set(i, true);
    }
  }
  return out;
}
BitVector &BitVector::operator<<=(const size_t off) {
  *this = *this << off;
  return *this;
}

BitVector BitVector::operator>>(const size_t off) const {
  if (off >= nbits)
    return BitVector(nbits, false);

  BitVector out(nbits, false);
  for (size_t i = 0; i < nbits - off; ++i) {
    if (get(i)) {
      out.set(i + off, true);
    }
  }
  return out;
}

BitVector &BitVector::operator>>=(const size_t off) {
  *this = *this >> off;
  return *this;
}

// Virtual print method for polymorphic output
void BitVector::print2() const {
  std::cout << "[";
  for (size_t i = 0; i < size(); ++i)
    std::cout << (get(i) ? '1' : '0');
  std::cout << "]";
}

// -- stream operators --
std::ostream &operator<<(std::ostream &os, const BitVector &bv) {
  for (size_t i = 0; i < bv.size(); ++i)
    os << (bv[i] ? '1' : '0');
  return os;
}

std::istream &operator>>(std::istream &is, BitVector &bv) {
  std::string s;
  if (!(is >> s))
    return is;

  for (char c : s)
    if (c != '0' && c != '1') {
      is.setstate(std::ios::failbit);
      return is;
    }

  bv = BitVector(s.size());
  for (size_t i = 0; i < s.size(); ++i)
    if (s[i] == '1')
      bv.set(i, true);

  return is;
}
