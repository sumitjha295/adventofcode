//
//  solution.cpp
//  adventofcode
//
//  Created by Sumit Jha on 02.12.20.
//

#include "solution.hpp"

namespace
{
    const std::string resource_dir = "data/";
    const std::unordered_set<std::string> colors = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
    const int current_day = 13;
    const std::vector<std::pair<size_t, size_t>> dirs = {{0,1},{1,0},{0,-1},{-1,0},{-1,-1},{-1,1},{1,-1},{1,1}};
    const std::unordered_map<char, std::complex<int>> ctable = {
        {'N', {0,1}},
        {'S', {0,-1}},
        {'E', {1,0}},
        {'W', {-1,0}},
        {'R', {0,-1}},
        {'L', {0,1}},
        {'F', {1,0}},
    };
}

const std::vector<std::function<void(void)>> Solution::S_SOLUTIONS = {
    std::bind(day1, resource_dir + "day1.txt", 2020),
    std::bind(day2, resource_dir + "day2.txt"),
    std::bind(day3, resource_dir + "day3.txt"),
    std::bind(day4, resource_dir + "day4.txt"),
    std::bind(day5, resource_dir + "day5.txt"),
    std::bind(day6, resource_dir + "day6.txt"),
    std::bind(day7, resource_dir + "day7.txt", "shiny gold"),
    std::bind(day8, resource_dir + "day8.txt"),
    std::bind(day9, resource_dir + "day9.txt", 25),
    std::bind(day10, resource_dir + "day10.txt"),
    std::bind(day11, resource_dir + "day11.txt"),
    std::bind(day12, resource_dir + "day12.txt"),
    std::bind(day13, resource_dir + "day13.txt"),
    std::bind(day14, resource_dir + "day14.txt"),
};

Solution::Solution()
{
}

void Solution::day14(const std::string& inputfile) {}

void Solution::day13(const std::string& inputfile) {
    int line_number  = 0;
    int start_time = 0;
    std::vector<int64_t> bus_ids;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        std::istringstream iss(line);
        if(line_number == 0) iss >> start_time;
        else if(line_number == 1){
            std::string bus_id;
            while(std::getline(iss, bus_id, ',')){
                uint64_t id = bus_id != "x" ? std::stoll(bus_id) : -1;
                bus_ids.push_back(id);
            }
        }
        ++line_number;
        return true;
    });

    auto [id, time] = get_earliest_id(bus_ids, start_time);
    int64_t ans1 = id *  (time - start_time);

    std::cout << "ans1: " << ans1 << std::endl;
    std::cout << "ans1: " << win_gold(bus_ids) << std::endl;
}

int64_t Solution::win_gold(const std::vector<int64_t>& ids) {
    int64_t inc  = 1, time = 0;
    /*
    // this works, however it can be optimized by sorting mod
    for(int i = 0; i < ids.size(); ++i){
        if(ids[i] == -1) continue;
        int64_t mod = ((-i%ids[i]) + ids[i])%ids[i]; //keeping -i%ids[i] positive
        while (time % ids[i] != mod) time+= inc;
        inc *= ids[i];
    }
    */
    // optimizing algorithm starts
    std::priority_queue<std::pair<int64_t, int64_t>> mods;
    for(int i = 0; i < ids.size(); ++i){
        if(ids[i] == -1) continue;
        int64_t mod = ((-i%ids[i]) + ids[i])%ids[i]; //keeping -i%ids[i] positive
        mods.push({mod, ids[i]});
    }

    while(!mods.empty()){
        auto [mod, id] = mods.top(); mods.pop();
        while (time % id != mod) time+= inc;
        inc *= id;
    }
    // optimizing algorithm ends
    return time;
}
std::pair<int64_t, int64_t> Solution::get_earliest_id(const std::vector<int64_t>& ids, int64_t earliest_time) {
    int64_t earliest = LONG_LONG_MAX;
    int64_t bus_id = -1;
    for(int64_t id: ids) {
        if(id == -1) continue;
        int64_t next_time = earliest_time + (id - earliest_time%id);
        if(earliest > next_time){
            earliest = next_time;
            bus_id = id;
        }
    }
    return {bus_id, earliest};
}

void Solution::day12(const std::string& inputfile) {
    std::vector<std::pair<char, int>> commands;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        std::istringstream iss(line);
        char command;
        int value;
        iss >> command >> value;
        commands.push_back({command, value});
        return true;
    });

    std::complex<int> start(0, 0), direction(1, 0), waypoint(10, 1);
    std::cout << "ans1: " << get_displacement(commands, start, direction, false) << std::endl;
    std::cout << "ans2: " << get_displacement(commands, start, waypoint, true) << std::endl;
}

uint32_t Solution::get_displacement(const std::vector<std::pair<char, int>>& commands,
                                             std::complex<int> position,
                                             std::complex<int> direction,
                                             bool waypoint){
    for(const auto& [command, value]: commands){
        auto itr = ctable.find(command);
        if(itr == ctable.end()) continue;
        const auto& [c, delta] = *itr;
        switch (c) {
            case 'R':
            case 'L':{
                int shilft90 = value% 360;
                shilft90 /= 90;
                while(shilft90--) direction *= delta;
                break;
            }
            case 'F':{
                position += value * direction;
                break;
            }
            default:{
                if(waypoint) direction += value * delta;
                else position += value * delta;
                break;
            }
        }
    }
    return std::abs(std::real(position)) + std::abs(std::imag(position));
}

void Solution::day11(const std::string& inputfile) {
    std::vector<std::string> layout;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        std::istringstream iss(line);
        std::string row;
        iss >> row;
        layout.push_back(row);
        return true;
    });
    std::cout << "ans1: " << occupied_seats(layout, true, 4) << std::endl;
    std::cout << "ans2: " << occupied_seats(layout, false, 5) << std::endl;
}

size_t Solution::occupied_seats(const std::vector<std::string>& layout, bool adjacent, int max_neigbhour){
    size_t m = layout.size();
    size_t n = m ? layout[0].size(): 0;
    std::vector<std::string> curr = layout;
    bool changed = true;
    while(changed){
        std::vector<std::string> next = curr;
        changed = false;
        for(size_t i = 0; i < m; ++i){
            for(size_t j = 0; j < n; ++j){
                if(curr[i][j] == 'L' && next[i][j] != '#' && count_neigbhour(curr, i, j, '#', adjacent) == 0){
                    next[i][j] = '#';
                    changed = true;
                }
                else if(curr[i][j] == '#' && next[i][j] != 'L' && count_neigbhour(curr, i, j, '#', adjacent) >= max_neigbhour){
                    next[i][j] = 'L';
                    changed = true;
                }
            }
        }
        curr = std::move(next);
    }
    return count_seats(curr, '#');
}

size_t Solution::count_seats(const std::vector<std::string>& layout, char seat){
    size_t occupied_seats = 0;
    for(const auto& row: layout){
        occupied_seats += std::count_if(row.begin(), row.end(), [seat](char c){ return c == seat;});
    }
    return occupied_seats;
}

size_t Solution::count_neigbhour(const std::vector<std::string>& layout, size_t x, size_t y, char seat,  bool adjacent){
    size_t m = layout.size();
    size_t n = m ? layout[0].size(): 0;
    size_t cnt = 0;
    for(auto& [dx, dy]: dirs){
        size_t len  = 1;
        size_t nx = x+len*dx;
        size_t ny = y+len*dy;
        // if adjacent = true, just check next neigbhours else check for all possible
        // of seats in current direction
        while(!adjacent && nx >= 0 && nx < m && ny >= 0 && ny < n && layout[nx][ny] == '.'){
                ++len;
                nx = x+len*dx;
                ny = y+len*dy;
                continue;
        }
        if(nx >= 0 && nx < m && ny >= 0 && ny < n && layout[nx][ny] == seat) ++cnt;
    }
    return cnt;
}

void Solution::day10(const std::string& inputfile) {
    int max_jolt = INT_MIN;
    std::unordered_set<int> data;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        std::istringstream iss(line);
        int val;
        iss >> val;
        data.insert(val);
        max_jolt = std::max(max_jolt, val);
        return true;
    });
    max_jolt +=3;
    data.insert(max_jolt);

    // PART 1
    auto [ones, threes] = steps_one_three(data, 0, max_jolt);
    uint64_t ans1 = ones * threes;

    // PART 2
    uint64_t ans2 = path_count(data, 0, max_jolt);
    
    std::cout<< "ans1 :" <<  ans1 << std::endl;
    std::cout<< "ans2 :" <<  ans2 << std::endl;
}

std::pair<uint64_t, uint64_t> Solution::steps_one_three(const std::unordered_set<int>& data, int first, int last) {
    // Time:  O(n) n n = last-first
    // Space: O(1)
    uint64_t ones = 0, threes = 0;
    while(first < last){
        if(data.find(first + 1) != data.end()) {
            ++ones;
            ++first;
        }
        else if(data.find(first + 3) != data.end()) {
            ++threes;
            first +=3;
        }
        else{
            std::cout << "corrupt data" <<  std::endl;
            break;
        }
    }
    return {ones, threes};
}

uint64_t Solution::path_count(const std::unordered_set<int>& data, int first, int last) {
    // Time:  O(n) n = last-first
    // Space: O(n)
    std::unordered_map<int, uint64_t> paths = {{0,1}};
    while(first <= last){
        for(int i = 1; i <= 3; ++i){
            if(data.find(first+i) != data.end()){
                paths[first+i] += paths[first];
            }
        }
        ++first;
    }
    return paths[last];
}

void Solution::day9(const std::string& inputfile, size_t preamble_size) {
    std::vector<int64_t> data;
    int64_t ans1 = 0, ans2 = 0;
    // // PART 1
    // Time:  O(n*p) n is size of input data, p is size of the preamble
    // Space: O(p)
    std::unordered_set<int64_t> preamble;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        std::istringstream iss(line);
        int64_t val;
        iss >> val;
        bool is_valid_number = preamble.size() < preamble_size;
        // prepare the preample with index and value
        if(preamble.size() == preamble_size){
            // check the validity of the number
            for(int i = 0; i < preamble_size; ++i){
                int64_t num1  = *std::next(data.rbegin(), i);
                int64_t num2 = val - num1;
                if(num1 == num2) continue;
                if(preamble.find(num2) != preamble.end()){
                    is_valid_number = true;
                    // Remove the first number from the preamble
                    preamble.erase(*next(data.rbegin(), preamble_size-1));
                    break;
                }
            }
        }
        if(is_valid_number){
            data.push_back(val);
            preamble.insert(val);
        }
        else{
            // Answer1 found, exit the loop.
            ans1 = val;
            return false;
        }
        return true;
    });
    // PART 2
    // Time:  O(n) n is size of input data,
    // Space: O(n)
    int64_t csum = 0, target = ans1;
    std::unordered_map<int64_t, int> cmap = {{0,-1}};
    for(int i = 0; i < data.size(); ++i){
        csum += data[i];
        if(cmap.find(csum-target) != cmap.end()){
            int j = cmap[csum-target];
            const auto [mi, mx] = std::minmax_element(std::next(data.begin(), j+1),
                                                      std::next(data.begin(), i+1));
            ans2 = *mi + *mx;
            break;
        }
        cmap[csum] = i;
    }
    std::cout<< "ans1 :" << ans1 <<  std::endl;
    std::cout<< "ans2 :" << ans2 <<  std::endl;
}

// Time:  O(n)
// Space: O(n)
void Solution::day8(const std::string& inputfile) {
    std::vector<std::pair<std::string, int>> instructions;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        std::istringstream iss(line);
        std::string instruction;
        int val;
        iss >> instruction >> val;
        instructions.push_back({instruction, val});
        return true;
    });
    fixCycle(instructions);
}


void Solution::fixCycle(std::vector<std::pair<std::string, int>>& instructions) {
    std::unordered_set<int> forward_visited;
    int ans1 = -1, ans2=-1;
    execute(instructions, [&](auto arg){
        // collect jmp and nop in a corrupt code
        if(instructions[arg.first].first != "acc" ) forward_visited.insert(arg.first);
        ans1 = arg.second;
    });
    
    // Create an Adj matrix from the bottom
    std::unordered_map<size_t, std::unordered_set<size_t>> backward_adj;
    for(size_t i = 0; i < instructions.size(); ++i){
        auto& [code, val] = instructions[i];
        if(code == "jmp") backward_adj[val+i].insert(i);
        else backward_adj[i+1].insert(i);
    }
    
    // Iterate over the instrunctions to find list of instruction
    // that can be reached in backward direction
    std::stack<size_t> st;
    std::unordered_set<size_t> backward_visited;
    st.push(instructions.size()-1);
    while(!st.empty()){
        size_t u = st.top(); st.pop();
        backward_visited.insert(u);
        for(auto& v: backward_adj[u]){
            if(backward_visited.find(v) == backward_visited.end()) st.push(v);
        }
    }
    
    // Find the corrupt index by checking the index that can be reached
    // from forward_visited to backward_visited by changind just one instruction
    for(auto u : forward_visited){
        auto& [code, val] = instructions[u];
        size_t v = code == "jmp" ? u+1 : u+val;
        if(backward_visited.find(v) != backward_visited.end()){
            // wrong index found, fix it!
            code = code == "jmp" ? "nop" : "jmp";
            break;
        }
    }
    
    execute(instructions, [&](auto arg){
        ans2 = arg.second;
    });
    std::cout<< "ans1 :" << ans1 <<  std::endl;
    std::cout<< "ans2 :" << ans2 <<  std::endl;
}

void Solution::execute(const std::vector<std::pair<std::string, int>> &instructions,
                       const std::function<void(std::pair<int, int>)>& handler) {
    int acc = 0, i = 0;
    std::unordered_set<int> used;
    while(i < instructions.size()-1){
        if(used.find(i) != used.end()) break;
        used.insert(i);
        auto& [code, value] = instructions[i];
        if(code == "jmp") {i += value;}
        else if(code == "acc") {acc +=value; ++i;}
        else if(code == "nop") ++i;
        handler({i,acc});
    }
}

// Time:  O(n)
// Space: O(n)
void Solution::day7(const std::string& inputfile, const std::string& target) {
    std::unordered_map<std::string, std::unordered_map<std::string, int>> forward_adj;
    std::unordered_map<std::string, std::unordered_set<std::string>> backward_adj;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        std::istringstream iss(line);
        std::string shade, color, dummy;
        int num;
        while(iss >> shade >> color >> dummy >> dummy){
            std::string parent_color = shade + " " + color;
            while(iss >> num >> shade >> color >> dummy){
                std::string next_color = shade + " " + color;
                forward_adj[parent_color].insert({next_color, num});
                backward_adj[next_color].insert(parent_color);
            }
        }
        return true;
    });
    
    std::cout<< "ans1 :" << dependencies(backward_adj, target) <<  std::endl;
    std::cout<< "ans2 :" << children(forward_adj, target)  << std::endl;
}

size_t Solution::dependencies(const std::unordered_map<std::string, std::unordered_set<std::string>> &adj,
                              const std::string &target) {
    std::unordered_set<std::string> visited;
    std::stack<std::string> st;
    st.push(target);
    while(!st.empty()){
        auto u = st.top(); st.pop();
        visited.insert(u);
        auto itr = adj.find(u);
        if(itr == adj.end()) continue;;
        for(auto& v : itr->second){
            if(visited.find(v) == visited.end()) st.push(v);
        }
    }
    return visited.size()-1;
}

size_t Solution::children(const std::unordered_map<std::string,std::unordered_map<std::string, int> >& adj,
                          const std::string& u) {
    auto itr = adj.find(u);
    if(itr == adj.end()) return 0;
    size_t res = 0;
    for(const auto& [v, cnt]: itr->second){
        res += cnt;
        res += cnt * children(adj, v);
    }
    return res;
}

// Time:  O(n)
// Space: O(n)
void Solution::day6(const std::string& inputfile) {
    size_t res1 = 0;
    size_t res2 = 0;
    std::unordered_map<char, size_t> tmp;
    size_t group_size = 0;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        if(!line.empty()) {
            for(auto c: line) ++tmp[c];
            ++group_size;
        }
        
        if(line.empty() || isLast)  {
            res1 += tmp.size();
            for(const auto& [c, cnt]: tmp) {
                if(group_size == cnt) ++res2;
            }
            tmp.clear();
            group_size = 0;
        }
        return true;
    });

    std::cout<< "ans1 :" << res1  << std::endl;
    std::cout<< "ans2 :" << res2  << std::endl;
}

// Time:  O(n)
// Space: O(n)
void Solution::day5(const std::string& inputfile) {
    int idMax = INT_MIN;
    const size_t ROW_SIZE = 7;
    const size_t COL_SIZE = 3;
    std::unordered_map<int, std::pair<int, int>> rows;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        if(line.size() != ROW_SIZE + COL_SIZE) return  true;

        int row = strToInt(line.substr(0, ROW_SIZE), 'B');
        int col = strToInt(line.substr(ROW_SIZE), 'R');
        ++rows[row].first;
        rows[row].second ^= col;
        idMax = std::max(idMax, row * 8 + col);
        return true;
    });
    int myId = 0;
    const size_t maxCol = ~(~0u << COL_SIZE);
    for(const auto& [r, val] : rows){
        if(val.first ==  maxCol) {
            myId = r * 8 + val.second;
            break;
        }
    }
    
    std::cout<< "ans1 :" << idMax  << std::endl;
    std::cout<< "ans2 :" << myId  << std::endl;
}

// Time:  O(n)
// Space: O(n)
void Solution::day4(const std::string& inputfile) {
    size_t res1 = 0;
    size_t res2 = 0;
    const std::unordered_map<std::string, std::function<bool(std::string)>> validators = {
        {"byr", std::bind(isValidYear, std::placeholders::_1, 1920, 2002)},
        {"iyr", std::bind(isValidYear, std::placeholders::_1, 2010, 2020)},
        {"eyr", std::bind(isValidYear, std::placeholders::_1, 2020, 2030)},
        {"hgt", std::bind(isValidHeight, std::placeholders::_1, 150, 193, 59, 76)},
        {"hcl", std::bind(isValidHairColor, std::placeholders::_1)},
        {"ecl", std::bind(isValidEyeColor, std::placeholders::_1, colors)},
        {"pid", std::bind(isValidPID, std::placeholders::_1)},
    };
    std::unordered_map<std::string, std::string> tmp;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        if(!line.empty()){
            std::istringstream iss(line);
            std::string pair;
            while(iss >> pair){
                std::string key = pair.substr(0,3);
                if(validators.find(key) != validators.end()) tmp[key] = pair.substr(4);
            }
        }
        if(line.empty() || isLast)  {
            if(tmp.size() == validators.size()) ++res1;
            if(isOkay(validators, tmp)) ++res2;
            tmp.clear();
        }
        return true;
    });
    
    std::cout<< "ans1 :" << res1 << std::endl;
    std::cout<< "ans2 :" << res2 << std::endl;
}

int Solution::strToInt(const std::string &str, char hiBit) {
    int p = 1, res = 0;
    for(auto it = str.rbegin(); it != str.rend(); ++it,  p <<= 1){
        res += (*it == hiBit) * p;
    }
    return res;
}

bool Solution::isValidYear(const std::string& arg, int yMin, int yMax) {
    if (arg.size() != 4) return false;
    
    std::istringstream iss(arg);
    int val;
    iss >> val;
    return isBetween(val, yMin, yMax);
}

bool Solution::isValidHeight(const std::string &arg, int cMin, int cMax, int iMin, int iMax) {
    if(arg.size() > 2){
        std::istringstream iss(arg);
        int val;
        std::string unit;
        iss >> val >> unit;
        if(unit == "cm") return isBetween(val, cMin, cMax);
        if(unit == "in") return isBetween(val, iMin, iMax);
    }
    return false;
}

bool Solution::isValidPID(const std::string &arg) {
    if(arg.size() != 9) return false;
    return std::find_if_not(arg.begin(),
                             arg.end(),
                             [](char c){ return std::isdigit(c);}) == arg.end();
}

bool Solution::isValidEyeColor(const std::string &arg, const std::unordered_set<std::string>& eColors) {
    std::istringstream iss(arg);
    std::string val;
    iss >> val;
    return eColors.find(val) != eColors.end();
}

bool Solution::isValidHairColor(const std::string &arg) {
    if(arg.size() != 7) return false;
    std::istringstream iss(arg);
    std::string val;
    char h;
    iss >> h >> val;
    if(h != '#') return false;
    return std::find_if_not(val.begin(),
                            val.end(),
                             [](char c){ return std::isxdigit(c);}) == val.end();
}

bool Solution::isBetween(int x, int a, int b){
    return x >= a && x <= b;
}

bool Solution::isOkay(const std::unordered_map<std::string, std::function<bool (std::string)> > &validators,
                      const std::unordered_map<std::string, std::string> &data) {
    if(validators.size() != data.size())  return false;
    
    for(const auto&[k, v]: data){
        const auto& [id, validator] = *validators.find(k);
        if(!validator(v)) return false;
    }
    return true;
}

// Time:  O(n)
// Space: O(n)
void Solution::day3(const std::string& inputfile) {
    const std::vector<std::pair<uint64_t, uint64_t>> moves = {{1,1},{3,1},{5,1},{7,1},{1,2}};
    std::vector<uint32_t> trees(moves.size());
    std::vector<uint32_t> pos(moves.size());
    int level = 0;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        size_t n = line.size();
        for(size_t i = 0; i < moves.size();++i){
            const auto& [x,y] = moves[i];
            if(level%y == 0){
                if(line[pos[i]%n] == '#') ++trees[i];
                pos[i] += x;
            }
        }
        ++level;
        return true;
    });
    uint64_t res = std::accumulate(trees.begin(), trees.end(), 1u, std::multiplies<uint64_t>());
    
    std::cout<< "ans1: " << trees[1] << std::endl;
    std::cout<< "ans2: " << res << std::endl;

}

// Time:  O(n)
// Space: O(1)
void Solution::day2(const std::string& inputfile) {
    size_t res1 = 0;
    size_t res2 = 0;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        std::istringstream iss(line);
        size_t lo, hi;
        char ph, c;
        std::string pwd;
        iss >> lo >> ph >> hi >> c >> ph >> pwd;
        size_t cnt = 0;
        for(auto x: pwd)
            if(x == c) ++cnt;

        if(cnt >= lo && cnt <= hi)  ++res1;
        if((pwd[lo-1] == c)^(pwd[hi-1] == c))  ++res2;
        return true;
    });
    std::cout<< "ans1: " << res1 << std::endl;
    std::cout<< "ans2: " << res2 << std::endl;
}

void Solution::day1(const std::string& inputfile, int arg1)
{
    //collect data
    std::vector<int64_t> data;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        std::istringstream iss(line);
        int64_t value;
        iss >> value;
        data.push_back(value);
        return  true;
    });
    // solve
    auto [a , b] = two_sum(data, arg1);
    auto [p , q, r] = three_sum(data, arg1);
    
    std::cout<< "ans1: " << a*b << std::endl;
    std::cout<< "ans2: " << p*q*r << std::endl;
}

// Time:  O(n)
// Space: O(n)
std::pair<int64_t, int64_t>  Solution::two_sum(const std::vector<int64_t>& data, int sum) {
    std::unordered_set<int64_t> cache;
    for(int64_t num: data){
        if(cache.find(sum-num) != cache.end()) return {num, sum-num};
        cache.insert(num);
    }
    return {0,0};
}


// Time:  O(n*n)
// Space: O(n)
std::tuple<int64_t, int64_t,int64_t> Solution::three_sum(const std::vector<int64_t>& data, int sum){
    std::unordered_map<size_t, int64_t> cache;
    for(size_t i = 0; i < data.size(); ++i) cache[data[i]] = i;

    for(size_t i = 0; i < data.size(); ++i){
        for(size_t j = i+1; j < data.size(); ++j){
            int64_t left = sum - data[i] - data[j];
            if(cache.find(left) == cache.end()) continue;
            
            size_t k = cache[left];
            if(k > j) return {data[i], data[j], data[k]};
        }
    }
    return {0,0,0};
}

void Solution::run(bool all) {
    std::cout<< "-------Started-----" << std::endl;
    if(all){
        int i = 0;
        for(const auto& solution : S_SOLUTIONS) {
            std::cout<< "-----day " << ++i << "-----" << std::endl << std::endl;
            solution();
            std::cout<< std::endl;
        }
    }
    else {
        if(current_day > 0 && current_day <= S_SOLUTIONS.size()){
            const auto& solution = S_SOLUTIONS[current_day-1];
            std::cout<< "-----day " << current_day << "-----" << std::endl << std::endl;
            solution();
            std::cout<< std::endl;
        }
    }
    std::cout<< "-------Finished-----" << std::endl;

}


void Solution::read_if(const std::string& filepath,
                       const std::function<bool(std::string, bool)>& handler){
    std::ifstream infile(filepath);
    std::istream_iterator<Line> begin(infile), end;
    for(;begin != end; ++begin)
        if(!handler(*begin, infile.peek() == EOF)) return;
}



