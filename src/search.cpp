/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "search.hpp"

void rateAll(std::vector<std::string> guess, std::vector<std::string> words, char genFile, std::string wordlist, std::string logLocation)
{
  std::map<unsigned long long int, std::pair<int, double>> ratingsMap;
  std::vector<std::string> forSure;
  std::vector<std::vector<std::string>> ambiguous;
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
      if(remaining.size() == 1)
      {
        forSure.push_back(remaining[0]);
      }
      else
      {
        ambiguous.push_back(remaining);
      }
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
  std::ofstream fout2(logLocation);
  std::cout << "Average bits of info: " << total[4] << std::endl;
  fout2 << "Average bits of info: " << total[4] << std::endl;
  std::cout << "Average remaining possibilities: " << total[0] << std::endl;
  fout2 << "Average remaining possibilities: " << total[0] << std::endl;
  std::cout << "1/n score: " << total[1] << std::endl;
  fout2 << "1/n score: " << total[1] << std::endl;
  std::cout << "Largest ambiguous set: " << total[3] << std::endl;
  fout2 << "Largest ambiguous set: " << total[3] << std::endl;
  std::cout << "Guaranteed solves: " << total[2] << "/" << words.size() << std::endl << "( ";
  fout2 << "Guaranteed solves: " << total[2] << "/" << words.size() << std::endl << "( ";
  for(unsigned int i = 0; i < 4 && i < forSure.size(); i++)
  {
    std::cout << forSure[i] << " ";
    fout2 << forSure[i] << " ";
  }
  if(forSure.size() > 4)
  {
    std::cout << "... " << forSure.size() - 4 << " more ";
    fout2 << "... " << forSure.size() - 4 << " more ";
  }
  std::cout << ")" << std::endl;
  fout2 << ")" << std::endl;
  std::cout << "Ambiguity: " << words.size() - total[2] << "/" << words.size() << std::endl << std::endl;
  fout2 << "Ambiguity: " << words.size() - total[2] << "/" << words.size() << std::endl << std::endl;
  if(genFile == 'y')
  {
    if(!ghc::filesystem::exists("ratelogs"))
    {
      ghc::filesystem::create_directory("ratelogs");
    }
    std::string name = "ratelogs/" + wordlist;
    for(unsigned int i = 0; i < guess.size(); i++)
    {
      name += "-";
      name += guess[i];
    }
    name += ".txt";
    std::ofstream fout;
    std::ifstream fin;
    fin.open(name);
    if(!fin)
    {
      fin.close();
      fout.open(name);
      for(unsigned int j = 0; j < ambiguous.size(); j++)
      {
        fout << "( ";
        for(unsigned int i = 0; i < ambiguous[j].size(); i++)
        {
          fout << ambiguous[j][i] << " ";
        }
        fout << ")" << std::endl;
      }
      fout.close();
    }
    else
    {
      fin.close();
    }
    std::cout << "Output file name: " << name << std::endl;
    fout2 << "Output file name: " << name << std::endl;
    std::cout << std::endl;
    fout2 << std::endl;
  }
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
    if((((total < lowestAve && (searchMode == 1 || searchMode == 4)) || (total > lowestAve && (searchMode == 2 || searchMode == 3 || searchMode == 5))) && !reversed) || (((total >= lowestAve && (searchMode == 1 || searchMode == 4)) || (total <= lowestAve && (searchMode == 2 || searchMode == 3 || searchMode == 5))) && reversed) || guess == 0)
    {
      lowest = guess;
      lowestAve = total;
      //std::cout << "new best!";
    }
    //std::cout << std::endl;
  }
  out = std::make_pair(validWords[lowest],lowestAve);
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
    else if((((min > results[i].second && (searchMode == 1 || searchMode == 4)) || (min < results[i].second && (searchMode == 2 || searchMode == 3 || searchMode == 5))) && !reversed) || (((min <= results[i].second && (searchMode == 1 || searchMode == 4)) || (min >= results[i].second && (searchMode == 2 || searchMode == 3 || searchMode == 5))) && reversed))
    {
      min = results[i].second;
      minWord = results[i].first;
    }
  }
  return std::make_pair(minWord,min);
}
