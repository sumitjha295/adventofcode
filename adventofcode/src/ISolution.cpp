//
//  CSolution.cpp
//  adventofcode
//
//  Created by Sumit Jha on 14.12.20.
//

#include "ISolution.hpp"
#include "solution_2020.hpp"
#include "solution_2019.hpp"

void ISolution::solve(int year){
    if (year == 2020){
        Solution2020 solution;
        solution.run();
    }
    else if (year == 2019){
        Solution2019 solution;
        solution.run();
    }
}

void ISolution::read_if(const std::string& filepath,
                       const std::function<bool(std::string, bool)>& handler){
    std::ifstream infile(filepath);
    std::string line;
    while(std::getline(infile, line))
        if(!handler(line, infile.peek() == EOF)) return;
}

void ISolution::run(int day) {
    std::cout<< "-------Started-----" << std::endl;
    if(day <= 0  || day > m_solutions.size()){
        int i = 0;
        for(const auto& solution : m_solutions) {
            std::cout<< "-----day " << ++i << "-----" << std::endl << std::endl;
            solution();
            std::cout<< std::endl;
        }
    }
    else {
        const auto& solution = m_solutions[day-1];
        std::cout<< "-----day " << day << "-----" << std::endl << std::endl;
        solution();
        std::cout<< std::endl;
    }
    std::cout<< "-------Finished-----" << std::endl;
}

