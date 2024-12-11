

# Efficient Maze Solving through Parallel Processing

# Project Overview

This repository contains the implementation of an efficient maze-solving algorithm using parallel
computing techniques. Maze-solving is a fundamental problem in robotics and autonomous
systems, particularly for navigation in unknown environments. Traditional sequential maze-
solving algorithms struggle with large-scale mazes due to slow processing times and suboptimal
performance.

To address these challenges, this project optimizes maze-solving by leveraging parallel
computing with OpenMP, a shared-memory multiprocessing model. The approach involves
using multiple particles that independently explore the maze paths to find the exit. By
parallelizing these tasks, the system achieves significant reductions in execution time and
improved efficiency, making it suitable for real-time applications.

# Getting Started

# Prerequisites

- Ensure gcc is installed on your system (tested with gcc- 1 4.2.0).
- A multi-core processor is recommended for best results.

# Compilation

Use the following command to compile the program:

g++ main.cpp -o main -fopenmp

# Execution

Run the program using:

./main <size_of_maze> <number_of_particles>

- Example:

```
./main 101 202
```

# Generic Instructions

- Maze Size:
    o Recommended: size_of_maze ≤ 101 to prevent excessive runtime and memory
       usage.
- Number of Particles:
    o Recommended: number_of_particles = 2 × size_of_maze for optimal
       performance.
- Debug Mode:
    o Set the debug flag to true to visualize the maze and the path discovered during
       execution.


