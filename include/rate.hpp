/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _RATE_HPP_
#define _RATE_HPP_

#include "util.hpp"

double rate(std::vector<std::string> guess, std::vector<std::string> words, int searchMode, std::vector<std::string> prefixColorings);
double rateInts(const std::vector<std::string> guess, const std::vector<std::string> words, int searchMode, std::vector<unsigned long long int> prefixColorings);
void rateAll(std::vector<std::string> guess, std::vector<std::string> words, std::string wordlist, bool rateFileColorings);

#endif
