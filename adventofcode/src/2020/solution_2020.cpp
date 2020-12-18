//
//  solution.cpp
//  adventofcode
//
//  Created by Sumit Jha on 02.12.20.
//

#include "solution_2020.hpp"

namespace
{
    const std::string resource_dir = "data/2020/";
    const std::unordered_set<std::string> colors = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
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

Solution2020::Solution2020(){
    m_solutions = {
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
        std::bind(day15),
        std::bind(day16, resource_dir + "day16.txt"),
        std::bind(day17, resource_dir + "day17.txt"),
        std::bind(day18, resource_dir + "day18.txt"),
    };
}


Solution2020::~Solution2020(){}

void Solution2020::run(int day){
    ISolution::run(16);
}

void Solution2020::day18(const std::string& inputfile) {
    uint64_t ans1 = 0, ans2 = 0;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        ans1 += evaluate_without_precedence(line);
        ans2 += evaluate_with_precedence(line);
        return true;
    });
    std::cout << "ans1: " << ans1 << std::endl;
    std::cout << "ans2: " << ans2 << std::endl;
}

uint64_t Solution2020::evaluate_with_precedence(const std::string &expression) {
    uint64_t num = 0;
    char action = ' ';
    std::vector<uint64_t> values;
    for(int i = 0; i < expression.size(); ++i){
        if(std::isspace(expression[i])) continue;
        if(std::isdigit(expression[i])) num = 10*num + (expression[i]-'0');
        
        if(!std::isdigit(expression[i]) || i == expression.size()-1){
            if(expression[i] == '('){
                int j = i, open = 0;;
                while(j < expression.size()) {
                    if(expression[j] == '(') ++open;
                    else if(expression[j] == ')') --open;
                    ++j;
                    if(open == 0) break;
                }
                num = evaluate_with_precedence(expression.substr(i+1, j-i-1));
                i = j;
            }

            if(action == '+') {
                num += !values.empty() ? values.back(): 0;
                if(!values.empty()) values.pop_back();
            }
            values.push_back(num);
            num = 0;
            while(i<expression.size() && std::isspace(expression[i])) ++i;
            if(i < expression.size()) action = expression[i];
        }
    }
    return std::accumulate(values.begin(), values.end(), 1ull, std::multiplies<uint64_t>());
}

uint64_t Solution2020::evaluate_without_precedence(const std::string &expression) {
    uint64_t num = 0;
    uint64_t res = 0;
    char action = ' ';
    for(int i = 0; i < expression.size(); ++i){
        if(std::isspace(expression[i])) continue;
        if(std::isdigit(expression[i])) num = 10*num + (expression[i]-'0');
        
        if(!std::isdigit(expression[i]) || i == expression.size()-1){
            if(expression[i] == '('){
                int j = i, open = 0;;
                while(j < expression.size()) {
                    if(expression[j] == '(') ++open;
                    else if(expression[j] == ')') --open;
                    ++j;
                    if(open == 0) break;
                }
                num = evaluate_without_precedence(expression.substr(i+1, j-i-1));
                i = j;
            }

            if(action == '+') res += num;
            else if(action == '*') res *= num;
            else res = num;
            num = 0;
            while(i < expression.size() && std::isspace(expression[i])) ++i;
            if(i < expression.size()) action = expression[i];
        }
    }
    return res;
}

void Solution2020::day17(const std::string& inputfile) {
    std::vector<std::string> input_plane;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        std::istringstream iss(line);
        std::string input;
        iss >> input;
        input_plane.push_back(input);
        return true;
    });
    std::cout << "ans1: " << solve3d(input_plane, 6) << std::endl;
    std::cout << "ans1: " << solve4d(input_plane, 6) << std::endl;
}

int Solution2020::solve4d(const std::vector<std::string>& input_plane, int nCycle) {
    std::string inactive_row(2*nCycle + input_plane[0].size(),'.');
    std::vector<std::string> plane, inactive_plane(2*nCycle + input_plane.size(), inactive_row);
    std::vector<std::vector<std::string>> inactive_cube(2*nCycle+1, inactive_plane);
    std::vector<std::vector<std::vector<std::string>>> dem(2*nCycle+1, inactive_cube);

    for(int i = 0; i < nCycle; ++i) plane.push_back(inactive_row);
    for(const auto& row: input_plane) plane.push_back(std::string(nCycle, '.') + row + std::string(nCycle, '.'));
    for(int i = 0; i < nCycle; ++i) plane.push_back(inactive_row);
    dem[nCycle][nCycle] = plane;
    
    size_t m = dem.size();
    size_t n = m ? dem[0].size() : 0;
    size_t o = n ? dem[0][0].size() : 0;
    size_t p = o ? dem[0][0][0].size() : 0;
    int ans = 0;
    for(int i = 0; i < nCycle; ++i){
        auto curr = dem;
        ans = 0;
        for(int i = 0; i < m; ++i){
            for(int j = 0; j < n; ++j){
                for(int k = 0; k < o; ++k){
                    for(int l = 0; l < p; ++l){
                        int cnt = count_neighbours(dem, i, j, k, l);
                        if(dem[i][j][k][l] == '#') {
                            curr[i][j][k][l] = (cnt == 2 || cnt == 3) ? '#' : '.';
                        }
                        else  curr[i][j][k][l] =  cnt == 3 ? '#' : '.';

                        ans += curr[i][j][k][l] == '#';
                    }
                }
            }
        }
        dem = std::move(curr);
    }
    return ans;
}

int Solution2020::count_neighbours(const std::vector<std::vector<std::vector<std::string>>>& dem,
                                   int x, int y, int z, int w){
    size_t m = dem.size();
    size_t n = m ? dem[0].size() : 0;
    size_t o = n ? dem[0][0].size() : 0;
    size_t p = o ? dem[0][0][0].size() : 0;
    int cnt = 0;
    for(int dx = -1; dx <= +1; ++dx){
        for(int dy = -1; dy <= +1; ++dy){
            for(int dz = -1; dz <= +1; ++dz){
                for(int dw = -1; dw <= +1; ++dw){
                    if(dx == 0 && dy == 0 && dz == 0 && dw == 0) continue;
                    int nx = x + dx;
                    int ny = y + dy;
                    int nz = z + dz;
                    int nw = w + dw;
                    if(nx < 0 || nx >= m) continue;
                    if(ny < 0 || ny >= n) continue;
                    if(nz < 0 || nz >= o) continue;
                    if(nw < 0 || nw >= p) continue;
                    cnt += dem[nx][ny][nz][nw] == '#';
                }
            }
        }
    }
    return cnt;
}

int Solution2020::solve3d(const std::vector<std::string>& input_plane, int nCycle) {
    std::string inactive_row(2*nCycle + input_plane[0].size(),'.');
    std::vector<std::string> plane, inactive_plane(2*nCycle + input_plane.size(), inactive_row);
    std::vector<std::vector<std::string>> cube(2*nCycle+1, inactive_plane);
    
    for(int i = 0; i < nCycle; ++i) plane.push_back(inactive_row);
    for(const auto& row: input_plane) plane.push_back(std::string(nCycle, '.') + row + std::string(nCycle, '.'));
    for(int i = 0; i < nCycle; ++i) plane.push_back(inactive_row);

    cube[nCycle] = plane;
    size_t m = cube.size();
    size_t n = m ? cube[0].size() : 0;
    size_t o = n ? cube[0][0].size() : 0;
    int ans = 0;
    for(int i = 0; i < nCycle; ++i){
        auto curr = cube;
        ans = 0;
        for(int i = 0; i < m; ++i){
            for(int j = 0; j < n; ++j){
                for(int k = 0; k < o; ++k){
                    int cnt = count_neighbours(cube, i, j, k);
                    if(cube[i][j][k] == '#') {
                        curr[i][j][k] = (cnt == 2 || cnt == 3) ? '#' : '.';
                    }
                    else  curr[i][j][k] =  cnt == 3 ? '#' : '.';
                    
                    ans += curr[i][j][k] == '#';
                }
            }
        }
        cube = std::move(curr);
    }
    return ans;
}

int Solution2020::count_neighbours(const std::vector<std::vector<std::string>>& cube,
                                   int x, int y, int z){
    
    size_t m = cube.size();
    size_t n = m ? cube[0].size() : 0;
    size_t o = n ? cube[0][0].size() : 0;
    int cnt = 0;
    for(int dx = -1; dx <= +1; ++dx){
        for(int dy = -1; dy <= +1; ++dy){
            for(int dz = -1; dz <= +1; ++dz){
                if(dx == 0 && dy == 0 && dz == 0) continue;
                int nx = x + dx;
                int ny = y + dy;
                int nz = z + dz;
                if(nx < 0 || nx >= m) continue;
                if(ny < 0 || ny >= n) continue;
                if(nz < 0 || nz >= o) continue;
                cnt += cube[nx][ny][nz] == '#';
            }
        }
    }
    return cnt;
}

void Solution2020::day16(const std::string& inputfile) {
    std::vector<std::vector<int>> tickets;
    std::vector<int> myticket;
    std::vector<std::function<bool(int)>> validators;
    std::vector<std::string> types;
    int data_type = 0;
    int ans1 = 0;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        if(line.empty()) ++data_type;
        else{
            if(data_type == 0){// collect validators
                std::string type, or_str;
                size_t pos = line.find(':');
                type = line.substr(0, pos);
                std::istringstream iss(line.substr(pos+1));
                char _str;
                int a1, b1, a2, b2;
                iss >> a1 >> _str >> b1 >> or_str >> a2 >> _str >> b2;
                validators.push_back([a1, b1, a2, b2](int x){
                    return  (x >= a1 && x <= b1) || (x >= a2 && x <= b2);
                });
                types.push_back(type);
            }
            else if(data_type == 1){ // collect my ticket
                if(std::isdigit(line[0])){
                    std::istringstream iss(line);
                    std::string x;
                    std::vector<int> ticket;
                    while(getline(iss, x, ',')) ticket.push_back(std::stoi(x));
                    tickets.push_back(ticket);
                    myticket = std::move(ticket);
                }
            }
            else if(data_type == 2){ // collect my nearby
                if(std::isdigit(line[0])){
                    std::istringstream iss(line);
                    std::string x;
                    std::vector<int> ticket;
                    while(getline(iss, x, ',')) ticket.push_back(std::stoi(x));
                    
                    int valid_row = true;
                    for(auto& val: ticket){
                        int valid_count = 0;
                        for(const auto&validator: validators){
                            valid_count += validator(val);
                        }
                        if(valid_count == 0) {
                            //PART 1
                            ans1 += val;
                            valid_row = false;
                        }
                    }
                    if(valid_row) tickets.push_back(std::move(ticket)); // collecting only valid tickets
                }
            }
        }
        return true;
    });
    
    //PART 2 Solution using bitmasks
    // Helper functiont to find index of highest bit 00001000->3
    auto get_index = [](uint32_t val) {
        int row = 0;
        while(val >>= 1) ++row;
        return row;
    };
   
    // prepare masks
    uint32_t all_high = (1 << validators.size()) -1; // all hight bits, meaning there are all possibile solutions
    std::vector<uint32_t> masks(validators.size(), all_high);
    std::unordered_map<uint32_t, uint32_t> solutions;
    for(auto& ticket: tickets){
        for(size_t j = 0; j < ticket.size(); ++j){
            for(int k = 0; k < validators.size(); ++k){
                if(!validators[k](ticket[j])) masks[j] &= (~(1<<k));  // Unset bit that are not possible
            }
        }
    }
    
    // start searching
    while(all_high){
        for(uint32_t j = 0; j < masks.size(); ++j){
            // Unset bit if already found
            masks[j] &= all_high;
            // if only one bit is high, row found.
            // Unset the corresponding bit
            if(masks[j] && !(masks[j]&(masks[j]-1))) {
                all_high &= ~masks[j];
                solutions[get_index(masks[j])] = j;
            }
        }
    }
    /*
    //PART 2 Solution using sets
    
    std::unordered_map<size_t, std::unordered_set<size_t>> all_options;
    std::unordered_map<size_t, size_t> solutions;
    std::unordered_set<size_t> initial_options;
    for(size_t i = 0; i < validators.size(); ++i) initial_options.insert(i);
    for(size_t i = 0; i < validators.size(); ++i) all_options[i] = initial_options;
    for(auto& ticket: tickets){
        for(size_t j = 0; j < ticket.size(); ++j){
            for(int k = 0; k < validators.size(); ++k){
                if(!validators[k](ticket[j])) all_options[j].erase(k);  // Unset bit that are not possible
            }
        }
    }
    
    while(solutions.size() != all_options.size()){
        for(auto& [column, options]: all_options){
            // Eliminate columns that are
            // already present in solution
            for(auto& [column, id]: solutions) options.erase(column);
            // If there is only one column left,
            // then update solutions
            if(options.size() == 1)  solutions[*options.begin()] = column;
        }
    }
     */
    // Finally show the product of columns that starts
    // with departure
    int64_t ans2 = 1;
    for(auto& [idx, col]: solutions){
        std::string name = types[idx];
        if(name.find("departure") != std::string::npos){
            ans2 *= myticket[col];
        }
    }
    std::cout << "ans1: " << ans1 << std::endl;
    std::cout << "ans2: " << ans2 << std::endl;

}

void Solution2020::day15() {
    std::cout << "ans1: " << find_nth_number({12,1,16,3,11,0}, 2020) << std::endl;
    std::cout << "ans2: " << find_nth_number({12,1,16,3,11,0}, 30000000) << std::endl;
}

int Solution2020::find_nth_number(const std::vector<int>& nums, int n) {
    std::unordered_map<int, int> imap;
    int last_spoken = 0, next_spoken = 0;
    int cturn = 1;
    for(;cturn < nums.size(); ++cturn){
        last_spoken = nums[cturn];
        imap[nums[cturn-1]] = cturn;
    }

    while(cturn < n){
        auto itr = imap.find(last_spoken);
        if(itr != imap.end()){
            auto& [num, turn] = *itr;
            next_spoken = cturn - turn;
        }
        else{
            next_spoken = 0;
        }
        imap[last_spoken] = cturn;
        last_spoken = next_spoken;
        ++cturn;
    }
    return last_spoken;
}
void Solution2020::day14(const std::string& inputfile) {
    const std::string imask = "mask";
    const std::string imem = "mem[";
    uint64_t unset_mask = 0, set_mask = 0, floating_mask = 0;
    std::unordered_map<uint64_t, uint64_t> address_map_v1, address_map_v2;
    std::vector<uint64_t> address_masks;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        std::istringstream iss(line);
        std::string info, value;
        char dummy;
        iss >> info >> dummy >> value;
        if(info == imask){
            set_mask = 0; unset_mask = 0;
            address_masks.clear();
            for(char m: value){
                // Preparing masks
                set_mask <<= 1;
                unset_mask <<= 1;
                floating_mask <<= 1;
                if(address_masks.empty()) address_masks.push_back(0);
                else for(uint64_t& address: address_masks)  address <<= 1;

                // Updating masks
                if(m == '0') unset_mask |= 1;
                else if(m == '1') set_mask |= 1;
                else if(m == 'X') {
                    floating_mask |= 1;
                    size_t size = address_masks.size();
                    for(size_t i = 0; i < size; ++i){
                        address_masks.push_back(address_masks[i]|1);
                    }
                }
            }
        }
        else {
            // Updating data
            uint64_t mem_add = std::stoull(info.substr(imem.size()));
            uint64_t mem_val = std::stoull(value);
            //PART 1
            address_map_v1[mem_add] = (mem_val | set_mask) & (~unset_mask);
            //PART 2
            for(uint64_t address_mask:  address_masks){
                mem_add &= (~floating_mask);
                mem_add |= (address_mask | set_mask);
                address_map_v2[mem_add] = mem_val;
            }
        }
        return true;
    });
    
    uint64_t sum1 = 0;
    for(const auto& [address, value]: address_map_v1) sum1 += value;
    
    uint64_t sum2 = 0;
    for(const auto& [address, value]: address_map_v2) sum2 += value;
    
    std::cout << "ans1: " << sum1 << std::endl;
    std::cout << "ans2: " << sum2 << std::endl;
}

void Solution2020::day13(const std::string& inputfile) {
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

int64_t Solution2020::win_gold(const std::vector<int64_t>& ids) {
    int64_t inc  = 1, time = 0;
    for(int i = 0; i < ids.size(); ++i){
        if(ids[i] == -1) continue;
        int64_t mod = ((-i%ids[i]) + ids[i])%ids[i]; //keeping -i%ids[i] positive
        while (time % ids[i] != mod) time+= inc;
        inc *= ids[i];
    }
    return time;
}
std::pair<int64_t, int64_t> Solution2020::get_earliest_id(const std::vector<int64_t>& ids, int64_t earliest_time) {
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

void Solution2020::day12(const std::string& inputfile) {
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

uint32_t Solution2020::get_displacement(const std::vector<std::pair<char, int>>& commands,
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

void Solution2020::day11(const std::string& inputfile) {
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

size_t Solution2020::occupied_seats(const std::vector<std::string>& layout, bool adjacent, int max_neigbhour){
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

size_t Solution2020::count_seats(const std::vector<std::string>& layout, char seat){
    size_t occupied_seats = 0;
    for(const auto& row: layout){
        occupied_seats += std::count_if(row.begin(), row.end(), [seat](char c){ return c == seat;});
    }
    return occupied_seats;
}

size_t Solution2020::count_neigbhour(const std::vector<std::string>& layout, size_t x, size_t y, char seat,  bool adjacent){
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

void Solution2020::day10(const std::string& inputfile) {
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

std::pair<uint64_t, uint64_t> Solution2020::steps_one_three(const std::unordered_set<int>& data, int first, int last) {
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

uint64_t Solution2020::path_count(const std::unordered_set<int>& data, int first, int last) {
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

void Solution2020::day9(const std::string& inputfile, size_t preamble_size) {
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
void Solution2020::day8(const std::string& inputfile) {
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


void Solution2020::fixCycle(std::vector<std::pair<std::string, int>>& instructions) {
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

void Solution2020::execute(const std::vector<std::pair<std::string, int>> &instructions,
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
void Solution2020::day7(const std::string& inputfile, const std::string& target) {
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

size_t Solution2020::dependencies(const std::unordered_map<std::string, std::unordered_set<std::string>> &adj,
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

size_t Solution2020::children(const std::unordered_map<std::string,std::unordered_map<std::string, int> >& adj,
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
void Solution2020::day6(const std::string& inputfile) {
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
void Solution2020::day5(const std::string& inputfile) {
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
void Solution2020::day4(const std::string& inputfile) {
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

int Solution2020::strToInt(const std::string &str, char hiBit) {
    int p = 1, res = 0;
    for(auto it = str.rbegin(); it != str.rend(); ++it,  p <<= 1){
        res += (*it == hiBit) * p;
    }
    return res;
}

bool Solution2020::isValidYear(const std::string& arg, int yMin, int yMax) {
    if (arg.size() != 4) return false;
    
    std::istringstream iss(arg);
    int val;
    iss >> val;
    return isBetween(val, yMin, yMax);
}

bool Solution2020::isValidHeight(const std::string &arg, int cMin, int cMax, int iMin, int iMax) {
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

bool Solution2020::isValidPID(const std::string &arg) {
    if(arg.size() != 9) return false;
    return std::find_if_not(arg.begin(),
                             arg.end(),
                             [](char c){ return std::isdigit(c);}) == arg.end();
}

bool Solution2020::isValidEyeColor(const std::string &arg, const std::unordered_set<std::string>& eColors) {
    std::istringstream iss(arg);
    std::string val;
    iss >> val;
    return eColors.find(val) != eColors.end();
}

bool Solution2020::isValidHairColor(const std::string &arg) {
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

bool Solution2020::isBetween(int x, int a, int b){
    return x >= a && x <= b;
}

bool Solution2020::isOkay(const std::unordered_map<std::string, std::function<bool (std::string)> > &validators,
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
void Solution2020::day3(const std::string& inputfile) {
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
void Solution2020::day2(const std::string& inputfile) {
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

void Solution2020::day1(const std::string& inputfile, int arg1)
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
std::pair<int64_t, int64_t>  Solution2020::two_sum(const std::vector<int64_t>& data, int sum) {
    std::unordered_set<int64_t> cache;
    for(int64_t num: data){
        if(cache.find(sum-num) != cache.end()) return {num, sum-num};
        cache.insert(num);
    }
    return {0,0};
}


// Time:  O(n*n)
// Space: O(n)
std::tuple<int64_t, int64_t,int64_t> Solution2020::three_sum(const std::vector<int64_t>& data, int sum){
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




