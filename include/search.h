#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <cmath>
std::vector<std::string> filter(std::vector<std::string> wordList, std::pair<std::string, std::vector<int>> filter);
std::vector<int> grade(std::string guess, std::string answer);
std::pair<std::string,double> findBest(std::vector<std::string> words, std::vector<std::string> validWords);
