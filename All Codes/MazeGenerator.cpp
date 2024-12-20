#include <iostream>
#include <stdio.h>
#include <vector>
#include <random>
using namespace std;

//gets random exit co-ordinates on right most column or the bottom most row 
vector<int> get_exit_coords(int &size, mt19937 &rng){
    vector<int> coords(2);
    uniform_int_distribution<int> random_coord(0, size - 1);
    uniform_int_distribution<int> random_axes(0, 1);
    int randomNum = random_coord(rng);
    while (randomNum % 2 == 0)
        randomNum = random_coord(rng);
    int randomAxis = random_axes(rng);
    if(randomAxis==0){
        coords[0] = size - 1;
        coords[1] = randomNum;
    }
    else{
        coords[1] = size - 1;
        coords[0] = randomNum;
    }
    return coords;
}

enum SYMBOLS{
    EMPTY = ' ',
    WALL = '#',
    START = 'S',
    SOL = 'o',
    EXIT = 'E'
};


// setting initial structure and boundaries of maze
void structurize_maze(vector<vector<char> > &maze, int &size, vector<int> &exit_coords){
    for(int i=0;i<size;++i){
        for(int j=0;j<size;++j){
            if(i==exit_coords[0] && j==exit_coords[1]){
                maze[i][j] = EXIT;
                continue;
            }
            if(i%2==0 || j%2==0){
                maze[i][j] = WALL;
            }
            else{
                maze[i][j] = EMPTY;
            }
        }
    }
}

// prints maze
void print_maze(vector<vector<char> > &maze, int &size){
    for(int i=0;i<size;++i){
        for(int j=0;j<size;++j){
            printf("%c ",maze[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// gets neighbors of current cell where we can move
vector<vector<int> > get_neighbors(vector<vector<char> > &maze, int &size, vector<vector<bool> > &visited, vector<int> &curr_coords, bool is_exit){
    vector<vector<int> > neighbors;
    int offset = is_exit? 1:2;
    for(int i=0;i<4;++i){
        int index = curr_coords[(i>=2)] + ((i%2)?offset:-offset);
        int j = (i<2)?1:0;
        if(index>=0 && index<size){
            if(i<2){
                if(!visited[index][curr_coords[j]] && maze[index][curr_coords[j]]!=WALL){
                    vector<int> temp;
                    temp.push_back(index);
                    temp.push_back(curr_coords[j]);
                    neighbors.push_back(temp);
                }
            }
            else{
                if(!visited[curr_coords[j]][index] && maze[curr_coords[j]][index]!=WALL){
                    vector<int> temp;
                    temp.push_back(curr_coords[j]);
                    temp.push_back(index);
                    neighbors.push_back(temp);
                }
            }
        }
    }
    return neighbors;
}

// making maze by removing walls from initial structure
void remove_wall_if_needed(vector<vector<char> > &maze, vector<int> &curr_cell, vector<int> &new_cell, bool &debug){
    int row_to_del = -1, col_to_del = -1;
    if(new_cell[1] == curr_cell[1]) {
        row_to_del = (new_cell[0] > curr_cell[0]) ? new_cell[0] - 1 : new_cell[0] + 1;
        col_to_del = new_cell[1];
    } else if (new_cell[0] == curr_cell[0]) {
        row_to_del = new_cell[0];
        col_to_del = (new_cell[1] > curr_cell[1]) ? new_cell[1] - 1 : new_cell[1] + 1;
    }
    if(row_to_del > -1 && col_to_del > -1) {
        maze[row_to_del][col_to_del] = EMPTY;
        if(debug){
            //printf("Cell to delete: %d, %d\n",row_to_del,col_to_del);
        }
    } else {
        printf("Unexpected error while generating the maze\n");
        exit(1);
    }
}

void dfs(vector<vector<char> > &maze, int &size, vector<vector<bool> > &visited, vector<int> &curr_coords, vector<vector<int> > &curr_track, bool is_exit, bool &debug,  mt19937 &rng);

// backtracking to see if there are any possible walls that can be removed
void backtrack(vector<vector<char> > &maze, int &size, vector<vector<bool> > &visited, vector<int> &curr_coords, vector<vector<int> > &curr_track, bool &debug, mt19937 &rng){
    int curr_size = curr_track.size();
    for(int i=curr_size-1;i>0;i--){
        curr_coords = curr_track[i];
        curr_track.pop_back();
        vector<vector<int> > neigh = get_neighbors(maze,size,visited,curr_coords,false);
        if(neigh.size()>0){
            dfs(maze,size,visited,curr_coords,curr_track,false,debug,rng);
            break;
        }
    }
}


// maze generator code
void dfs(vector<vector<char> > &maze, int &size, vector<vector<bool> > &visited, vector<int> &curr_coords, vector<vector<int> > &curr_track, bool is_exit, bool &debug, mt19937 &rng){
    vector<vector<int> > neighbors = get_neighbors(maze,size,visited,curr_coords,is_exit);
    while(neighbors.size()>0){
        int no_of_cells = neighbors.size();
        uniform_int_distribution<int> uniform_cells(0, no_of_cells - 1);
        int new_cell_index = uniform_cells(rng);
        vector<int> new_cell = neighbors[new_cell_index];
        neighbors.clear();
        if(!is_exit){
            remove_wall_if_needed(maze,curr_coords,new_cell,debug);
        }
        else{
            is_exit = false;
        }
        curr_coords = new_cell;
        visited[curr_coords[0]][curr_coords[1]] = true;
        curr_track.push_back(curr_coords);
        neighbors = get_neighbors(maze,size,visited,curr_coords,is_exit);
    }
    backtrack(maze,size,visited,curr_coords,curr_track,debug,rng);
}

void mazegenerator(vector<vector<char> > &maze, int &size, bool debug){
    printf("Generating Maze of size %d ...\n",size);
    random_device rd;  // Obtain a random seed from the operating system
    mt19937 rng(rd());
    vector<int> exit_coords = get_exit_coords(size,rng);
    if(debug){
        printf("Exit co-ordinates are %d, %d\n",exit_coords[0],exit_coords[1]);
    }
    structurize_maze(maze,size,exit_coords);

    vector<vector<bool> > visited(size, vector<bool> (size,false));
    vector<vector<int> > curr_track;
    curr_track.push_back(exit_coords);
    visited[exit_coords[0]][exit_coords[1]] = true;
    dfs(maze,size,visited,exit_coords,curr_track,true,debug,rng);
    maze[0][1] = START;
    if(debug){
        printf("Printing final maze...\n");
        print_maze(maze,size);
        printf("\n");
    }
}