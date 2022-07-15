#include "tools.hpp"
#include "search.hpp"

void permute(std::vector<std::string> wordSet, std::vector<std::string> validGuesses)
{
  std::string temp = "";
  bool anyFound = false;
  std::vector<std::string> letterPos(wordSet[0].length(), "");
  std::string comb = "";
  for(unsigned int i = 0; i < wordSet.size(); i++)
  {
    comb += wordSet[i];
    for(unsigned int j = 0; j < wordSet[0].length(); j++)
    {
      letterPos[j] += wordSet[i][j];
    }
  }
  std::unordered_map<char, int> m;
  for(unsigned int i = 0; i < comb.length(); i++)
  {
    m[comb[i]]++;
  }

  std::unordered_map<char, int> forceMults;
  std::unordered_map<char, int>::iterator it;
  for(it = m.begin(); it != m.end(); ++it)
  {
    if(it->second > 1)
    {
      std::cout << "How many of the " << it->second << " " << it->first << "'s need to be in the same word? ";
      getline(std::cin, temp);
      if(std::stoi(temp) > 1)
      {
      forceMults[it->first] = std::stoi(temp);
      }
    }
  }
  std::string freeLetters = "";

  std::cout << "How many letters are able to change positions? " ;
  getline(std::cin, temp);
  unsigned int numChangeable = std::stoi(temp);
  for(unsigned int i = 0; i < numChangeable; i++)
  {
    char letter = '.';
    int pos = 0;
    auto location = std::string::npos;
    while(location == std::string::npos)
    {
      std::cout << "Letter and position (example e4)? ";
      getline(std::cin, temp);
      letter = temp[0];
      pos = std::stoi(temp.substr(1));
      location = letterPos[pos-1].find(letter);
      if(location == std::string::npos)
      {
      std::cout << "That letter + position combo was not found, try again" << std::endl;
      }
    }
    letterPos[pos-1][location] = '*';
    freeLetters += letter;
  }

  std::vector<std::string> validGuessesPossible = {};
  for(unsigned int i = 0; i < validGuesses.size(); i++)
  {
    bool stillGood = true;
    for(unsigned int j = 0; j < validGuesses[0].length() && stillGood; j++)
    {
      stillGood = (letterPos[j].find(validGuesses[i][j]) != std::string::npos || freeLetters.find(validGuesses[i][j]) != std::string::npos);
    }
    if(stillGood)
    {
      validGuessesPossible.push_back(validGuesses[i]);
    }
  }
  int notIncluded = -1;
  while(!anyFound)
  {
    notIncluded++;
    std::vector<unsigned int> state = {};
    for(unsigned int i = 0; i < wordSet.size() - notIncluded; i++)
    {
      state.push_back(i);
    }
    bool done = !validGuessesPossible.size();
    while(!done)
    {
      unsigned int toIncrement = state.size()-1;
      auto tempLetterPos = letterPos;
      auto tempFreeLetters = freeLetters;
      bool good = true;
      for(unsigned int i = 0; i < (wordSet.size() - notIncluded) && good; i++)
      {
        for(unsigned int j = 0; j < wordSet[0].size() && good; j++)
        {
          auto pos = tempLetterPos[j].find(validGuessesPossible[state[i]][j]);
          if(pos == std::string::npos)
          {
            pos = tempLetterPos[j].find('*');
            auto pos2 = tempFreeLetters.find(validGuessesPossible[state[i]][j]);
            if(pos != std::string::npos && pos2 != std::string::npos)
            {
              tempLetterPos[j][pos] = '_';
              tempFreeLetters[pos2] = '_';
            }
            else
            {
              good = false;
              toIncrement = i;
            }
          }
          else
          {
            tempLetterPos[j][pos] = '_';
          }
        }
      }
      if(good)
      {
        if(forceMults.size())
        {
          std::vector<std::string> setChunks = {};
          for(unsigned int i = 0; i < (wordSet.size() - notIncluded); i++)
          {
          setChunks.push_back(validGuessesPossible[state[i]]);
          }
          if(notIncluded)
          {
            std::string tempChunk = "";
            for(unsigned int i = 0; i < tempLetterPos.size(); i++)
            {
              for(unsigned int j = 0; j < tempLetterPos[i].length(); j++)
              {
                if(tempLetterPos[i][j] != '_')
                {
                  tempChunk += tempLetterPos[i][j];
                }
              }
            }
            setChunks.push_back(tempChunk);
          }
          std::unordered_map<char, int>::iterator it;
          for(it = forceMults.begin(); it != forceMults.end() && good; ++it)
          {
            bool found = false;
            for(unsigned int i = 0; i < setChunks.size() && !found; i++)
            {
              std::unordered_map<char, int> m;
              for(unsigned int j = 0; j < setChunks[i].length(); j++)
              {
                m[setChunks[i][j]]++;
              }
              std::unordered_map<char, int> m2;
              for(unsigned int j = 0; j < tempFreeLetters.length(); j++)
              {
                m2[tempFreeLetters[j]]++;
              }
              found = m[it->first] + std::min(m['*'],m2[it->first]) >= it->second;
            }
            if(!found)
            {
              good = false;
            }
          }
        }
        if(good)
        {
          std::cout << validGuessesPossible[state[0]];
          for(unsigned int i = 1; i < (wordSet.size() - notIncluded); i++)
          {
            std::cout << "-" << validGuessesPossible[state[i]];
          }
          if(notIncluded)
          {
            std::cout << "-";
            for(unsigned int i = 0; i < tempLetterPos.size(); i++)
            {
              std::cout << "(";
              for(unsigned int j = 0; j < tempLetterPos[i].length(); j++)
              {
                if(tempLetterPos[i][j] != '_')
                {
                  std::cout << tempLetterPos[i][j];
                }
              }
              std::cout << ")";
            }
          }
          if(tempFreeLetters.length())
          {
            std::cout << "[";
            for(unsigned int i = 0; i < tempFreeLetters.length(); i++)
            {
              if(tempFreeLetters[i] != '_')
              {
              std::cout << tempFreeLetters[i];
              }
            }
            std::cout << "]";
          }
          std::cout << std::endl;
          anyFound = true;
        }
      }
      state[toIncrement]++;
      if(state.size() > 1)
      {
        if(state[toIncrement] == validGuessesPossible.size())
        {
          if(toIncrement == 0)
          {
            done = true;
          }
          else
          {
            bool stillCarrying = true;
            unsigned int on = state.size();
            for(unsigned int i = toIncrement - 1; i >= 0 && stillCarrying; i--)
            {
              on = i;
              state[i]++;
              state[i + 1] = 0;
              stillCarrying = (state[i] == validGuessesPossible.size() || state[i] + (state.size() - i - 1) >= validGuessesPossible.size());
              if(stillCarrying && i == 0)
              {
                done = true;
                stillCarrying = false;
              }
            }
            for(unsigned int j = on + 1; j < state.size(); j++)
            {
              state[j] = state[j - 1] + 1;
            }
          }
        }
      }
      else
      {
        if(state[0] == validGuessesPossible.size())
        {
          done = true;
        }
      }
    }
    if(!anyFound)
    {
      std::cout << "no matching sets found with " << wordSet.size() - notIncluded << " real words" << std::endl;
      if(wordSet.size() - notIncluded == 1)
      {
        anyFound = true;
      }
    }
    std::cout << std::endl;
  }
}

void order(std::vector<std::string> wordSet, std::vector<std::string> valids)
{
  std::vector<std::string> blank(valids.size(),"");
  for(unsigned int u = 1; u <= 6; u++)
  {
    for(unsigned int i = wordSet.size() - 1; i > 0; i--)
    {
      double bestScore = 0;
      unsigned int bestToExclude = 0;
      for(unsigned int j = 0; j <= i; j++)
      {
        std::vector<std::string> tempWordSet;
        for(unsigned int k = 0; k <= i; k++)
        {
          if(k != j)
          {
            tempWordSet.push_back(wordSet[k]);
          }
        }
        double score = rate(tempWordSet, valids, u, blank);
        if(!j || (score < bestScore && (u == 1 || u == 4)) || (score > bestScore && !(u == 1 || u == 4)))
        {
          bestScore = score;
          bestToExclude = j;
        }
      }
      std::swap(wordSet[bestToExclude],wordSet[i]);
    }
    std::cout << "Optimal ordering by search mode " << u << ": ";
    for(unsigned int i = 0; i < wordSet.size(); i++)
    {
      std::cout << wordSet[i] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}