MPI = mpic++
MPIFLAGS = -lmpi_cxx

SRC = ./src/maze.cpp
BFS = ./src/generator/bfs.cpp
KRUSKAL = ./src/generator/kruskal.cpp
DFS = ./src/solver/dfs.cpp
DIJKSTRA = ./src/solver/dijkstra.cpp
TARGET = ./maze.out

DEPS = ./src/generator/bfs.hpp \
       ./src/generator/kruskal.hpp \
       ./src/solver/dfs.hpp \
       ./src/solver/dijkstra.hpp

.PHONY: all clean

all: $(TARGET)

$(TARGET) : $(SRC) $(BFS) $(KRUSKAL) $(DFS) $(DIJKSTRA) $(DEPS)
	$(MPI) -o $(TARGET) $(SRC) $(BFS) $(KRUSKAL) $(DFS) $(DIJKSTRA) 

clean:
	rm -f $(TARGET)
	rm -f output.txt
