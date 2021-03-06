/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "util.hpp"

bool inputWordSet(std::vector<std::string> &wordSet, unsigned int correctSize)
{
  std::string wordliststring;
  while(true)
  {
    std::cin >> wordliststring;
    std::transform(wordliststring.begin(), wordliststring.end(), wordliststring.begin(), ::tolower);
    if(wordliststring.length() != correctSize)
    {
      while(true)
      {
        if(std::cin.peek() == '\n')
        {
          break;
        }
        std::cin >> wordliststring;
      }
      return false;
    }
    wordSet.push_back(wordliststring);
    if(std::cin.peek() == '\n')
    {
      getline(std::cin, wordliststring);
      std::transform(wordliststring.begin(), wordliststring.end(), wordliststring.begin(), ::tolower);
      return true;
    }
  }
}

int countDistinct(std::string s)
{
  std::unordered_map<char, int> m;
  for(unsigned int i = 0; i < s.length(); i++)
  {
    m[s[i]]++;
  }
  return m.size();
}

unsigned long long int positions[] = {1,3,9,27,81,243,729,2187,6561,19683,59049,177147,531441};

unsigned long long int grade2(std::string guess, std::string answer)
{
  const unsigned int size = guess.length();
  unsigned long long int out = 0;
  std::vector<bool> usedAnswer(size,false);
  for(unsigned int i = 0; i < size; i++)
  {
    if(guess[i] == answer.at(i))
    {
      out += 2 * positions[i];
      usedAnswer[i] = true;
    }
  }
  for(unsigned int guessSpot = 0; guessSpot < size; guessSpot++)
  {
    if((out / positions[guessSpot]) % 3 == 0)
    {
      bool stillSearching = true;
      for(unsigned int answerSpot = 0; answerSpot < size && stillSearching; answerSpot++)
      {
        if(!usedAnswer[answerSpot] && guess[guessSpot] == answer[answerSpot])
        {
          out += positions[guessSpot];
          usedAnswer[answerSpot] = true;
          stillSearching = false;
        }
      }
    }
  }
  return out;
}

std::string grade(std::string guess, std::string answer)
{
  const unsigned int size = guess.length();
  std::string output(size,'0');
  std::vector<bool> usedAnswer(size,false);
  for(unsigned int i = 0; i < size; i++)
  {
    if(guess[i] == answer.at(i))
    {
      output[i] = '2';
      usedAnswer[i] = true;
    }
  }
  for(unsigned int guessSpot = 0; guessSpot < size; guessSpot++)
  {
    if(output[guessSpot] == '0')
    {
      bool stillSearching = true;
      for(unsigned int answerSpot = 0; answerSpot < size && stillSearching; answerSpot++)
      {
        if(!usedAnswer[answerSpot] && guess[guessSpot] == answer[answerSpot])
        {
          output[guessSpot] = '1';
          usedAnswer[answerSpot] = true;
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

std::pair<std::string, std::string> parseoption(std::string arg)
{
  std::transform(arg.begin(), arg.end(), arg.begin(), ::tolower);

  if(arg[0] == '-')
  {
    if(arg.find('=') != std::string::npos)
    {
      return {arg.substr(1, arg.find('=') - 1), arg.substr(arg.find('=') + 1)};
    }
    else
    {
      return {arg.substr(1), ""};
    }
  }
  return {"", ""};
}
