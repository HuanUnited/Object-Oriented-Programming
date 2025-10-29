/*
В данной лабораторной требуется реализовать класс булевой матрицы. Данный класс
будет являться агрегирующим классом: матрица должна быть представлена как
динамический массив строк - булевых векторов. Таким образом, для реализации
данного класса необходимо использовать шаблонный класс "Динамический массив",
реализованный для лабораторной работы №2, а также класс "Булев вектор",
реализованный для лабораторной работы №3.

Этапы лабораторной:
1. [10 баллов] реализовать все методы, помеченные жёлтым цветом.
2. [5 баллов] дополнительно реализовать все методы, помеченные зелёным цветом.
3. [5 баллов] дополнительно реализовать все методы, не помеченные цветом.

Необходимые методы класса:
- конструкторы (по умолчанию, с параметрами (количество строк/столбцов и
значения разрядов), конструктор из матрицы char, конструктор копирования);
- деструктор;
- получение числа строк и столбцов матрицы;
- обмен содержимого с другой матрицей (swap);
- ввод/вывод в консоль (потоковый);
- вес матрицы (количество единичных компонент);
- конъюнкция всех строк (возвращает булев вектор);
- дизъюнкция всех строк (возвращает булев вектор);
- вес j-ой строки;
- инверсия в i-ой компоненты j-ой строки;
- инверсия k компонент j-ой строки, начиная с i-ой компоненты;
- установка в 0/1 i-ой компоненты j-ой строки;
- установка в 0/1 k компонент j-ой строки, начиная с i-ой компоненты;

Необходимые перегрузки:
- присваивание (=);
- получение строки ([ ]);
- построчное побитовое умножение (&, &=);
- построчное побитовое сложение (|, |=);
- построчное побитовое исключающее ИЛИ (^, ^=);
- построчная побитовая инверсия (~).
*/

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>

#include "DynamicArrays_2.hpp"
#include "bitvector.hpp"

class BitMatrix {
 private:
  DynamicArray<BitVector> m_matrix{};

  // Helper to check row index
  void check_row_index(size_t i) const {
    if (i >= m_matrix.size())
      throw std::out_of_range("Row index out of bounds");
  }

 public:
  // --- Constructors / Destructor / Assignment ---

  // Конструктор по умолчанию (Default Constructor) - DONE (Yellow)
  BitMatrix() : m_matrix() {}

  // С параметрами (количество строк/столбцов и значения разрядов) - DONE
  // (Yellow)
  BitMatrix(const size_t rows, const size_t columns, bool value = false)
      : m_matrix() {
    if (rows == 0 && columns == 0) {
      return;
    }
    // Reserve space in the dynamic array for rows
    m_matrix.reserve(rows);
    for (size_t i = 0; i < rows; i++) {
      // Add a BitVector of 'columns' size and initial 'value'
      m_matrix.push_back(BitVector(columns, value));
    }
    // m_matrix.size() should now equal m_matrix.size()
  }

  // Конструктор из матрицы char (Char Matrix Constructor) - DONE (Yellow)
  explicit BitMatrix(char** bitstr, const size_t rows) {
    if (!bitstr) throw std::invalid_argument("Null string array");
    m_matrix.reserve(rows);
    for (size_t i = 0; i < rows; ++i) {
      // BitVector constructor from const char* handles string validation and
      // length
      m_matrix.push_back(BitVector(bitstr[i]));
    }
    if (m_matrix.size() > 1) {
      // Ensure all rows have the same number of columns
      size_t first_cols = m_matrix[0].size();
      for (size_t i = 1; i < m_matrix.size(); ++i) {
        if (m_matrix[i].size() != first_cols) {
          throw std::invalid_argument(
              "Bit strings must have the same length (columns)");
        }
      }
    }
  }

  // Конструктор копирования (Copy Constructor) - DONE (Yellow)
  BitMatrix(const BitMatrix& other)
      : m_matrix(other.m_matrix) {}  // DynamicArray's copy constructor handles
                                     // deep copy

  // Деструктор (Destructor) - DONE (Yellow)
  // DynamicArray and BitVector use unique_ptr/raw pointers and handle memory
  // correctly.
  ~BitMatrix() = default;

  // Присваивание (=) (Assignment Operator) - DONE (Yellow)
  BitMatrix& operator=(const BitMatrix& other) {
    if (this != &other) {
      m_matrix =
          other.m_matrix;  // DynamicArray's copy assignment handles deep copy
    }
    return *this;
  }

  // Move constructor and assignment (good practice)
  BitMatrix(BitMatrix&&) noexcept = default;
  BitMatrix& operator=(BitMatrix&&) noexcept = default;

  // --- Capacity / Access ---

  // Получение числа строк (Get number of rows) - DONE (Yellow)
  size_t rows() const noexcept { return m_matrix.size(); }

  // Получение числа столбцов (Get number of columns) - DONE (Yellow)
  size_t columns() const noexcept {
    if (m_matrix.size() == 0) return 0;
    return m_matrix[0].size();
  }

  // Обмен содержимого с другой матрицей (swap) - DONE (Yellow)
  void swap(BitMatrix& other) noexcept {
    m_matrix.swap(other.m_matrix);  // Use DynamicArray's swap
  }
  friend void swap(BitMatrix& a, BitMatrix& b) noexcept { a.swap(b); }

  // Получение строки ([ ]) (Row access - read/write) - DONE (Yellow)
  BitVector& operator[](size_t i) {
    check_row_index(i);
    return m_matrix[i];
  }

  // Получение строки ([ ]) (Row access - read-only) - DONE (Yellow)
  const BitVector& operator[](size_t i) const {
    check_row_index(i);
    return m_matrix[i];
  }

  // --- Operations (Yellow) ---

  // Установка в 0/1 i-ой компоненты j-ой строки (Set i-th bit of j-th row to
  // value) - DONE (Yellow)
  void set(size_t j, size_t i, bool value) {
    check_row_index(j);
    // BitVector::set handles column index check
    m_matrix[j].set(i, value);
  }

  // --- Operations (Green) ---

  // Инверсия i-ой компоненты j-ой строки (Flip i-th bit of j-th row) - DONE
  // (Green)
  void flip(size_t j, size_t i) {
    check_row_index(j);
    // BitVector::flip handles column index check
    m_matrix[j].flip(i);
  }

  // Вес j-ой строки (Weight of j-th row - number of set bits) - DONE (Green)
  size_t row_weight(size_t j) const {
    check_row_index(j);
    return m_matrix[j].weight();
  }

  // --- Operations (White) ---

  // Вес матрицы (количество единичных компонент) (Matrix Weight) - DONE (White)
  size_t weight() const {
    size_t total_weight = 0;
    for (size_t i = 0; i < m_matrix.size(); ++i) {
      total_weight += m_matrix[i].weight();
    }
    return total_weight;
  }

  // Конъюнкция всех строк (All-row conjunction - returns BitVector) - DONE
  // (White)
  BitVector conjunction_rows() const {
    if (m_matrix.size() == 0) return BitVector(0, false);
    BitVector result = m_matrix[0];  // Start with the first row
    for (size_t i = 1; i < m_matrix.size(); ++i) {
      result &= m_matrix[i];  // BitVector operator&= handles the operation
    }
    return result;
  }

  // Дизъюнкция всех строк (All-row disjunction - returns BitVector) - DONE
  // (White)
  BitVector disjunction_rows() const {
    if (m_matrix.size() == 0) return BitVector(0, false);
    BitVector result = m_matrix[0];  // Start with the first row
    for (size_t i = 1; i < m_matrix.size(); ++i) {
      result |= m_matrix[i];  // BitVector operator|= handles the operation
    }
    return result;
  }

  // Инверсия k компонент j-ой строки, начиная с i-ой компоненты (Flip k bits in
  // j-th row starting from i) - DONE (White)
  void flip_range(size_t j, size_t i, size_t k) {
    check_row_index(j);
    if (i + k > columns())
      throw std::out_of_range("Range out of column bounds");
    for (size_t pos = i; pos < i + k; ++pos) {
      m_matrix[j].flip(pos);
    }
  }

  // Установка в 0/1 k компонент j-ой строки, начиная с i-ой компоненты (Set k
  // bits in j-th row starting from i to value) - DONE (White)
  void set_range(size_t j, size_t i, size_t k, bool value) {
    check_row_index(j);
    // BitVector::setRange handles column range check internally (though it's
    // better to check here too)
    if (i + k > columns())
      throw std::out_of_range("Range out of column bounds");
    m_matrix[j].setRange(i, k, value);
  }

  // --- Overloads: Bitwise (Row-wise) ---

 private:
  // Helper for row-wise bitwise operations
  BitMatrix row_wise_op(const BitMatrix& rhs, char op) const {
    if (m_matrix.size() != rhs.m_matrix.size() || columns() != rhs.columns()) {
      throw std::invalid_argument(
          "Matrices must have the same dimensions for bitwise operation");
    }
    BitMatrix result(*this);  // Start with a copy
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
      }
    }
    return result;
  }

 public:
  // Построчное побитовое умножение (&) (Row-wise bitwise AND) - DONE (Yellow)
  BitMatrix operator&(const BitMatrix& rhs) const {
    return row_wise_op(rhs, '&');
  }
  BitMatrix& operator&=(const BitMatrix& rhs) {
    *this = row_wise_op(rhs, '&');
    return *this;
  }

  // Построчное побитовое сложение (|) (Row-wise bitwise OR) - DONE (Yellow)
  BitMatrix operator|(const BitMatrix& rhs) const {
    return row_wise_op(rhs, '|');
  }
  BitMatrix& operator|=(const BitMatrix& rhs) {
    *this = row_wise_op(rhs, '|');
    return *this;
  }

  // Построчное побитовое исключающее ИЛИ (^) (Row-wise bitwise XOR) - DONE
  // (Yellow)
  BitMatrix operator^(const BitMatrix& rhs) const {
    return row_wise_op(rhs, '^');
  }
  BitMatrix& operator^=(const BitMatrix& rhs) {
    *this = row_wise_op(rhs, '^');
    return *this;
  }

  // Построчная побитовая инверсия (~) (Row-wise bitwise NOT) - DONE (Yellow)
  BitMatrix operator~() const {
    BitMatrix result(*this);
    for (size_t i = 0; i < m_matrix.size(); ++i) {
      result.m_matrix[i] =
          ~result.m_matrix[i];  // BitVector operator~ handles the operation
    }
    return result;
  }

  // Equality operator (for completeness)
  bool operator==(BitMatrix& other) const {
    if (m_matrix.size() != other.m_matrix.size() || columns() != other.columns()) return false;
    for (size_t i = 0; i < m_matrix.size(); ++i) {
      if (!(m_matrix[i] == other.m_matrix[i]))
        return false;  // BitVector operator==
    }
    return true;
  }
  bool operator!=(BitMatrix& other) const { return !(*this == other); }
};

// --- Потоковый ввод/вывод в консоль (Streaming I/O) --- - DONE (Yellow)

// Вывод (Output)
std::ostream& operator<<(std::ostream& os, const BitMatrix& bm) {
  if (bm.rows() == 0) {
    os << "[]";
    return os;
  }
  os << "[\n";
  for (size_t i = 0; i < bm.rows(); ++i) {
    os << "  " << bm[i];  // Use BitVector's stream operator
    if (i < bm.rows() - 1) os << ",\n";
  }
  os << "\n]";
  return os;
}

// Ввод (Input)
// Reads rows and columns, then rows number of bit strings
std::istream& operator>>(std::istream& is, BitMatrix& bm) {
  size_t rows, columns;
  std::cout << "Enter number of rows: ";
  if (!(is >> rows)) return is;
  std::cout << "Enter number of columns: ";
  if (!(is >> columns)) return is;

  // Temporary DynamicArray to build the matrix
  DynamicArray<BitVector> temp_matrix;
  temp_matrix.reserve(rows);

  std::cout << "Enter " << rows << " bit strings of length " << columns
            << ":\n";
  for (size_t i = 0; i < rows; ++i) {
    std::string s;
    if (!(is >> s)) {
      // If input fails before all rows are read, set failbit
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
      // BitVector constructor from string handles '0'/'1' validation
      temp_matrix.push_back(BitVector(s.c_str()));
    } catch (const std::invalid_argument& e) {
      std::cerr << "Error in row " << i << ": " << e.what() << "\n";
      is.setstate(std::ios::failbit);
      return is;
    }
  }

  // Only assign if all input was successful
  bm = BitMatrix(rows, columns);  // Create a new matrix with correct dimensions
  for (size_t i = 0; i < rows; ++i) {
    bm[i] = temp_matrix[i];  // Copy the valid bit vectors
  }

  return is;
}

int main() {
  size_t n{5}, m{10};

  char** matrix = new char*[n];

  for (size_t i = 0; i < n; ++i) {
    matrix[i] = new char[m];
    for (size_t j = 0; j < m; ++j) {
      std::cin >> matrix[i][j];
    }
  }

  /*
  101 000 111 0
  100 101 011 1
  011 110 111 1
  001 010 001 0
  110 100 100 0
  */

  BitMatrix bitmatrix(matrix, 5);

  std::cout << bitmatrix << '\n';

  BitVector v = bitmatrix.conjunction_rows();
  BitVector v2 = bitmatrix.disjunction_rows();


  std::cout << v << '\n';
  std::cout << v2 << '\n';


}