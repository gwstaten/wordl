/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "search.hpp"

std::vector<int> grade(std::string guess, std::string answer)
{
  std::vector<int> output(guess.length(),0);
  std::vector<int> usedAnswer(guess.length(),0);
  for(unsigned int i = 0; i < guess.length(); i++)
  {
    if(guess.at(i) == answer.at(i))
    {
      output[i] = 2;
      usedAnswer[i] = 1;
    }
  }
  for(unsigned int guessSpot = 0; guessSpot < guess.length(); guessSpot++)
  {
    if(output[guessSpot] == 0)
    {
      bool stillSearching = true;
      for(unsigned int answerSpot = 0; answerSpot < guess.length() && stillSearching; answerSpot++)
      {
        if(usedAnswer[answerSpot] == 0 && guess.at(guessSpot) == answer.at(answerSpot))
        {
          output[guessSpot] = 1;
          usedAnswer[answerSpot] = 1;
          stillSearching = false;
        }
      }
    }
  }
  return output;
}

std::vector<std::string> filter(std::vector<std::string> wordList, std::pair<std::string, std::vector<int>> filter)
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
std::vector<std::string> filterHM(std::vector<std::string> wordList, std::pair<std::string, std::vector<int>> filter)
{
  std::vector<std::string> stillGood;
  for(unsigned int wordn = 0; wordn < wordList.size(); wordn++)
  {
    std::string word = wordList[wordn];
    bool notDone = true;
    for(unsigned int filterL = 0; filterL < word.length() && notDone; filterL++)
    {
      if(filter.second[filterL] == 2)
      {
        if(word.at(filterL) != filter.first.at(filterL))
        {
          notDone = false;
        }
      }
    }
    for(unsigned int filterL = 0; filterL < word.length() && notDone; filterL++)
    {
      if(filter.second[filterL] == 0)
      {
        for(unsigned int i = 0; i < word.length() && notDone; i++)
        {
          if(word.at(i) == filter.first.at(filterL))
          {
            bool good = true;
            for(unsigned int j = 0; j < word.length() && good; j++)
            {
              if((filter.second[j] == 2 || filter.second[j] == 1) && filter.first.at(j) == filter.first.at(filterL) && i != filterL)
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
      if(filter.second[filterL] == 1)
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

void rateAll(std::vector<std::string> guess, std::vector<std::string> words)
{
  std::map<unsigned long long int, std::pair<int, double>> ratingsMap;
  for(unsigned int answer = 0; answer < words.size(); answer++)
  {
    std::vector<int> rating;
    std::vector<std::vector<int>> ratings;
    for(unsigned int i = 0; i < guess.size(); i++)
    {
      std::vector<int> tempRating = grade(guess[i], words[answer]);
      ratings.push_back(tempRating);
      for(unsigned int j = 0; j < tempRating.size(); j++)
      {
        rating.push_back(tempRating[j]);
      }
    }
    unsigned long long int total = 0;
    for(unsigned int i = 0; i < guess[0].length() * guess.size(); i++)
    {
      total *= 3;
      total += rating[i];
    }
    if(ratingsMap.find(total) == ratingsMap.end())
    {
      std::vector<std::string> remaining = words;
      for(unsigned int i = 0; i < guess.size(); i++)
      {
        remaining = filter(remaining, std::make_pair(guess[i],ratings[i]));
      }
      ratingsMap[total] = std::make_pair(0,remaining.size());
      /*if(remaining.size() == 10)
      {
        for(unsigned int i = 0; i < remaining.size(); i++)
        {
          std::cout << remaining[i] << std::endl;
        }
        std::cout << std::endl;
      }*/
    }
    ratingsMap[total].first++;
  }
  std::vector<double> total(5,0);
  std::map<unsigned long long int, std::pair<int, double>>::iterator it;
  total[1] = ratingsMap.size();
  for(it = ratingsMap.begin(); it != ratingsMap.end(); ++it)
  {
    for(unsigned int searchMode = 0; searchMode < total.size(); searchMode++)
    {
      switch(searchMode)
      {
        case 0:
          total[searchMode] += ((it->second).first * (it->second).second);
          break;
        case 2:
          if((it->second).second == 1)
          {
            total[searchMode]++;
          }
          break;
        case 3:
          if(total[searchMode] < (it->second).second)
          {
            total[searchMode] = (it->second).second;
          }
          break;
        case 4:
          total[searchMode] += (it->second).first * std::log((it->second).second / words.size()) / std::log(0.5);
          break;
      }
    }
  }
  total[0] /= words.size();
  total[4] /= words.size();
  std::cout << "Guaranteed solves: " << total[2] << "/" << words.size() << std::endl;
  std::cout << "Ambiguity: " << words.size() - total[2] << "/" << words.size() << std::endl;
  std::cout << "Average bits of info: " << total[4] << std::endl;
  std::cout << "Average remaining possibilities: " << total[0] << std::endl;
  std::cout << "1/n score: " << total[1] << std::endl;
  std::cout << "Largest ambiguous set: " << total[3] << std::endl << std::endl;
}

double rate(std::vector<std::string> guess, std::vector<std::string> words, int searchMode)
{
  std::map<unsigned long long int, std::pair<int, double>> ratingsMap;
  for(unsigned int answer = 0; answer < words.size(); answer++)
  {
    std::vector<int> rating;
    std::vector<std::vector<int>> ratings;
    for(unsigned int i = 0; i < guess.size(); i++)
    {
      std::vector<int> tempRating = grade(guess[i], words[answer]);
      ratings.push_back(tempRating);
      for(unsigned int j = 0; j < tempRating.size(); j++)
      {
        rating.push_back(tempRating[j]);
      }
    }
    unsigned long long int total = 0;
    for(unsigned int i = 0; i < guess[0].length() * guess.size(); i++)
    {
      total *= 3;
      total += rating[i];
    }
    if(ratingsMap.find(total) == ratingsMap.end())
    {
      if(searchMode == 1 || searchMode == 3 || searchMode == 4 || searchMode == 5)
      {
        std::vector<std::string> remaining = words;
        for(unsigned int i = 0; i < guess.size(); i++)
        {
          remaining = filter(remaining, std::make_pair(guess[i],ratings[i]));
        }
        ratingsMap[total] = std::make_pair(0,remaining.size());
      }
      else if(searchMode == 2)
      {
        ratingsMap[total] = std::make_pair(0,1);
      }
    }
    ratingsMap[total].first++;
  }
  double total = 0;
  std::map<unsigned long long int, std::pair<int, double>>::iterator it;
  for(it = ratingsMap.begin(); it != ratingsMap.end(); ++it)
  {
    switch(searchMode)
    {
      case 1:
        total += ((it->second).first * (it->second).second);
        break;
      case 2:
        total++;
        break;
      case 3:
        if((it->second).second == 1)
        {
          total++;
        }
        break;
      case 4:
        if(total < (it->second).second)
        {
          total = (it->second).second;
        }
        break;
      case 5:
        total += (it->second).first * std::log((it->second).second / words.size()) / std::log(0.5);
        break;
    }
  }
  if(searchMode == 1 || searchMode == 5)
  {
    total /= words.size();
  }
  return total;
}

void findBestThread(std::vector<std::string> words, std::vector<std::string> validWords, std::pair<std::string,double> &out, int searchMode, bool reversed)
{
  //std::cout << std::endl;
  int lowest = 0;
  double lowestAve = 10000;
  std::vector<double> scores;
  for(unsigned int guess = 0; guess < validWords.size(); guess++)
  {
    //std::cout << guess << " " << validWords[guess] << " ";
    std::vector<std::string> guessVec = {validWords[guess]};
    double total = rate(guessVec, words, searchMode);
    scores.push_back(total);
    //std::cout << total << " ";
    if((((total < lowestAve && (searchMode == 1 || searchMode == 4)) || (total > lowestAve && (searchMode == 2 || searchMode == 3 || searchMode == 5))) && !reversed) || (((total > lowestAve && (searchMode == 1 || searchMode == 4)) || (total < lowestAve && (searchMode == 2 || searchMode == 3 || searchMode == 5))) && reversed) || guess == 0)
    {
      lowest = guess;
      lowestAve = total;
      //std::cout << "new best!";
    }
    //std::cout << std::endl;
  }
  out = std::make_pair(validWords[lowest],lowestAve);
}

template<typename T>
std::vector<std::vector<T>> SplitVector(const std::vector<T>& vec, size_t n)
{
  std::vector<std::vector<T>> outVec;
  size_t length = vec.size() / n;
  size_t remain = vec.size() % n;
  size_t begin = 0;
  size_t end = 0;
  for (size_t i = 0; i < std::min(n, vec.size()); ++i)
  {
    end += (remain > 0) ? (length + !!(remain--)) : length;
    outVec.push_back(std::vector<T>(vec.begin() + begin, vec.begin() + end));
    begin = end;
  }
  return outVec;
}

std::pair<std::string,double> fbThreads(std::vector<std::string> words, std::vector<std::string> validWords, int threads, int searchMode, bool reversed)
{
  unsigned int numThreads = threads;
  if(numThreads > validWords.size() / 10)
  {
    numThreads = validWords.size() / 10;
  }
  if(validWords.size() / 10 < 1)
  {
    numThreads = 1;
  }
  std::vector<std::vector<std::string>> validWordsChunks;
  if(numThreads != 1)
  {
    validWordsChunks = SplitVector(validWords, numThreads);
  }
  else
  {
    validWordsChunks.push_back(validWords);
  }
  std::vector<std::pair<std::string,double>> results(numThreads,std::make_pair("",0));
  std::vector<std::thread> threadVector;
  for(unsigned int i = 0; i < numThreads; i++)
  {
    threadVector.push_back(std::thread(findBestThread,words, validWordsChunks[i], std::ref(results[i]), searchMode, reversed));
  }
  double min;
  std::string minWord;
  for(unsigned int i = 0; i < numThreads; i++)
  {
    if(threadVector[i].joinable())
    {
      threadVector[i].join();
    }
    if(i == 0)
    {
      min = results[i].second;
      minWord = results[i].first;
    }
    else if((((min > results[i].second && (searchMode == 1 || searchMode == 4)) || (min < results[i].second && (searchMode == 2 || searchMode == 3 || searchMode == 5))) && !reversed) || (((min < results[i].second && (searchMode == 1 || searchMode == 4)) || (min > results[i].second && (searchMode == 2 || searchMode == 3 || searchMode == 5))) && reversed))
    {
      min = results[i].second;
      minWord = results[i].first;
    }
  }
  return std::make_pair(minWord,min);
}
