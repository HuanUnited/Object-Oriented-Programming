// Rewriting:
// 1. Generate Matrix Struct
// 2. Solver Functions
// 3. Result Struct.
// 4. The function can run multiple times so that it can start from multiple cities
// 5. Best and worst solution.
// 6. Djikstra's Algorithm for Bruteforce
// 7. Implement Greedy Algorithm
// 8. Implement third heuristic.

#include <cassert>
#include<iostream>
#include <random>
#include <chrono>
using ll = long long;


struct Matrix{
ll **m_matrix = nullptr;
size_t m_size{}; 

  Matrix(int n = 0): m_size(n), m_matrix(nullptr) {
        if (n > 0) allocate(n);
    }

    void allocate(int n) {
        freeMatrix();
        m_size = n;
        m_matrix = new ll*[n];
        for (int i = 0; i < n; ++i) {
            m_matrix[i] = new ll[n];
        }
    }

     Matrix(const Matrix &o) {
        m_size = o.m_size;
        if (m_size > 0) {
            m_matrix = new ll*[m_size];
            for (int i = 0; i < m_size; ++i) {
                m_matrix[i] = new ll[m_size];
                for (int j = 0; j < m_size; ++j) m[i][j] = o.m[i][j];
            }
        } else m_matrix = nullptr;
    }~Matrix() { freeMatrix(); }

    void freeMatrix() {
        if (!m_matrix) return;
        for (int i = 0; i < m_size; ++i) delete[] m_matrix[i];
        delete[] m_matrix;
        m_matrix = nullptr;
        m_size = 0;
    }

    // clone (returns heap-allocated copy)
    Matrix* clone() const {
        return new Matrix(*this); // uses copy ctor
    }

    // generate random symmetric or asymmetric matrix with costs in [min_val, max_val]
    void generateRandomMatrix(int min_val, int max_val, bool symmetric = true, unsigned int seed = 0) {
        assert(min_val <= max_val);
        if (m_size <= 0) return;
        if (seed == 0) seed = static_cast<unsigned int>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count());
        std::mt19937_64 rng(seed);
        std::uniform_int_distribution<ll> dist(min_val, max_val);

        for (int i = 0; i < m_size; ++i) {
            for (int j = 0; j < m_size; ++j) {
                if (i == j) {
                    m_matrix[i][j] = 0; // no self-loop cost
                } else if (symmetric && j < i) {
                    // copy from earlier
                    m_matrix[i][j] = m_matrix[j][i];
                } else {
                    m_matrix[i][j] = dist(rng);
                }
            }
        }
    }
};

struct Result{


};

