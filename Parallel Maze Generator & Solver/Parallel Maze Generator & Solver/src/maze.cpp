#include<bits/stdc++.h>
#include <cstring>
// #include<mpi.h>
#include "generator/bfs.hpp"
#include "generator/kruskal.hpp"
#include "solver/dfs.hpp"
#include "solver/dijkstra.hpp"
using namespace std;

int main(int argc, char *argv[]) {
    
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " -g [bfs/kruskal] -s [dfs/dijkstra]" << std::endl;
        return 1; 
    }

    // cout << argv[2] << " " << argv[4] << endl;
    if ((strcmp(argv[2], "bfs") != 0 && strcmp(argv[2], "kruskal") != 0) ||
        (strcmp(argv[4], "dfs") != 0 && strcmp(argv[4], "dijkstra") != 0)) {
        std::cerr << "Invalid options provided." << std::endl;
        return 1; 
    }

    MPI_Init(NULL, NULL);
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    string arg1 = argv[2];
    string arg2 = argv[4];
    
    if (arg1 == "bfs") {
        // cout << "BFS to be done\n";
        bfs_generator(world_rank);
    } else if (arg1 == "kruskal") {
        // cout << "KRUSKAL to be done\n";
        kruskal_generator(world_rank);
    }

    if (arg2 == "dfs") {
        // cout << "DFS to be done\n";
        dfs_solver(world_rank);
    } else if (arg2 == "dijkstra") {
        // cout << "DIJKSTRA to be done\n";
        int x = dijkstra_solver(world_rank);
        if (x == 1) {
            MPI_Finalize();
            return 1;
        }
    }

    MPI_Finalize();
    return 0;
}