//
//  CSolution.hpp
//  adventofcode
//
//  Created by Sumit Jha on 14.12.20.
//
#pragma once

#include <string>
#include <functional>
#include <vector>

class ISolution
{
public:
    static void solve(int year);
    virtual void run(int day = -1);

protected:
    static void read_if(const std::string& filepath,
                        const std::function<bool(std::string, bool)>& handler);

    std::vector<std::function<void(void)>> m_solutions;
};



