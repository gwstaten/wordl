#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <cmath>
std::vector<std::string> filter(std::vector<std::string> wordList, std::pair<std::string, std::vector<int>> filter);
std::vector<int> grade(std::string guess, std::string answer);
double findBestInfo(std::vector<std::string> words);
std::pair<std::string,double> findBest2(std::vector<std::string> words);
std::pair<std::string,double> findBest1(std::vector<std::string> words);
std::pair<std::string,double> findBestDiff(std::vector<std::string> words, std::vector<std::string> validWords);
