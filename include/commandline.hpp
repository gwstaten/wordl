#include "util.hpp"

#ifndef _COMMANDLINE_HPP_
#define _COMMANDLINE_HPP_

namespace cmdl {
  class NAMES {
    public:
      const inline static std::string THREAD_ARG = "threads";
      const inline static std::string WORDLIST_ARG = "wordlist";
      const inline static std::string PARALLEL_ARG = "parallel";
      const inline static std::string HARDMODE_ARG = "hardmode";
      const inline static std::string SEARCHMODE_ARG = "searchmode";
      const inline static std::string FINDBEST_CMD = "findbest";
      const inline static std::string LIST_CMD = "list";
      const inline static std::string RATE_CMD = "rate";
      const inline static std::string FILTER_CMD = "filter";
  };

  const std::map<std::string, std::string> arguments {{NAMES::THREAD_ARG, "Sets the number of threads"}, {NAMES::WORDLIST_ARG, "Sets the wordlist"}, {NAMES::PARALLEL_ARG, "Sets the number of parallel wordles"}, {NAMES::HARDMODE_ARG, "Sets the hardmode"}, {NAMES::SEARCHMODE_ARG, "Sets the searchmode"}};

  const std::map<std::string, std::string> commands {{NAMES::FINDBEST_CMD, "Finds best words based on inputted guesses and ratings. Seperate guesses with spaces"}, {NAMES::LIST_CMD, "Lists remaining possible words based on inputted guesses and ratings. Seperate guesses with spaces"}, {NAMES::RATE_CMD, "Rates the inputted words. Seperate words with spaces"}, {NAMES::FILTER_CMD, "Shows amount of remaining words based on inputted guesses and ratings. Seperate guesses and ratings with spaces"}};

  const std::vector<std::string> examples {"./wordl -" + NAMES::THREAD_ARG + "=4 -" + NAMES::WORDLIST_ARG + "=nytimes2 -" + NAMES::PARALLEL_ARG + "=1 -" + NAMES::HARDMODE_ARG + "=n -" + NAMES::SEARCHMODE_ARG + "=2 " + NAMES::FINDBEST_CMD + " trace 00000 jazzy 00001"};

  const std::vector<std::string> notes {"You do not need to use all of the arguments, and they do not need to be in the example's specific order", "You can only use one command"};

  const std::vector<std::string> moreLoops {NAMES::FINDBEST_CMD, NAMES::LIST_CMD};
}

#endif