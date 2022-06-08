/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _UTIL_HPP_
#define _UTIL_HPP_

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <map>
#include <unordered_map>
#include <iterator>
#include <filesystem>
#include <thread>
#include <algorithm>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime> 
#include <random>

bool inputWordSet(std::vector<std::string> &wordSet, unsigned int correctSize);
int countDistinct(std::string s);

std::string grade(std::string guess, std::string answer);
std::vector<std::string> filter(std::vector<std::string> wordList, std::pair<std::string, std::string> filter);
std::vector<std::string> filterHM(std::vector<std::string> wordList, std::pair<std::string, std::string> filter);
std::vector<std::vector<std::string>> SplitVector(const std::vector<std::string>& vec, size_t n);

std::pair<std::string, std::string> parseoption(std::string arg);

#endif
