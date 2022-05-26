/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "util.hpp"
#include "search.hpp"

int main(int argc, char* argv[])
{
  int numThreads {}, number {}, searchMode {};
  char hardmode {};
  std::string in {};

  std::vector<std::string> prefix = {};

  std::map<std::string, std::string> preArguments;

  for(int pos = 1; pos < argc; ++pos)
  {
    std::pair<std::string, std::string> parsearg = parseoption(argv[pos]);

    if(parsearg.first != "")
    {
      try
      {
        if(parsearg.first == "help")
        {
          int setwWidth = 20;

          std::cout << "Usage: ./wordl [ARGS]" << std::endl;

          std::cout << std::endl;

          std::cout << "Arguments:" << std::endl;
          std::cout << std::left << std::setw(setwWidth) << "  -threads" << "Sets the number of threads" << std::endl;
          std::cout << std::left << std::setw(setwWidth) << "  -wordlist" << "Sets the wordlist" << std::endl;
          std::cout << std::left << std::setw(setwWidth) << "  -parallel" << "Sets the number of parallel wordles" << std::endl;
          std::cout << std::left << std::setw(setwWidth) << "  -hardmode" << "Sets the hardmode" << std::endl;
          std::cout << std::left << std::setw(setwWidth) << "  -searchmode" << "Sets the searchmode" << std::endl;

          std::cout << std::endl;

          std::cout << "Example:" << std::endl;
          std::cout << "  ./wordl -threads=4 -wordlist=nytimes2 -parallel=1 -hardmode=n -searchmode=2" << std::endl;

          std::cout << std::endl;

          std::cout << "Note:" << std::endl;
          std::cout << "  You do not need to use all of the arguments, and they do not need to be in the example's specific order" << std::endl;

          std::cout << std::endl;

          return 0;
        }
        else if(parsearg.first == "threads")
        {
          numThreads = std::stoi(parsearg.second);
        }
        else if(parsearg.first == "wordlist")
        {
          in = parsearg.second;
        }
        else if(parsearg.first == "parallel")
        {
          number = std::stoi(parsearg.second);
        }
        else if(parsearg.first == "hardmode")
        {
          hardmode = parsearg.second[0];
        }
        else if(parsearg.first == "searchmode")
        {
          searchMode = std::stoi(parsearg.second);
        }
        else
        {
          preArguments.insert({parsearg.first, parsearg.second});
        }
      }
      catch(std::exception& exception)
      {
        std::cout << "Argument for '" << parsearg.first << "' invalid: '" << parsearg.second << "'" << std::endl; 
      }
    }
  }

  std::srand((unsigned) std::time(NULL)); 

  if(!numThreads)
  {
    std::cout << "Number of threads? ";
    std::cin >> numThreads;
  }

  std::vector<std::string> validWords;
  std::vector<std::string> validWordss;

  if(in == "")
  {
    std::cout << "Word list? ";
    std::cin >> in;
    std::transform(in.begin(), in.end(), in.begin(), [](unsigned char c){ return std::tolower(c); });
  }

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

  if(!number)
  {
    std::cout << "Number of parallel wordls? ";
    std::cin >> number;
  }

  if(!hardmode)
  {
    std::cout << "Ultra hard mode, hard mode, or normal (u, h, n)? ";
    std::cin >> hardmode;
    hardmode = std::tolower(hardmode);
  }

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

  if(!searchMode)
  {
    std::cout << "Search mode (1 - 6)? ";
    std::cin >> searchMode;
  }
  
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
      std::cout << "Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? ";
      std::cin >> temp;
      temp = std::tolower(temp);
      std::string guess;
      if(temp == 'e')
      {
        return 1;
      }
      if(temp == 'a')
      {
        std::cout << std::endl << "Words to rate (list separated by spaces)? ";
        std::vector<std::string> wordSet;
        if(inputWordSet(std::ref(wordSet), valids[0][0].length()))
        {
          rateAll(wordSet, valids[0], in);
        }
        else
        {
          std::cout << "invalid word lengths" << std::endl << std::endl;
        }
      }
      if(temp == 's')
      {
        std::cout << std::endl << "Search mode (1 - 6)? ";
        std::cin >> searchMode;

        std::cout << "Add prefex (y / n)? ";
        char tempIn;
        std::cin >> tempIn;
        tempIn = std::tolower(tempIn);
        if(tempIn == 'y')
        {
          for(int j = 0; j < number; j++)
          {
            std::cout << "Prefix words (space separated)? ";
            std::vector<std::string> wordSet;
            if(inputWordSet(std::ref(wordSet), valids[0][0].size()))
            {
              prefix = wordSet;
            }
            else
            {
              std::cout << "invalid word lengths" << std::endl << std::endl;
            }
          }
        }
        std::cout << std::endl;
      }
      if(temp == 'f')
      {
        findbest(valids, validGuesses, numThreads, searchMode, prefix);
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
          if(valids[i].size() > 1 || valids.size() == 1)
          {
            std::string rating;
            std::cin >> rating;
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
