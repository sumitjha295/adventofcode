//
//  solution.cpp
//  adventofcode
//
//  Created by Sumit Jha on 02.12.20.
//

#include "solution_2019.hpp"

namespace
{
    const std::string resource_dir = "data/2019/";
    struct complex_hash {
        inline size_t operator()(const std::complex<int> &v) const {
            std::hash<int> int_hasher;
            return int_hasher(v.real()) ^ int_hasher(v.imag());
        }
    };
    const std::unordered_map<char, std::complex<int>> directions = {
        {'R', {1, 0}},
        {'L', {-1, 0}},
        {'U', {0, 1}},
        {'D', {0, -1}},
    };
}


Solution2019::Solution2019()
{
    m_solutions = {
        std::bind(day1, resource_dir + "day1.txt"),
        std::bind(day2, resource_dir + "day2.txt"),
        std::bind(day3, resource_dir + "day3.txt"),
        std::bind(day4, "367479-893698"),
    };
}

Solution2019::~Solution2019()
{
}

void Solution2019::run(int day){
    ISolution::run();
}
void Solution2019::day4(const std::string& range_string) {
    std::istringstream iss(range_string);
    int mi, mx;
    char dummy;
    iss >> mi >> dummy >> mx;
    int res1 = 0, res2 = 0;
    for(int i = mi; i <= mx; ++i){
        std::string num = std::to_string(i);
        bool increasing = false;
        bool hasdouble = false;
        bool exclusive = false;
        // PART 1 and 2
        for(int i = 1, cnt = 1; i < num.size(); ++i){
            if(num[i-1] > num[i]) {break;}
            if(num[i-1] == num[i]) ++cnt;
            if(num[i-1] != num[i] || i == num.size()-1) {
                if(i == num.size()-1) increasing = true;
                if(cnt == 2) exclusive = true;
                if(cnt > 1) hasdouble = true;
                cnt = 1;
            }
        }
        res1 += increasing && hasdouble;
        res2 += increasing && hasdouble && exclusive;
    }
    std::cout << "ans1: " << res1 << std::endl;
    std::cout << "ans2: " << res2 << std::endl;
}

void Solution2019::day3(const std::string& inputfile) {
    std::unordered_set<std::complex<int>, complex_hash> cords;
    std::unordered_map<std::complex<int>, int, complex_hash> cord_map;
    int line_number = 0;
    int dist = INT_MAX;
    int time = INT_MAX;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        std::istringstream iss(line);
        std::string move;
        char separator = ',';
        std::complex<int> pos(0,0);
        int steps = 0;
        if(line_number == 0){
            while(std::getline(iss, move, separator)){
                std::istringstream iss(move);
                char dir;
                int value;
                iss >> dir >> value;
                auto itr = directions.find(dir);
                if(itr == directions.end()) continue;;
                while(value--) {
                    pos += itr->second;
                    //PART 1
                    cords.insert(pos);
                    //PART 2
                    ++steps;
                    if(!cord_map.count(pos)) cord_map[pos] = steps;
                }
            }
        }
        else if(line_number == 1){
            while(std::getline(iss, move, separator)){
                std::istringstream iss(move);
                char dir;
                int value;
                iss >> dir >> value;
                auto itr = directions.find(dir);
                if(itr == directions.end()) continue;;
                while(value--) {
                    pos += itr->second;
                    //PART 1
                    if(cords.find(pos) != cords.end()){
                        dist = std::min(dist, abs(pos.real()) + abs(pos.imag()));
                    }
                    //PART 2
                    ++steps;
                    if(cord_map.count(pos)){
                        time = std::min(time, steps + cord_map[pos]);
                    }
                }

            }
        }
        ++line_number;
        return true;
    });
    //for(auto& [x, y]: cords) std::cout << x<<"," << y << std::endl;
    std::cout << "ans1: " << dist << std::endl;
    std::cout << "ans2: " << time << std::endl;
}

void Solution2019::day2(const std::string& inputfile) {
    std::vector<int> opcodes;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        std::istringstream iss(line);
        std::string val;
        while(std::getline(iss, val, ',')) opcodes.push_back(std::stoi(val));
        return false;
    });
    
    //PART 2
    int expected = 19690720;
    int res = 0;
    for(int i = 0; i < 100; ++i){
        for(int j = 0; j < 100; ++j){
            if(execute_opcode(opcodes, i, j) == expected){
                res = 100*i + j;
                break;
            }
        }
    }
    //PART 1
    std::cout << "ans1: " << execute_opcode(opcodes, 12, 2) << std::endl;
    std::cout << "ans2: " << res << std::endl;
}

int  Solution2019::execute_opcode(std::vector<int> opcodes, int val1, int val2){
    opcodes[1] = val1;
    opcodes[2] = val2;
    for(size_t i = 0; i < opcodes.size();++i){
        if(opcodes[i] == 1){
            opcodes[opcodes[i+3]]  = opcodes[opcodes[i+1]] + opcodes[opcodes[i+2]];
            i +=  3;
        }
        else if(opcodes[i] == 2){
            opcodes[opcodes[i+3]]  = opcodes[opcodes[i+1]] * opcodes[opcodes[i+2]];
            i +=  3;
        }
        else if(opcodes[i] == 99) break;
    }
    return opcodes[0];
}

void Solution2019::day1(const std::string& inputfile) {
    int64_t sum1 = 0, sum2 = 0;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        std::istringstream iss(line);
        int64_t fuel;
        iss >> fuel;
        // PART 1
        fuel /= 3;
        fuel -= 2;
        sum1 += fuel;
        
        //PART 2
        while(fuel > 0){
            sum2 += fuel;
            fuel /= 3;
            fuel -= 2;
        }
        return true;
    });
    std::cout << "ans1: " << sum1 << std::endl;
    std::cout << "ans2: " << sum2 << std::endl;
}
