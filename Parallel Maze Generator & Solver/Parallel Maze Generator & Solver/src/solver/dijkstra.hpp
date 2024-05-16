#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <limits>
#include <mpi.h>



using namespace std;


void solveMazeDijkstra(vector<vector<char>>& maze, int start_x, int start_y, int end_x, int end_y);

int dijkstra_solver(int rank);

#endif /* DIJKSTRA_HPP */
