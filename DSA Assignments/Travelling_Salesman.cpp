// Rewriting:
// 1. Generate Matrix Struct
// 2. Solver Functions
// 3. Result Struct.
// 4. The function can run multiple times so that it can start from multiple cities
// 5. Best and worst solution.
// 6. Djikstra's Algorithm for Bruteforce
// 7. Implement Greedy Algorithm
// 8. Implement third heuristic.

// tsp_no_vector.cpp
// No std::vector anywhere. Uses C-style arrays only.
// Includes own next_permutation_array implementation.
// Compile: g++ -std=c++17 -O2 tsp_no_vector.cpp -o tsp_no_vector

#include <cassert>
#include <chrono>
#include <climits>
#include <cstring>
#include <iostream>
#include <random>

using ll = long long;
static constexpr ll INF = (ll)9e15;

// ---------- Matrix (C-style) ----------
struct Matrix {
    ll **m_matrix = nullptr;
    size_t m_size{};

    Matrix(int n = 0): m_size(n), m_matrix(nullptr) {
        if (n > 0) allocate(n);
    }

    void allocate(int n) {
        freeMatrix();
        m_size = n;
        m_matrix = new ll*[n];
        for (int i = 0; i < n; ++i) m_matrix[i] = new ll[n];
    }

    Matrix(const Matrix &o) {
        m_size = o.m_size;
        if (m_size > 0) {
            m_matrix = new ll*[m_size];
            for (size_t i = 0; i < m_size; ++i) {
                m_matrix[i] = new ll[m_size];
                for (size_t j = 0; j < m_size; ++j)
                    m_matrix[i][j] = o.m_matrix[i][j];
            }
        } else {
            m_matrix = nullptr;
        }
    }

    ~Matrix() { freeMatrix(); }

    void freeMatrix() {
        if (!m_matrix) return;
        for (size_t i = 0; i < m_size; ++i) delete[] m_matrix[i];
        delete[] m_matrix;
        m_matrix = nullptr;
        m_size = 0;
    }

    Matrix* clone() const { return new Matrix(*this); }

    void generateRandomMatrix(int min_val, int max_val, bool symmetric = true, unsigned int seed = 0) {
        assert(min_val <= max_val);
        if (m_size == 0) return;
        if (seed == 0) seed = static_cast<unsigned int>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count());
        std::mt19937_64 rng(seed);
        std::uniform_int_distribution<ll> dist(min_val, max_val);
        for (size_t i = 0; i < m_size; ++i) {
            for (size_t j = 0; j < m_size; ++j) {
                if (i == j) m_matrix[i][j] = 0;
                else if (symmetric && j < i) m_matrix[i][j] = m_matrix[j][i];
                else m_matrix[i][j] = dist(rng);
            }
        }
    }

    size_t size() const { return m_size; }
    ll at(size_t i, size_t j) const { return m_matrix[i][j]; }
    ll & at(size_t i, size_t j) { return m_matrix[i][j]; }

    void print() const {
        std::cout << "Matrix " << m_size << "x" << m_size << ":\n";
        for (size_t i = 0; i < m_size; ++i) {
            for (size_t j = 0; j < m_size; ++j) {
                std::cout << m_matrix[i][j] << (j+1==m_size ? "" : " ");
            }
            std::cout << '\n';
        }
    }
};

// ---------- Result (owns C array path) ----------
struct Result {
    int *path = nullptr;
    size_t path_len = 0;
    ll cost = -1;
    long long time_ms = 0;

    Result() = default;
    // move semantics
    Result(Result &&o) noexcept {
        path = o.path; path_len = o.path_len; cost = o.cost; time_ms = o.time_ms;
        o.path = nullptr; o.path_len = 0; o.cost = -1; o.time_ms = 0;
    }
    Result& operator=(Result &&o) noexcept {
        if (this != &o) {
            clear();
            path = o.path; path_len = o.path_len; cost = o.cost; time_ms = o.time_ms;
            o.path = nullptr; o.path_len = 0; o.cost = -1; o.time_ms = 0;
        }
        return *this;
    }
    // disable copy to simplify ownership
    Result(const Result&) = delete;
    Result& operator=(const Result&) = delete;

    void clear() {
        if (path) delete[] path;
        path = nullptr;
        path_len = 0;
        cost = -1;
        time_ms = 0;
    }

    ~Result() { clear(); }

    void setPathFromArray(const int *arr, size_t len) {
        clear();
        if (len == 0) return;
        path = new int[len];
        path_len = len;
        for (size_t i = 0; i < len; ++i) path[i] = arr[i];
    }

    void print() const {
        if (!path || path_len == 0) {
            std::cout << "No valid path\n";
            return;
        }
        std::cout << "Cost: " << cost << "  Time: " << time_ms << " ms\nPath: ";
        for (size_t i = 0; i < path_len; ++i) {
            std::cout << (path[i] + 1) << (i+1==path_len ? "" : " ");
        }
        std::cout << "\n";
    }
};

// swap ints
static inline void swap_int(int &a, int &b) { int t = a; a = b; b = t; }

// lexicographic next_permutation for an int array [first..first+len)
// returns true if next permutation exists; arr is modified in-place.
// Implementation mirrors std::next_permutation.
bool next_permutation_array(int *arr, size_t len) {
    if (len < 2) return false;
    // find longest non-increasing suffix
    ssize_t i = (ssize_t)len - 2;
    while (i >= 0 && arr[i] >= arr[i+1]) --i;
    if (i < 0) {
        // last permutation; reorder to first (sorted ascending) and return false
        // reverse whole array
        size_t l = 0, r = len - 1;
        while (l < r) { swap_int(arr[l], arr[r]); ++l; --r; }
        return false;
    }
    // find rightmost element greater than arr[i]
    ssize_t j = (ssize_t)len - 1;
    while (arr[j] <= arr[i]) --j;
    swap_int(arr[i], arr[j]);
    // reverse suffix arr[i+1 .. end]
    size_t l = (size_t)i + 1, r = len - 1;
    while (l < r) { swap_int(arr[l], arr[r]); ++l; --r; }
    return true;
}

// reverse a segment arr[l..r] (inclusive)
static inline void reverse_segment(int *arr, size_t l, size_t r) {
    while (l < r) { swap_int(arr[l], arr[r]); ++l; --r; }
}

// compute tour cost for a path array of length n (closes cycle)
// alt is optional n x n ll** (APSP) else mat is used.
ll computeTourCostArray(const int *path, size_t n, const Matrix &mat, ll **alt = nullptr) {
    if (!path || n == 0) return -1;
    long long sum = 0;
    for (size_t i = 0; i + 1 < n; ++i) {
        ll w = (alt ? alt[path[i]][path[i+1]] : mat.at(path[i], path[i+1]));
        if (w >= INF) return -1;
        sum += w;
    }
    ll w = (alt ? alt[path[n-1]][path[0]] : mat.at(path[n-1], path[0]));
    if (w >= INF) return -1;
    sum += w;
    return sum;
}

// ---------- Dijkstra and APSP (matrix-based O(n^2) Dijkstra) ----------

// returns heap-allocated ll[n] distances; caller must delete[]
ll* dijkstra_as_array(const Matrix &mat, int src) {
    size_t n = mat.size();
    if (n == 0) return nullptr;
    ll *dist = new ll[n];
    bool *used = new bool[n];
    for (size_t i = 0; i < n; ++i) { dist[i] = INF; used[i] = false; }
    dist[src] = 0;
    for (size_t it = 0; it < n; ++it) {
        int v = -1;
        for (size_t i = 0; i < n; ++i) {
            if (!used[i] && (v == -1 || dist[i] < dist[v])) v = (int)i;
        }
        if (v == -1 || dist[v] == INF) break;
        used[v] = true;
        for (size_t to = 0; to < n; ++to) {
            if ((int)to == v) continue;
            ll w = mat.at(v, to);
            if (w >= INF) continue;
            if (dist[v] + w < dist[to]) dist[to] = dist[v] + w;
        }
    }
    delete[] used;
    return dist;
}

// compute APSP as ll** (n rows allocated, each row allocated). Caller must free all rows and pointer.
ll** computeAPSP_with_dijkstra(const Matrix &mat) {
    size_t n = mat.size();
    if (n == 0) return nullptr;
    ll **apsp = new ll*[n];
    for (size_t i = 0; i < n; ++i) apsp[i] = new ll[n];
    for (size_t i = 0; i < n; ++i) {
        ll *row = dijkstra_as_array(mat, (int)i);
        for (size_t j = 0; j < n; ++j) apsp[i][j] = row[j];
        delete[] row;
    }
    return apsp;
}

// ---------- Algorithms (brute, brute_apsp, greedy, heur3) ----------

// Brute force from fixed start; use_apsp toggles whether to use APSP distances
Result solveBruteForce_fixedStart(const Matrix &mat, int startIndex, bool use_apsp = false) {
    using clock = std::chrono::steady_clock;
    auto t0 = clock::now();

    Result res;
    size_t n = mat.size();
    if (n == 0 || startIndex < 0 || (size_t)startIndex >= n) return res;

    ll **apsp = nullptr;
    if (use_apsp) apsp = computeAPSP_with_dijkstra(mat);

    // prepare perm array of length n-1 (nodes != start) in ascending order
    size_t plen = (n > 0 ? n - 1 : 0);
    int *perm = (plen ? new int[plen] : nullptr);
    size_t idx = 0;
    for (size_t i = 0; i < n; ++i) if ((int)i != startIndex) perm[idx++] = (int)i;

    ll best_cost = INF;
    int *best_path = nullptr; size_t best_len = 0;

    if (plen == 0) {
        // trivial tour with single node
        best_path = new int[1];
        best_path[0] = startIndex;
        best_len = 1;
        best_cost = 0;
    } else {
        // iterate permutations lexicographically: we already initialized ascending
        bool first = true;
        while (true) {
            // build tour: tour[0] = start, then perm[0..plen-1]
            int *tour = new int[n];
            tour[0] = startIndex;
            for (size_t k = 0; k < plen; ++k) tour[k+1] = perm[k];
            // compute cost
            ll c = computeTourCostArray(tour, n, mat, apsp);
            if (c >= 0 && c < best_cost) {
                // update best
                if (best_path) delete[] best_path;
                best_len = n;
                best_path = new int[n];
                for (size_t t = 0; t < n; ++t) best_path[t] = tour[t];
                best_cost = c;
            }
            delete[] tour;
            // next permutation
            if (!next_permutation_array(perm, plen)) break;
        }
    }

    if (perm) delete[] perm;
    if (apsp) {
        for (size_t i = 0; i < n; ++i) delete[] apsp[i];
        delete[] apsp;
    }

    auto t1 = clock::now();
    if (best_path) {
        res.setPathFromArray(best_path, best_len);
        res.cost = best_cost;
        res.time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        delete[] best_path;
    }
    return res;
}

// Greedy nearest neighbor from fixed start
Result solveGreedy_fixedStart(const Matrix &mat, int startIndex) {
    using clock = std::chrono::steady_clock;
    auto t0 = clock::now();

    Result res;
    size_t n = mat.size();
    if (n == 0 || startIndex < 0 || (size_t)startIndex >= n) return res;

    bool *used = new bool[n];
    for (size_t i = 0; i < n; ++i) used[i] = false;

    int *tour = new int[n];
    size_t filled = 0;
    int cur = startIndex;
    tour[filled++] = cur;
    used[cur] = true;

    for (size_t step = 1; step < n; ++step) {
        ll best_w = INF; int best_j = -1;
        for (size_t j = 0; j < n; ++j) {
            if (used[j]) continue;
            ll w = mat.at((size_t)cur, j);
            if (w < best_w) { best_w = w; best_j = (int)j; }
        }
        if (best_j == -1) { // failed to find next
            filled = 0;
            break;
        }
        tour[filled++] = best_j;
        used[best_j] = true;
        cur = best_j;
    }
    delete[] used;

    auto t1 = clock::now();
    if (filled == n) {
        res.setPathFromArray(tour, n);
        res.cost = computeTourCostArray(res.path, res.path_len, mat, nullptr);
        res.time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    }
    delete[] tour;
    return res;
}

// 2-opt on C array tour (length n)
void run_2opt_on_array(int *tour, size_t n, const Matrix &mat) {
    if (n < 4) return;
    bool improved = true;
    auto tour_cost_arr = [&](int *t)->ll {
        long long sum = 0;
        for (size_t i = 0; i + 1 < n; ++i) {
            ll w = mat.at(t[i], t[i+1]);
            if (w >= INF) return -1;
            sum += w;
        }
        ll w = mat.at(t[n-1], t[0]);
        if (w >= INF) return -1;
        sum += w;
        return sum;
    };

    while (improved) {
        improved = false;
        ll base = tour_cost_arr(tour);
        if (base < 0) break;
        for (size_t i = 1; i + 1 < n; ++i) {
            for (size_t k = i + 1; k + 1 <= n - 1; ++k) {
                // reverse i..k
                reverse_segment(tour, i, k);
                ll cand = tour_cost_arr(tour);
                if (cand >= 0 && cand < base) {
                    improved = true;
                    base = cand;
                    goto next_iter;
                } else {
                    // undo
                    reverse_segment(tour, i, k);
                }
            }
        }
    next_iter:
        ;
    }
}

// heuristic3: greedy + 2-opt
Result solveHeuristic3_fixedStart(const Matrix &mat, int startIndex) {
    using clock = std::chrono::steady_clock;
    auto t0 = clock::now();

    Result base = solveGreedy_fixedStart(mat, startIndex);
    if (!base.path || base.path_len == 0) return base;

    int *tour = new int[base.path_len];
    for (size_t i = 0; i < base.path_len; ++i) tour[i] = base.path[i];

    run_2opt_on_array(tour, base.path_len, mat);

    auto t1 = clock::now();
    Result res;
    res.setPathFromArray(tour, base.path_len);
    res.cost = computeTourCostArray(res.path, res.path_len, mat, nullptr);
    res.time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    delete[] tour;
    return res;
}

// ---------- run all starts helpers ----------

// Func signature: Result f(const Matrix&, int start)
typedef Result (*SolverFunc)(const Matrix&, int);

// run solver f for each start, return heap-allocated Result[n] (moved into array), caller must delete[]
Result* runAllStarts_results(const Matrix &mat, SolverFunc f) {
    size_t n = mat.size();
    if (n == 0) return nullptr;
    Result *arr = new Result[n];
    for (size_t s = 0; s < n; ++s) {
        Result r = f(mat, (int)s);
        arr[s] = std::move(r); // move into slot
    }
    return arr;
}

int bestStartIndexFromResults(const Result *arr, size_t n) {
    int best = -1;
    ll best_cost = INF;
    for (size_t i = 0; i < n; ++i) {
        if (!arr[i].path || arr[i].path_len == 0) continue;
        if (arr[i].cost >= 0 && arr[i].cost < best_cost) {
            best_cost = arr[i].cost;
            best = (int)i;
        }
    }
    return best;
}

// ---------- main ----------

int main() {

    int n;
    std::cout << "Number of cities: ";
    if (!(std::cin >> n) || n <= 0) {
        std::cerr << "Invalid n\n";
        return 1;
    }

    Matrix M(n);
    M.generateRandomMatrix(1, 20, true);
    M.print();

    const int BRUTE_LIMIT = 15;
    bool allow_brute = (n <= BRUTE_LIMIT);
    if (!allow_brute) std::cout << "(n > " << BRUTE_LIMIT << " -> skipping brute-force algorithms)\n";

    // fixed list of algorithm names (C-style)
    const char *algo_names[] = { "brute", "brute_apsp", "greedy", "heur3" };
    const int algo_count = 4;

    // Summary arrays (C-style)
    bool ran[algo_count];
    int bestStart[algo_count];
    Result bestResult[algo_count]; // stores best result per algo (moved into slots)

    for (int a = 0; a < algo_count; ++a) {
        ran[a] = false;
        bestStart[a] = -1;
    }

    // 0) brute (direct)
    if (allow_brute) {
        Result *resArr = runAllStarts_results(M, [](const Matrix &mat, int s)->Result { return solveBruteForce_fixedStart(mat, s, false); });
        int best = bestStartIndexFromResults(resArr, M.size());
        if (best != -1) {
            ran[0] = true; bestStart[0] = best;
            bestResult[0] = std::move(resArr[best]);
            std::cout << "\nAlgorithm brute -> best start " << (best+1) << "\n";
            bestResult[0].print();
        } else {
            ran[0] = true; bestStart[0] = -1;
            std::cout << "\nAlgorithm brute produced no valid tours\n";
        }
        delete[] resArr;
    } else {
        ran[0] = false; bestStart[0] = -1;
    }

    // 1) brute_apsp
    if (allow_brute) {
        Result *resArr = runAllStarts_results(M, [](const Matrix &mat, int s)->Result { return solveBruteForce_fixedStart(mat, s, true); });
        int best = bestStartIndexFromResults(resArr, M.size());
        if (best != -1) {
            ran[1] = true; bestStart[1] = best;
            bestResult[1] = std::move(resArr[best]);
            std::cout << "\nAlgorithm brute_apsp -> best start " << (best+1) << "\n";
            bestResult[1].print();
        } else {
            ran[1] = true; bestStart[1] = -1;
            std::cout << "\nAlgorithm brute_apsp produced no valid tours\n";
        }
        delete[] resArr;
    } else {
        ran[1] = false; bestStart[1] = -1;
    }

    // 2) greedy
    {
        Result *resArr = runAllStarts_results(M, [](const Matrix &mat, int s)->Result { return solveGreedy_fixedStart(mat, s); });
        int best = bestStartIndexFromResults(resArr, M.size());
        if (best != -1) {
            ran[2] = true; bestStart[2] = best;
            bestResult[2] = std::move(resArr[best]);
            std::cout << "\nAlgorithm greedy -> best start " << (best+1) << "\n";
            bestResult[2].print();
        } else {
            ran[2] = true; bestStart[2] = -1;
            std::cout << "\nAlgorithm greedy produced no valid tours\n";
        }
        delete[] resArr;
    }

    // 3) heur3
    {
        Result *resArr = runAllStarts_results(M, [](const Matrix &mat, int s)->Result { return solveHeuristic3_fixedStart(mat, s); });
        int best = bestStartIndexFromResults(resArr, M.size());
        if (best != -1) {
            ran[3] = true; bestStart[3] = best;
            bestResult[3] = std::move(resArr[best]);
            std::cout << "\nAlgorithm heur3 -> best start " << (best+1) << "\n";
            bestResult[3].print();
        } else {
            ran[3] = true; bestStart[3] = -1;
            std::cout << "\nAlgorithm heur3 produced no valid tours\n";
        }
        delete[] resArr;
    }

    // pick overall best among those that ran and produced a path
    int bestAlgo = -1;
    ll bestCostVal = INF;
    for (int a = 0; a < algo_count; ++a) {
        if (!ran[a]) continue;
        if (bestStart[a] == -1) continue;
        if (bestResult[a].cost >= 0 && bestResult[a].cost < bestCostVal) {
            bestCostVal = bestResult[a].cost;
            bestAlgo = a;
        }
    }

    if (bestAlgo != -1) {
        std::cout << "\n=== OVERALL BEST ===\n";
        std::cout << "Algorithm: " << algo_names[bestAlgo] << "\n";
        std::cout << "Best start city: " << (bestStart[bestAlgo] + 1) << "\n";
        std::cout << "Score: " << bestResult[bestAlgo].cost << "\nPath: ";
        for (size_t i = 0; i < bestResult[bestAlgo].path_len; ++i)
            std::cout << (bestResult[bestAlgo].path[i] + 1) << (i+1==bestResult[bestAlgo].path_len ? "" : " ");
        std::cout << "\nTime (ms): " << bestResult[bestAlgo].time_ms << "\n";
    } else {
        std::cout << "\nNo valid results found across algorithms.\n";
    }

    // cleanup (bestResult destructors will free their paths)
    return 0;
}
