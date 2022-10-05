#include "rate.hpp"

void rateAll(std::vector<std::string> guess, std::vector<std::string> words, std::string wordlist, bool rateFileColorings)
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
            if((it->first)[i] == '2' && !usedGreen[wordpos])
            {
              usedGreen[wordpos] = true;
              total[searchMode] += (it->second);
            }
          }
          break;
      }
    }
  }
  total[0] /= words.size();
  total[4] /= words.size();
  total[5] /= words.size();
  std::cout << "Average bits of info: " << total[4] << std::endl;
  std::cout << "Average remaining possibilities: " << total[0] << std::endl;
  std::cout << "Colorings (1/n): " << total[1] << std::endl;
  std::cout << "Collisions (# answers - 1/n): " << words.size() - total[1] << std::endl;
  std::cout << "Complexity (1/n - Guaranteed solves): " << total[1]  - total[2] << std::endl;
  std::cout << "Largest ambiguous set: " << total[3] << std::endl;
  std::cout << "Average greens: " << total[5] << std::endl;
  std::cout << "Ambiguity: " << words.size() - total[2] << "/" << words.size() << std::endl << std::endl;
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
      if(rateFileColorings)
      {
        fout << it3->first << " ";
      }
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

double rate(const std::vector<std::string> guess, const std::vector<std::string> words, int searchMode, const std::vector<std::string> prefixColorings)
{
  std::unordered_map<std::string, double> ratingsMap;
  for(unsigned int answer = 0; answer < words.size(); answer++)
  {
    std::string total = prefixColorings[answer];
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
          if((it->first)[i] == '2' && !usedGreen[wordpos])
          {
            usedGreen[wordpos] = true;
            total += (it->second);
          }
        }
        break;
      case 7:
        if((it->second) == 1)
        {
          total++;
        }
        break;
    }
  }
  if(searchMode == 1 || searchMode == 5 || searchMode == 6)
  {
    total /= words.size();
  }
  if(searchMode == 7)
  {
    total = ratingsMap.size() - total;
  }
  return total;
}

double rateInts(const std::vector<std::string> guess, const std::vector<std::string> words, int searchMode, const std::vector<unsigned long long int> prefixColorings)
{
  unsigned long long int factor = std::pow(3,guess[0].size());
  std::unordered_map<unsigned long long int, double> ratingsMap;
  for(unsigned int answer = 0; answer < words.size(); answer++)
  {
    unsigned long long int total = prefixColorings[answer];
    for(unsigned int i = 0; i < guess.size(); i++)
    {
      total *= factor;
      total += grade2(guess[i], words[answer]);
    }
    ratingsMap[total]++;
  }
  if(searchMode == 2)
  {
    return ratingsMap.size();
  }
  double total = 0;
  std::unordered_map<unsigned long long int, double>::iterator it;
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
      case 7:
        if((it->second) == 1)
        {
          total++;
        }
        break;
    }
  }
  if(searchMode == 1 || searchMode == 5 || searchMode == 6)
  {
    total /= words.size();
  }
  if(searchMode == 7)
  {
    total = ratingsMap.size() - total;
  }
  return total;
}
