/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "search.hpp"
#include "rate.hpp"

void findBestThread(std::vector<std::string> words, std::vector<std::string> validWords, std::vector<std::pair<double,std::string>> &out, int searchMode, std::vector<std::string> prefix, std::vector<std::string> allguess, int setsize, int unique, bool newBestPrints, int threadNum, std::string forceInclude, std::vector<int> uniqueSteps, int updatePrintFrequency, std::string keyword, int fullRankingRequiredScore, std::vector<std::string> forceIncludePos, bool cont)
{
  std::ofstream fout;
  std::vector<unsigned int> positions(setsize, 0);
  bool notdone = true;
  bool first = true;
  double best = 0;
  std::string bestStr = "no valid sets found yet";
  bool bestTied = false;
  int last = 0;
  int lastWroteToFile = 0;
  unsigned int lastChanged = 0;
  unsigned long long int numberChecked = 0;
  bool forceIncludePosUsed = false;
  unsigned long long int timeRan = 0;
  for(unsigned int i = 0; i < forceIncludePos.size(); i++)
  {
    if(forceIncludePos[i].size())
    {
      forceIncludePosUsed = true;
    }
  }
  if(cont)
  {
    std::ifstream fin;
    fin.open("saves/" + keyword + "-thread" + std::to_string(threadNum) + "-results");
    std::string temp1;
    double temp2;
    fin >> temp1;
    if(temp1.size() == 1)
    {
      fin >> temp1;
    }
    fin >> temp2;
    while(!fin.eof())
    {
      if(first || (temp2 < best && (searchMode == 1 || searchMode == 4)) || (temp2 > best && !(searchMode == 1 || searchMode == 4)))
      {
        first = false;
        best = temp2;
        bestStr = temp1;
        bestTied = false;
      }
      else if(temp2 == best)
      {
        bestTied = true;
      }
      out.push_back(std::make_pair(temp2, temp1));
      fin >> temp1;
      if(temp1.size() == 1)
      {
        fin >> temp1;
      }
      fin >> temp2;
    }
    lastWroteToFile = out.size();
    fin.close();
    fin.open("saves/" + keyword + "-thread" + std::to_string(threadNum) + "-state");
    fin >> timeRan;
    fin >> numberChecked;
    for(unsigned int i = 0; i < positions.size(); i++)
    {
      unsigned int temp;
      fin >> temp;
      positions[i] = temp;
    }
    fin.close();
    fin.open("saves/" + keyword + "-thread" + std::to_string(threadNum) + "-wordsToCheck");
    validWords.clear();
    std::string temp;
    fin >> temp;
    while(!fin.eof())
    {
      validWords.push_back(temp);
      fin >> temp;
    }
    fin.close();
  }
  else
  {
    if(keyword.length())
    {
      fout.open("saves/" + keyword + "-thread" + std::to_string(threadNum) + "-wordsToCheck");
      for(unsigned int i = 0; i < validWords.size(); i++)
      {
        fout << validWords[i] << std::endl;
      }
      fout.close();
      fout.open("saves/" + keyword + "-thread" + std::to_string(threadNum) + "-results");
      fout.close();
      fout.open("saves/" + keyword + "-thread" + std::to_string(threadNum) + "-state");
      fout << "0 0";
      for(int i = 0; i < setsize; i++)
      {
        fout << " 0";
      }
      fout.close();
    }
  }

  bool intColorings = (words[0].size() <= 11 && searchMode != 6 && words[0].size() * (prefix.size() + setsize) <= 40);

  std::vector<std::string> prefixColorings = {};
  for(unsigned int i = 0; i < words.size(); i++)
  {
    std::string prefixColoring = "";
    for(unsigned int j = 0; j < prefix.size(); j++)
    {
      prefixColoring += grade(prefix[j], words[i]);
    }
    prefixColorings.push_back(prefixColoring);
  }
  std::vector<unsigned long long int> prefixColorings2 = {};
  if(intColorings)
  {
    unsigned long long int factor = std::pow(3,words[0].size());
    for(unsigned int i = 0; i < words.size(); i++)
    {
      unsigned long long int prefixColor2 = 0;
      for(unsigned int j = 0; j < prefix.size(); j++)
      {
        prefixColor2 *= factor;
        prefixColor2 += grade2(prefix[j], words[i]);
      }
      prefixColorings2.push_back(prefixColor2);
    }
  }

  std::string prefixStarter = "";
  if(prefix.size())
  {
    for(unsigned int i = 0; i < prefix.size(); i++)
    {
      if(i)
      {
        prefixStarter += "-";
      }
      prefixStarter += prefix[i];
    }
    prefixStarter += "-";
  }

  auto startTime = std::chrono::system_clock::now();
  if(positions[0] != validWords.size())
  {
    for(unsigned int guess = 0; notdone; guess++)
    {
      std::chrono::duration<double> diff = std::chrono::system_clock::now() - startTime;
      if(updatePrintFrequency)
      {
        if(((int)diff.count() + threadNum) % updatePrintFrequency == 0 && (int)diff.count() + threadNum != last)
        {
          last = (int)diff.count() + threadNum;
          unsigned long long int timeRanCur = timeRan + diff.count();
          std::string hour = std::to_string((int)(timeRanCur / 3600));
          std::string minute = std::to_string((int)((timeRanCur % 3600) / 60));
          if(minute.length() == 1)
          {
            minute = "0" + minute;
          }
          std::string second = std::to_string(timeRanCur % 60);
          if(second.length() == 1)
          {
            second = "0" + second;
          }
          std::cout << "(Thread " << threadNum << ") Update - ~" << 
                    (((double)positions[0] / (double)validWords.size()) + (positions.size() > 1 ? (double)positions[1] / (double)allguess.size() / (double)validWords.size() : 0)) * 100 << "% - " << 
                    hour + ":" + minute + ":" + second << 
                    " - has checked " << numberChecked << 
                    " sets with all filters - currently on start " << validWords[positions[0]] + (positions.size() > 1 ? "-" + allguess[positions[1]] : "") << 
                    " - current best " << bestStr << " " << best << (bestTied ? " (with a tie)" : "") << std::endl;
          if(keyword.length())
          {
            fout.open("saves/" + keyword + "-thread" + std::to_string(threadNum) + "-results", std::ios_base::app);
            for(unsigned int i = lastWroteToFile; i < out.size(); i++)
            {
              fout << out[i].second << " " << out[i].first << std::endl;
            }
            lastWroteToFile = out.size();
            fout.close();
            fout.open("saves/" + keyword + "-thread" + std::to_string(threadNum) + "-state");
            fout << timeRan + (int)diff.count() << " " << numberChecked;
            for(int i = 0; i < setsize; i++)
            {
              fout << " " << positions[i];
            }
            fout.close();
          }
        }
      }
      int toIncrement = setsize - 1;

      std::vector<std::string> guessVec;
      std::string comb = prefixStarter + validWords[positions[0]];
      guessVec.push_back(validWords[positions[0]]);
      std::string prior = validWords[positions[0]];
      bool alpha = true;
      if(setsize > 1)
      {
        if(countDistinct(comb) < uniqueSteps[0] && uniqueSteps[0])
        {
          alpha = false;
          toIncrement = 0;
        }
      }
      for(unsigned int i = 1; i < positions.size() && alpha; i++)
      {
        guessVec.push_back(allguess[positions[i]]);
        comb = comb + "-" + allguess[positions[i]];
        if(i >= lastChanged)
        {
          if(prior >= allguess[positions[i]])
          {
            alpha = false;
            toIncrement = i;
          }
          if(i < positions.size() - 1)
          {
            if(countDistinct(comb) - 1 < uniqueSteps[i] && uniqueSteps[i])
            {
              alpha = false;
              toIncrement = i;
            }
          }
          if(i < positions.size() - 1)
          {
            bool stillPossible = true;
            int numOccured = 0;
            std::string combNotUsed = comb;
            for(unsigned int i = 0; i < forceInclude.length() && stillPossible; i++)
            {
              if(combNotUsed.find(forceInclude[i]) != std::string::npos)
              {
                numOccured++;
                combNotUsed[combNotUsed.find(forceInclude[i])] = '_';
              }
            }
            if(numOccured + words[0].length() * positions.size() - i < forceInclude.length())
            {
              alpha = false;
              toIncrement = i;
            }
          }
        }
        prior = allguess[positions[i]];
      }
      if(alpha)
      {
        if(forceIncludePosUsed)
        {
          bool stillGood = true;
          for(unsigned int i = 0; i < forceIncludePos.size() && stillGood; i++)
          {
            for(unsigned int j = 0; j < forceIncludePos[i].length() && stillGood; j++)
            {
              bool found = false;
              for(unsigned int k = 0; k < guessVec.size() && !found; k++)
              {
                found = (guessVec[k][i] == forceIncludePos[i][j]);
              }
              stillGood = found;
            }
          }
          alpha = stillGood;
        }
      }
      if(alpha && (countDistinct(comb) >= unique + 1 || !unique || (guessVec.size() == 1 && countDistinct(comb) >= unique)))
      {
        bool stillGood = true;
        std::string combNotUsed = comb;
        for(unsigned int i = 0; i < forceInclude.length() && stillGood; i++)
        {
          stillGood = combNotUsed.find(forceInclude[i]) != std::string::npos;
          if(stillGood)
          {
            combNotUsed[combNotUsed.find(forceInclude[i])] = '_';
          }
        }
        if(stillGood)
        {
          double total;
          if(intColorings)
          {
            total = rateInts(guessVec, words, searchMode, prefixColorings2);
          }
          else
          {
            total = rate(guessVec, words, searchMode, prefixColorings);
          }
          numberChecked++;
          if(first || (total < best && (searchMode == 1 || searchMode == 4)) || (total > best && !(searchMode == 1 || searchMode == 4)))
          {
            first = false;
            best = total;
            bestStr = comb;
            bestTied = false;
            if(newBestPrints)
            {
              std::cout << "(Thread " << threadNum << ") New Best - " << comb << " " << total << std::endl;
            }
            out.push_back(std::make_pair(total, comb));
          }
          else if(newBestPrints && (first || best == total))
          {
            if(newBestPrints)
            {
              std::cout << "(Thread " << threadNum << ") Tied Best - " << comb << " " << total << std::endl;
            }
            bestTied = true;
            out.push_back(std::make_pair(total, comb));
          }
          else if(fullRankingRequiredScore == -1 || ((total <= fullRankingRequiredScore && (searchMode == 1 || searchMode == 4)) || (total >= fullRankingRequiredScore && !(searchMode == 1 || searchMode == 4))))
          {
            out.push_back(std::make_pair(total, comb));
          }
        }
      }

      positions[toIncrement]++;
      bool stillCarrying = true;
      for(unsigned int i = toIncrement; i > 0 && stillCarrying; i--)
      {
        if(positions[i] == allguess.size())
        {
          positions[i] = 0;
          positions[i - 1]++;
          lastChanged = i - 1;
        }
        else
        {
          stillCarrying = false;
        }
      }
      if(positions[0] == validWords.size())
      {
        notdone = false;
      }
    }
  }
  if(updatePrintFrequency)
  {
    std::chrono::duration<double> diff = std::chrono::system_clock::now() - startTime;
    unsigned long long int timeRanCur = timeRan + diff.count();
    std::string hour = std::to_string((int)(timeRanCur / 3600));
    std::string minute = std::to_string((int)((timeRanCur % 3600) / 60));
    if(minute.length() == 1)
    {
      minute = "0" + minute;
    }
    std::string second = std::to_string(timeRanCur % 60);
    if(second.length() == 1)
    {
      second = "0" + second;
    }
    std::cout << "(Thread " << threadNum << ") Finished! " << numberChecked << " sets were checked! " << hour << ":" << minute << ":" << second << std::endl;
    if(keyword.length())
    {
      fout.open("saves/" + keyword + "-thread" + std::to_string(threadNum) + "-results", std::ios_base::app);
      for(unsigned int i = lastWroteToFile; i < out.size(); i++)
      {
        fout << out[i].second << " " << out[i].first << std::endl;
      }
      lastWroteToFile = out.size();
      fout.close();
      fout.open("saves/" + keyword + "-thread" + std::to_string(threadNum) + "-state");
      fout << timeRan + (int)diff.count() << " " << numberChecked;
      for(int i = 0; i < setsize; i++)
      {
        fout << " " << positions[i];
      }
      fout.close();
    }
  }
}

struct greater
{
    template<class T>
    bool operator()(T const &a, T const &b) const { return a > b; }
};

std::vector<std::pair<double,std::string>> fbThreads(std::vector<std::string> words, std::vector<std::string> validWordList, int threads, int searchMode, std::vector<std::string> prefix, int setSize, int unique, bool newBestPrints, std::string forceInclude, std::string forceExclude, std::vector<int> uniqueSteps, std::vector<std::string> forceExcludePos, std::vector<std::string> forceIncludePos, int updatePrintFrequency, std::string keyword, int fullRankingRequiredScore, bool cont)
{
  std::vector<std::string> validWords;
  unsigned int uniqueUn = unique;
  for(unsigned int i = 0; i < validWordList.size(); i++)
  {
    bool stillGood = true;
    for(unsigned int j = 0; j < forceExclude.length() && stillGood; j++)
    {
      stillGood = validWordList[i].find(forceExclude[j]) == std::string::npos;
    }
    for(unsigned int j = 0; j < forceExcludePos.size() && stillGood; j++)
    {
      stillGood = forceExcludePos[j].find(validWordList[i][j]) == std::string::npos;
    }
    if(stillGood)
    {
      std::string combNotUsed = validWordList[i];
      int numOccured = 0;
      for(unsigned int i = 0; i < forceInclude.length(); i++)
      {
        if(combNotUsed.find(forceInclude[i]) != std::string::npos)
        {
          numOccured++;
          combNotUsed[combNotUsed.find(forceInclude[i])] = '_';
        }
      }
      stillGood = numOccured + ((setSize + prefix.size() - 1) * words[0].length()) >= forceInclude.length();
    }
    if(stillGood)
    {
      stillGood = countDistinct(validWordList[i]) + ((setSize + prefix.size() - 1) * words[0].length()) >= uniqueUn;
    }
    if(stillGood)
    {
      validWords.push_back(validWordList[i]);
    }
  }

  std::vector<std::string> validWordsOrder = validWords;
  auto rd = std::random_device{};
  auto rng = std::default_random_engine{rd()};
  std::shuffle(std::begin(validWords), std::end(validWords), rng);

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
    threadVector.push_back(std::thread(findBestThread,words, validWordsChunks[i], std::ref(results[i]), searchMode, prefix, validWordsOrder, setSize, unique, newBestPrints, i + 1, forceInclude, uniqueSteps, updatePrintFrequency, keyword, fullRankingRequiredScore, forceIncludePos, cont));
  }
  for(unsigned int i = 0; i < numThreads; i++)
  {
    if(threadVector[i].joinable())
    {
      threadVector[i].join();
    }
  }
  for(unsigned int i = 1; i <= numThreads; i++)
  {
    std::string name = "saves/" + keyword + "-thread" + std::to_string(i) + "-results";
    std::remove(name.c_str());
    name = "saves/" + keyword + "-thread" + std::to_string(i) + "-state";
    std::remove(name.c_str());
    name = "saves/" + keyword + "-thread" + std::to_string(i) + "-wordsToCheck";
    std::remove(name.c_str());
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

void findbest(std::string keyword)
{
  std::ifstream fin;
  std::vector<std::string> valids = {};
  std::vector<std::string> validGuesses = {};

  fin.open("saves/" + keyword + "-valids");
  std::string tempIn;
  fin >> tempIn;
  while(!fin.eof())
  {
    valids.push_back(tempIn);
    fin >> tempIn;
  }
  fin.close();

  fin.open("saves/" + keyword + "-validGuesses");
  fin >> tempIn;
  while(!fin.eof())
  {
    validGuesses.push_back(tempIn);
    fin >> tempIn;
  }
  fin.close();

  int numThreads, searchMode, setSize, unique, updatePrintFrequency, fullRankingRequiredScore;
  std::vector<std::string> prefix = {};
  bool fullRankingOut, newBestPrints, answersOnlyFirst;
  std::string forceInclude, forceExclude, wordlist;
  std::vector<int> uniqueSteps;
  std::vector<std::string> forceExcludePos = {};
  std::vector<std::string> forceIncludePos = {};

  fin.open("saves/" + keyword);
  fin >> numThreads;
  fin >> searchMode;
  char temp;
  fin >> temp;
  answersOnlyFirst = (temp == 'y');
  fin >> temp;
  fullRankingOut = (temp == 'y');
  fin >> fullRankingRequiredScore;
  fin >> setSize;
  fin >> unique;
  fin >> temp;
  newBestPrints = (temp == 'y');
  fin >> forceInclude;
  forceInclude = forceInclude.substr(1);
  fin >> updatePrintFrequency;
  fin >> forceExclude;
  forceExclude = forceExclude.substr(1);
  unsigned int prefixSize;
  fin >> prefixSize;
  for(unsigned int i = 0; i < prefixSize; i++)
  {
    fin >> tempIn;
    prefix.push_back(tempIn);
  }
  for(int i = 0; i < setSize - 1; i++)
  {
    int tempInt;
    fin >> tempInt;
    uniqueSteps.push_back(tempInt);
  }
  for(unsigned int i = 0; i < valids[0].length(); i++)
  {
    fin >> tempIn;
    forceExcludePos.push_back(tempIn.substr(1));
  }
  for(unsigned int i = 0; i < valids[0].length(); i++)
  {
    fin >> tempIn;
    forceIncludePos.push_back(tempIn.substr(1));
  }
  fin.close();
  std::cout << std::endl << "Best guess: ";
  std::vector<std::pair<double, std::string>> bestAnswers;
  std::vector<std::pair<double, std::string>> bestGuesses;
  std::string searchKey = "-wordlist-" + wordlist + "-searchmode-" + std::to_string(searchMode) + "-setsize-" + std::to_string(setSize);
  if(answersOnlyFirst)
  {
    fin.open("saves/" + keyword + "-answerResults");
    if(fin)
    {
      double temp1;
      std::string temp2;
      fin >> temp2;
      fin >> temp1;
      while(!fin.eof())
      {
        bestAnswers.push_back(std::make_pair(temp1, temp2));
        fin >> temp2;
        fin >> temp1;
      }
      fin.close();
      bestGuesses = fbThreads(valids, validGuesses, numThreads, searchMode, prefix, setSize, unique, newBestPrints, forceInclude, forceExclude, uniqueSteps, forceExcludePos, forceIncludePos, updatePrintFrequency, keyword, fullRankingRequiredScore, true);
      if(fullRankingOut)
      {
        std::ofstream fout("rankings/guessesRating" + searchKey + ".txt");
        for(unsigned int i = 0; i < bestGuesses.size(); i++)
        {
          fout << bestGuesses[i].second << " " << bestGuesses[i].first << std::endl;
        }
        fout.close();
      }
    }
    else
    {
      bestAnswers = fbThreads(valids, valids, numThreads, searchMode, prefix, setSize, unique, newBestPrints, forceInclude, forceExclude, uniqueSteps, forceExcludePos, forceIncludePos, updatePrintFrequency, keyword, fullRankingRequiredScore, true);
      if(fullRankingOut)
      {
        if(!std::filesystem::exists("rankings"))
        {
          std::filesystem::create_directory("rankings");
        }
        std::ofstream fout("rankings/answersRating" + searchKey + ".txt");
        for(unsigned int i = 0; i < bestAnswers.size(); i++)
        {
          fout << bestAnswers[i].second << " " << bestAnswers[i].first << std::endl;
        }
        fout.close();
      }
      if(valids != validGuesses)
      {
        if(keyword.length())
        {
          std::ofstream fout2;
          fout2.open("saves/" + keyword + "-answerResults");
          for(unsigned int i = 0; i < bestAnswers.size(); i++)
          {
            fout2 << bestAnswers[i].second << " " << bestAnswers[i].first << std::endl;
          }
        }
        bestGuesses = fbThreads(valids, validGuesses, numThreads, searchMode, prefix, setSize, unique, newBestPrints, forceInclude, forceExclude, uniqueSteps, forceExcludePos, forceIncludePos, updatePrintFrequency, keyword, fullRankingRequiredScore, false);
        if(fullRankingOut)
        {
          std::ofstream fout("rankings/guessesRating" + searchKey + ".txt");
          for(unsigned int i = 0; i < bestGuesses.size(); i++)
          {
            fout << bestGuesses[i].second << " " << bestGuesses[i].first << std::endl;
          }
          fout.close();
        }
      }
    }
    if(bestGuesses.size())
    {
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
    bestGuesses = fbThreads(valids, validGuesses, numThreads, searchMode, prefix, setSize, unique, newBestPrints, forceInclude, forceExclude, uniqueSteps, forceExcludePos, forceIncludePos, updatePrintFrequency, keyword, fullRankingRequiredScore, true);
    if(fullRankingOut)
    {
      std::ofstream fout("rankings/guessesRating" + searchKey + ".txt");
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
    std::cout << "- score of " << bestGuesses[0].first << std::endl << std::endl;
  }
  tempIn = "saves/" + keyword;
  std::remove(tempIn.c_str());
  tempIn = "saves/" + keyword + "-valids";
  std::remove(tempIn.c_str());
  tempIn = "saves/" + keyword + "-validGuesses";
  std::remove(tempIn.c_str());
  tempIn = "saves/" + keyword + "-answerResults";
  std::remove(tempIn.c_str());
}

void findbest(std::vector<std::string> valids, std::vector<std::string> validGuesses, int numThreads, int searchMode, std::vector<std::string> prefix, bool fullRankingOut, int fullRankingRequiredScore, int setSize, int unique, bool newBestPrints, std::string forceInclude, std::string forceExclude, std::vector<int> uniqueSteps,  int updatePrintFrequency, std::string wordlist, std::vector<std::string> forceExcludePos, std::vector<std::string> forceIncludePos, bool answersOnlyFirst, std::string keyword)
{
  if(keyword.length())
  {
    if(!std::filesystem::exists("saves"))
    {
      std::filesystem::create_directory("saves");
    }
    std::ofstream fout;
    fout.open("saves/" + keyword);
    fout << numThreads << " " << searchMode << " " << (answersOnlyFirst ? "y" : "n") << " " << (fullRankingOut ? "y" : "n") << " " << fullRankingRequiredScore << " " << setSize << " " << unique << " " << (newBestPrints ? "y" : "n") << " -" << forceInclude << " " << updatePrintFrequency << " -" << forceExclude << std::endl;
    fout << prefix.size();
    for(unsigned int i = 0; i < prefix.size(); i++)
    {
      fout << " " << prefix[i];
    }
    fout << std::endl;
    for(unsigned int i = 0; i < uniqueSteps.size(); i++)
    {
      fout << uniqueSteps[i] << std::endl;
    }
    for(unsigned int i = 0; i < forceExcludePos.size(); i++)
    {
      fout << "-" << forceExcludePos[i] << std::endl;
    }
    for(unsigned int i = 0; i < forceIncludePos.size(); i++)
    {
      fout << "-" << forceIncludePos[i] << std::endl;
    }
    fout.close();
    fout.open("saves/" + keyword + "-valids");
    for(unsigned int i = 0; i < valids.size(); i++)
    {
      fout << valids[i] << std::endl;
    }
    fout.close();
    fout.open("saves/" + keyword + "-validGuesses");
    for(unsigned int i = 0; i < validGuesses.size(); i++)
    {
      fout << validGuesses[i] << std::endl;
    }
    fout.close();
  }
  std::string searchKey = "-wordlist-" + wordlist + "-searchmode-" + std::to_string(searchMode) + "-setsize-" + std::to_string(setSize);
  if(valids.size() > 2)
  {
    std::cout << std::endl << "Best guess: ";
    std::vector<std::pair<double, std::string>> bestAnswers;
    std::vector<std::pair<double, std::string>> bestGuesses;
    if(answersOnlyFirst)
    {
      bestAnswers = fbThreads(valids, valids, numThreads, searchMode, prefix, setSize, unique, newBestPrints, forceInclude, forceExclude, uniqueSteps, forceExcludePos, forceIncludePos, updatePrintFrequency, keyword, fullRankingRequiredScore, false);
      if(fullRankingOut)
      {
        if(!std::filesystem::exists("rankings"))
        {
          std::filesystem::create_directory("rankings");
        }
        std::ofstream fout("rankings/answersRating" + searchKey + ".txt");
        for(unsigned int i = 0; i < bestAnswers.size(); i++)
        {
          fout << bestAnswers[i].second << " " << bestAnswers[i].first << std::endl;
        }
        fout.close();
      }
    }
    if(valids != validGuesses || !answersOnlyFirst)
    {
      if(keyword.length())
      {
        std::ofstream fout2;
        fout2.open("saves/" + keyword + "-answerResults");
        for(unsigned int i = 0; i < bestAnswers.size(); i++)
        {
          fout2 << bestAnswers[i].second << " " << bestAnswers[i].first << std::endl;
        }
      }
      bestGuesses = fbThreads(valids, validGuesses, numThreads, searchMode, prefix, setSize, unique, newBestPrints, forceInclude, forceExclude, uniqueSteps, forceExcludePos, forceIncludePos, updatePrintFrequency, keyword, fullRankingRequiredScore, false);
      if(fullRankingOut)
      {
        std::ofstream fout("rankings/guessesRating" + searchKey + ".txt");
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
      std::cout << "- score of " << bestGuesses[0].first << std::endl;
      if(answersOnlyFirst)
      {
        std::cout << "Best of answers: ";
      }
      else
      {
        std::cout << std::endl;
      }
    }
    if(answersOnlyFirst)
    {
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
  }
  else
  {
    std::cout << std::endl << "Best Guess: " << valids[0];
    if(valids.size() == 2)
    {
      std::cout << " / " << valids[1];
    }
    std::cout << std::endl << std::endl;
  }
  std::string temp = "saves/" + keyword;
  std::remove(temp.c_str());
  temp = "saves/" + keyword + "-valids";
  std::remove(temp.c_str());
  temp = "saves/" + keyword + "-validGuesses";
  std::remove(temp.c_str());
  temp = "saves/" + keyword + "-answerResults";
  std::remove(temp.c_str());
}
