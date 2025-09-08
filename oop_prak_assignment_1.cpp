#include <iostream>


// Input matrix is a 2d matrix, where each row and column is equivalent of one city.
/*
   a,b,c,d,e
 a[0,2,3,4,5] = 1
 b[1,0,3,4,5] = 2
 c[3,4,0,6,7] = 3
 d[7,8,9,0,1] = 4
 e[1,2,3,4,0] = 5

 each index (i,i) = 0, because the city distance is zero.
 
 for each city traversed to and from, the corresponding index is the removed from the matrix.
 
 the input is the number of cities (which means the number of rows and columns), matrix of distance (from what city to what city), and the index of the beginning city.

 implementation of the bruteforce method would 1, for each element in a row, calculate its sum with an element in the next rows.
 so n! calculations for the whole of each element in the matrix.

 this means that in a euclidean coordinate system, we start at a point, and then move to another point, total distance travelled would be from this point to the next point, and then from the next to point to the next next point etc.

 the output isn't important, but let's give the output of all of the hamiltonian cycles from the matrix (and their costs), and then choose out the best (shortest hamiltonian cycle) and it's cost.

 we should also measure the runtime of the program itself, to see how long it takes to calculate.

 we should also calculate how many calculations we need to do.

 to calculate the routes itself it would be a loop, something like
    given a n by n matrix
    int n; //number of cities
    int matrix[n][n];
    int hamiltonian_cycles = (n-1)!/2;
    int total_cost[hamiltonian_cycles] = {0};

    int starting_city = {};
    cout << "Input starting city: ";
    cin << starting_city;
    cout << endl;
    //This would be the index of the starting city.


    // This calculates for all indexes, how would we know which city?

    **Then what if we have a matrix, where the line represents a city, and it's respective distance to the other cities?**

    Take a look at the whiteboard: :/whiteboard.tldr -->

    which means this loop will be reworked as such:
    int row = starting_city;
    int column{}; // would be equal to any index not equal to the starting city (which is not any index that has the value of zero)

    for example, we start at b, from b we can go to any a,c,d,e
    how do we calculate this, by adding it into the total_cost array: say += matrix[row][column_of_a]; //moves to a
    then row = column_of_a; -> t.e row_of_a;
    from row of a we have the choice to move to any row that hasn't been moved to before: which means we should have a hash-map for the cities that we've visited.
    
    bool visited[n] = {false};
    if (visited[column_of_city] == true){continue;} //skip this city.
    
    have the loop go until we visit the next unvisited city: 
    which means: while(visited[column_of_city] == true){continue;}

    for each city visited, cities_visited++; until cities_visited == n; //number of cities, which then it stops.


    for(int i = 0; i < n; i++){
    matrix[row][column];
    }



 for(int i = 0; i < n; i++){
 printf("from index %d, we have these cycles:")
 for(int j = i ; j < n; j++){
    if(matrix[i][j] == 0) continue;
    cost[i] += matrix[i][j];
    printf(" -> %d", j);
 }
 printf(". Which has the cost of %d.\n", cost[i]);
 }

 and this is the brute force method.

 Next thing is should i put each cycle into an array, and then give it out. This only applies for small n.

 Next is to implement a random matrix generator.

 Next is to implement the nearest neighbor solution, which means it will pick from the row, the smallest value.
*/

class Matrix{

};

class Input{

};


int main(){


}