#pragma once
// matrix_utils.hpp
// Header-only utilities for generating matrices, saving/loading, timing and
// dataset creation.
//
// Usage:
//   #include "matrix_utils.hpp"
//   using Mat = matrix_utils::Matrix<double>;
//   Mat A = matrix_utils::random_dense_matrix<double>(100,100);
//   matrix_utils::save_matrix_txt("A.txt", A);
//   auto t = matrix_utils::timeit([&]{ some_work(A); });

#include <chrono>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

#include "bitmatrix.hpp"

namespace m_utils {

// -----------------------------
// Basic matrix container (row-major)
// -----------------------------
template <typename T> struct Matrix {
  using value_type = T;
  std::size_t rows = 0;
  std::size_t cols = 0;
  std::vector<T> data;

  Matrix() = default;
  Matrix(std::size_t r, std::size_t c, T fill = T{})
      : rows(r), cols(c), data(r * c, fill) {}
  void resize(std::size_t r, std::size_t c, T fill = T{}) {
    rows = r;
    cols = c;
    data.assign(r * c, fill);
  }
  T &operator()(std::size_t i, std::size_t j) { return data.at(i * cols + j); }
  const T &operator()(std::size_t i, std::size_t j) const {
    return data.at(i * cols + j);
  }
  std::size_t size() const noexcept { return data.size(); }
  bool empty() const noexcept { return data.empty(); }
};

// -----------------------------
// RNG helpers
// -----------------------------
struct RNG {
  using clock = std::mt19937_64;
  clock eng;
  std::uint64_t seed_used;

  explicit RNG(std::uint64_t seed = 0) {
    if (seed == 0) {
      // pick unpredictable seed
      std::random_device rd;
      seed_used = (static_cast<std::uint64_t>(rd()) << 32) ^
                  static_cast<std::uint64_t>(rd());
    } else {
      seed_used = seed;
    }
    eng.seed(seed_used);
  }

  // Return the seed we actually used (does not call eng())
  std::uint64_t seed() const { return seed_used; }
};

// -----------------------------
// Generate random dense matrix (uniform real or integer)
// -----------------------------
template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
Matrix<T> random_dense_matrix(std::size_t rows, std::size_t cols, T lo = T{0},
                              T hi = T{1}, uint64_t seed = 0) {
  RNG rng(seed ? seed : std::random_device{}());
  Matrix<T> M(rows, cols);
  if constexpr (std::is_integral_v<T>) {
    std::uniform_int_distribution<T> dist(lo, hi);
    for (auto &v : M.data)
      v = dist(rng.eng);
  } else {
    std::uniform_real_distribution<T> dist(lo, hi);
    for (auto &v : M.data)
      v = dist(rng.eng);
  }
  return M;
}

// -----------------------------
// Generate Bernoulli / sparse matrix: values 0 or val with probability p
// -----------------------------
template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
Matrix<T> bernoulli_matrix(std::size_t rows, std::size_t cols, double p = 0.1,
                           T val = T{1}, uint64_t seed = 0) {
  if (p < 0.0 || p > 1.0)
    throw std::invalid_argument("p must be in [0,1]");
  RNG rng(seed ? seed : std::random_device{}());
  Matrix<T> M(rows, cols, T{0});
  std::bernoulli_distribution d(p);
  for (auto &x : M.data) {
    if (d(rng.eng))
      x = val;
  }
  return M;
}

// -----------------------------
// Generate random BitMatrix with given density
// density: probability that each bit is 1 (default 0.5)
// -----------------------------
inline BitMatrix random_bitmatrix(std::size_t rows, std::size_t cols,
                                  double density = 0.1, uint64_t seed = 0) {
  if (density < 0.0 || density > 1.0)
    throw std::invalid_argument("density must be in [0,1]");

  RNG rng(seed ? seed : std::random_device{}());
  BitMatrix bm(rows, cols, false);
  std::bernoulli_distribution d(density);

  for (std::size_t i = 0; i < rows; ++i) {
    for (std::size_t j = 0; j < cols; ++j) {
      if (d(rng.eng)) {
        bm.set(i, j, true);
      }
    }
  }

  return bm;
}

// -----------------------------
// Alternative: Generate random BitMatrix from pattern
// pattern: "uniform" (all bits have equal density)
//          "diagonal" (1s on diagonal, rest according to density)
//          "upper" (upper triangle has higher density)
//          "lower" (lower triangle has higher density)
// -----------------------------
inline BitMatrix
random_bitmatrix_pattern(std::size_t rows, std::size_t cols,
                         const std::string &pattern = "uniform",
                         double density = 0.5, uint64_t seed = 0) {
  if (density < 0.0 || density > 1.0)
    throw std::invalid_argument("density must be in [0,1]");

  RNG rng(seed ? seed : std::random_device{}());
  BitMatrix bm(rows, cols, false);
  std::bernoulli_distribution d(density);

  for (std::size_t i = 0; i < rows; ++i) {
    for (std::size_t j = 0; j < cols; ++j) {
      bool set_bit = false;

      if (pattern == "uniform") {
        set_bit = d(rng.eng);
      } else if (pattern == "diagonal") {
        if (i == j) {
          set_bit = true; // Always set diagonal
        } else {
          set_bit = d(rng.eng);
        }
      } else if (pattern == "upper") {
        if (j >= i) {
          set_bit = d(rng.eng);
        }
      } else if (pattern == "lower") {
        if (i >= j) {
          set_bit = d(rng.eng);
        }
      } else {
        throw std::invalid_argument("Unknown pattern: " + pattern);
      }

      if (set_bit) {
        bm.set(i, j, true);
      }
    }
  }

  return bm;
}

// -----------------------------
// Save BitMatrix to text file
// Format: rows cols on first line, then each row as bit string
// -----------------------------
inline void save_bitmatrix_txt(const std::string &path, const BitMatrix &bm) {
  std::ofstream os(path);
  if (!os)
    throw std::runtime_error("Cannot open file for writing: " + path);

  os << bm.rows() << " " << bm.columns() << "\n";
  for (std::size_t i = 0; i < bm.rows(); ++i) {
    for (std::size_t j = 0; j < bm.columns(); ++j) {
      os << (bm[i][j] ? '1' : '0');
    }
    os << '\n';
  }
}

// -----------------------------
// Load BitMatrix from text file
// -----------------------------
inline BitMatrix load_bitmatrix_txt(const std::string &path) {
  std::ifstream is(path);
  if (!is)
    throw std::runtime_error("Cannot open file for reading: " + path);

  std::size_t rows, cols;
  if (!(is >> rows >> cols))
    throw std::runtime_error("Failed to read BitMatrix dimensions");

  BitMatrix bm(rows, cols, false);
  std::string line;
  std::getline(is, line); // consume newline after dimensions

  for (std::size_t i = 0; i < rows; ++i) {
    if (!std::getline(is, line))
      throw std::runtime_error("Failed to read BitMatrix row " +
                               std::to_string(i));

    if (line.length() != cols)
      throw std::runtime_error("Row " + std::to_string(i) +
                               " has wrong length");

    for (std::size_t j = 0; j < cols; ++j) {
      if (line[j] != '0' && line[j] != '1')
        throw std::runtime_error("Invalid bit character at row " +
                                 std::to_string(i));
      if (line[j] == '1')
        bm.set(i, j, true);
    }
  }

  return bm;
}

// -----------------------------
// IO: text format (rows cols newline values row-major, whitespace separated)
// Example:
//
// 3 2
// 1 2
// 3 4
// 5 6
// -----------------------------
template <typename T>
void save_matrix_txt(const std::string &path, const Matrix<T> &M,
                     bool human_friendly = true, int precision = 6) {
  std::ofstream os(path);
  if (!os)
    throw std::runtime_error("Cannot open file for writing: " + path);
  os << M.rows << " " << M.cols << "\n";
  if (human_friendly && !std::is_integral_v<T>)
    os << std::fixed << std::setprecision(precision);
  for (std::size_t i = 0; i < M.rows; ++i) {
    for (std::size_t j = 0; j < M.cols; ++j) {
      if (j)
        os << ' ';
      os << M(i, j);
    }
    os << '\n';
  }
}

// -----------------------------
// IO: load text
// -----------------------------
template <typename T> Matrix<T> load_matrix_txt(const std::string &path) {
  std::ifstream is(path);
  if (!is)
    throw std::runtime_error("Cannot open file for reading: " + path);
  std::size_t r, c;
  if (!(is >> r >> c))
    throw std::runtime_error("Failed to read matrix dimensions");
  Matrix<T> M(r, c);
  for (std::size_t i = 0; i < r; ++i) {
    for (std::size_t j = 0; j < c; ++j) {
      T tmp;
      if (!(is >> tmp))
        throw std::runtime_error("Failed to read matrix element");
      M(i, j) = tmp;
    }
  }
  return M;
}

// -----------------------------
// Simple binary save / load (raw row-major T values, with header r,c as 64-bit)
// -----------------------------
template <typename T>
void save_matrix_bin(const std::string &path, const Matrix<T> &M) {
  std::ofstream os(path, std::ios::binary);
  if (!os)
    throw std::runtime_error("Cannot open file for writing: " + path);
  uint64_t r = static_cast<uint64_t>(M.rows);
  uint64_t c = static_cast<uint64_t>(M.cols);
  os.write(reinterpret_cast<const char *>(&r), sizeof(r));
  os.write(reinterpret_cast<const char *>(&c), sizeof(c));
  if (!M.empty()) {
    os.write(reinterpret_cast<const char *>(M.data.data()),
             sizeof(T) * M.data.size());
  }
}

template <typename T> Matrix<T> load_matrix_bin(const std::string &path) {
  std::ifstream is(path, std::ios::binary);
  if (!is)
    throw std::runtime_error("Cannot open file for reading: " + path);
  uint64_t r, c;
  is.read(reinterpret_cast<char *>(&r), sizeof(r));
  is.read(reinterpret_cast<char *>(&c), sizeof(c));
  Matrix<T> M(static_cast<std::size_t>(r), static_cast<std::size_t>(c));
  if (M.size())
    is.read(reinterpret_cast<char *>(M.data.data()), sizeof(T) * M.data.size());
  return M;
}

// -----------------------------
// Timing utilities
// -----------------------------
using clock_t = std::chrono::high_resolution_clock;
using dur_t = std::chrono::duration<double, std::milli>; // milliseconds

template <typename F, typename... Args> auto timeit(F &&f, Args &&...args) {
  auto t0 = clock_t::now();
  if constexpr (std::is_same_v<void, std::invoke_result_t<F, Args...>>) {
    std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    auto t1 = clock_t::now();
    return dur_t(t1 - t0);
  } else {
    auto res = std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    auto t1 = clock_t::now();
    return std::make_pair(res, dur_t(t1 - t0));
  }
}

// -----------------------------
// Simple benchmark helper: generate matrix, run a callable on it, record time
// and optionally save returns pair<result_of_f(optional), milliseconds>
// -----------------------------
template <typename MatT, typename Fn>
auto benchmark_generate_and_run(std::size_t rows, std::size_t cols, Fn &&work,
                                std::string savepath = {}, uint64_t seed = 0,
                                bool use_binary = false) {
  using value_t = typename MatT::value_type;
  MatT M =
      random_dense_matrix<value_t>(rows, cols, value_t{0}, value_t{1}, seed);
  if (!savepath.empty()) {
    if (use_binary)
      save_matrix_bin(savepath, M);
    else
      save_matrix_txt(savepath, M);
  }
  return timeit(std::forward<Fn>(work), M);
}

// -----------------------------
// Batch dataset generator:
// - creates N matrices according to parameters and writes to out_dir
// - produces a CSV manifest: filename,rows,cols,seed,created_time_ms
// -----------------------------
struct DatasetSpec {
  std::size_t rows = 100;
  std::size_t cols = 100;
  std::size_t count = 1;
  double density = 1.0; // if <1, use bernoulli with p=density (and val=1)
  bool binary = false;
  uint64_t seed = 0;
  std::string filename_prefix = "mat";
};

inline std::string join_paths(const std::string &a, const std::string &b) {
#ifdef _WIN32
  const char sep = '\\';
#else
  const char sep = '/';
#endif
  if (a.empty())
    return b;
  if (a.back() == sep)
    return a + b;
  return a + sep + b;
}

inline std::uint64_t now_ms_since_epoch() {
  return static_cast<std::uint64_t>(
      std::chrono::duration_cast<std::chrono::milliseconds>(
          clock_t::now().time_since_epoch())
          .count());
}

template <typename T = double>
void generate_dataset(const std::string &out_dir, const DatasetSpec &spec) {
  // try to create directory? we won't create it here to keep header-only
  // portable; user should ensure out_dir exists
  std::ofstream manifest(
      join_paths(out_dir, spec.filename_prefix + "_manifest.csv"));
  if (!manifest)
    throw std::runtime_error("Cannot open manifest for writing");

  manifest << "filename,rows,cols,density,seed,created_ms\n";
  uint64_t base_seed = spec.seed ? spec.seed : std::random_device{}();
  for (std::size_t k = 0; k < spec.count; ++k) {
    uint64_t s = base_seed + static_cast<uint64_t>(k);
    std::string fname = spec.filename_prefix + "_" + std::to_string(k) +
                        (spec.binary ? ".bin" : ".txt");
    std::string fullpath = join_paths(out_dir, fname);

    if (spec.density >= 1.0) {
      auto M = random_dense_matrix<T>(spec.rows, spec.cols, T{0}, T{1}, s);
      if (spec.binary)
        save_matrix_bin(fullpath, M);
      else
        save_matrix_txt(fullpath, M);
    } else {
      // Bernoulli with val = 1
      auto M = bernoulli_matrix<T>(spec.rows, spec.cols, spec.density, T{1}, s);
      if (spec.binary)
        save_matrix_bin(fullpath, M);
      else
        save_matrix_txt(fullpath, M);
    }

    manifest << fname << "," << spec.rows << "," << spec.cols << ","
             << spec.density << "," << s << "," << now_ms_since_epoch() << "\n";
    // basic progress to stdout
    std::cout << "Wrote " << fullpath << "\n";
  }
}

// -----------------------------
// Small utility: print a small matrix to stdout
// -----------------------------
template <typename T>
void print_matrix(const Matrix<T> &M, std::size_t max_rows = 10,
                  std::size_t max_cols = 10) {
  std::size_t r = std::min(M.rows, max_rows);
  std::size_t c = std::min(M.cols, max_cols);
  for (std::size_t i = 0; i < r; ++i) {
    for (std::size_t j = 0; j < c; ++j) {
      std::cout << M(i, j);
      if (j + 1 < c)
        std::cout << ' ';
    }
    if (i + 1 < r)
      std::cout << '\n';
  }
  if (M.rows > r)
    std::cout << "\n... (remaining rows omitted)\n";
}

} // namespace m_utils
