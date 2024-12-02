#include <iostream>
#include <stdlib.h>
#include "./MazeGenerator/mazegenerator.cpp"
#include "./MazeSolver/parallel.cpp"
#include <omp.h>
using namespace std;

int main(int argc, char *argv[]){
    // Check if arguments are correct
    if(argc < 3){
        printf("Usage: %s <size_of_maze> <no_of_particles>\n",argv[0]);
        exit(0);
    }

    // Parse the maze size from the first argument
    int size = atoi(argv[1]);
    // Ensure the maze size is odd
    if(size % 2 == 0){
        size++;
        printf("To maintain symmetry, maze size is generally always odd. Creating maze of size %d\n", size);
    }

    // Generate the maze
    vector<vector<char>> maze(size, vector<char>(size));
    //pass the maze, size and debug flag, change flag to true to print generated matrix
    mazegenerator(maze, size, false);
    printf("Maze generated.\n\n");

    // Run the maze solver with different thread counts
    vector<int> thread_counts = {1, 2, 4, 8};
    for (int num_threads : thread_counts) {
        omp_set_num_threads(num_threads);
        printf("Running with %d threads...\n", num_threads);


    vector<vector<char>> mazeForParallel = maze;
    double start;
    double end;
    int particles = atoi(argv[2]);

    start = omp_get_wtime();  
    n_particles_parallel(mazeForParallel,size,particles,false); //change debug flag to true to print solved matrix
    end = omp_get_wtime();
    printf("Execution time for parallel code with %d particles is %f seconds\n\n",particles,end-start);
    }
    return 0;