#pragma once

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>

#include "DynamicArrays_2.hpp"
#include "bitvector.hpp"

class BitMatrix {
private:
  DynamicArray<BitVector> m_matrix;

  // Helper to check row index
  void check_row_index(size_t i) const;

  // Helper for row-wise bitwise operations
  BitMatrix row_wise_op(const BitMatrix &rhs, char op) const;

public:
  // --- Constructors / Destructor / Assignment ---
  BitMatrix(); // default
  BitMatrix(const size_t rows, const size_t columns, bool value = false);
  explicit BitMatrix(char **bitstr, const size_t rows);
  BitMatrix(const BitMatrix &other); // copy ctor
  ~BitMatrix();                      // dtor

  BitMatrix &operator=(const BitMatrix &other);          // copy assignment
  BitMatrix(BitMatrix &&) noexcept = default;            // move ctor
  BitMatrix &operator=(BitMatrix &&) noexcept = default; // move assign

  // --- Capacity / Access ---
  size_t rows() const noexcept;
  size_t columns() const noexcept;

  void swap(BitMatrix &other) noexcept;
  friend void swap(BitMatrix &a, BitMatrix &b) noexcept;

  BitVector &operator[](size_t i);
  const BitVector &operator[](size_t i) const;

  // --- Operations ---
  void set(size_t j, size_t i, bool value);
  void flip(size_t j, size_t i);
  size_t row_weight(size_t j) const;

  size_t weight() const;
  BitVector conjunction_rows() const;
  BitVector disjunction_rows() const;

  void flip_range(size_t j, size_t i, size_t k);
  void set_range(size_t j, size_t i, size_t k, bool value);

  // --- Overloads: Bitwise (Row-wise) ---
  BitMatrix operator&(const BitMatrix &rhs) const;
  BitMatrix &operator&=(const BitMatrix &rhs);

  BitMatrix operator|(const BitMatrix &rhs) const;
  BitMatrix &operator|=(const BitMatrix &rhs);

  BitMatrix operator^(const BitMatrix &rhs) const;
  BitMatrix &operator^=(const BitMatrix &rhs);

  BitMatrix operator~() const;

  // Equality
  bool operator==(const BitMatrix &other) const;
  bool operator!=(const BitMatrix &other) const;
};

// Streaming I/O
std::ostream &operator<<(std::ostream &os, const BitMatrix &bm);
std::istream &operator>>(std::istream &is, BitMatrix &bm);
