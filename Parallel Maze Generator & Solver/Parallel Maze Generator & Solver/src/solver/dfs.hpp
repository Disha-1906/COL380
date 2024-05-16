#ifndef DFS_HPP
#define DFS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <stack>
#include <mpi.h>


bool isValid(int x, int y);
void dfs(int x, int y, int px, int py);
void readMazeFromFile(std::string filename);
void simulateMPICommunication();
void markShortestPath(int startX, int startY, int endX, int endY);
void dfs_solver(int world_rank);

#endif // DFS_HPP
