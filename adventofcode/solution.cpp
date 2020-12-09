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
    const std::string current = "day9";
}

const std::unordered_map<std::string, std::function<void(void)>> Solution::S_SOLUTIONS = {
    {"day1_1" , std::bind(day1_1, resource_dir + "day1.txt", 2020)},
    {"day1_2" , std::bind(day1_2, resource_dir + "day1.txt", 2020)},
    {"day2" , std::bind(day2, resource_dir + "day2.txt")},
    {"day3" , std::bind(day3, resource_dir + "day3.txt")},
    {"day4" , std::bind(day4, resource_dir + "day4.txt")},
    {"day5" , std::bind(day5, resource_dir + "day5.txt")},
    {"day6" , std::bind(day6, resource_dir + "day6.txt")},
    {"day7" , std::bind(day7, resource_dir + "day7.txt", "shiny gold")},
    {"day8" , std::bind(day8, resource_dir + "day8.txt")},
    {"day9" , std::bind(day9, resource_dir + "day9.txt", 25)},
};

Solution::Solution()
{
}
// Time:  O(n*p) n is size of input data, p is size of the preamble
// Space: O(p)
void Solution::day9(const std::string& inputfile, size_t preamble_size) {
    std::vector<int64_t> data;
    int64_t ans1 = 0, ans2 = 0;
    // // PART 1
    std::unordered_set<int64_t> preamble;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        std::istringstream iss(line);
        int64_t val;
        iss >> val;
        bool is_valid_number = preamble.size() < preamble_size;
        // prepare the preample with index and value
        if(preamble.size() == preamble_size){
            // check the validity of the number
            for(auto it = data.rbegin();
                it != next(data.rbegin(), preamble_size);
                ++it ){
                int64_t num1  = *it;
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
    std::unordered_map<std::string, size_t> inorder;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        std::istringstream iss(line);
        std::string shade, color, dummy;
        int num;
        while(iss >> shade >> color >> dummy >> dummy){
            std::string parent_color = shade + " " + color;
            if(!inorder.count(parent_color)) inorder[parent_color] = 0;
            while(iss >> num >> shade >> color >> dummy){
                std::string next_color = shade + " " + color;
                forward_adj[parent_color].insert({next_color, num});
                backward_adj[next_color].insert(parent_color);
                ++inorder[next_color];
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
    const std::vector<std::pair<int, int>> moves = {{1,1},{3,1},{5,1},{7,1},{1,2}};
    std::vector<int> trees(moves.size());
    std::vector<int> pos(moves.size());
    int level = 0;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        size_t n = line.size();
        for(int i = 0; i < moves.size();++i){
            const auto& [x,y] = moves[i];
            if(level%y == 0){
                if(line[pos[i]%n] == '#') ++trees[i];
                pos[i] += x;
            }
        }
        ++level;
        return true;
    });
    std::cout<< "trees("<<  moves[1].first << "," <<  moves[1].second << "): " << trees[1] << std::endl;
    long long res = 1;
    for(const auto& tree: trees){
        std::cout<< tree << "  ";
        res *= tree;
    }
    std::cout<< " = " << res << std::endl;
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
    std::cout<< "1. valid passwords:" << res1 << std::endl;
    std::cout<< "2. valid passwords:" << res2 << std::endl;
}

// Time:  O(n)
// Space: O(n)
void Solution::day1_1(const std::string& inputfile, int arg1)
{
    std::unordered_set<int> data;
    int sum = arg1;
    long long res = 0;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        int num = stoi(line);
        if(data.find(sum-num) != data.end()){
            res = num*(sum-num);
            std::cout<< num << "+" << (sum-num) << "=" << sum <<  std::endl;
            std::cout<< num << "*" << (sum-num) << "=" << res <<  std::endl;
            return false;
        }
        data.insert(num);
        return  true;
    });
}

// Time:  O(n*n)
// Space: O(n)
void Solution::day1_2(const std::string& inputfile, int arg1) {
   std::vector<int> data;
   std::unordered_map<int, size_t> idx;
   int sum = arg1;
   long long res = 0;
    read_if(inputfile, [&](const std::string& line, bool isLast){
       int num = stoi(line);
       idx.insert({num, data.size()});
       data.push_back(num);
       return  true;
   });

   for(size_t i = 0; i < data.size(); ++i){
       for(size_t j = i+1; j < data.size(); ++j){
            int num = data[i] + data[j];
            int left = sum-num;
            if(idx.find(left) != idx.end()){
                size_t k = idx[left];
                if(k > j){
                    res = data[i] * data[j] * left;
                    std::cout<< data[i] << "+" << data[j] << "+" << left << "=" << num+left <<  std::endl;
                    std::cout<< data[i] << "*" << data[j] << "*" << left << "=" << res <<  std::endl;
                    return;
                }
            }
       }
   }
}

void Solution::run(bool all) {
    std::cout<< "-------Started-----" << std::endl;
    if(all){
        for(const auto& [day, solution] : S_SOLUTIONS) {
            std::cout<< "-----" << day << "-----" << std::endl << std::endl;
            solution();
            std::cout<< std::endl;
        }
    }
    else {
        auto itr = S_SOLUTIONS.find(current);
        if(itr != S_SOLUTIONS.end()){
            const auto& [day, solution] = *itr;
            std::cout<< "-----" << day << "-----" << std::endl << std::endl;
            solution();
            std::cout<< std::endl;
        }
    }
    std::cout<< "-------Finished-----" << std::endl;

}


void Solution::read_if(const std::string& filepath,
                         const std::function<bool(std::string, bool)>& handler){
    std::ifstream infile(filepath);
    std::string line;
    while (std::getline(infile, line))
        if(!handler(line, infile.peek() == EOF)) return;
}


