#include <chrono>   
#include <iostream>  
#include <string>    
#include <thread>    
#include <vector>    

void read_map(std::vector<std::string> &grid) {
    size_t cols, rows;
    std::cin >> cols >> rows;
    grid.resize(rows);

    for (size_t i = 0; i < rows; ++i) {
        std::cin >> grid[i];
    }
}

void update_map(std::vector<std::string> &grid, const char move, int &player_x, int &player_y) {
    int new_x = player_x, new_y = player_y;
    if (move == 'U') --new_y;
    else if (move == 'D') ++new_y;
    else if (move == 'L') --new_x;
    else if (move == 'R') ++new_x;

    char target = grid[new_y][new_x];
    if (target == '.') {
        grid[new_y][new_x] = 'S';
    } else if (target == 'T') {
        grid[new_y][new_x] = 'N';
    } else if (target == 'B' || target == 'R') {
        int box_new_x = new_x + (new_x - player_x);
        int box_new_y = new_y + (new_y - player_y);

        char box_target = grid[box_new_y][box_new_x];
        if (box_target == '.' || box_target == 'T') {
            if (target == 'B') {
                grid[new_y][new_x] = 'S';
            } else if (target == 'R') {
                grid[new_y][new_x] = 'N';
            }

            grid[box_new_y][box_new_x] = (box_target == 'T') ? 'R' : 'B';
        }
    }

    if (grid[player_y][player_x] == 'S') {
        grid[player_y][player_x] = '.';
    } else if (grid[player_y][player_x] == 'N') {
        grid[player_y][player_x] = 'T';
    }
    player_x = new_x;
    player_y = new_y;
}

void print_map(const std::vector<std::string> &grid) {
    for (const auto &row : grid) {
        for (char cell : row) {
            if (cell == 'N') {
                std::cout << 'S'; 
            } else {
                std::cout << cell;
            }
        }
        std::cout << '\n';
    }
}

int main() {
    std::vector<std::string> grid;
    read_map(grid);

    int player_x = -1, player_y = -1;
    for (size_t y = 0; y < grid.size(); ++y) {
        for (size_t x = 0; x < grid[y].size(); ++x) {
            if (grid[y][x] == 'S') {
                player_x = x;
                player_y = y;
                break;
            }
        }
        if (player_x != -1) break;
    }

    std::string moves;
    std::cin >> moves;

    for (char move : moves) {
        update_map(grid, move, player_x, player_y);
        print_map(grid);
        std::cout << '\n'; 
    }

    return 0;
}
