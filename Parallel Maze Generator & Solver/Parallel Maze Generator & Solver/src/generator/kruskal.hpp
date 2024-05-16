#ifndef KRUSKAL_HPP
#define KRUSKAL_HPP

#include <vector>
#include <mpi.h>

struct Edge {
    int u, v, weight;
};

struct Cell {
    int x, y;
};


int find(int x, std::vector<int>& parent);
int generateMaze(std::vector<std::vector<char>>& maze, int rank, int size);
void kruskal_generator(int world_rank);

#endif // KRUSKAL_HPP
