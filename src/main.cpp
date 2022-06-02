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
  int numThreads {}, parallel {}, searchMode {}, uletter {};
  char hardmode {};
  std::string wordlist {}, temp {}, GorA {};
  std::ifstream wordlistStream;

  std::vector<std::string> prefix = {};

  bool fullRankingOutput = false;
  int setSize = 1;
  int unique = 0;

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
          numThreads = std::stoi(parsearg.second);

          if(numThreads < 0)
          {
            std::cout << "Invalid thread count: '" << numThreads << "'" << std::endl;
            numThreads = 0;
          }
        }
        else if(parsearg.first == cmdl::NAMES::WORDLIST_ARG)
        {
          wordlist = parsearg.second;

          wordlistStream = std::ifstream("wordlists/" + wordlist);
          if(!wordlistStream)
          {
            std::cout << "Invalid word list name: '" << wordlist << "'" << std::endl;
            wordlist = "";
          }
        }
        else if(parsearg.first == cmdl::NAMES::PARALLEL_ARG)
        {
          parallel = std::stoi(parsearg.second);

          if(parallel < 0)
          {
            std::cout << "Invalid parallel count: '" << parallel << "'" << std::endl;
            parallel = 0;
          }
        }
        else if(parsearg.first == cmdl::NAMES::HARDMODE_ARG)
        {
          hardmode = parsearg.second[0];

          if(hardmode != 'u' && hardmode != 'h' && hardmode != 'n')
          {
            std::cout << "Unknown hardmode: '" << hardmode << "'" << std::endl;
            hardmode = {};
          }
        }
        else if(parsearg.first == cmdl::NAMES::SEARCHMODE_ARG)
        {
          searchMode = std::stoi(parsearg.second);

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
        for(int i = 0; i < parallel; ++i)
        {
          guesses.push_back(argv[pos + i + 1]);
        }
        commandGuesses.insert({arg, guesses});
        pos += parallel;
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

  if(!parallel)
  {
    std::cout << "Number of parallel wordls? ";
    getline(std::cin, temp);

    try
    {
      parallel = stoi(temp);
    }
    catch(const std::exception& e)
    {
      std::cout << "Parallels must be an integer: '" << temp << "'" << std::endl;
    }

    if(parallel < 0)
    {
      std::cout << "Invalid parallel count: '" << parallel << "'" << std::endl;
      return 0;
    }
  }

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
    std::vector<std::vector<std::string>> valids;
    std::vector<std::vector<std::string>> validGuesses;
    for(int i = 0; i < parallel; i++)
    {
      valids.push_back(validWordss);
    }
    for(int i = 0; i < parallel; i++)
    {
      validGuesses.push_back(validWords);
    }

    std::cout << std::endl << "Wordlist initialized with " << valids[0].size() << " answers";
    if(alternateWordlist.is_open())
    {
      std::cout << " and an additional " << validGuesses[0].size() << " guesses";
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
            if(word.length() != valids[0][0].length())
            {
              std::cout << "Invalid word length of " << word.length() << ": '" << word << "'" << std::endl;
              return 0;
            }
          }
        }

        if(wordSet.size() > 0 || inputWordSet(std::ref(wordSet), valids[0][0].length()))
        {
          rateAll(wordSet, valids[0], wordlist);
        }
        else
        {
          std::cout << "invalid word lengths" << std::endl << std::endl;
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
          for(int j = 0; j < parallel; j++)
          {
            std::cout << "Prefix words (space separated)? ";
            std::vector<std::string> wordSet;
            if(inputWordSet(std::ref(wordSet), valids[0][0].size()))
            {
              prefix = wordSet;
            }
            else
            {
              std::cout << "Invalid word lengths" << std::endl << std::endl;
            }
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

        std::cout << "Number of required unique (put 0 for no requirement)? ";
        getline(std::cin, temp);
        unique = std::stoi(temp);

        std::cout << std::endl;
      }
      else if(userInput == 'f' || (command == cmdl::NAMES::FINDBEST_CMD && commandGuesses.size() == 0))
      {
        findbest(valids, validGuesses, numThreads, searchMode, prefix, fullRankingOutput, setSize, unique);

        if(command != "")
        {
          return 0;
        }
      }
      else if(userInput == 'l' || (command == cmdl::NAMES::LIST_CMD && commandGuesses.size() == 0))
      {
        for(unsigned int j = 0; j <valids.size(); j++)
        {
          std::cout << std::endl;
          for(unsigned int i = 0; i < valids[j].size(); i++)
          {
            std::cout << valids[j][i] << " ";
          }
          std::cout << std::endl;
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
          std::transform(guess.begin(), guess.end(), guess.begin(), ::tolower);
        }
        else
        {
          guess = commandGuesses.begin()->first;
        }
        for(int i = 0; i < parallel; i++)
        {
          if(valids[i].size() > 1 || valids.size() == 1)
          {
            std::string rating;
            if(command == "")
            {
              std::cout << "rating: ";
              getline(std::cin, rating);
            }
            else
            {
              rating = commandGuesses.begin()->second[i];
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
        if(commandGuesses.size() > 0)
        {
          commandGuesses.erase(commandGuesses.begin());
        }
      }
      else if(userInput == 'u')
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
        for(int i = 0; i < parallel; i++)
        {
          if(GorA == "a")
          {
            for(unsigned int j = 0; j < valids[i].size(); j++)
            {
              if(countDistinct(valids[i][j]) != uletter)
              {
                valids[i].erase(valids[i].begin() + j);
                j--;
              }
            }
            std::cout << "There are now " << valids[i].size() << " remaining possibilities" << std::endl;
          }
          else if(GorA == "g")
          {
            for(unsigned int j = 0; j < validGuesses[i].size(); j++)
            {
              if(countDistinct(validGuesses[i][j]) != uletter)
              {
                validGuesses[i].erase(validGuesses[i].begin() + j);
                j--;
              }
            }
            std::cout << "There are now " << validGuesses[i].size() << " remaining guesses" << std::endl;
          }
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
