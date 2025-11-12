// tests.cpp
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "bitmatrix_d.cpp"

// helper: build char** from vector<string> (rows)
static char **make_char_matrix(const std::vector<std::string> &rows) {
  size_t n = rows.size();
  if (n == 0)
    return nullptr;
  size_t m = rows[0].size();
  char **mat = new char *[n];
  for (size_t i = 0; i < n; ++i) {
    mat[i] = new char[m + 1];
    // copy chars and terminate
    for (size_t j = 0; j < m; ++j)
      mat[i][j] = rows[i][j];
    mat[i][m] = '\0';
  }
  return mat;
}

static void free_char_matrix(char **mat, size_t n) {
  if (!mat)
    return;
  for (size_t i = 0; i < n; ++i)
    delete[] mat[i];
  delete[] mat;
}

int main() {
  int errors = 0;
  auto check = [&](bool cond, const std::string &msg) {
    if (!cond) {
      std::cerr << "FAILED: " << msg << "\n";
      ++errors;
    } else {
      std::cout << "ok: " << msg << "\n";
    }
  };

  // ---------- 1) Default constructor, rows/cols ----------
  {
    BitMatrix a;
    check(a.rows() == 0, "default ctor rows == 0");
    check(a.columns() == 0, "default ctor cols == 0");
  }

  // ---------- 2) Parametric ctor and element set/get ----------
  {
    BitMatrix m(3, 5, false); // 3 rows, 5 cols, all zero
    check(m.rows() == 3, "param ctor rows == 3");
    check(m.columns() == 5, "param ctor cols == 5");

    // set some bits and check row_weight / weight
    m.set(0, 0, true);
    m.set(1, 2, true);
    m.set(2, 4, true);
    check(m.row_weight(0) == 1, "row 0 weight == 1");
    check(m.row_weight(1) == 1, "row 1 weight == 1");
    check(m.row_weight(2) == 1, "row 2 weight == 1");
    check(m.weight() == 3, "matrix total weight == 3");

    // flip bit
    m.flip(0, 0);
    check(m.row_weight(0) == 0, "flip cleared bit => row 0 weight == 0");
  }

  // ---------- 3) Constructor from char** and uniform columns check ----------
  {
    std::vector<std::string> rows = {"10101", "00011", "11111"};
    char **cm = make_char_matrix(rows);
    BitMatrix fromC(cm, rows.size());
    check(fromC.rows() == 3, "char** ctor rows == 3");
    check(fromC.columns() == 5, "char** ctor cols == 5");
    check(fromC[0].size() == 5, "row vector size exposed correctly");
    // check content via weight
    check(fromC.row_weight(0) == 3, "row 0 weight == 3 (10101)");
    check(fromC.row_weight(1) == 2, "row 1 weight == 2 (00011)");
    check(fromC.row_weight(2) == 5, "row 2 weight == 5 (11111)");
    free_char_matrix(cm, rows.size());
  }

  // ---------- 4) Copy constructor and assignment ----------
  {
    BitMatrix a(2, 4, false);
    a.set(0, 1, true);
    a.set(1, 2, true);

    BitMatrix b = a; // copy ctor
    check(b == a, "copy ctor produces equal matrix");

    b.set(0, 1, false);
    check(!(b == a), "modifying copy doesn't affect original");

    BitMatrix c;
    c = a; // copy assignment
    check(c == a, "copy assignment produces equal matrix");
  }

  // ---------- 5) Move constructor / assignment (basic smoke test) ----------
  {
    BitMatrix a(1, 3, false);
    a.set(0, 0, true);
    BitMatrix tmp = a;
    BitMatrix moved = std::move(tmp); // move ctor
    check(moved.rows() == 1 && moved.columns() == 3, "move ctor basic dims");
    BitMatrix dest;
    dest = std::move(moved); // move assignment
    check(dest.rows() == 1 && dest.columns() == 3,
          "move assignment basic dims");
  }

  // ---------- 6) swap ----------
  {
    BitMatrix x(1, 3, false);
    BitMatrix y(2, 3, false);
    x.set(0, 0, true);
    y.set(1, 2, true);
    x.swap(y);
    check(x.rows() == 2 && y.rows() == 1, "swap swapped rows count");
    // verify bits moved with rows
    check(x[1].size() == 3, "swap preserves columns");
  }

  // ---------- 7) conjunction_rows / disjunction_rows ----------
  {
    std::vector<std::string> rows = {"1100", "1001", "1101"};
    char **cm = make_char_matrix(rows);
    BitMatrix bm(cm, rows.size());
    free_char_matrix(cm, rows.size());
    BitVector conj = bm.conjunction_rows(); // bitwise AND of rows
    BitVector disj = bm.disjunction_rows(); // bitwise OR of rows
    // conjunction: 1100 & 1001 & 1101 = 1000 (weight 1)
    // disjunction: OR = 1101 (weight 3)
    check(conj.weight() == 1, "conjunction weight == 1");
    check(disj.weight() == 3, "disjunction weight == 3");
  }

  // ---------- 8) flip_range and set_range ----------
  {
    BitMatrix r(1, 8, false);
    r.set_range(0, 2, 3, true); // set bits 2,3,4
    check(r.row_weight(0) == 3, "set_range set 3 bits");
    r.flip_range(0, 3, 2); // flip bits 3 and 4 (3->0,4->0)
    check(r.row_weight(0) == 1, "flip_range flipped 2 bits -> weight 1");
  }

  // ---------- 9) bitwise row-wise operators (&,|,^ and their assign forms)
  // ----------
  {
    std::vector<std::string> rowsA = {"1010", "0101"};
    std::vector<std::string> rowsB = {"1001", "1111"};
    char **a_mat = make_char_matrix(rowsA);
    char **b_mat = make_char_matrix(rowsB);
    BitMatrix A(a_mat, rowsA.size());
    BitMatrix B(b_mat, rowsB.size());
    free_char_matrix(a_mat, rowsA.size());
    free_char_matrix(b_mat, rowsB.size());

    BitMatrix anded = A & B;
    BitMatrix ored = A | B;
    BitMatrix xored = A ^ B;

    // compute expected weights per row:
    // row0: A=1010 B=1001 => AND=1000 (1), OR=1011 (3), XOR=0011 (2)
    check(anded.row_weight(0) == 1, "A&B row0 weight == 1");
    check(ored.row_weight(0) == 3, "A|B row0 weight == 3");
    check(xored.row_weight(0) == 2, "A^B row0 weight == 2");

    // test compound assignment forms don't throw and produce expected values:
    BitMatrix C = A;
    C &= B;
    check(C == anded, "C &= B equals A & B");

    BitMatrix D = A;
    D |= B;
    check(D == ored, "D |= B equals A | B");

    BitMatrix E = A;
    E ^= B;
    check(E == xored, "E ^= B equals A ^ B");

    // test bitwise NOT (~)
    BitMatrix notA = ~A;
    // ~A row0 of width 4: A=1010 => ~A = 0101 -> weight 2
    check(notA.row_weight(0) == 2, "~A row0 weight == 2");
  }

  // ---------- 10) operator[] read/write ----------
  {
    BitMatrix m(2, 4, false);
    m[0].set(1, true);
    m[1].set(2, true);
    check(m.row_weight(0) == 1 && m.row_weight(1) == 1,
          "operator[] read/write works");
  }

  // ---------- 11) streaming output (operator<<) ----------
  {
    BitMatrix m(2, 3, false);
    m.set(0, 0, true);
    std::ostringstream oss;
    oss << m;
    std::string out = oss.str();
    check(!out.empty() && out.find('[') != std::string::npos,
          "operator<< produces output");
  }

  // ---------- 12) streaming input (operator>>) - simulate input ----------
  {
    // We'll construct an input stream with row/col and rows strings
    std::istringstream iss("2 3\n101\n010\n");
    BitMatrix m;
    // operator>> may prompt to cout, but it should parse correctly from iss
    iss >> m;
    check(m.rows() == 2 && m.columns() == 3, "operator>> read dims");
    check(m[0].size() == 3 && m[0].weight() == 2,
          "operator>> read data correctly");
  }

  // ---------- 13) exceptions: out-of-range row access and invalid ranges
  // ----------
  {
    BitMatrix z(2, 4, false);
    bool threw = false;
    try {
      (void)z[10]; // should throw
    } catch (const std::out_of_range &) {
      threw = true;
    } catch (...) {
      // other exception counts as fail
    }
    check(threw, "out-of-range operator[] throws");

    bool threw_range = false;
    try {
      z.flip_range(0, 3, 5); // i+k > columns -> should throw
    } catch (const std::out_of_range &) {
      threw_range = true;
    }
    check(threw_range, "flip_range with out-of-bounds throws");
  }

  // ---------- final result ----------
  if (errors == 0) {
    std::cout << "\nALL TESTS PASSED\n";
    return 0;
  } else {
    std::cerr << "\nSOME TESTS FAILED: " << errors << " error(s)\n";
    return 2;
  }
}
