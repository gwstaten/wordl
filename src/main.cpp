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
  int numThreads {}, number {}, searchMode {};
  char hardmode {};
  std::string in {};

  std::vector<std::string> prefix = {};

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
        if(parsearg.first == "help")
        {
          int setwWidth = 20;

          std::cout << "Usage: ./wordl [ARGUMENTS] <COMMAND>" << std::endl;

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
        }
        else if(parsearg.first == cmdl::NAMES::WORDLIST_ARG)
        {
          in = parsearg.second;
        }
        else if(parsearg.first == cmdl::NAMES::PARALLEL_ARG)
        {
          number = std::stoi(parsearg.second);
        }
        else if(parsearg.first == cmdl::NAMES::HARDMODE_ARG)
        {
          hardmode = parsearg.second[0];
        }
        else if(parsearg.first == cmdl::NAMES::SEARCHMODE_ARG)
        {
          searchMode = std::stoi(parsearg.second);
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
        for(int i = 0; i < number; ++i) {
          guesses.push_back(argv[pos + i + 1]);
        }
        commandGuesses.insert({arg, guesses});
        pos += number;
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

    char temp = 'n';
    while(temp != 'r')
    {
      if(command == "")
      {
        temp = 'n';
        std::cout << "Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? ";
        std::cin >> temp;
        temp = std::tolower(temp);
      }

      if(temp == 'e')
      {
        return 1;
      }
      if(temp == 'a' || command == cmdl::NAMES::RATE_CMD)
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
      if(temp == 'f' || (command == cmdl::NAMES::FINDBEST_CMD && commandGuesses.size() == 0))
      {
        findbest(valids, validGuesses, numThreads, searchMode, prefix);

        if(command != "")
        {
          return 0;
        }
      }
      if(temp == 'l' || (command == cmdl::NAMES::LIST_CMD && commandGuesses.size() == 0))
      {
        for(unsigned int j = 0; j <valids.size(); j++)
        {
          for(unsigned int i = 0; i < valids[j].size(); i++)
          {
            std::cout << valids[j][i] << "  ";
          }
          std::cout << std::endl;
        }

        if(command != "")
        {
          return 0;
        }
      }
      if(temp == 'g' || ((command == cmdl::NAMES::FINDBEST_CMD || command == cmdl::NAMES::FILTER_CMD || command == cmdl::NAMES::LIST_CMD) && commandGuesses.size() > 0))
      {
        std::string guess;
        if(command == "")
        {
          std::cout << std::endl << "guess: ";
          std::cin >> guess;
          std::transform(guess.begin(), guess.end(), guess.begin(), [](unsigned char c){ return std::tolower(c); });
        }
        else
        {
          guess = commandGuesses.begin()->first;
        }
        for(int i = 0; i < number; i++)
        {
          if(valids[i].size() > 1 || valids.size() == 1)
          {
            std::string rating;
            if(command == "")
            {
              std::cin >> rating;
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

      if(command != "" && std::find(cmdl::moreLoops.begin(), cmdl::moreLoops.end(), command) == cmdl::moreLoops.end() && commandGuesses.size() == 0)
      {
        return 0;
      }
    }
  }
}
