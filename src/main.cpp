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

std::vector<std::string> split(std::string str, std::string token)
{
    std::vector<std::string> result;
    while(str.size())
    {
        unsigned int index = str.find(token);
        if(index != std::string::npos)
        {
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }
        else
        {
            result.push_back(str);
            str = "";
        }
    }
    return result;
}

int main()
{
  int numThreads;
  std::cout << "Number of threads? ";
  std::cin >> numThreads;

  int number;
  std::string in;
  std::vector<std::string> validWords;
  std::vector<std::vector<std::string>> valids;
  std::vector<std::vector<std::string>> validGuesses;

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
    validWords.push_back(temp);
    fin >> temp;
  }

  std::cout << "Number of parallel wordls? ";
  std::cin >> number;
  for(int i = 0; i < number; i++)
  {
    valids.push_back(validWords);
  }

  std::ifstream fin2("wordlists/&" + in);
  if(fin2.is_open())
  {
    validWords.clear();
    fin2 >> temp;
    while(!fin2.eof())
    {
      validWords.push_back(temp);
      fin2 >> temp;
    }
  }
  for(int i = 0; i < number; i++)
  {
    validGuesses.push_back(validWords);
  }

  char hardmode;
  std::cout << "Hard mode (y / n)? ";
  std::cin >> hardmode;
  hardmode = std::tolower(hardmode);

  int searchMode;
  std::cout << "Search mode (1, 2, 3, 4, 5)? ";
  std::cin >> searchMode;

  std::string filePath;
  if(hardmode == 'y')
  {
    filePath = "log/" + std::to_string(searchMode) + "/" + in + "-hard";
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
  while(true)
  {
    loops++;
    char temp = 'n';
    std::cout << "Find best (f), list (l), guess (g), rate (a), or exit (e)? ";
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
        std::cout << std::endl << "Words to rate (list separated by commas)? ";
        std::string wordliststring;
        std::cin >> wordliststring;
        std::transform(wordliststring.begin(), wordliststring.end(), wordliststring.begin(), [](unsigned char c){ return std::tolower(c); });

        std::vector<std::string> wordSet = split(wordliststring, ",");
        std::cout << "Guaranteed solves: " << rate(wordSet, valids[j], 3) << "/" << valids[j].size() << std::endl;
        std::cout << "Average bits of info: " << rate(wordSet, valids[j], 5) << std::endl;
        std::cout << "Average remaining possibilities: " << rate(wordSet, valids[j], 1) << std::endl << std::endl;
      }
    }
    if(temp == 'f')
    {
      for(int j = 0; j < number; j++)
      {
        if(valids[j].size() > 2)
        {
          std::cout << "Best guess for " << valids[j].size() << " possibilities: ";
          std::ifstream fin;
          fin.open(fileLocation[j]);
          if(fin)
          {
            std::string g;
            fin >> g;
            double a;
            fin >> a;
            std::cout << g << std::endl;
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
            fin.close();
          }
          else
          {
            fin.close();
            std::pair<std::string, double> best = fbThreads(valids[j], valids[j], numThreads, searchMode);
            std::pair<std::string, double> best2 = fbThreads(valids[j], validGuesses[j], numThreads, searchMode);
            std::ofstream fout(fileLocation[j]);
            if((best2.second >= best.second && (searchMode == 1 || searchMode == 4)) || (best2.second <= best.second && (searchMode == 2 || searchMode == 3 || searchMode == 5)))
            {
              fout << best.first << " " << best.second;
              std::cout << best.first << std::endl;
              if(searchMode == 1)
              {
                std::cout << "Narrows down to " << best.second << " possibilities on average" << std::endl;
              }
              else if(searchMode == 2)
              {
                std::cout << "Splits up into " << best.second << " groups" << std::endl;
              }
              else if(searchMode == 3)
              {
                std::cout << "Will get it on the following guess for " << best.second << " words" << std::endl;
              }
              else if(searchMode == 4)
              {
                std::cout << "Narrows down to " << best.second << " possibilities in the worst case scenario" << std::endl;
              }
              else
              {
                std::cout << "Gives " << best.second << " bits of information on average" << std::endl;
              }
            }
            else
            {
              fout << best2.first << " " << best2.second;
              std::cout << best2.first << std::endl;
              if(searchMode == 1)
              {
                std::cout << "Narrows down to " << best2.second << " possibilities on average" << std::endl;
              }
              else if(searchMode == 2)
              {
                std::cout << "Splits up into " << best2.second << " groups" << std::endl;
              }
              else if(searchMode == 3)
              {
                std::cout << "Will get it on the following guess for " << best2.second << " words" << std::endl;
              }
              else if(searchMode == 4)
              {
                std::cout << "Narrows down to " << best2.second << " possibilities in the worst case scenario" << std::endl;
              }
              else
              {
                std::cout << "Gives " << best2.second << " bits of information on average" << std::endl;
              }
            }
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
        if(valids[i].size() > 1)
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
          if(hardmode == 'y')
          {
            validGuesses[i] = filter(validGuesses[i],std::make_pair(guess, rating));
            std::cout << " and " << validGuesses[i].size() << " guesses";
          }
          std::cout << " remaining" << std::endl;
        }
      }
      std::cout << std::endl;
    }
  }
}
