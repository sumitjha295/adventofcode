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
}


Solution2019::Solution2019()
{
    m_solutions = {
        std::bind(day1, resource_dir + "day1.txt"),
        std::bind(day2, resource_dir + "day2.txt"),
    };
}

Solution2019::~Solution2019()
{
}

void Solution2019::run(int day){
    ISolution::run();
}

void Solution2019::day2(const std::string& inputfile) {
    std::vector<int64_t> opcodes;
    read_if(inputfile, [&](const std::string& line, bool isLast){
        std::istringstream iss(line);
        std::string val;
        while(std::getline(iss, val, ',')) opcodes.push_back(std::stoll(val));
        return false;
    });
    
    //PART 1
    opcodes[1] = 12;
    opcodes[2] = 2;
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
    
    std::cout << "ans1: " << opcodes[0] << std::endl;
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
