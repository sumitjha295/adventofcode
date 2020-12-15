//
//  solution.hpp
//  adventofcode
//
//  Created by Sumit Jha on 02.12.20.
//


#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <filesystem>
#include <vector>
#include <list>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <map>
#include <complex>

#include "ISolution.hpp"

class Solution2019: public ISolution
{
public:

    Solution2019();
    virtual void run(int day = -1) override;
    virtual ~Solution2019();
    
private:
    static void day1(const std::string& inputfile);
    static void day2(const std::string& inputfile);
    
    static const std::vector<std::function<void(void)>> S_SOLUTIONS;
};
