/*
Необходимые методы класса:
- конструкторы (по умолчанию, с параметрами (размер и значение - одно и то же для всех разрядов), конструктор из массива const char *, конструктор копирования);
- деструктор;
- длина (количество бит) вектора;
- обмен содержимого с другим вектором (swap);
- ввод/вывод в консоль (потоковый);
- инверсия всех компонент вектора;
- инверсия i-ой компоненты;
- установка в 0/1 i-ой компоненты;
- установка в 0/1 k компонент, начиная с i-ой;
- установка в 0/1 всех компонент вектора;
- вес вектора (количество единичных компонент).

Необходимые перегрузки:
- получение компоненты ([ ], см. примечание ниже);
- побитовое умножение (&, &=);
- побитовое сложение (|, |=);
- побитовое исключающее ИЛИ (^, ^=);
- побитовые сдвиги (<<, >>, <<=, >>=);
- побитовая инверсия (~);
- присваивание (=).

*/


#include <iostream>
#include <sstream>
#include <cassert>
#include "bitvector.hpp"

void testConstructors() {
    // Default constructor
    BitVector bv1;
    assert(bv1.size() == 0);

    // Size + default value
    BitVector bv2(10, true);
    for (size_t i = 0; i < bv2.size(); ++i)
        assert(bv2[i] == true);

    BitVector bv3(10, false);
    for (size_t i = 0; i < bv3.size(); ++i)
        assert(bv3[i] == false);

    // Constructor from string
    const char* s = "101001";
    BitVector bv4(s);
    assert(bv4.size() == 6);
    assert(bv4[0] == true);
    assert(bv4[1] == false);
    assert(bv4[2] == true);

    // Copy constructor
    BitVector bv5(bv4);
    assert(bv5.size() == bv4.size());
    for (size_t i = 0; i < bv5.size(); ++i)
        assert(bv5[i] == bv4[i]);

    // Assignment operator
    BitVector bv6;
    bv6 = bv4;
    assert(bv6.size() == bv4.size());
    for (size_t i = 0; i < bv6.size(); ++i)
        assert(bv6[i] == bv4[i]);
}

void testSetGetFlip() {
    BitVector bv(8, false);
    bv.set(2, true);
    assert(bv[2] == true);

    bv.flip(2);
    assert(bv[2] == false);

    bv.setAll(true);
    for (size_t i = 0; i < bv.size(); ++i)
        assert(bv[i] == true);

    bv.flipAll();
    for (size_t i = 0; i < bv.size(); ++i)
        assert(bv[i] == false);

    bv.setRange(2, 3, true);
    assert(bv[0] == false);
    assert(bv[2] == true);
    assert(bv[3] == true);
    assert(bv[4] == true);
}

void testWeight() {
    BitVector bv("1010101"); // 4 ones
    assert(bv.weight() == 4);
    bv.setAll(false);
    assert(bv.weight() == 0);
}

void testBoolRef() {
    BitVector bv(5, false);
    bv[1] = true;
    assert(bv[1] == true);

    bv[1].flip();
    assert(bv[1] == false);

    BitVector::BoolRef ref = bv[2];
    ref = true;
    assert(bv[2] == true);
}

void testBitwiseOperators() {
    BitVector a("1010"), b("0110");

    BitVector c = a & b;
    assert(c[0] == 0 && c[1] == 0 && c[2] == 1 && c[3] == 0);

    BitVector d = a | b;
    assert(d[0] == 1 && d[1] == 1 && d[2] == 1 && d[3] == 0);

    BitVector e = a ^ b;
    assert(e[0] == 1 && e[1] == 1 && e[2] == 0 && e[3] == 0);

    BitVector f = ~a;
    assert(f[0] == 0 && f[1] == 1 && f[2] == 0 && f[3] == 1);
}

void testSwap() {
    BitVector a("1010"), b("0101");
    a.swap(b);
    assert(a[0] == 0 && b[0] == 1);
}

void testIO() {
    std::stringstream ss;
    BitVector bv("10101");
    ss << bv;
    std::string out;
    ss >> out;
    assert(out == "10101");

    BitVector read;
    std::stringstream ss2("1100");
    ss2 >> read;
    assert(read.size() == 4);
    assert(read[0] == 1 && read[1] == 1 && read[2] == 0 && read[3] == 0);
}

void testShiftOperators() {
    // Example vector: 12 bits -> 1101 0110 1011
    BitVector bv("110101101011");

    // --- RIGHT SHIFT TESTS ---
    BitVector r0 = bv >> 0;  // shift 0: should be identical
    for (size_t i = 0; i < bv.size(); ++i)
        assert(r0[i] == bv[i]);

    BitVector r1 = bv >> 1;  // shift 1: each bit moves right by 1
    assert(r1.size() == bv.size());
    for (size_t i = 0; i < bv.size() - 1; ++i)
        assert(r1[i+1] == bv[i]);
    assert(r1[0] == 0);  // MSB filled with 0

    BitVector r4 = bv >> 4;  // shift by 4
    for (size_t i = 0; i < bv.size() - 4; ++i)
        assert(r4[i+4] == bv[i]);
    for (size_t i = 0; i < 4; ++i)
        assert(r4[i] == 0);  // leading zeros

    BitVector rfull = bv >> bv.size();  // shift by full length -> all zeros
    for (size_t i = 0; i < bv.size(); ++i)
        assert(rfull[i] == 0);

    // --- LEFT SHIFT TESTS ---
    BitVector l0 = bv << 0;  // shift 0: same
    for (size_t i = 0; i < bv.size(); ++i)
        assert(l0[i] == bv[i]);

    BitVector l1 = bv << 1;  // shift 1: each bit moves left by 1
    for (size_t i = 1; i < bv.size(); ++i)
        assert(l1[i-1] == bv[i]);
    assert(l1[bv.size()-1] == 0); // LSB filled with 0

    BitVector l4 = bv << 4;  // shift by 4
    for (size_t i = 4; i < bv.size(); ++i)
        assert(l4[i-4] == bv[i]);
    for (size_t i = bv.size()-4; i < bv.size(); ++i)
        assert(l4[i] == 0);  // trailing zeros

    BitVector lfull = bv << bv.size(); // shift by full length -> all zeros
    for (size_t i = 0; i < bv.size(); ++i)
        assert(lfull[i] == 0);

    std::cout << "All shift operator tests passed!" << std::endl;
}

int main() {
    testConstructors();
    testSetGetFlip();
    testWeight();
    testBoolRef();
    testBitwiseOperators();
    testSwap();
    testIO();
    testShiftOperators();

    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}
