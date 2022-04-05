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
int main()
{
  int numThreads;
  std::cout << "Number of threads? ";
  std::cin >> numThreads;

  int number;
  std::string in;
  std::vector<std::string> validWords;
  std::vector<std::vector<std::string>> valids;

  std::cout << "Word list? ";
  std::cin >> in;

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

  char hardmode;
  std::cout << "Hard mode? (y / n) ";
  std::cin >> hardmode;

  std::string filePath;
  if(hardmode == 'y')
  {
    filePath = "log/" + in + "-hard";
  }
  else
  {
    filePath = "log/" + in;
  }

  if(!ghc::filesystem::exists(filePath ))
  {
    std::cout << "Creating log directory " << in << "..." << std::endl;
    ghc::filesystem::create_directories(filePath);
  }

  filePath += "/*";
  std::vector<std::string> fileLocation(number,filePath);

  int loops = 0;
  while(true)
  {
    loops++;
    bool stillGood = true;
    for(unsigned int k = 0; k < valids.size() && stillGood; k++)
    {
      if(valids[k].size() > 2)
      {
        stillGood = false;
      }
    }
    char temp = 'n';
    if(stillGood)
    {
      temp = 'y';
    }
    else
    {
      std::cout << "Find best (y) or guess (n)? " << std::endl;
      std::cin >> temp;
    }
    std::string guess;
    if(temp == 'y')
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
            std::cout << "Narrows down to " << a << " possibilities on average" << std::endl;
            fin.close();
          }
          else
          {
            fin.close();
            std::pair<std::string, double> best = fbThreads(valids[j], valids[j], numThreads);
            std::pair<std::string, double> best2 = fbThreads(valids[j], validWords, numThreads);
            std::ofstream fout(fileLocation[j]);
            if(best2.second >= best.second || hardmode == 'y')
            {
              fout << best.first << " " << best.second;
              std::cout << best.first << std::endl;
              std::cout << "Narrows down to " << best.second << " possibilities on average" << std::endl;
            }
            else
            {
              fout << best2.first << " " << best2.second;
              std::cout << best2.first << std::endl;
              std::cout << "Narrows down to " << best2.second << " possibilities on average" << std::endl;
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
    if(stillGood)
    {
      return 1;
    }
    std::cout << std::endl << "guess: ";
    std::cin >> guess;
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
      }
    }
  }
}
