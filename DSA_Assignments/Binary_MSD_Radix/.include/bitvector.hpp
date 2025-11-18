#pragma once

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>

using byte_t = uint8_t;
using pair = std::pair<size_t, size_t>;
static constexpr size_t BITS_PER_BYTE = CHAR_BIT; // usually 8

class BitVector {
public:
  // Byte to Bit converter
  static size_t bytes_for_bits(size_t bits);

  // Constructors / destructor / assignment
  BitVector() = default;
  BitVector(size_t size, bool value = false);
  explicit BitVector(const char *bitstr);
  BitVector(const BitVector &other);
  BitVector &operator=(const BitVector &other);
  BitVector(BitVector &&) noexcept = default;
  BitVector &operator=(BitVector &&) noexcept = default;
  ~BitVector() = default;

  // position: byte and offset:
  pair coord(size_t i) const;

  // Clean garbage bits and preserve valid bits (valid bit filter)
  byte_t last_byte_mask() const;

  // Range Checker
  void check_index(size_t i) const;

  // swap
  void swap(BitVector &other) noexcept;
  friend void swap(BitVector &a, BitVector &b) noexcept;

  // length
  size_t size() const noexcept;

  // value
  int value() const;

  // BoolRef proxy
  class BoolRef {
    BitVector &bv;
    size_t idx;

  public:
    BoolRef(BitVector &parent, size_t i);
    BoolRef &operator=(bool val);
    BoolRef &operator=(const BoolRef &other);
    operator bool() const;
    BoolRef &flip();
  };

  // accessors & mutators
  bool get(size_t i) const;
  void set(size_t i, bool value);
  void flip(size_t i);
  void flipAll();
  void setRange(size_t i, size_t k, bool value);
  void setAll(bool value);
  size_t weight() const;

  // operator[]
  BoolRef operator[](size_t i);
  bool operator[](size_t i) const;

  // comparisons
  bool operator==(BitVector &rhs) const;

  // bitwise operators
  BitVector operator&(const BitVector &rhs) const;
  BitVector &operator&=(const BitVector &rhs);

  BitVector operator|(const BitVector &rhs) const;
  BitVector &operator|=(const BitVector &rhs);

  BitVector operator^(const BitVector &rhs) const;
  BitVector &operator^=(const BitVector &rhs);

  BitVector operator~() const;

  // shifts
  BitVector operator<<(const size_t off) const;
  BitVector &operator<<=(const size_t off);

  BitVector operator>>(const size_t off) const;
  BitVector &operator>>=(const size_t off);

private:
  std::unique_ptr<byte_t[]> data;
  size_t nbits = 0;
};

// stream operators (non-member)
std::ostream &operator<<(std::ostream &os, const BitVector &bv);
std::istream &operator>>(std::istream &is, BitVector &bv);
