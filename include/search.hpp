/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <cmath>
#include <map>
#include <cassert>
#include <future>
#include <vector>
#include <unordered_map>

#include "filesystem.hpp"

#ifdef _WIN32
  #include "mingw.thread.h"
#else
  #include <thread>
#endif


std::vector<std::string> filter(std::vector<std::string> wordList, std::pair<std::string, std::vector<int>> filter);
std::vector<int> grade(std::string guess, std::string answer);
std::pair<std::string,double> fbThreads(std::vector<std::string> words, std::vector<std::string> validWords, int threads, int searchMode);
