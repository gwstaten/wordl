/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "search.hpp"

int countDistinct(std::string s)
{
  std::unordered_map<char, int> m;
  for(unsigned int i = 0; i < s.length(); i++)
  {
    m[s[i]]++;
  }
  return m.size();
}

void printBest(double a, int searchMode)
{
  if(searchMode == 1)
  {
    std::cout << "Narrows down to " << a << " possibilities on average" << std::endl;
  }
  else if(searchMode == 2)
  {
    std::cout << "Splits up into " << a << " groups" << std::endl;
  }
  else if(searchMode == 3)
  {
    std::cout << "Will get it on the following guess for " << a << " words" << std::endl;
  }
  else if(searchMode == 4)
  {
    std::cout << "Narrows down to " << a << " possibilities in the worst case scenario" << std::endl;
  }
  else if(searchMode == 5)
  {
    std::cout << "Gives " << a << " bits of information on average" << std::endl;
  }
  else
  {
    std::cout << "Gets " << a << " greens on average" << std::endl;
  }
}

std::string grade(std::string guess, std::string answer)
{
  std::string output(guess.length(),'0');
  std::vector<int> usedAnswer(guess.length(),0);
  for(unsigned int i = 0; i < guess.length(); i++)
  {
    if(guess.at(i) == answer.at(i))
    {
      output.at(i) = '2';
      usedAnswer[i] = 1;
    }
  }
  for(unsigned int guessSpot = 0; guessSpot < guess.length(); guessSpot++)
  {
    if(output.at(guessSpot) == '0')
    {
      bool stillSearching = true;
      for(unsigned int answerSpot = 0; answerSpot < guess.length() && stillSearching; answerSpot++)
      {
        if(usedAnswer[answerSpot] == 0 && guess.at(guessSpot) == answer.at(answerSpot))
        {
          output.at(guessSpot) = '1';
          usedAnswer[answerSpot] = 1;
          stillSearching = false;
        }
      }
    }
  }
  return output;
}

std::vector<std::string> filter(std::vector<std::string> wordList, std::pair<std::string, std::string> filter)
{
  std::vector<std::string> stillGood;
  for(unsigned int wordn = 0; wordn < wordList.size(); wordn++)
  {
    std::string word = wordList[wordn];
    if(grade(filter.first, word) == filter.second)
    {
      stillGood.push_back(word);
    }
  }
  return stillGood;
}

std::vector<std::string> filterHM(std::vector<std::string> wordList, std::pair<std::string, std::string> filter)
{
  std::vector<std::string> stillGood;
  for(unsigned int wordn = 0; wordn < wordList.size(); wordn++)
  {
    std::string word = wordList[wordn];
    bool notDone = true;
    for(unsigned int filterL = 0; filterL < word.length() && notDone; filterL++)
    {
      if(filter.second.at(filterL) == '2')
      {
        if(word.at(filterL) != filter.first.at(filterL))
        {
          notDone = false;
        }
      }
    }
    for(unsigned int filterL = 0; filterL < word.length() && notDone; filterL++)
    {
      if(filter.second.at(filterL) == '0')
      {
        for(unsigned int i = 0; i < word.length() && notDone; i++)
        {
          if(word.at(i) == filter.first.at(filterL))
          {
            bool good = true;
            for(unsigned int j = 0; j < word.length() && good; j++)
            {
              if((filter.second.at(j) == '2' || filter.second.at(j) == '1') && filter.first.at(j) == filter.first.at(filterL) && i != filterL)
              {
                good = false;
              }
            }
            if(good)
            {
              notDone = false;
            }
          }
        }
      }
    }
    for(unsigned int filterL = 0; filterL < word.length() && notDone; filterL++)
    {
      if(filter.second.at(filterL) == '1')
      {
        bool found = false;
        for(unsigned int i = 0; i < word.length() && !found; i++)
        {
          if(filter.first.at(filterL) == word.at(i))
          {
            found = true;
          }
        }
        if(!found)
        {
          notDone = false;
        }
      }
    }
    if(notDone)
    {
      stillGood.push_back(word);
    }
  }
  return stillGood;
}

std::vector<std::vector<std::string>> SplitVector(const std::vector<std::string>& vec, size_t n)
{
  std::vector<std::vector<std::string>> outVec;
  size_t length = vec.size() / n;
  size_t remain = vec.size() % n;
  size_t begin = 0;
  size_t end = 0;
  for (size_t i = 0; i < std::min(n, vec.size()); ++i)
  {
    end += (remain > 0) ? (length + !!(remain--)) : length;
    outVec.push_back(std::vector<std::string>(vec.begin() + begin, vec.begin() + end));
    begin = end;
  }
  return outVec;
}