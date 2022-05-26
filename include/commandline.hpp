#include "util.hpp"

#ifndef _COMMANDLINE_HPP_
#define _COMMANDLINE_HPP_

std::map<std::string, std::string> arguments {{"threads", "Sets the number of threads"}, {"wordlist", "Sets the wordlist"}, {"parallel", "Sets the number of parallel wordles"}, {"hardmode", "Sets the hardmode"}, {"searchmode", "Sets the searchmode"}};

std::map<std::string, std::string> commands {{"findbest", "Finds best words based on inputted guesses. Seperate guesses with spaces"}, {"list", "Lists remaining possible words. Seperate guesses with spaces"}, {"rate", "Rates the inputted words. Seperate words with spaces"}};

std::vector<std::string> examples {"./wordl -threads=4 -wordlist=nytimes2 -parallel=1 -hardmode=n -searchmode=2 findbest trace 00000 jazzy 00101"};

std::vector<std::string> notes {"You do not need to use all of the arguments, and they do not need to be in the example's specific order", "You can only use one command"};

#endif
