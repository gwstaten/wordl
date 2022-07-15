/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _SEARCH_HPP_
#define _SEARCH_HPP_

#include "util.hpp"

void findbest(std::vector<std::string> valids, std::vector<std::string> validGuesses, int numThreads, int searchMode, std::vector<std::string> prefix, bool fullRankingOut, int fullRankingRequiredScore, int setSize, int unique, bool newBestPrints, std::string forceInclude, std::string forceExclude, std::vector<int> uniqueSteps, int updatePrintFrequency, std::string wordlist,  std::vector<std::string> forceExcludePos,  std::vector<std::string> forceIncludePos, bool answersOnlyFirst, std::string keyword);
void findbest(std::string keyword);

#endif
