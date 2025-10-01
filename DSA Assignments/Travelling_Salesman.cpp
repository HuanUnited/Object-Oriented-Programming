#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <vector>
#include <iomanip>

using ll = long long;
using Clock = std::chrono::steady_clock;
static constexpr ll INF = std::numeric_limits<ll>::max() / 4;

// ----- Utilities -----
struct TSPResult {
    std::vector<int> path; // order of city indices (0..n-1)
    ll cost = INF;
    long long time_ms = 0;
};

// Simplified Matrix
using Matrix = std::vector<std::vector<ll>>;

// compute cost of a cyclic tour (path[0] -> path[1] -> ... -> path[n-1] -> path[0])
ll tourCost(const Matrix &mat, const std::vector<int> &path) {
    if (path.empty()) return INF;
    int n = (int)path.size();
    ll sum = 0;
    for (int i = 0; i < n - 1; ++i) {
        sum += mat[path[i]][path[i+1]];
    }
    sum += mat[path[n-1]][path[0]];
    return sum;
}

// generate random matrix with zero diagonal
Matrix generateRandomMatrix(int n, int minv, int maxv, unsigned int seed = 0, bool symmetric = false) {
    Matrix M(n, std::vector<ll>(n, 0));
    if (n <= 0) return M;
    if (seed == 0) {
        seed = (unsigned int)Clock::now().time_since_epoch().count();
    }
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<ll> dist(minv, maxv);

    if (symmetric){
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            ll v = dist(rng);
            M[i][j] = v;
            M[j][i] = v;
        }}
    return M;
    }
    else {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j){
                M[i][j] = 0;
            }
            ll v = dist(rng);
            M[i][j] = v;
        }}
    return M;    
    }
    
}

// print matrix (small)
void printMatrix(const Matrix &M) {
    int n = (int)M.size();

    if (n > 15) return; // Stops printing if matrix is too big.

    std::cout << "Matrix " << n << "x" << n << ":\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << M[i][j] << (j+1==n ? "" : " ");
        }
        std::cout << "\n";
    }
}

// ----- Brute-force exact solver -----
// For given start city s, generate all permutations of the other cities and compute costs.
// Returns best tour and also worst tour (both with start fixed at s).
// A time limit (ms) may be provided: if exceeded, function still returns whatever it has found
// but sets a flag 'timedOut' (returned as a bool).
struct BruteResultBundle {
    TSPResult best;
    TSPResult worst;
    bool timedOut = false;
};

BruteResultBundle solveBruteExact(const Matrix &mat, int startCity, long long time_limit_ms = 0) {
    BruteResultBundle out;
    
    int n = (int)mat.size(); // gets matrix size
    if (n <= 0) return out; // error halt.

    // prepare list of cities with start fixed at front
    std::vector<int> cities(n);
    for (int i = 0; i < n; ++i) cities[i] = i;
    // move startCity to index 0
    if (startCity != 0) std::swap(cities[0], cities[startCity]);

    // We'll permute cities[1..n-1] using std::vector permutation function.
    std::vector<int> current = cities;
    std::vector<int> toPerm(current.begin() + 1, current.end());

    // Best and worst path values
    ll bestCost = INF;
    std::vector<int> bestPath;
    ll worstCost = -INF;
    std::vector<int> worstPath;

    // Brute Force Solving using std::next permutation.
    auto t0 = Clock::now();
    bool timedOut = false;
    std::sort(toPerm.begin(), toPerm.end());
    do {
        // construct full path with fixed start
        std::vector<int> path;
        path.reserve(n); // avoid errors.
        path.push_back(cities[0]); //adding into path starting city
        for (int x : toPerm) path.push_back(x); // adding into path next permutation.
        ll c = tourCost(mat, path);
        if (c < bestCost) {
            bestCost = c;
            bestPath = path;
        }
        if (c > worstCost) {
            worstCost = c;
            worstPath = path;
        }

        // If past time limit // Too long.
        if (time_limit_ms > 0) {
            auto now = Clock::now();
            long long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - t0).count();
            if (elapsed > time_limit_ms) {
                timedOut = true;
                break;
            }
        }
    } while (std::next_permutation(toPerm.begin(), toPerm.end()));

    auto t1 = Clock::now();
    long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

    if (!bestPath.empty()) {
        out.best.path = std::move(bestPath); // move semantics
        out.best.cost = bestCost;
        out.best.time_ms = ms;
    }
    if (!worstPath.empty()) {
        out.worst.path = std::move(worstPath); // move semantics
        out.worst.cost = worstCost;
        out.worst.time_ms = ms;
    }
    out.timedOut = timedOut; // if timed out.
    return out;
}

// ----- Heuristic: nearest neighbor + 2-opt -----
/*
Takes in the matrix and starting city, uses std pair to output the algorithm's chosen path and score.
*/
std::pair<std::vector<int>, ll> nearestNeighbor(const Matrix &mat, int startCity) {
    int n = (int)mat.size();
    std::vector<int> path;
    path.reserve(n); //error.
    std::vector<char> used(n, 0); //Checks to see if city has been used
    int cur = startCity;
    path.push_back(cur);
    used[cur] = 1; // Marks starting city.

    for (int step = 1; step < n; ++step) { // nearestNeighbor selection.
        int bestNext = -1;
        ll bestCost = INF;
        for (int j = 0; j < n; ++j) {
            if (used[j]) continue;
            if (mat[cur][j] < bestCost) {
                bestCost = mat[cur][j];
                bestNext = j;
            }
        }
        if (bestNext == -1) break;
        path.push_back(bestNext);
        used[bestNext] = 1;
        cur = bestNext;
    }

    // if something went wrong (not all cities added), fill remaining
    for (int j = 0; j < n; ++j) if (!used[j]) path.push_back(j);

    ll c = tourCost(mat, path);
    return {path, c};
}

// 2-opt improvement (in-place modifies path)
/*Opt 2 Algorithm
https://www.youtube.com/watch?v=wsEzZ4F_bS4
*/
bool tryTwoOptImprove(const Matrix &mat, std::vector<int> &path, ll &costRef) {
    int n = (int)path.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int k = i + 1; k < n; ++k) {
            int a = path[i];
            int b = path[(i+1) % n];
            int c = path[k];
            int d = path[(k+1) % n];
            ll delta = - mat[a][b] - mat[c][d] + mat[a][c] + mat[b][d];
            if (delta < 0) {
                std::reverse(path.begin() + i + 1, path.begin() + k + 1);
                costRef += delta;
                return true;
            }
        }
    }
    return false;
}

std::pair<std::vector<int>, ll> heuristicWith2Opt(const Matrix &mat, int startCity, int max2optIter = 1000) {
    auto [path, cost] = nearestNeighbor(mat, startCity); // See if 2 opt will improve the nearest neighbor heuristics.
    int iter = 0;
    while (iter++ < max2optIter) {
        bool improved = tryTwoOptImprove(mat, path, cost);
        if (!improved) break;
    }
    return {path, cost};
}

// ----- Experiment runner & reporting -----
// compute quality percent where:
// 0% -> heuristic cost == worstCost
// 100% -> heuristic cost == bestCost
double computeQualityPercent(ll heuristicCost, ll bestCost, ll worstCost) {
    if (bestCost == INF || worstCost == -INF) return 0.0;
    if (bestCost == worstCost) {
        return (heuristicCost == bestCost) ? 100.0 : 0.0;
    }
    double numerator = double(worstCost - heuristicCost);
    double denom = double(worstCost - bestCost);
    double p = (numerator / denom) * 100.0;
    if (p < 0.0) p = 0.0;
    if (p > 100.0) p = 100.0;
    return p;
}

// New: runExperiments accepts startCity:
//  - if startCity == -1 -> run over all starts (previous behavior)
//  - if startCity in [0..n-1] -> run only that start for brute and heuristic
void runExperiments(const std::vector<int> &sizes, int runsPerSize,
                    int minCost, int maxCost, unsigned int seedBase = 0,
                    int brute_time_limit_ms = 10000 /*10s*/,
                    int startCity = -1 /* -1 => all starts */) {

    std::cout << "TSP experiments\n";
    std::cout << "Sizes:";
    for (int s: sizes) std::cout << " " << s;
    std::cout << "; runs per size: " << runsPerSize << "\n";
    std::cout << "Cost range: [" << minCost << "," << maxCost << "]\n";
    if (startCity < 0) std::cout << "Start city: ALL (will test every possible start)\n";
    else std::cout << "Start city: " << startCity << " (only this start)\n";
    std::cout << "Brute time limit (per-start): " << brute_time_limit_ms << " ms\n\n";

    for (int n : sizes) {
        std::cout << "=== Matrix size: " << n << " ===\n";
        double avgBruteTime = 0.0;
        double avgHeurTime = 0.0;
        int completedBruteRuns = 0;
        for (int run = 0; run < runsPerSize; ++run) {
            unsigned int seed = seedBase + (unsigned int)(n*1000 + run);
            Matrix M = generateRandomMatrix(n, minCost, maxCost, seed); // non symmetric
            std::cout << "[Run " << (run+1) << "/" << runsPerSize << "] seed=" << seed << "\n";

            const int BRUTE_ALLOW_N = 15;
            bool allowBrute = (n <= BRUTE_ALLOW_N);

            ll globalBestCost = INF, globalWorstCost = -INF;
            std::vector<int> globalBestPath, globalWorstPath;
            long long bruteTotalMs = 0;
            bool bruteExceeded = false;

            // decide which starts to run
            std::vector<int> startsToTest;
            if (startCity >= 0) {
                if (startCity < n) startsToTest.push_back(startCity);
                else {
                    std::cout << "  Warning: requested startCity " << startCity << " is >= n=" << n << ". Will run all starts instead.\n";
                    for (int s = 0; s < n; ++s) startsToTest.push_back(s);
                }
            } else {
                for (int s = 0; s < n; ++s) startsToTest.push_back(s);
            }

            if (allowBrute) {
                for (int s : startsToTest) {
                    auto t0 = Clock::now();
                    BruteResultBundle bundle = solveBruteExact(M, s, brute_time_limit_ms);
                    auto t1 = Clock::now();
                    long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
                    bruteTotalMs += ms;
                    if (bundle.timedOut) {
                        bruteExceeded = true;
                        std::cout << "  Brute (start " << s << ") timed out after " << ms << " ms\n";
                    } else {
                        std::cout << "  Brute (start " << s << "): best=" << bundle.best.cost
                                  << ", worst=" << bundle.worst.cost << ", time=" << bundle.best.time_ms << " ms\n";
                    }
                    if (bundle.best.cost < globalBestCost) {
                        globalBestCost = bundle.best.cost;
                        globalBestPath = bundle.best.path;
                    }
                    if (bundle.worst.cost > globalWorstCost) {
                        globalWorstCost = bundle.worst.cost;
                        globalWorstPath = bundle.worst.path;
                    }
                }
                if (!bruteExceeded) completedBruteRuns++;
                avgBruteTime += double(bruteTotalMs);
            } else {
                std::cout << "  (Skipping brute-force for n=" << n << " — too large.)\n";
            }

            // Heuristic: either single start or test all starts and pick best heuristic result across starts
            ll bestHeurCost = INF;
            std::vector<int> bestHeurPath;
            long long heurTotalMs = 0;
            for (int s : startsToTest) {
                auto t0 = Clock::now();
                auto [path, cost] = heuristicWith2Opt(M, s, 1000);
                auto t1 = Clock::now();
                long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
                heurTotalMs += ms;
                if (cost < bestHeurCost) {
                    bestHeurCost = cost;
                    bestHeurPath = path;
                }
                std::cout << "  Heuristic (start " << s << "): cost=" << cost << ", time(ms)=" << ms << "\n";
            }
            avgHeurTime += double(heurTotalMs);

            std::cout << "  Best heuristic (over tested starts) cost=" << bestHeurCost << ", total heuristic time(ms)=" << heurTotalMs << "\n";

            if (allowBrute && globalBestCost != INF && globalWorstCost != -INF) {
                double quality = computeQualityPercent(bestHeurCost, globalBestCost, globalWorstCost);
                std::cout << "  Brute overall best=" << globalBestCost << ", worst=" << globalWorstCost << ", brute_total_time(ms)=" << bruteTotalMs << "\n";
                std::cout << "  Quality of heuristic vs brute: " << std::fixed << std::setprecision(2) << quality << " %\n";
            } else {
                std::cout << "  No brute results available for comparison (n too large or timed out)\n";
            }

            if (allowBrute && bruteExceeded) {
                std::cout << "  NOTE: brute search timed out for at least one tested start — n=" << n << " may be impractical for exact search under current time limit\n";
            }
            std::cout << "\n";
        } // end runsPerSize

        if (completedBruteRuns > 0) {
            avgBruteTime /= (double)runsPerSize;
            std::cout << "Summary for n=" << n << ": average brute time per run (ms): " << avgBruteTime << "\n";
        } else {
            std::cout << "Summary for n=" << n << ": brute not completed in any run or skipped.\n";
        }
        avgHeurTime /= (double)runsPerSize;
        std::cout << "Summary for n=" << n << ": average heuristic time per run (ms total over tested starts): " << avgHeurTime << "\n\n";
    }
}

// ----- main: interactive start choice -----
int main() {

    int number_of_sizes{};
    std::cout << "input number of sizes: ";
    std::cin >> number_of_sizes;
    std::cout << '\n';
    if(number_of_sizes < 0) {std::cout <<"Number of sizes smaller than zero"<< '\n'; return 0;} //error check

    std::vector<int> sizes{};
    std::cout << "input number of cities: ";
    for (int i = 0; i < number_of_sizes; i++){
        int x{};
        std::cin >> x;
        if(x <= 2) {std::cout <<"number of cities smaller than 2" << '\n'; return 0;} //error check
        sizes.push_back(x);
    }
    std::cout << '\n';

    int runsPerSize{};
    std::cout << "input number of runs per size: ";
    std::cin >> runsPerSize;
    if(runsPerSize < 1) {std::cout <<"number of runs smaller than 0" << '\n'; return 0;} //error check
    std::cout << '\n';

    int minCost = 1;
    int maxCost = 20;
    unsigned int seedBase = 12345u;
    int brute_time_limit_ms = 8000; // per-start time limit (ms)

    std::cout << "TSP experiment runner (exact + heuristic)\n";
    std::cout << "Default sizes: ";
    for (int s : sizes) std::cout << s << " ";
    std::cout << "\n";

    std::cout << "Enter starting city index to test (0 .. n-1), or -1 to test ALL starts: ";
    int startCityInput = -1;
    std::cin >> startCityInput;
    if (!std::cin) {
        std::cerr << "Invalid input. Exiting.\n";
        return 1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Press Enter to run experiments with startCity=" << startCityInput << " (or Ctrl-C to cancel)...\n";
    std::cin.get();

    runExperiments(sizes, runsPerSize, minCost, maxCost, seedBase, brute_time_limit_ms, startCityInput);

    std::cout << "Done.\n";
    return 0;
}
