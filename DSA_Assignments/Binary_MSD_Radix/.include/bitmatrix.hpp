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

#pragma once

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "bitvector.hpp"
#include "dynamic_array.hpp"

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
  BitMatrix(const std::vector<unsigned int> &vec);
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

  std::vector<unsigned int> to_vec();
};

// Streaming I/O
std::ostream &operator<<(std::ostream &os, const BitMatrix &bm);
std::istream &operator>>(std::istream &is, BitMatrix &bm);
