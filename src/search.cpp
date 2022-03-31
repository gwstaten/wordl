#include "search.hpp"

std::vector<std::string> filter(std::vector<std::string> wordList, std::pair<std::string, std::vector<int>> filter)
{
  for(unsigned int wordn = 0; wordn < wordList.size(); wordn++)
  {
    std::vector<bool> accountedfor(wordList.size(),false);
    std::string word = wordList[wordn];
    bool notDone = true;
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
      else if(filter.second[filterL] == 1)
      {
        bool found = false;
        if(!(filter.first.at(filterL) == word.at(filterL)))
        {
          for(unsigned int i = 0; i < word.length() && !found; i++)
          {
            if(filter.first.at(filterL) == word.at(i) && i != filterL && !accountedfor[i] && !(filter.first.at(i) == word.at(i) && filter.second[i] == 2))
            {
              accountedfor[i] = true;
              found = true;
            }
          }
        }
        if(!found)
        {
          notDone = false;
        }
      }
      else
      {
        if(word.at(filterL) != filter.first.at(filterL))
        {
          notDone = false;
        }
      }
    }
    if(!notDone)
    {
      wordList.erase(wordList.begin()+wordn);
      wordn--;
    }
  }
  return wordList;
}

std::vector<int> grade(std::string guess, std::string answer)
{
  std::vector<int> output(guess.length(),0);
  std::vector<int> usedAnswer(guess.length(),0);
  for(unsigned int i = 0; i < guess.length(); i++)
  {
    if(guess.at(i) == answer.at(i))
    {
      //std::cout << i << " 2    ";
      output[i] = 2;
      usedAnswer[i] = 1;
    }
  }
  for(unsigned int guessSpot = 0; guessSpot < guess.length(); guessSpot++)
  {
    if(output[guessSpot] == 0)
    {
      for(unsigned int answerSpot = 0; answerSpot < guess.length(); answerSpot++)
      {
        if(usedAnswer[answerSpot] == 0 && guess.at(guessSpot) == answer.at(answerSpot))
        {
          //std::cout << guessSpot << " 1    ";
          output[guessSpot] = 1;
          usedAnswer[answerSpot] = 1;
        }
      }
    }
  }
  return output;
}

std::pair<std::string,double> findBest(std::vector<std::string> words, std::vector<std::string> validWords)
{
  //std::cout << std::endl;
  int lowest = 0;
  double lowestAve = 10000;
  std::vector<double> scores;
  for(unsigned int guess = 0; guess < validWords.size(); guess++)
  {
    //std::cout << guess << " " << validWords[guess] << " ";
    std::map<unsigned long long int, std::pair<int, double>> ratingsMap;
    for(unsigned int answer = 0; answer < words.size(); answer++)
    {
      std::vector<int> rating = grade(validWords[guess], words[answer]);
      unsigned long long int total = 0;
      for(unsigned int i = 0; i < validWords[guess].length(); i++)
      {
        total *= 3;
        total += rating[i];
      }
      if(ratingsMap.find(total) == ratingsMap.end())
      {
        std::vector<std::string> remaining = filter(words, std::make_pair(validWords[guess],rating));
        ratingsMap[total] = std::make_pair(0,remaining.size());
      }
      ratingsMap[total].first++;
    }
    double total = 0;
    std::map<unsigned long long int, std::pair<int, double>>::iterator it;
    for(it = ratingsMap.begin(); it != ratingsMap.end(); ++it)
    {
        total += ((it->second).first * (it->second).second);
    }
    total /= words.size();
    scores.push_back(total);
    //std::cout << total << " ";
    if(total <= lowestAve || guess == 0)
    {
      lowest = guess;
      lowestAve = total;
      //std::cout << "new or tied best!";
    }
    //std::cout << std::endl;
    if(guess % 10 == 9)
    {
      for(unsigned int i = 0; i < scores.size(); i++)
      {
        //std::cout << scores[i] << ", ";
      }
    }
  }
  for(unsigned int i = 0; i < scores.size(); i++)
  {
    //std::cout << scores[i] << ", ";
  }
  return std::make_pair(validWords[lowest],lowestAve);
}
