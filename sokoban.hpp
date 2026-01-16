#include <vector>
#include <string>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <utility>
#include <algorithm>
#include <sstream>
#include <cstdint>


/**
 * @brief  Read your map from stdin
 * @note   Input format: See project description
 * @param  &grid: You should first resize the grid! Modify it inplace.
 * @retval None
 */
void read_map(std::vector<std::string> &grid)
{
    size_t cols, rows;
    std::cin >> cols >> rows;
    grid.resize(rows);

    for (size_t i = 0; i < rows; ++i)
    {
        std::cin >> grid[i];
    }
}


bool isSolved(std::vector<int16_t> boxes, std::vector<int16_t> targets)
{
    for (const auto &box : boxes)
    {
        if (std::find(targets.begin(), targets.end(), box) == targets.end())
        {
            return false;
        }
    }
    return true;
}

struct State {
    int16_t px, py;                                     
    std::vector<int16_t> box_positions; 
    std::string path;   
};

int16_t PosReduction(size_t col, int16_t x, int16_t y) {
    return x * static_cast<int16_t>(col) + y;
}

std::string StateString(const State &s) {
    std::vector<int16_t> sorted_positions = s.box_positions;
    std::sort(sorted_positions.begin(), sorted_positions.end());

    std::ostringstream oss;
    oss << s.px << " "<< s.py << " "; 

    for (int16_t pos : sorted_positions) {
        oss << pos << " "; 
    }

    return oss.str();
}



inline bool isDeadlock(
    const std::vector<int16_t>& box_positions, 
    const std::vector<int16_t>& targets, 
    const std::vector<std::string>& grid, 
    size_t rows, 
    size_t cols
) {
    for (int16_t box_pos : box_positions) {
        bool is_target = (std::find(targets.begin(), targets.end(), box_pos) != targets.end());
        if (is_target) {
            continue; 
        }

        int16_t x = box_pos / static_cast<int16_t>(cols);
        int16_t y = box_pos % static_cast<int16_t>(cols);

        if (box_positions.size() >= 2) {

            int16_t col_num = static_cast<int16_t>(cols);

            bool up_box = false;
            bool down_box = false;
            bool left_box = false;
            bool right_box = false; 
            bool up_left_box = false;
            bool up_right_box = false;
            bool down_left_box = false;
            bool down_right_box = false;

            int16_t up_pos = (x - 1) * col_num + y;
            int16_t down_pos = (x + 1) * col_num + y;
            int16_t left_pos = x * col_num + y - 1;
            int16_t right_pos = x * col_num  + y + 1;
            int16_t up_left_pos = (x - 1) * col_num + y - 1;
            int16_t up_right_pos = (x - 1) * col_num + y + 1;
            int16_t down_left_pos = (x + 1) * col_num + y - 1;
            int16_t down_right_pos = (x + 1) * col_num + y + 1;

            for (size_t i = 0; i < box_positions.size(); ++i) {
                if (box_positions[i] == up_pos) {
                    up_box = true;
                } else if (box_positions[i] == down_pos) {
                    down_box = true;
                } else if (box_positions[i] == left_pos)  {
                    left_box = true;
                } else if (box_positions[i] == right_pos) {
                    right_box = true;
                } else if (box_positions[i] == up_left_pos) {
                    up_left_box = true;
                } else if (box_positions[i] == up_right_pos) {
                    up_right_box = true;
                } else if (box_positions[i] == down_left_pos)  {
                    down_left_box = true;
                } else if (box_positions[i] == down_right_pos) {
                    down_right_box = true;
                }
            }

            bool up_wall = (grid[static_cast<size_t>(x - 1)][static_cast<size_t>(y)] == '#');
            bool down_wall = (grid[static_cast<size_t>(x + 1)][static_cast<size_t>(y)] == '#');
            bool left_wall = (grid[static_cast<size_t>(x)][static_cast<size_t>(y - 1)] == '#');
            bool right_wall = (grid[static_cast<size_t>(x)][static_cast<size_t>(y + 1)] == '#');
            bool up_left_wall = (grid[static_cast<size_t>(x - 1)][static_cast<size_t>(y - 1)] == '#');
            bool up_right_wall = (grid[static_cast<size_t>(x - 1)][static_cast<size_t>(y + 1)] == '#');
            bool down_left_wall = (grid[static_cast<size_t>(x + 1)][static_cast<size_t>(y - 1)] == '#');
            bool down_right_wall = (grid[static_cast<size_t>(x + 1)][static_cast<size_t>(y + 1)] == '#');

            if (up_box && left_wall) {
                return up_left_box || up_left_wall || up_right_wall;    
            } 
            if (up_box && right_wall) {
                return up_right_box || up_right_wall || up_left_wall;
            } 

            if (down_box && left_wall) {
                return down_left_box || down_left_wall || down_right_wall;
            }        
            if (down_box && right_wall) {
                return down_right_box || down_right_wall || down_left_wall;
            } 
             
            if (left_box && up_wall) {
                return up_left_box || up_left_wall || down_left_wall;
            } 
            if (left_box && down_wall) {
                return down_left_box || down_left_wall || up_left_wall;    
            } 

            if (right_box && up_wall) {
                return up_right_box || up_right_wall || down_right_wall;
            } 
            if (right_box && down_wall) {
                return down_right_box || down_right_wall || up_right_wall;
            } 
            
            if (right_box && up_box && up_right_box) {
                return true;
            } 
            if (left_box && up_box && up_left_box) {
                return true;
            } 
            if (right_box && down_box && down_right_box) {
                return true;
            } 
            if (left_box && down_box && down_left_box) {
                return true;
            } 
            
            // if (down_right_wall && up_left_wall) {
            //     return (up_box && right_box) || (down_box && left_box);
            // }
            // if (down_left_wall && up_right_wall) {
            //     return (up_box && left_box) || (down_box && right_box);
            // }

        }
        
        if (x + 1 == static_cast<int16_t>(rows) || x - 1 == 0) {
            int16_t box_cnt = 0, target_cnt = 0;
            for (int16_t c = 0; c < static_cast<int16_t>(cols); c++) {
                int16_t pos = x * static_cast<int16_t>(cols) + c;
                if (std::find(targets.begin(), targets.end(), pos) != targets.end()) {
                    target_cnt ++;
                }
                if (std::find(box_positions.begin(), box_positions.end(), pos) != box_positions.end()) {
                    box_cnt ++;
                }
            }
            if (box_cnt > target_cnt) {
                return true;
            }
        } else if (y + 1 == static_cast<int16_t>(cols) || y -1 == 0) {
            int16_t box_cnt = 0, target_cnt = 0;
            for (int16_t r = 0; r < static_cast<int16_t>(rows);  r++) {
                int16_t pos = r * static_cast<int16_t>(cols) + y;
                if (std::find(targets.begin(), targets.end(), pos) != targets.end()) {
                    target_cnt ++;
                }
                if (std::find(box_positions.begin(), box_positions.end(), pos) != box_positions.end()) {
                    box_cnt ++;
                }
            }
            if (box_cnt > target_cnt) {
                return true;
            }
        }    
    }

    return false; 
}


/**
 * @brief  Solve the sokoban described by the grid
 * @note   Output string format: See project description
 * @param  &grid: The grid you read by `read_map'
 * @retval
 */
std::string solve(std::vector<std::string> &grid)
{
    const size_t rows = grid.size();
    const size_t cols = grid[0].size();

    for (size_t c = 0; c < cols; ++c) {
        if (grid[0][c] != '#' || grid[rows - 1][c] != '#') {
            return "No solution!";
        }
    }

    for (size_t r = 1; r < rows - 1; ++r) {
        if (grid[r][0] != '#' || grid[r][cols - 1] != '#') {
            return "No solution!";
        }
    }

    const std::vector<int16_t> dir_x = {-1, 1, 0, 0}; 
    const std::vector<int16_t> dir_y = {0, 0, -1, 1}; 
    const std::string moves = "UDLR";            

    int16_t player_x = -1, player_y = -1;
    std::vector<int16_t> boxes;
    std::vector<int16_t> targets;

    int16_t count = 0;

    for (size_t r = 0; r < rows; ++r)
    {
        for (size_t c = 0; c < cols; ++c)
        {
            if (grid[r][c] == 'S') {
                player_x = static_cast<int16_t>(r);
                player_y = static_cast<int16_t>(c);
                count ++;
            } else if (grid[r][c] == 'B') {
                boxes.emplace_back(PosReduction(cols, static_cast<int16_t>(r), static_cast<int16_t>(c)));
            } else if (grid[r][c] == 'T' ) {
                targets.emplace_back(PosReduction(cols, static_cast<int16_t>(r), static_cast<int16_t>(c)));
            } else if (grid[r][c] == 'R') {
                boxes.emplace_back(PosReduction(cols, static_cast<int16_t>(r), static_cast<int16_t>(c)));
                targets.emplace_back(PosReduction(cols, static_cast<int16_t>(r), static_cast<int16_t>(c)));
            } 
        }
    }

    if (count != 1 || boxes.empty() || boxes.size() > targets.size()) {
        return "No solution!";
    }

    if (isDeadlock(boxes, targets, grid, rows, cols)) {
        return "No solution!";
    }

    std::queue<State> q;
    std::unordered_set<std::string> visited_hash; 

    State start_state{player_x, player_y, boxes, ""};
    q.push(start_state);
    visited_hash.insert(StateString(start_state));

    std::vector<bool> isStaticDeadlockCell(rows * cols, false);
    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            if (grid[r][c] == '#') continue;

            bool left_wall = (grid[r][c - 1] == '#');
            bool right_wall = (grid[r][c + 1] == '#');
            bool up_wall = (grid[r - 1][c] == '#');
            bool down_wall = (grid[r + 1][c] == '#');

            if ((left_wall && up_wall) || (right_wall && up_wall) || (left_wall && down_wall) || (right_wall && down_wall)) {
                int16_t pos = PosReduction(cols, static_cast<int16_t>(r), static_cast<int16_t>(c));
                if (std::find(targets.begin(), targets.end(), pos) == targets.end()) {
                    isStaticDeadlockCell[static_cast<size_t>(pos)] = true;
                }
            }
        }
    }

    while (!q.empty())
    {
        State current = q.front();
        q.pop();

        if (isSolved(current.box_positions, targets))
        {
            return current.path;
        }

        // 移动
        for (size_t d = 0; d < 4; ++d)
        {
            int16_t new_px = current.px + dir_x[d];
            int16_t new_py = current.py + dir_y[d];

            // 遇到墙无法移动
            if (grid[static_cast<size_t>(new_px)][static_cast<size_t>(new_py)] == '#') {
                continue;
            }

            // 推箱子：
            bool isBoxMoved = false;
            std::vector<int16_t> new_box_positions = current.box_positions;
            auto it = std::find(new_box_positions.begin(), new_box_positions.end(), PosReduction(cols, new_px, new_py));
            if (it != new_box_positions.end())
            {
                isBoxMoved = true;
                int16_t new_box_x = new_px + dir_x[d];
                int16_t new_box_y = new_py + dir_y[d];
                int16_t new_box_pos_reduced = PosReduction(cols, new_box_x, new_box_y);
                // 箱子被阻挡
                if (grid[static_cast<size_t>(new_box_x)][static_cast<size_t>(new_box_y)] == '#' ||
                    std::find(new_box_positions.begin(), new_box_positions.end(), new_box_pos_reduced) != new_box_positions.end()) 
                {
                    continue;
                }
                if (isStaticDeadlockCell[static_cast<size_t>(new_box_pos_reduced)]) {
                    continue;
                }     
                *it = new_box_pos_reduced;
            }

            State new_state{new_px, new_py, new_box_positions, current.path + moves[d]};
            std::string new_string_state = StateString(new_state);

            if(isBoxMoved) {
                if (isDeadlock(new_state.box_positions, targets, grid, rows, cols)) {
                    continue;
                }
            }

            if (visited_hash.find(new_string_state) != visited_hash.end()) {
                continue;
            }

            visited_hash.insert(new_string_state);
            q.push(new_state);

        }
    }

    return "No solution!";
}

// For big cases, paste your answers here (i.e. replace "ans for big 1" with your answer)
// Do not remove the first element, it should be left blank.
// NOTE: Ensure the order is correct!
// Your answer should look like "UUDDLLRR..."
const std::vector<std::string> answers = {
    "__leave_this_blank__",
    "UUUUUULLLDRDLLLLLLLLUULLLLRDRRRDLLDLUDLLULDDDDLDRRRRRDLDDRRDRDDDRRRRDRRULLLRDDDLLUUURULLDLUUULURRR",
    "RUULDDLDDLLUULUUURRDLDDRRDDRRUULUULLULLDDRURRRULDRDDDLDLLUURRDRUUDDRDLLL",
    "DRURRLLUUULUURDRRRDDDRRDDLLUURRUURRUULLDDDDLLDDRRURULDDLLULLLUUULUURDRRRRLDDRRDDDLLULLLUUULURRRRDDRRDDDLLDLURRRUUULLDDUUUULLLDDDDRRDRUUURRDDDLRUUUURRUULLDLLLLRRRRDDLLUDDDLDDRUUUURRDLULDDLDDRURRURULULLDDLDRUUURRUULLDDDDLLLUUULUURDRRURDDDDULDDLLUUULURRRURDRRDDDLLULDURRRUULLDDRDL",
    "RRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRRLLLLDDRRRR",
    "RRUUUULURRRRRRRRRURDDDDRDLDLLURDRUUUURULLLDLUUULUURRDLLDDDLLLLLDDDDDLLLUURRDRUUULURRRRRRRRRURDDDDRDLDLLURDRUUUURULLLDLUUULULLULDRRRURRDLLDDDLLLLLDDDDRRRDLLRRDDLLLUUUUUULURRRRRRRRRURDDDDRDLDLLURDRUUUURULLLDLUUULULLDLURRRURRDLLDDDLLLLLDDDDDRRRRRULLLLDLUUUULURRRRRRRRRURDDDDRDLDLLURDRUUUURULLLDLUUULULLLLRRRRURRDLLLLLRUL",
    "URRUULULLRRDRDDLUUDDLDLLURUURUURRDLDDRDDLLRURUULUURDDLLLDDRDRRULUURDULLLDDRDRUURULDDDRULLLLDRRURUURUULLLDDDUUURRRDDLUDDDLDLLURUURURRDLDDRDLLLURDRUUURULDDDRUU",
    "LUURRDRUUULURRRRRRRRRURDDDDRDLDLLURDRUUUURULLLDLUUULUURRDLLDDDLLLLLDDDDDRRULDLUUUULURRRRRRRRRURDDDDRDLDLLURDRUUUURULLLDLUUULULLDLURRRURRDLLDDDLLLLLDDDDRRRDRRULLLLDLUUUULURRRRRRRRRURDDDDRDLDLLURDRUUUURULLLDLUUULULLULLRDRRRURRDLLDDDLLLLLDDDDDDDRRRUURULLLDLUUUULURRRRRRRRRURDDDDRDLDLLURDRUUUURULLLDLUUULULLDLURRRURRDLLDDDLLLLLDDDLLLDDRRURDRRRDDLLLUUUUUULURRRRRRRRRURDDDDRDLDLLURDRUUUURULLLDLUUULULLLLRRRRURRDLLLLLRUL",
    "ULDDDRDLLLDLLURRRLLUURDUURURRDLDDLLUURURDLLDDRDLDLLULUURRLURRDLLLDDRDRRULLRRRURRDLLLDLLURURRDRRULDLLDLURUUURURRDLDDRDLLLUUURURDDDUULLDDDRRLLDLLURRUUULLDRDUURRRDDLLDDLLURURRRUULLLLDLDLDRUURURRDLDRLDDRUULUURRRDDLRDL",
    "RDDLLLDDLLURDRRRDRUUULLLDLDRRLUURRDRDDLULLULLUUUUURRDDDDDLDRRRURDUUUULUURDDDDLDDLLULLUUUURURDDDDDLDRRLUURRDDLLULLUURLUURRDDDDLDRRR",
    "DDRUUUULDDDRDLLLRURULLLULUURDDLDDRUUURR",
    "LLLLLLURRRRRRURRRRRLLLLLUURRRRRRLLLLLLDRRRRRRRDDLUUDDLLLDLUDLUURRRRRRRUUUUDLLLDLLLLLDRRRRRRRDRUDDRUU",
    "ULLLLUUUUUURULLLLLDLURRRURRRRURDRDLLLLLLLLLDLUUDRRRRRRDDDRUURULLLLLLLDLURRRRDDDDDDDDRUUUUUUURULLLLLLRUURULL",
    "LLLUUURRRUUUUURDRRDDDDLDLULUUURRURDDUUURDDDDDUURRDL",
    "RRUUUUUDDRRDRUUUDLUURRRRRRRLLLLLLLLURURRRRLLLLDRRRRRUDLLLLLLLLDRRRRRRRRRUU",
    "UUUUUURUURRRRRRRRUURDDLLDRRDDLLLLLDDDLLLUUUUUULURRRRRRRRRDRUDDDLLLLDDDDDLLLLUUUUULURRRRRRRRRRDLLLLDDDLLLLLLLLLLUUUULURRRRRRRRRRRRRRDRUUU"
    };

// Don't modify this.
std::string print_answer(int index)
{
    if (index < 1 || index >= (int)answers.size())
    {
        return "invalid";
    }
    return answers[(size_t)index];
}
