#include <chrono>              
#include <iostream>           
#include <string>
#include <thread>
#include <vector>

#include <SFML/Graphics.hpp> 


const int CELL_SIZE = 50; 


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


void draw_map(sf::RenderWindow &window, const std::vector<std::string> &grid) {
    window.clear(sf::Color::White);

    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));

    for (size_t y = 0; y < grid.size(); ++y) {
        for (size_t x = 0; x < grid[y].size(); ++x) {
            char symbol = grid[y][x];
            if (symbol == '#') {
                cell.setFillColor(sf::Color::Black);
            } else if (symbol == '.') {
                cell.setFillColor(sf::Color::White); 
            } else if (symbol == 'S' || symbol == 'N') {
                cell.setFillColor(sf::Color::Blue);  
            } else if (symbol == 'B') {
                cell.setFillColor(sf::Color::Red);   
            } else if (symbol == 'T') {
                cell.setFillColor(sf::Color::Green);
            } else if (symbol == 'R') {
                cell.setFillColor(sf::Color::Yellow); 
            }

            cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
            window.draw(cell);
        }
    }

    window.display();
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

    sf::RenderWindow window(sf::VideoMode(grid[0].size() * CELL_SIZE, grid.size() * CELL_SIZE), "Sokoban Solve");

    for (char move : moves) {
        update_map(grid, move, player_x, player_y);
        draw_map(window, grid);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }

    return 0;
}
