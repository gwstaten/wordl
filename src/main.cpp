/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "util.hpp"
#include "search.hpp"
#include "commandline.hpp"

int main(int argc, char* argv[])
{
  int numThreads {}, searchMode {}, uletter {};
  char hardmode {};
  std::string wordlist {}, temp {}, GorA {};
  std::ifstream wordlistStream;

  std::vector<std::string> prefix = {};

  bool fullRankingOutput = false;
  int setSize = 1;
  int unique = 0;
  bool newBestPrints = false;
  int updatePrintFrequency = 0;
  std::string forceInclude = "";
  std::string forceExclude = "";
  std::vector<int> uniqueSteps = {};
  std::string keyword = "";

  std::map<std::string, std::vector<std::string>> commandGuesses;
  std::vector<std::string> commandWordrates;

  std::string command {};

  for(int pos = 1; pos < argc; ++pos)
  {
    std::string arg = std::string(argv[pos]);
    std::pair<std::string, std::string> parsearg = parseoption(arg);

    if(parsearg.first != "")
    {
      try
      {
        if(parsearg.first == cmdl::NAMES::HELP_ARG)
        {
          int setwWidth = 20;

          std::cout << "Usage: ./wordl [ARGUMENTS] <COMMAND> (FILTERS)" << std::endl;

          std::cout << std::endl;

          std::cout << "Arguments:" << std::endl;
          for(const auto& [name, def] : cmdl::arguments)
          {
            std::cout << std::left << std::setw(setwWidth) << "  -" + name << def << std::endl;
          }

          std::cout << std::endl;

          std::cout << "Commands:" << std::endl;
          for(const auto& [name, def] : cmdl::commands)
          {
            std::cout << std::left << std::setw(setwWidth) << "  " + name << def << std::endl;
          }

          std::cout << "Example:" << std::endl;
          for(const std::string& example : cmdl::examples)
          {
            std::cout << "  " << example << std::endl;
          }

          std::cout << std::endl;

          std::cout << "Note:" << std::endl;
          for(const std::string& note : cmdl::notes)
          {
            std::cout << "  " << note << std::endl;
          }

          std::cout << std::endl;

          return 0;
        }
        else if(parsearg.first == cmdl::NAMES::THREAD_ARG)
        {
          numThreads = parsearg.second == "" ? std::stoi(cmdl::defaults.at(parsearg.first)), std::stoi("12") : std::stoi(parsearg.second);

          if(numThreads < 0)
          {
            std::cout << "Invalid thread count: '" << numThreads << "'" << std::endl;
            numThreads = 0;
          }
        }
        else if(parsearg.first == cmdl::NAMES::WORDLIST_ARG)
        {
          wordlist = parsearg.second == "" ? cmdl::defaults.at(parsearg.first) : parsearg.second;

          wordlistStream = std::ifstream("wordlists/" + wordlist);
          if(!wordlistStream)
          {
            std::cout << "Invalid word list name: '" << wordlist << "'" << std::endl;
            wordlist = "";
          }
        }
        else if(parsearg.first == cmdl::NAMES::HARDMODE_ARG)
        {
          hardmode = parsearg.second == "" ? cmdl::defaults.at(parsearg.first)[0] : parsearg.second[0];

          if(hardmode != 'u' && hardmode != 'h' && hardmode != 'n')
          {
            std::cout << "Unknown hardmode: '" << hardmode << "'" << std::endl;
            hardmode = {};
          }
        }
        else if(parsearg.first == cmdl::NAMES::SEARCHMODE_ARG)
        {
          searchMode = parsearg.second == "" ? std::stoi(cmdl::defaults.at(parsearg.first)) : std::stoi(parsearg.second);

          if(searchMode < 1 || searchMode > 6)
          {
            std::cout << "Unknown searchmode: '" << searchMode << "'" << std::endl;
            searchMode = 0;
          }
        }
        else if(parsearg.first == cmdl::NAMES::PREFIX_ARG)
        {
          std::istringstream prefixStream(parsearg.second);
          while(prefixStream >> temp)
          {
            prefix.push_back(temp);
          }
        }
        else if(parsearg.first == cmdl::NAMES::ULETTER_ARG)
        {
          unique = stoi(parsearg.second);
          uletter = stoi(parsearg.second);
        }
        else if(parsearg.first == cmdl::NAMES::UFILTERBY_ARG)
        {
          GorA = parsearg.second;
          if(GorA != "g" && GorA != "a")
          {
            std::cout << "Unknown ufilterby mode: '" << GorA << "'" << std::endl;

            GorA = {};
          }
        }
        else
        {
          std::cout << "Unknown argument: '" << parsearg.first << "'" << std::endl;
        }
      }
      catch(std::exception& exception)
      {
        std::cout << "Argument for '" << parsearg.first << "' invalid: '" << parsearg.second << "'" << std::endl; 
      }
    }
    else
    {
      if(command == "")
      {
        if(cmdl::commands.find(arg) == cmdl::commands.end())
        {
          std::cout << "Unknown command: '" << arg << "'" << std::endl;
          break;
        }
        else
        {
          command = arg; 
        }
      }
      else if(command == cmdl::NAMES::FINDBEST_CMD || command == cmdl::NAMES::LIST_CMD || command == cmdl::NAMES::FILTER_CMD)
      {
        std::vector<std::string> guesses;
        guesses.push_back(argv[1]);
        commandGuesses.insert({arg, guesses});
        pos ++;
      }
      else if(command == cmdl::NAMES::RATE_CMD)
      {
        commandWordrates.push_back(arg);
      }
    }
  }

  if(!numThreads)
  {
    std::cout << "Number of threads? ";
    getline(std::cin, temp);
  
    try
    {
      numThreads = stoi(temp);
    }
    catch(std::exception& e)
    {
      std::cout << "Thread count must be an integer: '" << temp << "'" << std::endl;
      return 0;
    }

    if(numThreads < 0)
    {
      std::cout << "Invalid thread count: '" << numThreads << "'" << std::endl;
      return 0;
    }
  }

  std::vector<std::string> validWords;
  std::vector<std::string> validWordss;

  if(wordlist == "")
  {
    std::cout << "Word list? ";
    getline(std::cin, wordlist);
    std::transform(wordlist.begin(), wordlist.end(), wordlist.begin(), ::tolower);

    wordlistStream = std::ifstream("wordlists/" + wordlist);

    if(!wordlistStream)
    {
      std::cout << "Invalid word list name: '" << wordlist << "'" << std::endl;
      return 0;
    }
  } 

  wordlistStream >> temp;
  while(!wordlistStream.eof())
  {
    validWordss.push_back(temp);
    wordlistStream >> temp;
  }
  
  std::vector<std::string> forceExcludePos(validWordss[0].length(), "");

  if(!hardmode)
  {
    std::cout << "Ultra hard mode, hard mode, or normal (u, h, n)? ";
    getline(std::cin, temp);
    
    hardmode = std::tolower(temp[0]);

    if(hardmode != 'u' && hardmode != 'h' && hardmode != 'n')
    {
      std::cout << "Unknown hardmode: '" << hardmode << "'" << std::endl;
      return 0;
    }
  }

  validWords = validWordss;
  std::ifstream alternateWordlist("wordlists/&" + wordlist);
  if(alternateWordlist.is_open())
  {
    alternateWordlist >> temp;
    while(!alternateWordlist.eof())
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
      alternateWordlist >> temp;
    }
  }
  
  std::sort(validWords.begin(), validWords.end());
  std::sort(validWordss.begin(), validWordss.end());

  if(!searchMode)
  {
    std::cout << "Search mode (1 - 6)? ";
    getline(std::cin, temp);

    try
    {
      searchMode = stoi(temp);
    }
    catch(const std::exception& e)
    {
      std::cout << "Searchmode must be an integer between 1 and 6: '" << temp << "'" << std::endl;
    }

    if(searchMode < 1 || searchMode > 6)
    {
      std::cout << "Unknown searchmode: '" << searchMode << "'" << std::endl;
      return 0;
    }
  }
  
  while(true)
  {
    std::vector<std::string> valids;
    std::vector<std::string> validGuesses;
    valids = validWordss;
    validGuesses = validWords;

    std::cout << std::endl << "Wordlist initialized with " << valids.size() << " answers";
    if(alternateWordlist.is_open())
    {
      std::cout << " and an additional " << validGuesses.size() << " guesses";
    }
    std::cout << std::endl << std::endl;

    char userInput {};
    while(true)
    {
      if(command == "")
      {
        userInput = 'n';
        std::cout << "Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? ";
        getline(std::cin, temp);
        userInput = std::tolower(temp[0]);
      }

      if(userInput == 'e')
      {
        return 1;
      }
      else if(userInput == 'a' || command == cmdl::NAMES::RATE_CMD)
      {
        std::vector<std::string> wordSet;
        if(command == "")
        {
          std::cout << std::endl << "Words to rate (list separated by spaces)? ";
        }
        else
        {
          wordSet = commandWordrates;

          for(const std::string& word : wordSet)
          {
            if(word.length() != valids[0].length())
            {
              std::cout << "Invalid word length of " << word.length() << ": '" << word << "'" << std::endl;
              return 0;
            }
          }
        }

        std::transform(wordSet.begin(), wordSet.end(), wordSet.begin(), [](std::string &word) { 
          std::transform(word.begin(), word.end(), word.begin(), ::tolower);
          return word;
        });

        if(wordSet.size() > 0 || inputWordSet(std::ref(wordSet), valids[0].length()))
        {
          rateAll(wordSet, valids, wordlist);
        }
        else
        {
          std::cout << "invalid word lengths" << std::endl << std::endl;
          std::cin.ignore();
        }
      }
      else if(userInput == 's')
      {
        char usePrefix {};
        std::cout << std::endl << "Search mode (1 - 6)? ";
        getline(std::cin, temp);

        try
        {
          searchMode = stoi(temp);
        }
        catch(const std::exception& e)
        {
          std::cout << "Searchmode must be an integer between 1 and 6: '" << temp << "'" << std::endl;
        }

        if(searchMode < 1 || searchMode > 6)
        {
          std::cout << "Unknown searchmode: '" << searchMode << "'" << std::endl;
          return 0;
        }

        std::cout << "Use prefex (y / n)? ";
        getline(std::cin, temp);
        usePrefix = std::tolower(temp[0]);
        if(usePrefix == 'y')
        {
          std::cout << "Prefix words (space separated)? ";
          std::vector<std::string> wordSet;
          if(inputWordSet(std::ref(wordSet), valids[0].size()))
          {
            prefix = wordSet;
          }
          else
          {
            std::cout << "Invalid word lengths" << std::endl << std::endl;
          }
        }
        else
        {
          prefix = {};
        }

        std::cout << "Print full rankings to file (y / n)? ";
        getline(std::cin, temp);
        fullRankingOutput = std::tolower(temp.at(0)) == 'y';

        std::cout << "Set size? ";
        getline(std::cin, temp);
        setSize = std::stoi(temp);

        std::cout << "Number of required unique for whole set (put 0 for no requirement)? ";
        getline(std::cin, temp);
        unique = std::stoi(temp);

        uniqueSteps = {};
        if(setSize > 1)
        {
          std::cout << "Num of required unique for each step (0 for no requirement)? ";
          for(int i = 0; i < setSize - 1; i++)
          {
            getline(std::cin, temp);
            uniqueSteps.push_back(std::stoi(temp));
          }
        }

        std::cout << "New best prints (y / n)? ";
        getline(std::cin, temp);
        newBestPrints = std::tolower(temp.at(0)) == 'y';

        std::cout << "Update print frequency (0 for no update prints)? ";
        getline(std::cin, temp);
        updatePrintFrequency = std::stoi(temp);

        std::cout << "Forced include letters (y / n)? ";
        getline(std::cin, temp);
        if(std::tolower(temp.at(0)) == 'y')
        {
          std::cout << "What letters? ";
          getline(std::cin, temp);
          std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
          forceInclude = temp;
        }
        else
        {
          forceInclude = "";
        }

        std::cout << "Forced exclude letters (y / n)? ";
        getline(std::cin, temp);
        if(std::tolower(temp.at(0)) == 'y')
        {
          std::cout << "What letters? ";
          getline(std::cin, temp);
          std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
          forceExclude = temp;
        }
        else
        {
          forceExclude = "";
        }

        std::cout << "Forced exclude letter positions (y / n)? ";
        getline(std::cin, temp);
        if(std::tolower(temp.at(0)) == 'y')
        {
          for(unsigned int i = 0; i < forceExcludePos.size(); i++)
          {
            std::cout << "Pos " << i + 1 << ": ";
            getline(std::cin, temp);
            std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
            forceExcludePos[i] = temp;
          }
        }
        else
        {
          std::fill(forceExcludePos.begin(), forceExcludePos.end(), "");
        }

        std::cout << "Store updates for later continuation (y / n)? ";
        getline(std::cin, temp);
        if(std::tolower(temp.at(0)) == 'y')
        {
          std::cout << "Keyword? ";
          getline(std::cin, temp);
          keyword = temp;
        }
        else
        {
          keyword = "";
        }

        std::cout << std::endl;
      }
      else if(userInput == 'f' || (command == cmdl::NAMES::FINDBEST_CMD && commandGuesses.size() == 0))
      {
        findbest(valids, validGuesses, numThreads, searchMode, prefix, fullRankingOutput, setSize, unique, newBestPrints, forceInclude, forceExclude, uniqueSteps, updatePrintFrequency, wordlist, forceExcludePos, keyword);

        if(command != "")
        {
          return 0;
        }
      }
      else if(userInput == 'l' || (command == cmdl::NAMES::LIST_CMD && commandGuesses.size() == 0))
      {
        std::cout << std::endl;
        for(unsigned int i = 0; i < valids.size(); i++)
        {
          std::cout << valids[i] << " ";
        }
        std::cout << std::endl;
        if(command != "")
        {
          return 0;
        }
      }
      else if(userInput == 'g' || ((command == cmdl::NAMES::FINDBEST_CMD || command == cmdl::NAMES::FILTER_CMD || command == cmdl::NAMES::LIST_CMD) && commandGuesses.size() > 0))
      {
        std::string guess;
        if(command == "")
        {
          std::cout << std::endl << "guess: ";
          getline(std::cin, guess);
        }
        else
        {
          guess = commandGuesses.begin()->first;
        }

        std::transform(guess.begin(), guess.end(), guess.begin(), ::tolower);

        if(valids.size() > 1 || valids.size() == 1)
        {
          std::string rating;
          if(command == "")
          {
            std::cout << "rating: ";
            getline(std::cin, rating);
          }
          else
          {
            rating = commandGuesses.begin()->second[0];
          }

          valids = filter(valids,std::make_pair(guess, rating));
          std::cout << "There are now " << valids.size() << " answers";
          if(hardmode == 'u')
          {
            validGuesses = filter(validGuesses,std::make_pair(guess, rating));
            std::cout << " and " << validGuesses.size() << " guesses";
          }
          else if(hardmode == 'h')
          {
            validGuesses = filterHM(validGuesses,std::make_pair(guess, rating));
            std::cout << " and " << validGuesses.size() << " guesses";
          }
          std::cout << " remaining" << std::endl;
        }

        std::cout << std::endl;
        if(commandGuesses.size() > 0)
        {
          commandGuesses.erase(commandGuesses.begin());
        }
      }
      else if(userInput == 'u' || command == cmdl::NAMES::UFILTER_CMD)
      {
        if(!uletter)
        {
          std::cout << std::endl << "Number of unique letters? ";
          getline(std::cin, temp);
          uletter = std::stoi(temp);
        }

        if(GorA == "")
        {
          std::cout << "Filter answers or guesses (g / a)? ";
          
          getline(std::cin, GorA);
          GorA = std::tolower(GorA.at(0));
        }
        if(GorA == "a")
        {
          for(unsigned int j = 0; j < valids.size(); j++)
          {
            if(countDistinct(valids[j]) != uletter)
            {
              valids.erase(valids.begin() + j);
              j--;
            }
          }
          std::cout << "There are now " << valids.size() << " remaining possibilities" << std::endl;
        }
        else if(GorA == "g")
        {
          for(unsigned int j = 0; j < validGuesses.size(); j++)
          {
            if(countDistinct(validGuesses[j]) != uletter)
            {
              validGuesses.erase(validGuesses.begin() + j);
              j--;
            }
          }
          std::cout << "There are now " << validGuesses.size() << " remaining guesses" << std::endl;
        }
        std::cout << std::endl;
      }
      else if(userInput == 'r')
      {
        break;
      }
      else
      {
        std::cout << std::endl << "Unknown command: '" << userInput << "'" << std::endl << std::endl;
      }

      if(command != "" && std::find(cmdl::moreLoops.begin(), cmdl::moreLoops.end(), command) == cmdl::moreLoops.end() && commandGuesses.size() == 0)
      {
        return 0;
      }
    }
  }
}
