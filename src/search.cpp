/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "search.hpp"

void rateAll(std::vector<std::string> guess, std::vector<std::string> words, char genFile, std::string wordlist, std::string logLocation)
{
  std::map<std::string, double> ratingsMap;
  std::map<std::string, std::vector<std::string>> sets;
  for(unsigned int answer = 0; answer < words.size(); answer++)
  {
    std::string total;
    std::vector<std::vector<int>> ratings;
    for(unsigned int i = 0; i < guess.size(); i++)
    {
      total += grade(guess[i], words[answer]);
    }
    sets[total].push_back(words[answer]);
    ratingsMap[total]++;
  }
  std::vector<double> total(6,0);
  std::map<std::string, double>::iterator it;
  total[1] = ratingsMap.size();
  unsigned int wordpos = 0;
  for(it = ratingsMap.begin(); it != ratingsMap.end(); ++it)
  {
    std::vector<bool> usedGreen(guess[0].length(),false);
    for(unsigned int searchMode = 0; searchMode < total.size(); searchMode++)
    {
      switch(searchMode)
      {
        case 0:
          total[searchMode] += ((it->second) * (it->second));
          break;
        case 2:
          if((it->second) == 1)
          {
            total[searchMode]++;
          }
          break;
        case 3:
          if(total[searchMode] < (it->second))
          {
            total[searchMode] = (it->second);
          }
          break;
        case 4:
          total[searchMode] += (it->second) * std::log((it->second) / words.size()) / std::log(0.5);
          break;
        case 5:
          for(unsigned int i = 0; i < (it->first).length(); i++)
          {
            wordpos++;
            if(wordpos == guess[0].length())
            {
              wordpos = 0;
            }
            if((it->first).at(i) == '2' && !usedGreen[wordpos])
            {
              usedGreen[wordpos] = true;
              total[searchMode] += (it->second);
            }
          }
      }
    }
  }
  total[0] /= words.size();
  total[4] /= words.size();
  total[5] /= words.size();
  std::ofstream fout2(logLocation);
  std::cout << "Average bits of info: " << total[4] << std::endl;
  fout2 << "Average bits of info: " << total[4] << std::endl;
  std::cout << "Average remaining possibilities: " << total[0] << std::endl;
  fout2 << "Average remaining possibilities: " << total[0] << std::endl;
  std::cout << "1/n score: " << total[1] << std::endl;
  fout2 << "1/n score: " << total[1] << std::endl;
  std::cout << "Largest ambiguous set: " << total[3] << std::endl;
  fout2 << "Largest ambiguous set: " << total[3] << std::endl;
  std::cout << "Average greens: " << total[5] << std::endl;
  fout2 << "Average greens: " << total[5] << std::endl;
  std::cout << "Guaranteed solves: " << total[2] << "/" << words.size() << std::endl << "( ";
  fout2 << "Guaranteed solves: " << total[2] << "/" << words.size() << std::endl << "( ";
  std::map<std::string, std::vector<std::string>>::iterator it2;
  int num = 0;
  for(it2 = sets.begin(); it2 != sets.end(); ++it2)
  {
    if((it2->second).size() == 1)
    {
      num++;
      if(num < 5)
      {
        std::cout << (it2->second)[0] << " ";
        fout2 << (it2->second)[0] << " ";
      }
    }
  }
  if(num > 4)
  {
    std::cout << "... " << num - 4 << " more ";
    fout2 << "... " << num - 4 << " more ";
  }
  std::cout << ")" << std::endl;
  fout2 << ")" << std::endl;
  std::cout << "Ambiguity: " << words.size() - total[2] << "/" << words.size() << std::endl << std::endl;
  fout2 << "Ambiguity: " << words.size() - total[2] << "/" << words.size() << std::endl << std::endl;
  if(genFile == 'y')
  {
    if(!std::filesystem::exists("ratelogs"))
    {
      std::filesystem::create_directory("ratelogs");
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
      fout << "Guaranteed:" << std::endl << "( ";
      std::map<std::string, std::vector<std::string>>::iterator it3;
      for(it2 = sets.begin(); it2 != sets.end(); ++it2)
      {
        if((it2->second).size() == 1)
        {
          fout << (it2->second)[0] << " ";
        }
      }
      fout << " )" << std::endl << "Ambiguous sets: " << std::endl;
      for(it3 = sets.begin(); it3 != sets.end(); ++it3)
      {
        if((it3->second).size() != 1)
        {
          fout << "( ";
          for(unsigned int i = 0; i < (it3->second).size(); i++)
          {
            fout << (it3->second)[i] << " ";
          }
          fout << ")" << std::endl;
        }
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

float probFinder( unsigned max, unsigned diff, unsigned length )
{
    float result = 1;
    float cur1 = max;
    float cur2 = max - diff;
    for(unsigned int i = 0; i < length - 1; i++)
    {
      result *= cur2;
      result /= cur1;
      cur1--;
      cur2--;
    }
    return result;
}
/*
double rateFinal(std::vector<std::string> guess, std::vector<std::string> words, int searchMode)
{
  std::unordered_map<std::string, double> ratingsMap;
  for(unsigned int answer = 0; answer < words.size(); answer++)
  {
    std::string total = "";
    for(unsigned int i = 0; i < guess.size(); i++)
    {
      total += grade(guess[i], words[answer]);
    }
    ratingsMap[total]++;
  }
  if(searchMode == 2)
  {
    return ratingsMap.size();
  }
  double total = 0;
  std::unordered_map<std::string, double>::iterator it;
  unsigned int wordpos = 0;
  for(it = ratingsMap.begin(); it != ratingsMap.end(); ++it)
  {
    std::vector<bool> usedGreen(guess[0].length(),false);
    switch(searchMode)
    {
      case 1:
        total += ((it->second) * (it->second));
        break;
      case 3:
        if((it->second) == 1)
        {
          total++;
        }
        break;
      case 4:
        if(total < (it->second))
        {
          total = (it->second);
        }
        break;
      case 5:
        total += (it->second) * std::log((it->second) / words.size()) / std::log(0.5);
        break;
      case 6:
        for(unsigned int i = 0; i < (it->first).length(); i++)
        {
          wordpos++;
          if(wordpos == guess[0].length())
          {
            wordpos = 0;
          }
          if((it->first).at(i) == '2' && !usedGreen[wordpos])
          {
            usedGreen[wordpos] = true;
            total += (it->second);
          }
        }
        break;
      case 7:
        if(total < (it->second))
        {
          total = (it->second);
        }
        break;
    }
  }
  if(searchMode == 7)
  {
    std::vector<int> frequencies(total + 1, 0);
    for(it = ratingsMap.begin(); it != ratingsMap.end(); ++it)
    {
      frequencies[it->second]+=it->second;
    }
    frequencies[1] -= guess.size();
    int currentSum = 0;
    float probSum = 0;
    float odds = 0;
    for(unsigned int i = 0; i < frequencies.size(); i++)
    {
      if(frequencies[i] != 0)
      {
        currentSum += frequencies[i];
        float prob = ((1 - probFinder(words.size() - guess.size(), currentSum, 4)) - probSum);
        probSum += prob;
        prob /= i;
        odds += prob;
      }
    }
    total = odds;
  }
  if(searchMode == 1 || searchMode == 5 || searchMode == 6)
  {
    total /= words.size();
  }
  return total;
}
*/
double rate(std::vector<std::string> guess, std::vector<std::string> words, int searchMode)
{
  std::unordered_map<std::string, double> ratingsMap;
  std::unordered_map<std::string, std::vector<std::string>> ratingsMapWords;
  for(unsigned int answer = 0; answer < words.size(); answer++)
  {
    std::string total = "";
    for(unsigned int i = 0; i < guess.size(); i++)
    {
      total += grade(guess[i], words[answer]);
    }
    ratingsMap[total]++;
    if(ratingsMap[total] == 1)
    {
      ratingsMapWords[total] = {words[answer]};
    }
    else
    {
      ratingsMapWords[total].push_back(words[answer]);
    }
  }
  if(searchMode == 2)
  {
    return ratingsMap.size();
  }
  double total = 0;
  std::unordered_map<std::string, double>::iterator it;
  unsigned int wordpos = 0;
  for(it = ratingsMap.begin(); it != ratingsMap.end(); ++it)
  {
    std::vector<bool> usedGreen(guess[0].length(),false);
    switch(searchMode)
    {
      case 1:
        total += ((it->second) * (it->second));
        break;
      case 3:
        if((it->second) == 1)
        {
          total++;
        }
        break;
      case 4:
        if(total < (it->second))
        {
          total = (it->second);
        }
        break;
      case 5:
        total += (it->second) * std::log((it->second) / words.size()) / std::log(0.5);
        break;
      case 6:
        for(unsigned int i = 0; i < (it->first).length(); i++)
        {
          wordpos++;
          if(wordpos == guess[0].length())
          {
            wordpos = 0;
          }
          if((it->first).at(i) == '2' && !usedGreen[wordpos])
          {
            usedGreen[wordpos] = true;
            total += (it->second);
          }
        }
        break;
      case 7:
        if(total < (it->second))
        {
          total = (it->second);
        }
        break;
    }
  }
  if(searchMode == 7)
  {
    std::vector<int> frequencies(total + 1, 0);
    std::vector<std::vector<std::string>> options(total + 1, {"-"});
    for(it = ratingsMap.begin(); it != ratingsMap.end(); ++it)
    {
      frequencies[it->second]+=it->second;
      options[it->second].insert(options[it->second].end(), ratingsMapWords[it->first].begin(), ratingsMapWords[it->first].end());
    }
    for(unsigned int i = 0; i < options.size(); i++)
    {
      options[i].erase(options[i].begin());
    }
    frequencies[1] -= guess.size();
    int currentSum = 0;
    float probSum = 0;
    float odds = 0;
    for(unsigned int i = 0; i < frequencies.size(); i++)
    {
      if(frequencies[i] != 0)
      {
        currentSum += frequencies[i];
        float prob = ((1 - probFinder(words.size() - guess.size(), currentSum, 4)) - probSum);
        probSum += prob;
        prob /= i;
        /*float intermediateProb = 0;
        for(unsigned int j = 0; j < options[i].size(); j++)
        {
          intermediateProb += rateFinal({guess[0], options[i][j]}, words, 7);
        }
        intermediateProb /= options[i].size();
        prob *= intermediateProb;*/
        odds += prob;
      }
    }
    total = odds;
  }
  if(searchMode == 1 || searchMode == 5 || searchMode == 6)
  {
    total /= words.size();
  }
  return total;
}

void findBestThread(std::vector<std::string> words, std::vector<std::string> validWords, std::vector<std::pair<double,std::string>> &out, int searchMode, std::vector<std::string> prefix)
{
  for(unsigned int guess = 0; guess < validWords.size(); guess++)
  {
    std::vector<std::string> guessVec = prefix;
    guessVec.push_back(validWords[guess]);
    double total = rate(guessVec, words, searchMode);
    out.push_back(std::make_pair(total, validWords[guess]));
    //std::cout << validWords[guess] << " " << total << std::endl;
  }
}

struct greater
{
    template<class T>
    bool operator()(T const &a, T const &b) const { return a > b; }
};

std::vector<std::pair<double,std::string>> fbThreads(std::vector<std::string> words, std::vector<std::string> validWords, int threads, int searchMode, std::vector<std::string> prefix)
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
  std::vector<std::vector<std::pair<double,std::string>>> results(numThreads,{std::make_pair(0,"")});
  std::vector<std::thread> threadVector;
  for(unsigned int i = 0; i < numThreads; i++)
  {
    threadVector.push_back(std::thread(findBestThread,words, validWordsChunks[i], std::ref(results[i]), searchMode, prefix));
  }
  for(unsigned int i = 0; i < numThreads; i++)
  {
    if(threadVector[i].joinable())
    {
      threadVector[i].join();
    }
  }
  std::vector<std::pair<double,std::string>> compiledResults;
  for(unsigned int i = 0; i < results.size(); i++)
  {
    compiledResults.insert(compiledResults.end(),results[i].begin()+1,results[i].end());
  }
  if(searchMode == 1 || searchMode == 4)
  {
    std::sort(compiledResults.begin(), compiledResults.end());
  }
  else
  {
    std::sort(compiledResults.begin(), compiledResults.end(), greater());
  }
  /*std::ofstream fout("log.txt");
  for(unsigned int i = 0; i < compiledResults.size(); i++)
  {
    fout << i+1 << " " << compiledResults[i].second << " " << compiledResults[i].first << std::endl;
  }*/
  return compiledResults;
}
