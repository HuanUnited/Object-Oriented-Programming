// Iteration number 2 of the first assignment, this should have been named DSA,
// but I forgot, so it is technically DSA

// Changes made
/*
1. Added print all permutations function.
2. 2D matrix with a function calculates all possible permuation.
2. A function that swaps the selected city to the beginning of the city array.
<for brute-force calculations> 2.1. A search function that searches for the city
in that array.
3. A distance score calculation method.
4. A global best distance tracker.

5. Changed string cities, to int cities. Better tracking, more cities, easier
indexing for cities.
+ By default cities will be organized by their rows and columns.
+ By choosing a starting city, the rows and columns of that city will be moved
to the front of the matrix.

*/

#include <iostream>
using namespace std;

#define N 5
int matrix[N][N] = {
    {0,2,3,5,7},
    {3,0,9,2,4},
    {1,3,0,4,2},
    {4,2,1,0,1},
    {6,3,1,5,0},
};

int best_cost = 1e9;
int best_path[100];

void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

int calcCost(int *route, int len, int matrix[N][N]) {
    int sum = 0;
    for (int i = 0; i < len - 1; i++) {
        int from = route[i];
        int to   = route[i + 1];
        sum += matrix[from][to];
    }
    // close the cycle: return to starting city (always at index 0 now)
    sum += matrix[route[len - 1]][route[0]];
    return sum;
}

void intcpy(int *dest, int *src, int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

void printRoute(int *route, int len) {
    for (int i = 0; i < len; i++) {
        cout << route[i] << " ";
    }
}

void permute(int *a, int l, int r, int matrix[N][N]) {
    if (l == r) {
        int cost = calcCost(a, r + 1, matrix);
        cout << "Path: ";
        for (int i = 0; i <= r; i++) cout << a[i] << " ";
        cout << " cost= " << cost << endl;

        if (cost < best_cost) {
            best_cost = cost;
            for (int i = 0; i <= r; i++){
                best_path[i] = a[i];
            }
            cout << "Current best cost = " << best_cost << endl;
            cout << "Best path = ";
            printRoute(best_path, r + 1);
            cout << endl;
        }
    } else {
        for (int i = l; i <= r; i++) {
            swap((a + l), (a + i));
            permute(a, l + 1, r, matrix);
            swap((a + l), (a + i)); // backtrack
        }
    }
}


void moveCityToFront(int *cities, int n, int chosen, int matrix[N][N]) {
    int pos = -1;
    for (int i = 0; i < n; i++) {
        if (cities[i] == chosen) { pos = i; break; }
    }
    if (pos > 0) {
        // swap in cities array
        int temp = cities[0];
        cities[0] = cities[pos];
        cities[pos] = temp;

        // swap rows in matrix
        for (int j = 0; j < n; j++) {
            int tmp = matrix[0][j];
            matrix[0][j] = matrix[pos][j];
            matrix[pos][j] = tmp;
        }

        // swap columns in matrix
        for (int i = 0; i < n; i++) {
            int tmp = matrix[i][0];
            matrix[i][0] = matrix[i][pos];
            matrix[i][pos] = tmp;
        }
    }
}


int main() {
    int number_of_cities = N;

    int *cities = new int[number_of_cities];
    for (int i = 0; i < number_of_cities; i++) {
        cities[i] = i;
    }

    int starting_city;
    cout << "Choose a starting city (0.." << number_of_cities-1 << "): ";
    cin >> starting_city;

    moveCityToFront(cities, number_of_cities, starting_city,matrix);

    permute(cities, 1, number_of_cities - 1,matrix);

    cout << "Best path = ";
    printRoute(best_path, number_of_cities);
    cout << " Cost = " << best_cost << endl;

    delete[] cities;
    return 0;
}
