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
  else
  {
    std::cout << "Gives " << a << " bits of information on average" << std::endl;
  }
}

void findbest(std::vector<std::vector<std::string>> valids, std::vector<std::vector<std::string>> validGuesses, int numThreads, int searchMode, std::vector<std::string> fileLocation, bool reversed)
{
  for(unsigned int j = 0; j < valids.size(); j++)
  {
    if(valids[j].size() > 2 || reversed)
    {
      if(reversed)
      {
        std::cout << "Worst guess for " << valids[j].size() << " possibilities: ";
      }
      else
      {
        std::cout << "Best guess for " << valids[j].size() << " possibilities: ";
      }
      std::ifstream fin;
      if(reversed)
      {
        fin.open(fileLocation[j] + "-worst");
      }
      else
      {
        fin.open(fileLocation[j]);
      }
      if(fin)
      {
        std::string g;
        fin >> g;
        double a;
        fin >> a;
        std::cout << g << std::endl;
        printBest(a, searchMode);
        fin.close();
      }
      else
      {
        fin.close();
        std::pair<std::string, double> best = fbThreads(valids[j], validGuesses[j], numThreads, searchMode, reversed);
        std::ofstream fout(fileLocation[j] + "-worst");
        fout << best.first << " " << best.second;
        std::cout << best.first << std::endl;
        printBest(best.second, searchMode);
      }
      if(valids[j].size() < 10)
      {
        for(unsigned int i = 0; i < valids[j].size(); i++)
        {
          std::cout << valids[j][i] << std::endl;
        }
      }
    }
    else
    {
      std::cout << "Only " << valids[j].size() << " possibility remaining: " << valids[j][0];
      if(valids[j].size() == 2)
      {
        std::cout << std::endl << valids[j][1];
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
}

int main()
{
  std::srand((unsigned) std::time(NULL)); 
  int numThreads;
  std::cout << "Number of threads? ";
  std::cin >> numThreads;

  int number;
  std::string in;
  std::vector<std::string> validWords;
  std::vector<std::string> validWordss;

  std::cout << "Word list? ";
  std::cin >> in;
  std::transform(in.begin(), in.end(), in.begin(), [](unsigned char c){ return std::tolower(c); });

  std::ifstream fin("wordlists/" + in);
  if(!fin)
  {
    std::cout << "Invalid word list name" << std::endl;
    return 0;
  }
  std::string temp;
  fin >> temp;
  while(!fin.eof())
  {
    validWordss.push_back(temp);
    fin >> temp;
  }

  std::cout << "Number of parallel wordls? ";
  std::cin >> number;

  char hardmode;
  std::cout << "Ultra hard mode, hard mode, or normal (u, h, n)? ";
  std::cin >> hardmode;
  hardmode = std::tolower(hardmode);

  validWords = validWordss;
  std::ifstream fin2("wordlists/&" + in);
  if(fin2.is_open())
  {
    fin2 >> temp;
    while(!fin2.eof())
    {
      bool found = false;
      for(unsigned int i = 0; i < validWordss.size() && !found; i++)
      {
        found = (temp == validWordss[i]);
      }
      if(!found)
      {
        validWords.push_back(temp);
      }
      fin2 >> temp;
    }
  }

  int searchMode;
  std::cout << "Search mode (1, 2, 3, 4, 5)? ";
  std::cin >> searchMode;
  while(true)
  {
    std::vector<std::vector<std::string>> valids;
    std::vector<std::vector<std::string>> validGuesses;
    for(int i = 0; i < number; i++)
    {
      valids.push_back(validWordss);
    }
    for(int i = 0; i < number; i++)
    {
      validGuesses.push_back(validWords);
    }
    std::string filePath;
    if(hardmode == 'h')
    {
      filePath = "log/" + std::to_string(searchMode) + "/" + in + "-hard";
    }
    else if(hardmode == 'u')
    {
      filePath = "log/" + std::to_string(searchMode) + "/" + in + "-ultrahard";
    }
    else
    {
      filePath = "log/" + std::to_string(searchMode) + "/" + in;
    }

    if(!ghc::filesystem::exists(filePath))
    {
      std::cout << "Creating log directory " << in << "..." << std::endl;
      ghc::filesystem::create_directories(filePath);
    }

    filePath += "/S";
    std::vector<std::string> fileLocation(number,filePath);


    std::cout << std::endl << "Wordlist initialized with " << valids[0].size() << " answers";
    if(fin2.is_open())
    {
      std::cout << " and an additional " << validGuesses[0].size() << " guesses";
    }
    std::cout << std::endl << std::endl;

    int loops = 0;
    char temp = 'n';
    while(temp != 'r')
    {
      loops++;
      temp = 'n';
      std::cout << "Find best (f), find worst (w), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? ";
      std::cin >> temp;
      temp = std::tolower(temp);
      std::string guess;
      if(temp == 'e')
      {
        return 1;
      }
      if(temp == 'a')
      {
        for(int j = 0; j < number; j++)
        {
          std::cout << std::endl << "Words to rate (list separated by spaces)? ";
          std::string wordliststring;
          std::vector<std::string> wordSet;
          bool exited = false;
          while(true)
          {
            std::cin >> wordliststring;
            std::transform(wordliststring.begin(), wordliststring.end(), wordliststring.begin(), [](unsigned char c){ return std::tolower(c); });
            if(wordliststring.length() != valids[j][0].length())
            {
              std::cout << "Invalid word lengths" << std::endl << std::endl;
              exited = true;
              while(true)
              {
                if(std::cin.peek() == '\n')
                {
                  break;
                }
                std::cin >> wordliststring;
              }
              break;
            }
            wordSet.push_back(wordliststring);
            if(std::cin.peek() == '\n')
            {
              break;
            }
          }
          if(!exited)
          {
            std::cout << "Generate ambiguity file (y / n)? ";
            char genFile;
            std::cin >> genFile;
            genFile = std::tolower(genFile);
            rateAll(wordSet, valids[j], genFile, in);
          }
        }
      }
      if(temp == 'f')
      {
        findbest(valids, validGuesses, numThreads, searchMode, fileLocation, false);
      }
      if(temp == 'w')
      {
        findbest(valids, validGuesses, numThreads, searchMode, fileLocation, true);
      }
      if(temp == 'l')
      {
        for(unsigned int j = 0; j <valids.size(); j++)
        {
          for(unsigned int i = 0; i < valids[j].size(); i++)
          {
            std::cout << valids[j][i] << std::endl;
          }
          std::cout << std::endl;
        }
      }
      if(temp == 'g')
      {
        std::cout << std::endl << "guess: ";
        std::cin >> guess;
        std::transform(guess.begin(), guess.end(), guess.begin(), [](unsigned char c){ return std::tolower(c); });
        for(int i = 0; i < number; i++)
        {
          fileLocation[i] = fileLocation[i] + guess;
          if(valids[i].size() > 1 || valids.size() == 1)
          {
            std::string temp;
            std::vector<int> rating;
            std::cin >> temp;
            fileLocation[i] = fileLocation[i] + temp;
            for(unsigned int i = 0; i < guess.length(); i++)
            {
              rating.push_back(temp.at(i) - '0');
            }
            valids[i] = filter(valids[i],std::make_pair(guess, rating));
            std::cout << "There are now " << valids[i].size() << " answers";
            if(hardmode == 'u')
            {
              validGuesses[i] = filter(validGuesses[i],std::make_pair(guess, rating));
              std::cout << " and " << validGuesses[i].size() << " guesses";
            }
            else if(hardmode == 'h')
            {
              validGuesses[i] = filterHM(validGuesses[i],std::make_pair(guess, rating));
              std::cout << " and " << validGuesses[i].size() << " guesses";
            }
            std::cout << " remaining" << std::endl;
          }
        }
        std::cout << std::endl;
      }
    }
  }
}
