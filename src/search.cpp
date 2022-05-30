/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "search.hpp"

void rateAll(std::vector<std::string> guess, std::vector<std::string> words, std::string wordlist)
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
  std::cout << "Average bits of info: " << total[4] << std::endl;
  std::cout << "Average remaining possibilities: " << total[0] << std::endl;
  std::cout << "1/n score: " << total[1] << std::endl;
  std::cout << "Largest ambiguous set: " << total[3] << std::endl;
  std::cout << "Average greens: " << total[5] << std::endl;
  std::cout << "Guaranteed solves: " << total[2] << "/" << words.size() << std::endl << "( ";
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
      }
    }
  }
  if(num > 4)
  {
    std::cout << "... " << num - 4 << " more ";
  }
  std::cout << ")" << std::endl;
  std::cout << "Ambiguity: " << words.size() - total[2] << "/" << words.size() << std::endl << std::endl;
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
  std::cout << "Output file name: " << name << std::endl;
  std::cout << std::endl;
}

double rate(std::vector<std::string> guess, std::vector<std::string> words, int searchMode)
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
    }
  }
  if(searchMode == 1 || searchMode == 5 || searchMode == 6)
  {
    total /= words.size();
  }
  return total;
}

void findBestThread(std::vector<std::string> words, std::vector<std::string> validWords, std::vector<std::pair<double,std::string>> &out, int searchMode, std::vector<std::string> prefix, std::vector<std::string> allguess, int setsize, int unique)
{
  std::vector<unsigned int> positions(setsize,0);
  bool notdone = true;
  for(unsigned int guess = 0; notdone; guess++)
  {
    std::vector<std::string> guessVec = prefix;
    std::string comb = validWords[positions[0]];
    guessVec.push_back(validWords[positions[0]]);
    std::string prior = comb;
    bool alpha = true;
    for(unsigned int i = 1; i < positions.size() && alpha; i++)
    {
      guessVec.push_back(allguess[positions[i]]);
      comb = comb + " " + allguess[positions[i]];
      if(prior > allguess[positions[i]])
      {
        alpha = false;
      }
      prior = allguess[positions[i]];
    }
    if(alpha && (countDistinct(comb) >= unique + 1 || !unique))
    {
      double total = rate(guessVec, words, searchMode);
      out.push_back(std::make_pair(total, comb));
      //std::cout << comb << " " << total << std::endl;
    }

    positions[0]++;
    if(positions[0] == validWords.size())
    {
      positions[0] = 0;
      if(positions.size() > 1)
      { 
        positions[1]++;
        bool stillCarrying = true;
        for(unsigned int i = 1; i < positions.size() && stillCarrying; i++)
        {
          if(positions[i] == allguess.size())
          {
            positions[i] = 0;
            if(i == positions.size() - 1)
            {
              notdone = false;
            }
            else
            {
              positions[i + 1]++;
            }
          }
          else
          {
            stillCarrying = false;
          }
        }
      }
      else
      {
        notdone = false;
      }
    }
  }
}

struct greater
{
    template<class T>
    bool operator()(T const &a, T const &b) const { return a > b; }
};

std::vector<std::pair<double,std::string>> fbThreads(std::vector<std::string> words, std::vector<std::string> validWords, int threads, int searchMode, std::vector<std::string> prefix, int setSize, int unique)
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
    threadVector.push_back(std::thread(findBestThread,words, validWordsChunks[i], std::ref(results[i]), searchMode, prefix, validWords, setSize, unique));
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
  return compiledResults;
}

void findbest(std::vector<std::vector<std::string>> valids, std::vector<std::vector<std::string>> validGuesses, int numThreads, int searchMode, std::vector<std::string> prefix, bool fullRankingOut, int setSize, int unique)
{
  for(unsigned int j = 0; j < valids.size(); j++)
  {
    if(valids[j].size() > 2)
    {
      std::cout << std::endl << "Best guess: ";
      std::vector<std::pair<double, std::string>> bestAnswers = fbThreads(valids[j], valids[j], numThreads, searchMode, prefix, setSize, unique);
      std::vector<std::pair<double, std::string>> bestGuesses;
      if(fullRankingOut)
      {
        std::remove("answersRating.txt");
        std::remove("guessesRating.txt");
        std::ofstream fout("answersRating.txt");
        for(unsigned int i = 0; i < bestAnswers.size(); i++)
        {
          fout << bestAnswers[i].second << " " << bestAnswers[i].first << std::endl;
        }
        fout.close();
      }
      if(valids[j] != validGuesses[j])
      {
        bestGuesses = fbThreads(valids[j], validGuesses[j], numThreads, searchMode, prefix, setSize, unique);
        if(fullRankingOut)
        {
          std::ofstream fout("guessesRating.txt");
          for(unsigned int i = 0; i < bestGuesses.size(); i++)
          {
            fout << bestGuesses[i].second << " " << bestGuesses[i].first << std::endl;
          }
          fout.close();
        }
        bool still = true;
        for(unsigned int i = 0; i < 10 && still; i++)
        {
          if(bestGuesses[i].first == bestGuesses[0].first)
          {
            if(i != 0)
            {
              std::cout << "\\ ";
            }
            std::cout << bestGuesses[i].second << " ";
          }
          else
          {
            still = false;
          }
        }
        if(still)
        {
          std::cout << "... and more ";
        }
        std::cout << "- score of " << bestGuesses[0].first << std::endl << "Best of answers: ";
      }
      bool still = true;
      for(unsigned int i = 0; i < bestAnswers.size() && still; i++)
      {
        if(bestAnswers[i].first == bestAnswers[0].first)
        {
          if(i != 0)
          {
            std::cout << "\\ ";
          }
          std::cout << bestAnswers[i].second << " ";
        }
        else
        {
          still = false;
        }
      }
      std::cout << "- score of " << bestAnswers[0].first << std::endl << std::endl;
    }
    else
    {
      std::cout << std::endl << "Best Guess: " << valids[j][0];
      if(valids[j].size() == 2)
      {
        std::cout << " / " << valids[j][1];
      }
      std::cout << std::endl << std::endl;
    }
  }
}
