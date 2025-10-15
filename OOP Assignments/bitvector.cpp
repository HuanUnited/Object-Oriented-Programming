/*
Витовый Вектор

#TODO Yellow
// DONE
// - конструкторы (по умолчанию, с параметрами (размер и значение - одно и то же
// для всех разрядов), конструктор из массива const char *, конструктор
// копирования);
// - деструктор;
// - обмен содержимого с другим вектором (swap);
// - присваивание (=).


- длина (количество бит) вектора;
- ввод/вывод в консоль (потоковый);
- инверсия всех компонент вектора;
- установка в 0/1 i-ой компоненты;
- получение компоненты ([ ], см. примечание ниже);
- побитовое умножение (&, &=);
- побитовое сложение (|, |=);
- побитовое исключающее ИЛИ (^, ^=);
- побитовая инверсия (~);
Примечание: получение компоненты подразумевается только на чтение (т.е.,
возвращается значение бита в виде int/char/bool, а записать значение в бит
нельзя - для этого можно использовать соответствующую функцию).

#TODO Green
- инверсия i-ой компоненты;
- вес вектора (количество единичных компонент).

#TODO White
- установка в 0/1 k компонент, начиная с i-ой;
- установка в 0/1 всех компонент вектора;

Done

*/

#include <algorithm>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <iterator>
#include <memory>

class BitVector {
 public:
  using byte_t = uint8_t;
  static constexpr size_t BITS_PER_BYTE = CHAR_BIT;

  // Constructors + Destructor

  BitVector() = default;

  BitVector(size_t size, bool value = false) : data(nullptr), nbits(size) {
    size_t nbytes = bytes_for_bits(size);
    if (nbytes) {
      data = std::make_unique<byte_t[]>(nbytes);
      std::fill_n(data.get(), nbytes, value ? byte_t(0xFF) : byte_t(0x00));
      if (value && (nbits % BITS_PER_BYTE != 0))
        data[nbytes - 1] &= last_byte_mask();
    }
  }

  BitVector(const BitVector& other) : nbits(other.nbits) {
    size_t nbytes = bytes_for_bits(nbits);
    if (nbytes) {
      data = std::make_unique<byte_t[]>(nbytes);
      std::memcpy(data.get(), other.data.get(), nbytes);
    }
  }

  BitVector& operator=(const BitVector& other) {
    if (this == &other) return *this;
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

  BitVector& operator[](const BitVector& other){}
  BitVector& operator&(const BitVector& other){}
  BitVector& operator&=(const BitVector& other){}
  BitVector& operator|(const BitVector& other){}
  BitVector& operator|=(const BitVector& other){}
  BitVector& operator^(const BitVector& other){}
  BitVector& operator^=(const BitVector& other){}
  BitVector& operator~(){}


  explicit BitVector(const char* bitstr) {
    if (!bitstr) throw std::invalid_argument("Null string");
    nbits = std::strlen(bitstr);
    size_t nbytes = bytes_for_bits(nbits);
    data = std::make_unique<byte_t[]>(nbytes);
    std::fill_n(data.get(), nbytes, 0x00);  // start clean - 0x00 = false

    for (size_t i = 0; i < nbits; ++i) {
      char c = bitstr[i];
      if (c != '0' && c != '1')
        throw std::invalid_argument("Bit string must contain only '0' or '1'");
      if (c == '1') {
        size_t byte_index = i / BITS_PER_BYTE;
        size_t bit_offset = i % BITS_PER_BYTE;
        data[byte_index] |= static_cast<byte_t>(1u << bit_offset);
      }
    }
  }

  BitVector(BitVector&&) noexcept = default;
  BitVector& operator=(BitVector&&) noexcept = default;

  ~BitVector() = default;

  // Methods:

  void swap(BitVector &other) noexcept {
    data.swap(other.data);
    std::swap(nbits,other.nbits);
  }


 private:
  std::unique_ptr<byte_t[]> data;
  size_t nbits = 0;

  static size_t bytes_for_bits(size_t bits) {
    return (bits + BITS_PER_BYTE - 1) / BITS_PER_BYTE;
  }

  byte_t last_byte_mask() const {
    size_t rem = nbits % BITS_PER_BYTE;
    if (rem == 0) return static_cast<byte_t>(~byte_t(0));
    return static_cast<byte_t>((1u << rem) - 1u);
  }

  void check_index(size_t i) const {
    if (i >= nbits) throw std::out_of_range("Index");
  }
};

class BoolRank {
 public:
 private:
};
