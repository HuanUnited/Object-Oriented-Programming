// Changes made
/*
1. Added print all permutations function.
2. 2D matrix with a function calculates all possible permuation:
eg: the matrix int **cities;
   A B C
A [0,2,3]
B [3,0,5]
C [4,3,0]
--------> Read top down, left to right.
Rows count as the from.
Columns count as the to.

First we should calculate all the possible permutations, and at the end of each
permutation we calculate the result of the path. for example ABC A -> A to B to
C back to A: 0 + 2 + 5 + 4.

what if it is ACB: A -> A to C to B back to A:

would we need to use a recursive algorithm for solving this problem?
Int Distance_Sum(char **cities, char *route){
   // Reads the last char of the city string and the char before that, which would translate to two int values correlating the city and the matrix.
   // For example A has index 0, B has index 1, C has index 2:
   // distance_sum += cities[index_from][index_to];

   // We can then create a loop so that it'll run from the beginning to the end of the string.

   for(int i = length_of_route; i >= 0, i--){
   // Down loop because we're running until the end of the string.
   // distance_sum += cities[route[i]][route[i-1]];

   // Here arises the problem of turning a char in the route, to an actual number corresponding to that char.
   // Suppose we can use something like a int TAB[255]; where each TAB[route[i]] would be given a value in that TAB[255];
   // For example: TAB[A] = 1 = TAB[route[i] = A];
   // Which then means before this we should have a in TAB array that alreadyhas all of the values corresponding to each city in the route.
   // which then makes the formula into:
   distance_sum += cities[TAB[route[i]]][TAB[route[i-1]]];
   // This does feel like a handfull.

   // The problem here is what if the names of the ciites are not given, how will we calculate all the possible permuations with just the 2D array alone?

   // First there is a better way to calculate the indexing which is by taking the char and minusing A.
   }

   We can also implement a global tracker for the best possible path and best score.

TBA:
2. A function that swaps the selected city to the beginning of the city array. <for brute-force calculations> 
2.1. A search function that searches for the city in that array.
3. A distance score calculation method.
4. A global best distance tracker.


}

*/

#include <cstring>
#include <iostream>
using namespace std;

#define N 5
int matrix[N][N] = {{0,2,3,5,7},
                    {3,0,9,2,4},
                    {1,3,0,4,2},
                    {4,2,1,0,1},
                    {6,3,1,5,0},

                  
                  };
// A B C D E 
int best_cost = 1e9;
char best_path[100];

void swap(char *x, char *y) {
  char temp = *x;
  *x = *y;
  *y = temp;
}

int calcCost(char *route, int len) {
  int sum = 0;
  for (int i = 0; i < len - 1; i++) {
    int from = route[i] - 'A';
    int to = route[i + 1] - 'A';
    sum += matrix[from][to];
  }
  // calculates the whole cycle, by returning to the starting city.
  sum += matrix[route[len - 1] - 'A'][route[0] - 'A'];
  return sum;
}

void permute(char *a, int l, int r) {
  if (l == r) {
    int cost = calcCost(a, r + 1);
    cout << a << " cost=" << cost << endl;
    if (cost < best_cost) {
      best_cost = cost;
      strcpy(best_path, a);
    }
  } else {
    for (int i = l; i <= r; i++) {
      swap((a + l), (a + i));
      permute(a, l + 1, r);
      swap((a + l), (a + i)); // backtrack
    }
  }
}


int main() {
  char str[] = "ABCDE";
  int n = strlen(str);
  permute(str, 0, n - 1);
  cout << "Best path = " << best_path << " cost=" << best_cost << endl;
  return 0;
}
