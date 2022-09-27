/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "util.hpp"
#include "search.hpp"
#include "commandline.hpp"
#include "tools.hpp"
#include "rate.hpp"

int main(int argc, char* argv[])
{
  int numThreads {}, searchMode {}, uletter {};
  char hardmode {};
  std::string wordlist {}, temp {}, GorA {};
  std::ifstream wordlistStream;

  std::vector<std::string> prefix = {};

  bool fullRankingOutput = false;
  int fullRankingRequiredScore = -1;
  int setSize = 1;
  int unique = 0;
  bool newBestPrints = false;
  bool answersOnlyFirst = true;
  bool preciseSearchTimer = false;
  int updatePrintFrequency = 0;
  bool rateFileColorings = false;
  std::string forceInclude = "";
  std::string forceExclude = "";
  std::vector<int> uniqueSteps = {};
  std::string keyword = "";

  std::vector<std::pair<std::string, std::string>> commandGuesses;
  std::vector<std::string> commandWords;

  std::string command {};

  unsigned int rsetSize = 0;

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

          std::cout << "Notes:" << std::endl;
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
        else if(parsearg.first == cmdl::NAMES::RATE_FILE_COLORINGS_ARG)
        {
          rateFileColorings = parsearg.second == "true" || parsearg.second == "1" || parsearg.second.empty();
        }
        else if(parsearg.first == cmdl::NAMES::DEFAULT_ARG)
        {
          numThreads = stoi(cmdl::defaults.at(cmdl::NAMES::THREAD_ARG));
          wordlist = cmdl::defaults.at(cmdl::NAMES::WORDLIST_ARG);
          hardmode = cmdl::defaults.at(cmdl::NAMES::HARDMODE_ARG)[0];
          searchMode = stoi(cmdl::defaults.at(cmdl::NAMES::SEARCHMODE_ARG));

          wordlistStream = std::ifstream("wordlists/" + wordlist);
        }
        else if(parsearg.first == cmdl::NAMES::SSIZE_ARG)
        {
          rsetSize = stoi(parsearg.second);
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
      else if(command == cmdl::NAMES::FINDBEST_CMD || command == cmdl::NAMES::LIST_CMD || command == cmdl::NAMES::FILTER_CMD || (command == cmdl::NAMES::RATE_CMD && !(rsetSize == 0 || commandWords.size() != rsetSize)))
      {
        commandGuesses.push_back({arg, argv[pos + 1]});
        pos++;
      }
      else if((command == cmdl::NAMES::RATE_CMD && (rsetSize == 0 || commandWords.size() < rsetSize)) || command == cmdl::NAMES::ORDER_CMD)
      {
        commandWords.push_back(arg);
      }
    }
  }

  if(std::filesystem::exists("saves"))
  {
    std::vector<std::string> keys = {};
    for (const auto & entry : std::filesystem::directory_iterator("saves/"))
    {
      std::string path = entry.path().u8string();
      if(path.find('-') == std::string::npos)
      {
        keys.push_back(path.substr(path.find('/') + 1));
      }
    }
    if(keys.size())
    {
      std::cout << "Unfinished searches found with key(s) ";
      for(unsigned int i = 0; i < keys.size(); i++)
      {
        std::cout << keys[i];
        if(i != keys.size() - 1)
        {
          std::cout << ", ";
        }
      }
      std::cout << std::endl << "Would you like to continue one of these searches (y / n)? ";
      getline(std::cin, temp);
      if(std::tolower(temp.at(0)) == 'y')
      {
        temp = "";
        bool first = true;
        while(!std::count(keys.begin(), keys.end(), temp))
        {
          if(!first)
          {
            std::cout << "Invalid key" << std::endl;
          }
          first = false;
          std::cout << "What key? ";
          getline(std::cin, temp);
        }
        findbest(temp);
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
    bool found = false;
    for(unsigned int i = 0; i < validWordss.size() && !found; i++)
    {
      found = (temp == validWordss[i]);
    }
    if(!found)
    {
      validWordss.push_back(temp);
    }
    wordlistStream >> temp;
  }

  std::vector<std::string> forceExcludePos(validWordss[0].length(), "");
  std::vector<std::string> forceIncludePos(validWordss[0].length(), "");

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
        std::cout << "Find best (f), list (l), guess (g), rate (a), find order (o), restart with same settings (r), or exit (e)? ";
        getline(std::cin, temp);
        userInput = std::tolower(temp[0]);
      }

      if(userInput == 'e')
      {
        return 1;
      }
      else if(userInput == 'o' || command == cmdl::NAMES::ORDER_CMD)
      {
        std::vector<std::string> wordSet;
        if(command == "")
        {
          std::cout << std::endl << "Set to order (list separated by spaces)? ";
        }
        else if(commandWords.size() > 0)
        {
          wordSet = commandWords;

          for(const std::string& word : wordSet)
          {
            if(word.length() != valids[0].length())
            {
              std::cout << "Invalid word length of " << word.length() << ": '" << word << "'" << std::endl;
              return 0;
            }
          }
        }
        else
        {
          std::cout << "No inputted words" << std::endl;
          return 0;
        }

        std::transform(wordSet.begin(), wordSet.end(), wordSet.begin(), [](std::string &word) {
          std::transform(word.begin(), word.end(), word.begin(), ::tolower);
          return word;
        });

        if(wordSet.size() > 0 || inputWordSet(std::ref(wordSet), valids[0].length()))
        {
          order(wordSet, valids);
        }
        else
        {
          std::cout << "invalid word lengths" << std::endl << std::endl;
          std::cin.ignore();
        }
      }
      else if(userInput == 'a' || (command == cmdl::NAMES::RATE_CMD && commandGuesses.size() == 0))
      {
        std::vector<std::string> wordSet;
        if(command == "")
        {
          std::cout << std::endl << "Words to rate (list separated by spaces)? ";
        }
        else if(commandWords.size() > 0)
        {
          wordSet = commandWords;
          commandWords = {};

          for(const std::string& word : wordSet)
          {
            if(word.length() != valids[0].length())
            {
              std::cout << "Invalid word length of " << word.length() << ": '" << word << "'" << std::endl;
              return 0;
            }
          }
        }
        else
        {
          std::cout << "No inputted words" << std::endl;
          return 0;
        }

        std::transform(wordSet.begin(), wordSet.end(), wordSet.begin(), [](std::string &word) {
          std::transform(word.begin(), word.end(), word.begin(), ::tolower);
          return word;
        });

        if(wordSet.size() > 0 || inputWordSet(std::ref(wordSet), valids[0].length()))
        {
          rateAll(wordSet, valids, wordlist, rateFileColorings);
        }
        else
        {
          std::cout << "invalid word lengths" << std::endl << std::endl;
          std::cin.ignore();
        }
      }
      else if(userInput == 'p')
      {
        std::vector<std::string> wordSet;
        if(command == "")
        {
          std::cout << std::endl << "Words to find permutations of (list separated by spaces)? ";
        }
        else if(commandWords.size() > 0)
        {
          wordSet = commandWords;

          for(const std::string& word : wordSet)
          {
            if(word.length() != valids[0].length())
            {
              std::cout << "Invalid word length of " << word.length() << ": '" << word << "'" << std::endl;
              return 0;
            }
          }
        }
        else
        {
          std::cout << "No inputted words" << std::endl;
          return 0;
        }

        std::transform(wordSet.begin(), wordSet.end(), wordSet.begin(), [](std::string &word) {
          std::transform(word.begin(), word.end(), word.begin(), ::tolower);
          return word;
        });

        if(wordSet.size() > 0 || inputWordSet(std::ref(wordSet), valids[0].length()))
        {
          permute(wordSet, validGuesses);
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

        std::cout << "Precise search timer (y / n)? ";
        getline(std::cin, temp);
        preciseSearchTimer = std::tolower(temp.at(0)) == 'y';

        std::cout << "Do answers only search first (y / n)? ";
        getline(std::cin, temp);
        answersOnlyFirst = std::tolower(temp[0]) == 'y';

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

        std::cout << "Require score cutoff for considered sets (y / n)? ";
        getline(std::cin, temp);
        if(std::tolower(temp.at(0)) == 'y')
        {
          std::cout << "Cutoff score? ";
          getline(std::cin, temp);
          fullRankingRequiredScore = std::stoi(temp);
        }
        else
        {
          fullRankingRequiredScore = -1;
        }

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

        if(updatePrintFrequency)
        {
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
        }
        else
        {
          keyword = "";
        }

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

        /*std::cout << "Extra conditions for duplicate letters (y / n)? ";
        getline(std::cin, temp);
        if(std::tolower(temp.at(0)) == 'y')
        {
          std::unordered_map<char, int> m;
          for(unsigned int i = 0; i < forceInclude.length(); i++)
          {
            m[forceInclude[i]]++;
          }
          std::unordered_map<char, int>::iterator it;
          for(it = m.begin(); it != m.end(); ++it)
          {
            if(it->second > 1)
            {
              std::cout << it->first << " - how many of the " << it->second << " need to be in the same word? ";
              getline(std::cin, temp);
            }
            std::cout << it->first << " - allow more than " << it->second << " to be included (y / n)? ";
            getline(std::cin, temp);
          }
        }
        else
        {
          // set to some default
        }*/

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

        std::cout << "Forced include letter positions (y / n)? ";
        getline(std::cin, temp);
        if(std::tolower(temp.at(0)) == 'y')
        {
          for(unsigned int i = 0; i < forceIncludePos.size(); i++)
          {
            std::cout << "Pos " << i + 1 << ": ";
            getline(std::cin, temp);
            std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
            forceIncludePos[i] = temp;
          }
        }
        else
        {
          std::fill(forceIncludePos.begin(), forceIncludePos.end(), "");
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

        std::cout << std::endl;
      }
      else if(userInput == 'f' || (command == cmdl::NAMES::FINDBEST_CMD && commandGuesses.size() == 0))
      {
        auto start = std::chrono::high_resolution_clock::now();
        findbest(valids, validGuesses, numThreads, searchMode, prefix, fullRankingOutput, fullRankingRequiredScore, setSize, unique, newBestPrints, forceInclude, forceExclude, uniqueSteps, updatePrintFrequency, wordlist, forceExcludePos, forceIncludePos, answersOnlyFirst, keyword);
        if(preciseSearchTimer)
        {
          auto finish = std::chrono::high_resolution_clock::now();
          std::cout << (double)std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() / 1000000000 << "s\n\n";
        }

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
      else if(userInput == 'g' || ((command == cmdl::NAMES::FINDBEST_CMD || command == cmdl::NAMES::FILTER_CMD || command == cmdl::NAMES::LIST_CMD || command == cmdl::NAMES::RATE_CMD) && commandGuesses.size() > 0))
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
            rating = commandGuesses.begin()->second;
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
      else if(userInput == 'c')
      {
        std::cout << std::endl << "Coloring to filter by: ";
        getline(std::cin, temp);
        for(unsigned int i = 0; i < valids.size(); i++)
        {
          bool found = false;
          for(unsigned int j = 0; j < validGuesses.size() && !found; j++)
          {
            found = grade(validGuesses[j], valids[i]) == temp;
          }
          if(!found)
          {
            valids.erase(valids.begin() + i);
            i--;
          }
        }
        std::cout << "There are now " << valids.size() << " remaining possibilities" << std::endl << std::endl;
      }
      else
      {
        std::cout << std::endl << "Unknown command: '" << userInput << "'" << std::endl << std::endl;
      }

      if(command != "" && std::find(cmdl::moreLoops.begin(), cmdl::moreLoops.end(), command) == cmdl::moreLoops.end() && commandGuesses.size() == 0 && (commandWords.size() == 0 || command == cmdl::NAMES::ORDER_CMD))
      {
        return 0;
      }
    }
  }
}
