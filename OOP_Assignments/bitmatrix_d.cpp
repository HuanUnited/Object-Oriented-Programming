// BitMatrix.cpp (or continuation of single-file)
#include "bitmatrix_o.hpp"

// --- Private helpers ---

void BitMatrix::check_row_index(size_t i) const {
  if (i >= m_matrix.size())
    throw std::out_of_range("Row index out of bounds");
}

BitMatrix BitMatrix::row_wise_op(const BitMatrix &rhs, char op) const {
  if (m_matrix.size() != rhs.m_matrix.size() || columns() != rhs.columns()) {
    throw std::invalid_argument(
        "Matrices must have the same dimensions for bitwise operation");
  }
  BitMatrix result(*this); // Start with a copy
  for (size_t i = 0; i < m_matrix.size(); ++i) {
    switch (op) {
    case '&':
      result.m_matrix[i] &= rhs.m_matrix[i];
      break;
    case '|':
      result.m_matrix[i] |= rhs.m_matrix[i];
      break;
    case '^':
      result.m_matrix[i] ^= rhs.m_matrix[i];
      break;
    default:
      break;
    }
  }
  return result;
}

// --- Constructors / Destructor / Assignment ---

BitMatrix::BitMatrix() : m_matrix() {}

BitMatrix::BitMatrix(const size_t rows, const size_t columns, bool value)
    : m_matrix() {
  if (rows == 0 && columns == 0) {
    return;
  }
  m_matrix.reserve(rows);
  for (size_t i = 0; i < rows; i++) {
    m_matrix.push_back(BitVector(columns, value));
  }
}

BitMatrix::BitMatrix(char **bitstr, const size_t rows) {
  if (!bitstr)
    throw std::invalid_argument("Null string array");
  m_matrix.reserve(rows);
  for (size_t i = 0; i < rows; ++i) {
    m_matrix.push_back(BitVector(bitstr[i]));
  }
  if (m_matrix.size() > 1) {
    size_t first_cols = m_matrix[0].size();
    for (size_t i = 1; i < m_matrix.size(); ++i) {
      if (m_matrix[i].size() != first_cols) {
        throw std::invalid_argument(
            "Bit strings must have the same length (columns)");
      }
    }
  }
}

BitMatrix::BitMatrix(const BitMatrix &other) : m_matrix(other.m_matrix) {}

BitMatrix::~BitMatrix() = default;

BitMatrix &BitMatrix::operator=(const BitMatrix &other) {
  if (this != &other) {
    m_matrix = other.m_matrix;
  }
  return *this;
}

// --- Capacity / Access ---

size_t BitMatrix::rows() const noexcept { return m_matrix.size(); }

size_t BitMatrix::columns() const noexcept {
  if (m_matrix.size() == 0)
    return 0;
  return m_matrix[0].size();
}

void BitMatrix::swap(BitMatrix &other) noexcept {
  m_matrix.swap(other.m_matrix);
}
void swap(BitMatrix &a, BitMatrix &b) noexcept { a.swap(b); }

BitVector &BitMatrix::operator[](size_t i) {
  check_row_index(i);
  return m_matrix[i];
}
const BitVector &BitMatrix::operator[](size_t i) const {
  check_row_index(i);
  return m_matrix[i];
}

// --- Operations ---

void BitMatrix::set(size_t j, size_t i, bool value) {
  check_row_index(j);
  m_matrix[j].set(i, value);
}

void BitMatrix::flip(size_t j, size_t i) {
  check_row_index(j);
  m_matrix[j].flip(i);
}

size_t BitMatrix::row_weight(size_t j) const {
  check_row_index(j);
  return m_matrix[j].weight();
}

size_t BitMatrix::weight() const {
  size_t total_weight = 0;
  for (size_t i = 0; i < m_matrix.size(); ++i) {
    total_weight += m_matrix[i].weight();
  }
  return total_weight;
}

BitVector BitMatrix::conjunction_rows() const {
  if (m_matrix.size() == 0)
    return BitVector(0, false);
  BitVector result = m_matrix[0];
  for (size_t i = 1; i < m_matrix.size(); ++i) {
    result &= m_matrix[i];
  }
  return result;
}

BitVector BitMatrix::disjunction_rows() const {
  if (m_matrix.size() == 0)
    return BitVector(0, false);
  BitVector result = m_matrix[0];
  for (size_t i = 1; i < m_matrix.size(); ++i) {
    result |= m_matrix[i];
  }
  return result;
}

void BitMatrix::flip_range(size_t j, size_t i, size_t k) {
  check_row_index(j);
  if (i + k > columns())
    throw std::out_of_range("Range out of column bounds");
  for (size_t pos = i; pos < i + k; ++pos) {
    m_matrix[j].flip(pos);
  }
}

void BitMatrix::set_range(size_t j, size_t i, size_t k, bool value) {
  check_row_index(j);
  if (i + k > columns())
    throw std::out_of_range("Range out of column bounds");
  m_matrix[j].setRange(i, k, value);
}

// --- Bitwise overloads (row-wise) ---

BitMatrix BitMatrix::operator&(const BitMatrix &rhs) const {
  return row_wise_op(rhs, '&');
}
BitMatrix &BitMatrix::operator&=(const BitMatrix &rhs) {
  *this = row_wise_op(rhs, '&');
  return *this;
}

BitMatrix BitMatrix::operator|(const BitMatrix &rhs) const {
  return row_wise_op(rhs, '|');
}
BitMatrix &BitMatrix::operator|=(const BitMatrix &rhs) {
  *this = row_wise_op(rhs, '|');
  return *this;
}

BitMatrix BitMatrix::operator^(const BitMatrix &rhs) const {
  return row_wise_op(rhs, '^');
}
BitMatrix &BitMatrix::operator^=(const BitMatrix &rhs) {
  *this = row_wise_op(rhs, '^');
  return *this;
}

BitMatrix BitMatrix::operator~() const {
  BitMatrix result(*this);
  for (size_t i = 0; i < result.m_matrix.size(); ++i) {
    result.m_matrix[i] = ~result.m_matrix[i];
  }
  return result;
}

bool BitMatrix::operator==(const BitMatrix &other) const {
  if (m_matrix.size() != other.m_matrix.size() || columns() != other.columns())
    return false;
  for (size_t i = 0; i < m_matrix.size(); ++i) {
    BitVector _other = other.m_matrix[i];
    if (!(m_matrix[i] == _other))
      return false;
  }
  return true;
}
bool BitMatrix::operator!=(const BitMatrix &other) const {
  return !(*this == other);
}

// --- Streaming I/O ---

std::ostream &operator<<(std::ostream &os, const BitMatrix &bm) {
  if (bm.rows() == 0) {
    os << "[]";
    return os;
  }
  os << "[\n";
  for (size_t i = 0; i < bm.rows(); ++i) {
    os << "  " << bm[i];
    if (i < bm.rows() - 1)
      os << ",\n";
  }
  os << "\n]";
  return os;
}

std::istream &operator>>(std::istream &is, BitMatrix &bm) {
  size_t rows, columns;
  std::cout << "Enter number of rows: ";
  if (!(is >> rows))
    return is;
  std::cout << "Enter number of columns: ";
  if (!(is >> columns))
    return is;

  DynamicArray<BitVector> temp_matrix;
  temp_matrix.reserve(rows);

  std::cout << "Enter " << rows << " bit strings of length " << columns
            << ":\n";
  for (size_t i = 0; i < rows; ++i) {
    std::string s;
    if (!(is >> s)) {
      is.setstate(std::ios::failbit);
      return is;
    }

    if (s.length() != columns) {
      std::cerr << "Error: Row " << i << " has length " << s.length()
                << ", expected " << columns << ".\n";
      is.setstate(std::ios::failbit);
      return is;
    }

    try {
      temp_matrix.push_back(BitVector(s.c_str()));
    } catch (const std::invalid_argument &e) {
      std::cerr << "Error in row " << i << ": " << e.what() << "\n";
      is.setstate(std::ios::failbit);
      return is;
    }
  }

  bm = BitMatrix(rows, columns);
  for (size_t i = 0; i < rows; ++i) {
    bm[i] = temp_matrix[i];
  }

  return is;
}
