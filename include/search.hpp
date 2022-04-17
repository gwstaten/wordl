/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
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


std::vector<std::string> filter(std::vector<std::string> wordList, std::pair<std::string, std::vector<int>> filter);
std::vector<std::string> filterHM(std::vector<std::string> wordList, std::pair<std::string, std::vector<int>> filter);
std::vector<int> grade(std::string guess, std::string answer);
std::pair<std::string,double> fbThreads(std::vector<std::string> words, std::vector<std::string> validWords, int threads, int searchMode, bool reversed);
double rate(std::vector<std::string> guess, std::vector<std::string> words, int searchMode);
void rateAll(std::vector<std::string> guess, std::vector<std::string> words, char genFile, std::string wordlist, std::string logLocation);

std::vector<std::vector<std::string>> SplitVector(const std::vector<std::string>& vec, size_t n);

int countDistinct(std::string s);
void printBest(double a, int searchMode);