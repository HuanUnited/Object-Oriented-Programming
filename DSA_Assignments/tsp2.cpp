#include <algorithm>
#include <chrono>
#include <cstddef>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

using ll = long long;
using Clock = std::chrono::steady_clock;
static constexpr ll INF = std::numeric_limits<ll>::max() / 4;

// ----- Utilities -----
struct TSPResult {
  std::vector<int> path;
  ll cost = INF;
  long long time_ms = 0;
};

using Matrix = std::vector<std::vector<ll>>;

ll tourCost(const Matrix &mat, const std::vector<int> &path) {
  if (path.empty())
    return INF;
  int n = (int)path.size();
  ll sum = 0;
  for (int i = 0; i < n - 1; ++i) {
    sum += mat[path[i]][path[i + 1]];
  }
  sum += mat[path[n - 1]][path[0]];
  return sum;
}

Matrix generateRandomMatrix(int n, int minv = 1, int maxv = 20,
                            unsigned int seed = 0, bool symmetric = false) {
  Matrix M(n, std::vector<ll>(n, 0));
  if (n <= 0)
    return M;
  if (seed == 0) {
    seed = (unsigned int)Clock::now().time_since_epoch().count();
  }
  std::mt19937_64 rng(seed);
  std::uniform_int_distribution<ll> dist(minv, maxv);

  if (symmetric) {
    for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j < n; ++j) {
        int v = dist(rng);
        M[i][j] = v;
        M[j][i] = v;
      }
    }
  } else {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        if (i == j)
          M[i][j] = 0;
        else
          M[i][j] = dist(rng);
      }
    }
  }
  return M;
}

void printMatrix(const Matrix &M) {
  int n = (int)M.size();
  if (n > 15)
    return;
  std::cout << "Matrix " << n << "x" << n << ":\n";
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cout << M[i][j] << (j + 1 == n ? "" : " ");
    }
    std::cout << "\n";
  }
}

// ----- Brute-force exact solver -----
struct BruteResultBundle {
  TSPResult best;
  TSPResult worst;
  bool timedOut = false;
};

BruteResultBundle solveBruteExact(const Matrix &mat, int startCity,
                                  long long time_limit_ms = 0) {
  BruteResultBundle out;
  int n = (int)mat.size();
  if (n <= 0)
    return out;

  std::vector<int> cities(n);
  for (int i = 0; i < n; ++i)
    cities[i] = i;
  if (startCity != 0)
    std::swap(cities[0], cities[startCity]);

  std::vector<int> current = cities;
  std::vector<int> toPerm(current.begin() + 1, current.end());

  ll bestCost = INF;
  std::vector<int> bestPath;
  ll worstCost = -INF;
  std::vector<int> worstPath;

  auto t0 = Clock::now();
  bool timedOut = false;
  std::sort(toPerm.begin(), toPerm.end());
  do {
    std::vector<int> path;
    path.reserve(n);
    path.push_back(cities[0]);
    for (int x : toPerm)
      path.push_back(x);
    ll c = tourCost(mat, path);
    if (c < bestCost) {
      bestCost = c;
      bestPath = path;
    }
    if (c > worstCost) {
      worstCost = c;
      worstPath = path;
    }

    if (time_limit_ms > 0) {
      auto now = Clock::now();
      long long elapsed =
          std::chrono::duration_cast<std::chrono::milliseconds>(now - t0)
              .count();
      if (elapsed > time_limit_ms) {
        timedOut = true;
        break;
      }
    }
  } while (std::next_permutation(toPerm.begin(), toPerm.end()));

  auto t1 = Clock::now();
  long long ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

  if (!bestPath.empty()) {
    out.best.path = std::move(bestPath);
    out.best.cost = bestCost;
    out.best.time_ms = ms;
  }
  if (!worstPath.empty()) {
    out.worst.path = std::move(worstPath);
    out.worst.cost = worstCost;
    out.worst.time_ms = ms;
  }
  out.timedOut = timedOut;
  return out;
}

// ----- 2-opt Local Search Improvement -----
// Improves a given tour using 2-opt swaps
// Локальный поиск воспользуя 2-opt
std::pair<std::vector<int>, ll> twoOpt(const Matrix &mat,
                                       std::vector<int> path) {
  int n = (int)path.size();
  if (n <= 3)
    return {path, tourCost(mat, path)};

  bool improved = true;
  while (improved) {
    improved = false;
    for (int i = 0; i < n - 1; ++i) {
      for (int j = i + 2; j < n; ++j) {
        // Calculate current cost of edges (i, i+1) and (j, j+1)
        int next_i = (i + 1) % n;
        int next_j = (j + 1) % n;

        ll currentCost =
            mat[path[i]][path[next_i]] + mat[path[j]][path[next_j]];
        ll newCost = mat[path[i]][path[j]] + mat[path[next_i]][path[next_j]];

        if (newCost < currentCost) {
          // Reverse the segment between i+1 and j
          std::reverse(path.begin() + i + 1, path.begin() + j + 1);
          improved = true;
        }
      }
    }
  }

  return {path, tourCost(mat, path)};
}

// ----- Heuristic 1: Nearest Neighbor -----
std::pair<std::vector<int>, ll> nearestNeighbor(const Matrix &mat,
                                                int startCity) {
  int n = (int)mat.size();
  std::vector<int> path;
  path.reserve(n);
  std::vector<char> used(n, 0);
  int cur = startCity;
  path.push_back(cur);
  used[cur] = 1;

  for (int step = 1; step < n; ++step) {
    int bestNext = -1;
    ll bestCost = INF;
    for (int j = 0; j < n; ++j) {
      if (used[j])
        continue;
      if (mat[cur][j] < bestCost) {
        bestCost = mat[cur][j];
        bestNext = j;
      }
    }
    if (bestNext == -1)
      break;
    path.push_back(bestNext);
    used[bestNext] = 1;
    cur = bestNext;
  }

  for (int j = 0; j < n; ++j)
    if (!used[j])
      path.push_back(j);

  ll c = tourCost(mat, path);
  return {path, c};
}

// ----- Heuristic 2: Cheapest Insertion -----
std::pair<std::vector<int>, ll> cheapestInsertion(const Matrix &mat,
                                                  int startCity) {
  int n = (int)mat.size();
  if (n <= 0)
    return {{}, INF};

  std::vector<int> path;
  std::vector<bool> used(n, false);

  // Start with initial city
  path.push_back(startCity);
  used[startCity] = true;

  // Find nearest city to start
  if (n > 1) {
    int nearest = -1;
    ll minDist = INF;
    for (int i = 0; i < n; ++i) {
      if (i != startCity && mat[startCity][i] < minDist) {
        minDist = mat[startCity][i];
        nearest = i;
      }
    }
    path.push_back(nearest);
    used[nearest] = true;
  }

  // Insert remaining cities at position with minimum cost increase
  while (path.size() < (size_t)n) {
    int bestCity = -1;
    int bestPos = -1;
    ll bestIncrease = INF;

    // For each unvisited city
    for (int city = 0; city < n; ++city) {
      if (used[city])
        continue;

      // Try inserting at each position in current path
      for (size_t pos = 0; pos < path.size(); ++pos) {
        int prev = path[pos];
        int next = path[(pos + 1) % path.size()];

        // Cost increase = cost(prev->city) + cost(city->next) -
        // cost(prev->next)
        ll increase = mat[prev][city] + mat[city][next] - mat[prev][next];

        if (increase < bestIncrease) {
          bestIncrease = increase;
          bestCity = city;
          bestPos = pos + 1;
        }
      }
    }

    if (bestCity != -1) {
      path.insert(path.begin() + bestPos, bestCity);
      used[bestCity] = true;
    } else {
      break;
    }
  }

  ll cost = tourCost(mat, path);
  return {path, cost};
}

// ----- Heuristic 3: Greedy (Sorted Edges) -----
std::pair<std::vector<int>, ll> greedySortedEdges(const Matrix &mat,
                                                  int startCity) {
  int n = (int)mat.size();
  if (n <= 0)
    return {{}, INF};

  // Create list of all edges with costs
  struct Edge {
    int from, to;
    ll cost;
    bool operator<(const Edge &other) const { return cost < other.cost; }
  };

  std::vector<Edge> edges;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (i != j) {
        edges.push_back({i, j, mat[i][j]});
      }
    }
  }

  // Sort edges by cost
  std::sort(edges.begin(), edges.end());

  // Track degree of each node (max 2 for TSP)
  std::vector<int> degree(n, 0);
  std::vector<std::vector<int>> adj(n);

  // Add edges greedily while maintaining TSP constraints
  int edgesAdded = 0;
  for (const auto &edge : edges) {
    if (edgesAdded >= n)
      break;

    // Check if adding this edge would violate constraints
    if (degree[edge.from] >= 2 || degree[edge.to] >= 2)
      continue;

    // Check if adding this edge would create a premature cycle
    if (edgesAdded < n - 1) {
      // Would this edge close a cycle?
      std::vector<bool> visited(n, false);
      std::function<bool(int, int, int)> dfs = [&](int node, int target,
                                                   int parent) -> bool {
        visited[node] = true;
        if (node == target)
          return true;
        for (int next : adj[node]) {
          if (next == parent)
            continue;
          if (visited[next])
            continue;
          if (dfs(next, target, node))
            return true;
        }
        return false;
      };

      if (dfs(edge.from, edge.to, -1))
        continue;
    }

    // Add edge
    adj[edge.from].push_back(edge.to);
    adj[edge.to].push_back(edge.from);
    degree[edge.from]++;
    degree[edge.to]++;
    edgesAdded++;
  }

  // Build path from adjacency list starting from startCity
  std::vector<int> path;
  std::vector<bool> visited(n, false);
  int current = startCity;
  path.push_back(current);
  visited[current] = true;

  while (path.size() < (size_t)n) {
    int next = -1;
    for (int neighbor : adj[current]) {
      if (!visited[neighbor]) {
        next = neighbor;
        break;
      }
    }
    if (next == -1) {
      // Find unvisited city
      for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
          next = i;
          break;
        }
      }
      if (next == -1)
        break;
    }
    path.push_back(next);
    visited[next] = true;
    current = next;
  }

  ll cost = tourCost(mat, path);
  return {path, cost};
}

// ----- Heuristic 4: Farthest Insertion -----
std::pair<std::vector<int>, ll> farthestInsertion(const Matrix &mat,
                                                  int startCity) {
  int n = (int)mat.size();
  if (n <= 0)
    return {{}, INF};

  std::vector<int> path;
  std::vector<bool> used(n, false);

  path.push_back(startCity);
  used[startCity] = true;

  // Find farthest city from start
  if (n > 1) {
    int farthest = -1;
    ll maxDist = -INF;
    for (int i = 0; i < n; ++i) {
      if (i != startCity && mat[startCity][i] > maxDist) {
        maxDist = mat[startCity][i];
        farthest = i;
      }
    }
    path.push_back(farthest);
    used[farthest] = true;
  }

  // Insert remaining cities
  while (path.size() < (size_t)n) {
    // Find farthest city from current tour
    int farthestCity = -1;
    ll maxMinDist = -INF;

    for (int city = 0; city < n; ++city) {
      if (used[city])
        continue;

      // Find minimum distance from this city to any city in tour
      ll minDist = INF;
      for (int inTour : path) {
        minDist = std::min(minDist, mat[city][inTour]);
      }

      if (minDist > maxMinDist) {
        maxMinDist = minDist;
        farthestCity = city;
      }
    }

    if (farthestCity == -1)
      break;

    // Find best position to insert this city
    int bestPos = -1;
    ll bestIncrease = INF;

    for (size_t pos = 0; pos < path.size(); ++pos) {
      int prev = path[pos];
      int next = path[(pos + 1) % path.size()];

      ll increase =
          mat[prev][farthestCity] + mat[farthestCity][next] - mat[prev][next];

      if (increase < bestIncrease) {
        bestIncrease = increase;
        bestPos = pos + 1;
      }
    }

    path.insert(path.begin() + bestPos, farthestCity);
    used[farthestCity] = true;
  }

  ll cost = tourCost(mat, path);
  return {path, cost};
}

// ----- Experiment runner -----
double computeQualityPercent(ll heuristicCost, ll bestCost, ll worstCost) {
  if (bestCost == INF || worstCost == -INF)
    return 0.0;
  if (bestCost == worstCost) {
    return (heuristicCost == bestCost) ? 100.0 : 0.0;
  }
  double numerator = double(worstCost - heuristicCost);
  double denom = double(worstCost - bestCost);
  double p = (numerator / denom) * 100.0;
  if (p < 0.0)
    p = 0.0;
  if (p > 100.0)
    p = 100.0;
  return p;
}

void runExperiments(const std::vector<int> &sizes, int runsPerSize, int minCost,
                    int maxCost, unsigned int seedBase = 0,
                    int brute_time_limit_ms = 10000, int startCity = -1,
                    bool symmetric = false, bool use2opt = false) {

  std::cout << "TSP experiments with multiple heuristics\n";
  std::cout << "Sizes:";
  for (int s : sizes)
    std::cout << " " << s;
  std::cout << "; runs per size: " << runsPerSize << "\n";
  std::cout << "Cost range: [" << minCost << "," << maxCost << "]\n";
  if (startCity < 0)
    std::cout << "Start city: ALL (will test every possible start)\n";
  else
    std::cout << "Start city: " << startCity << " (only this start)\n";
  std::cout << "2-opt improvement: " << (use2opt ? "ENABLED" : "DISABLED")
            << "\n";
  std::cout << "Brute time limit (per-start): " << brute_time_limit_ms
            << " ms\n\n";

  for (int n : sizes) {
    std::cout << "=== Matrix size: " << n << " ===\n";
    double avgBruteTime = 0.0;
    std::vector<double> avgHeurTimes(4, 0.0); // 4 heuristics
    int completedBruteRuns = 0;

    for (int run = 0; run < runsPerSize; ++run) {
      unsigned int seed = seedBase + (unsigned int)(n * 1000 + run);
      Matrix M = generateRandomMatrix(n, minCost, maxCost, seed, symmetric);

      std::cout << "[Run " << (run + 1) << "/" << runsPerSize
                << "] seed=" << seed << "\n";
      printMatrix(M);

      const int BRUTE_ALLOW_N = 11;
      bool allowBrute = (n <= BRUTE_ALLOW_N);

      ll globalBestCost = INF, globalWorstCost = -INF;
      std::vector<int> globalBestPath, globalWorstPath;
      long long bruteTotalMs = 0;
      bool bruteExceeded = false;

      std::vector<int> startsToTest;
      if (startCity >= 0) {
        if (startCity < n)
          startsToTest.push_back(startCity);
        else {
          std::cout << "  Warning: requested startCity " << startCity
                    << " is >= n=" << n << ". Will run all starts instead.\n";
          for (int s = 0; s < n; ++s)
            startsToTest.push_back(s);
        }
      } else {
        for (int s = 0; s < n; ++s)
          startsToTest.push_back(s);
      }

      if (allowBrute) {
        for (int s : startsToTest) {
          auto t0 = Clock::now();
          BruteResultBundle bundle = solveBruteExact(M, s, brute_time_limit_ms);
          auto t1 = Clock::now();
          long long ms =
              std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0)
                  .count();
          bruteTotalMs += ms;
          if (bundle.timedOut) {
            bruteExceeded = true;
            std::cout << "  Brute (start " << s << ") timed out after " << ms
                      << " ms\n";
          } else {
            std::cout << "  Brute (start " << s
                      << "): best=" << bundle.best.cost
                      << ", worst=" << bundle.worst.cost
                      << ", time=" << bundle.best.time_ms << " ms\n";
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
        if (!bruteExceeded)
          completedBruteRuns++;
        avgBruteTime += double(bruteTotalMs);
      } else {
        std::cout << "  (Skipping brute-force for n=" << n
                  << " — too large.)\n";
      }

      // Test all heuristics
      std::vector<std::string> heurNames = {
          "Nearest Neighbor", "Cheapest Insertion", "Greedy (Sorted Edges)",
          "Farthest Insertion"};

      std::vector<ll> bestHeurCosts(4, INF);
      std::vector<long long> heurTotalMs(4, 0);

      for (int s : startsToTest) {
        // 1. Nearest Neighbor
        auto t0 = Clock::now();
        auto [path1, cost1] = nearestNeighbor(M, s);
        if (use2opt)
          std::tie(path1, cost1) = twoOpt(M, path1);
        auto t1 = Clock::now();
        long long ms1 =
            std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0)
                .count();
        heurTotalMs[0] += ms1;
        if (cost1 < bestHeurCosts[0])
          bestHeurCosts[0] = cost1;

        // 2. Cheapest Insertion
        t0 = Clock::now();
        auto [path2, cost2] = cheapestInsertion(M, s);
        if (use2opt)
          std::tie(path2, cost2) = twoOpt(M, path2);
        t1 = Clock::now();
        long long ms2 =
            std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0)
                .count();
        heurTotalMs[1] += ms2;
        if (cost2 < bestHeurCosts[1])
          bestHeurCosts[1] = cost2;

        // 3. Greedy
        t0 = Clock::now();
        auto [path3, cost3] = greedySortedEdges(M, s);
        if (use2opt)
          std::tie(path3, cost3) = twoOpt(M, path3);
        t1 = Clock::now();
        long long ms3 =
            std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0)
                .count();
        heurTotalMs[2] += ms3;
        if (cost3 < bestHeurCosts[2])
          bestHeurCosts[2] = cost3;

        // 4. Farthest Insertion
        t0 = Clock::now();
        auto [path4, cost4] = farthestInsertion(M, s);
        if (use2opt)
          std::tie(path4, cost4) = twoOpt(M, path4);
        t1 = Clock::now();
        long long ms4 =
            std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0)
                .count();
        heurTotalMs[3] += ms4;
        if (cost4 < bestHeurCosts[3])
          bestHeurCosts[3] = cost4;
      }

      // Report results
      for (int h = 0; h < 4; ++h) {
        std::cout << "  " << heurNames[h] << (use2opt ? " + 2-opt" : "")
                  << ": best_cost=" << bestHeurCosts[h]
                  << ", total_time(ms)=" << heurTotalMs[h] << "\n";
        avgHeurTimes[h] += double(heurTotalMs[h]);

        if (allowBrute && globalBestCost != INF && globalWorstCost != -INF) {
          double quality = computeQualityPercent(
              bestHeurCosts[h], globalBestCost, globalWorstCost);
          std::cout << "    Quality vs brute: " << std::fixed
                    << std::setprecision(2) << quality << " %\n";
        }
      }

      if (allowBrute && globalBestCost != INF) {
        std::cout << "  Brute overall: best=" << globalBestCost
                  << ", worst=" << globalWorstCost
                  << ", total_time(ms)=" << bruteTotalMs << "\n";
      }

      std::cout << "\n";
    }

    // Summary
    if (completedBruteRuns > 0) {
      avgBruteTime /= (double)runsPerSize;
      std::cout << "Summary for n=" << n
                << ": average brute time per run (ms): " << avgBruteTime
                << "\n";
    }
    for (int h = 0; h < 4; ++h) {
      avgHeurTimes[h] /= (double)runsPerSize;
      std::cout << "Summary for n=" << n << ": average "
                << (h == 0   ? "Nearest Neighbor"
                    : h == 1 ? "Cheapest Insertion"
                    : h == 2 ? "Greedy"
                             : "Farthest Insertion")
                << " time (ms): " << avgHeurTimes[h] << "\n";
    }
    std::cout << "\n";
  }
}

int main() {
  int number_of_sizes{};
  std::cout << "input number of sizes: ";
  std::cin >> number_of_sizes;
  std::cout << "Number of sizes of matrices are: " << number_of_sizes << '\n';
  if (number_of_sizes < 0) {
    std::cout << "Number of sizes smaller than zero" << '\n';
    return 0;
  }

  std::vector<int> sizes{};
  for (int i = 0; i < number_of_sizes; i++) {
    std::cout << "input number of cities for size " << i << ": ";
    int x{};
    std::cin >> x;
    if (x <= 2) {
      std::cout << "\n number of cities smaller than 2" << '\n';
      return 0;
    }
    sizes.push_back(x);
    std::cout << "\n size " << i << " has " << x << "cities." << '\n';
  }

  int runsPerSize{};
  std::cout << "input number of runs per size: ";
  std::cin >> runsPerSize;
  if (runsPerSize < 1) {
    std::cout << "number of runs smaller than 0" << '\n';
    return 0;
  }
  std::cout << '\n';

  bool symmetric{};
  std::cout << "symmetric? - 1 for yes, 0 for no: ";
  size_t t;
  std::cin >> t;
  if (t == 1)
    symmetric = true;
  else if (t == 0)
    symmetric = false;
  else {
    std::cout << "error, only 0 or 1." << '\n';
    return 0;
  }
  std::cout << '\n';

  bool use2opt{};
  std::cout << "use 2-opt improvement? - 1 for yes, 0 for no: ";
  std::cin >> t;
  if (t == 1)
    use2opt = true;
  else if (t == 0)
    use2opt = false;
  else {
    std::cout << "error, only 0 or 1." << '\n';
    return 0;
  }
  std::cout << '\n';

  int minCost = 1;
  int maxCost = 20;
  unsigned int seedBase = 12345u;
  int brute_time_limit_ms = 8000;

  std::cout << "TSP experiment runner with multiple heuristics\n";
  std::cout << "Enter starting city index, or -1 to test ALL starts: ";

  int startCityInput = -1;
  std::cin >> startCityInput;
  if (!std::cin) {
    std::cerr << "Invalid input. Exiting.\n";
    return 1;
  }
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  std::cout << "Press Enter to run experiments...\n";
  std::cin.get();

  runExperiments(sizes, runsPerSize, minCost, maxCost, seedBase,
                 brute_time_limit_ms, startCityInput, symmetric, use2opt);

  std::cout << "Done.\n";
  return 0;
}