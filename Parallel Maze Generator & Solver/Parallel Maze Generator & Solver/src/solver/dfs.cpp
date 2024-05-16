#include "dfs.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <stack>
#include <mpi.h>

using namespace std;

const int N = 64;
char graph[N][N];
pair<int,int> parent[N][N];

const vector<pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

bool isValid(int x, int y){
    return x >= 0 && x < N && y >= 0 && y < N;
}

void dfs(int x, int y, int px, int py){
    parent[x][y] = {px, py};
    for(auto [dx, dy] : directions){
        int nx = x + dx, ny = y + dy;
        if(!isValid(nx, ny) || graph[nx][ny] == '*' || (nx == px && ny == py)) 
            continue;
        dfs(nx, ny, x, y);
    }
}
// int MPI_Init(int*, char***) { return MPI_SUCCESS; }
// int MPI_Finalize() { return MPI_SUCCESS; }
void readMazeFromFile(string filename) {
    ifstream inputFile(filename);
    if (inputFile.is_open()) {
        for (int i = 0; i < N; i++) {
            string line;
            if (getline(inputFile, line)) {
                for (int j = 0; j < N; j++) {
                    graph[i][j] = line[j];
                }
            } else {
                cout << "Error reading from file.\n";
                return;
            }
        }
        inputFile.close();
    } else {
        cout << "Error opening the file.\n";
        return;
    }
}
void simulateMPICommunication() {
    // Dummy MPI send and receive calls
    int startX, endX;
    MPI_Send(&startX, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    MPI_Recv(&endX, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Dummy MPI broadcast call
    MPI_Bcast(&graph[0][0], N*N, MPI_CHAR, 0, MPI_COMM_WORLD);
}
void markShortestPath(int startX, int startY, int endX, int endY) {
    int x = endX, y = endY;
    while (x != startX || y != startY) {
        graph[x][y] = 'P';
        auto [px, py] = parent[x][y];
        x = px;
        y = py;
    }
}

void dfs_solver(int world_rank) {
    // MPI_Init(NULL, NULL);
    if (world_rank == 0) {
    readMazeFromFile("output.txt");
    // MPI_Bcast(&graph[0][0], N*N, MPI_CHAR, 0, MPI_COMM_WORLD);
    int startX = N - 1, startY = 0;
    dfs(0, N - 1, -1, -1);
    markShortestPath(0, N - 1, startX, startY);
    graph[0][N-1] = 'P';
    // Print the maze with the shortest path marked
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            if(graph[i][j] == '*') cout << "*";
            else if(graph[i][j] == 'P') {
                if (i==0 && j == (N-1)) {
                    cout << 'S';
                } else if (i==(N-1) && j == 0) {
                    cout << 'E';
                } else {
                    cout << "P";
                }
            } 
            else cout << " ";
        }
        cout << "\n";
    }
    // MPI_Finalize();
    }
    return;
}
