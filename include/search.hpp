/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _SEARCH_HPP_
#define _SEARCH_HPP_

#include "util.hpp"

std::vector<std::string> filter(std::vector<std::string> wordList, std::pair<std::string, std::string> filter);
std::vector<std::string> filterHM(std::vector<std::string> wordList, std::pair<std::string, std::string> filter);
std::string grade(std::string guess, std::string answer);
double rate(std::vector<std::string> guess, std::vector<std::string> words, int searchMode);
void rateAll(std::vector<std::string> guess, std::vector<std::string> words, std::string wordlist);

std::vector<std::vector<std::string>> SplitVector(const std::vector<std::string>& vec, size_t n);

int countDistinct(std::string s);
bool inputWordSet(std::vector<std::string> &wordSet, unsigned int correctSize);

double rate(std::vector<std::string> guess, std::vector<std::string> words, int searchMode);
void findbest(std::vector<std::string> valids, std::vector<std::string> validGuesses, int numThreads, int searchMode, std::vector<std::string> prefix, bool fullRankingOut, int fullRankingRequiredScore, int setSize, int unique, bool newBestPrints, std::string forceInclude, std::string forceExclude, std::vector<int> uniqueSteps, int updatePrintFrequency, std::string wordlist,  std::vector<std::string> forceExcludePos,  std::vector<std::string> forceIncludePos, bool answersOnlyFirst, std::string keyword);
void findbest(std::string keyword);
#endif
