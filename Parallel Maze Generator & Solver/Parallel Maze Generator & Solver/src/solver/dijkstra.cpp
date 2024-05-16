#include "dijkstra.hpp"
// #include <iostream>
// #include <vector>
// #include <queue>
// #include <utility>
// #include <limits>
#include<bits/stdc++.h>
// #include <mpi.h>

#define MAZE_SIZE 64
#define INF std::numeric_limits<int>::max()

using namespace std;

// Structure to represent a cell in the maze
struct Cell {
    int x, y, dist;
    Cell(int _x, int _y, int _dist) : x(_x), y(_y), dist(_dist) {}
    // Define the Cell class
    // Comparison operator for greater than
    bool operator>(const Cell& other) const {
        return dist > other.dist;
    };

};

// Function to solve the maze using Dijkstra's algorithm
void solveMazeDijkstra( vector<vector<char>> &maze, int start_x, int start_y, int end_x, int end_y) {
    // Create a priority queue to store cells along with their distances
    
    priority_queue<Cell, vector<Cell>, greater<Cell>> pq;

    // Create a 2D vector to store distances of cells from the source
    vector<vector<int>> dist(MAZE_SIZE, vector<int>(MAZE_SIZE, INF));
    vector<vector<pair<int, int>>> prev(MAZE_SIZE, vector<pair<int, int>>(MAZE_SIZE, make_pair(-1, -1)));

    // Push the source cell into the priority queue with distance 0
    pq.push(Cell(start_x, start_y, 0));
    dist[start_x][start_y] = 0;

    // Define possible moves: up, down, left, right
    vector<pair<int, int>> moves = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    // Dijkstra's algorithm
    while (!pq.empty()) {
        // Extract the cell with minimum distance
        Cell current = pq.top();
        pq.pop();

        // Check if the current cell is the destination
        if (current.x == end_x && current.y == end_y) {
            break; // Exit loop if destination is reached
        }

        // Explore all possible moves from the current cell
        for (const auto &move : moves) {
            int new_x = current.x + move.first;
            int new_y = current.y + move.second;

            // Check if the new position is valid and not a wall
            if (new_x >= 0 && new_x < MAZE_SIZE && new_y >= 0 && new_y < MAZE_SIZE && maze[new_x][new_y] == ' ') {
                int new_dist = current.dist + 1; // Increment distance by 1 for each move

                // Update distance if the new distance is shorter
                if (new_dist < dist[new_x][new_y]) {
                    dist[new_x][new_y] = new_dist;
                    pq.push(Cell(new_x, new_y, new_dist));
                    prev[new_x][new_y] = make_pair(current.x, current.y);
                }
            }
        }
    }
    int x = end_x, y = end_y;
    while (x != start_x || y != start_y) {
        int prev_x = prev[x][y].first;
        int prev_y = prev[x][y].second;
        maze[x][y] = '%'; // Mark cell on shortest path
        x = prev_x;
        y = prev_y;
    }
    // Output the solved maze
    for (int i = 0; i < MAZE_SIZE; ++i) {
        for (int j = 0; j < MAZE_SIZE; ++j) {
            if (maze[i][j] == '*') {
                cout << "*"; // Wall cell
            } else if (i == start_x && j == start_y) {
                cout << "S"; // Entry cell
            } else if (i == end_x && j == end_y) {
                cout << "E"; // Exit cell
            } else if (maze[i][j] == '%') {
                cout << "P"; // Cell in solution path
            } else {
                cout << " "; // Non-wall cell not in solution path
            }
        }
        cout << endl;
    }
}

int dijkstra_solver(int rank) {
    // int rank;
    // MPI_Init(NULL,NULL);
    
    // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<vector<char>> maze(MAZE_SIZE, vector<char>(MAZE_SIZE, '*')); // Initialize maze with all cells as non-wall

    // Input maze (here you can input your maze)

    // Example:
    if(rank == 0) {
        ifstream inputFile("output.txt"); 
            string c;
            if (inputFile.is_open()) {
                for (int i = 0; i < MAZE_SIZE; i++) {
                    if (getline(inputFile, c)) {
                        for (int j = 0; j < MAZE_SIZE; j++) {
                            maze[i][j] = c[j];
                        }
                    } else {
                        cout << "Error reading from file.\n";
                        return 1; 
                    }
                }
                inputFile.close();
            } else {
                cout << "Error opening the file.\n";
                return 1; 
            }
        // cout<<"Maze generation done\n";

        MPI_Send(&maze[0][0], MAZE_SIZE * MAZE_SIZE, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    } else if(rank==1){
        char loc_buff[MAZE_SIZE][MAZE_SIZE];
        MPI_Recv(&loc_buff, MAZE_SIZE * MAZE_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    // Assuming the maze is already defined, let's solve it
    if(rank==0){
        solveMazeDijkstra(maze, 0, MAZE_SIZE - 1, MAZE_SIZE - 1, 0); // Start from top-right and end at bottom-left
    }
    int distance = 5;
    int l1, l2;
    MPI_Scatter(&distance, 1, MPI_INT, &l1, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(&l2, 1, MPI_INT, &distance, 1, MPI_INT, 0, MPI_COMM_WORLD);

    
    // MPI_Finalize();
    return 0;
}
