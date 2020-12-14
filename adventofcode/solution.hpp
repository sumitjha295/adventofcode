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


class Solution
{
    class Line : public std::string {};
    friend std::istream& operator>>(std::istream& is, Line& line) {
        return std::getline(is, line);
    };
public:

    Solution();
    static void run(bool all = false);

private:
    static void day14(const std::string& inputfile);

    static void day13(const std::string& inputfile);
    static std::pair<int64_t, int64_t> get_earliest_id(const std::vector<int64_t>& ids, int64_t earliest_time);
    static int64_t win_gold(const std::vector<int64_t>& ids);

    static void day12(const std::string& inputfile);
    static uint32_t get_displacement(const std::vector<std::pair<char, int>>& commands,
                                     std::complex<int> position,
                                     std::complex<int> direction,
                                     bool waypoint);


    static void day11(const std::string& inputfile);
    static size_t occupied_seats(const std::vector<std::string>& layout, bool adjacent, int max_neigbhour);
    static size_t count_neigbhour(const std::vector<std::string>& layout, size_t x, size_t y, char seat, bool adjacent = true);
    static size_t count_seats(const std::vector<std::string>& layout, char seat);

    static void day10(const std::string& inputfile);
    static std::pair<uint64_t, uint64_t> steps_one_three(const std::unordered_set<int>& data, int first, int last);
    static uint64_t path_count(const std::unordered_set<int>& data, int first, int last);

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

    static void day1(const std::string& inputfile, int arg1);
    static std::pair<int64_t, int64_t> two_sum(const std::vector<int64_t>& data, int sum);
    static std::tuple<int64_t, int64_t,int64_t> three_sum(const std::vector<int64_t>& data, int sum);
    
    
    static void read_if(const std::string& filepath,
                          const std::function<bool(std::string, bool)>& handler);
    
    static const std::vector<std::function<void(void)>> S_SOLUTIONS;
};
