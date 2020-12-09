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

class Solution
{
public:

    Solution();
    static void run(bool all = false);

private:
    static void day9(const std::string& inputfile, size_t preamble_size);
    
    static void day8(const std::string& inputfile);
    static void execute(const std::vector<std::pair<std::string, int>>& instructions,
                        const std::function<void(std::pair<int, int>)>& handler);

    static void fixCycle(std::vector<std::pair<std::string, int>>& instructions);
    
    static void day7(const std::string& inputfile,  const std::string &target);
    static size_t children(const std::unordered_map<std::string, std::unordered_map<std::string, int>>& adj, const std::string& u);
    static size_t  dependencies(const std::unordered_map<std::string, std::unordered_set<std::string>> &adj,
                               const std::string &target);
    
    static void day6(const std::string& inputfile);
    
    static void day5(const std::string& inputfile);
    static int strToInt(const std::string& string, char hiBit);
    
    static void day4(const std::string& inputfile);
    static bool isValidYear(const std::string& arg, int yMin, int yMax);
    static bool isValidHeight(const std::string& arg, int cMin, int cMax, int iMin, int iMax);
    static bool isBetween(int x, int a, int b);
    static bool isOkay(const std::unordered_map<std::string, std::function<bool(std::string)>>& validators,
                       const std::unordered_map<std::string, std::string>& data);
    static bool isValidHairColor(const std::string& arg);
    static bool isValidEyeColor(const std::string& arg, const std::unordered_set<std::string>& eColors);
    static bool isValidPID(const std::string& arg);
    
    static void day3(const std::string& inputfile);
    
    static void day2(const std::string& inputfile);

    static void day1_1(const std::string& inputfile, int arg1);
    static void day1_2(const std::string& inputfile, int arg1);
    
    static void read_if(const std::string& filepath,
                          const std::function<bool(std::string, bool)>& handler);
    
    static const std::unordered_map<std::string, std::function<void(void)>> S_SOLUTIONS;

};
