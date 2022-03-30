#include "search.hpp"
int main()
{
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
  if(fin2)
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

  filePath += "/";
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
    if(loops != 1)
    {
      if(stillGood)
      {
        temp = 'y';
      }
      else
      {
        std::cout << "Find best (y) or guess (n)? " << std::endl;
        std::cin >> temp;
      }
    }
    std::string guess;
    if(temp == 'y')
    {
      if(loops != 1)
      {
        for(int j = 0; j < number; j++)
        {
          if(valids[j].size() > 2)
          {
            std::cout << "Best guess for " << valids[j].size() << " possibilities: ";
            std::ifstream fin;
            if(fileLocation[j].at(fileLocation[j].length()-1) == '/')
            {
              fin.open("start");
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
              std::cout << "Narrows down to " << a << " possibilities on average" << std::endl;
              fin.close();
            }
            else
            {
              fin.close();
              std::ofstream fout(fileLocation[j]);
              std::pair<std::string, double> best = findBest(valids[j], valids[j]);
              std::pair<std::string, double> best2 = findBest(valids[j], validWords);
              if(best2.second == best.second || hardmode == 'y')
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
            //std::cout << std::endl;
          }
          //std::cout << std::endl;
        }
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

  /*std::ifstream fin("wordlists/n");
  std::string temp;
  while(!fin.eof())
  {
    fin >> temp;
    validWords.push_back(temp);
  }
  double guesses = 0;
  int max = 0;
  int priorGuesses = 0;
  std::vector<int> frequencies(6,0);
  for(double i = 0; i < validWords.size(); i++)
  {
    std::string fileLocation = "log/n/";
    std::string answer = validWords[i];
    std::vector<std::string> wordsLeft = validWords;
    bool done = false;
    int round = 0;
    while(!done)
    {
      guesses++;
      std::string guess;
      if(round == 0)
      {
        guess = "salet";
      }
      else
      {
        std::ifstream fin;
        fin.open(fileLocation);
        if(fin)
        {
          fin >> guess;
          fin.close();
        }
        else
        {
          fin.close();
          if(wordsLeft.size() >= 3)
          {
            std::ofstream fout(fileLocation);
            std::pair<std::string, double> best = findBest1(wordsLeft);
            std::pair<std::string, double> best2 = findBestDiff(wordsLeft, validWords);
            if(best2.second == best.second)
            {
              fout << best.first << " " << best.second;
              guess = best.first;
            }
            else
            {
              fout << best2.first << " " << best2.second;
              guess = best2.first;
            }
            fout.close();
          }
          else
          {
            guess = wordsLeft[0];
          }
        }
      }
      fileLocation+=guess;
      round++;
      std::vector<int> rating = grade(guess,answer);
      int sum = 0;
      for(unsigned int j = 0; j < guess.length(); j++)
      {
        sum+=rating[j];
        fileLocation+=std::to_string(rating[j]);
        std::cout << rating[j];
      }
      if(sum != 10)
      {
        wordsLeft = filter(wordsLeft, std::make_pair(guess, rating));
        std::cout << "    " << wordsLeft.size() << std::endl;
      }
      else
      {
        std::cout << std::endl << answer << " " << i << " " << (i+1)/2309 << std::endl;
        done = true;
      }
    }
    if(guesses - priorGuesses > max)
    {
      max = guesses - priorGuesses;
    }
    frequencies[guesses - priorGuesses] = frequencies[guesses - priorGuesses] + 1;
    priorGuesses = guesses;
    std::cout << guesses / (i+1) << "       " << max << std::endl;
    for(int i = 1; i < 6; i++)
    {
      std::cout << i << ": " << frequencies[i] << std::endl;
    }
    std::cout << std::endl;
  }
  for(int i = 1; i < 6; i++)
  {
    std::cout << i << ": " << frequencies[i] << std::endl;
  }*/
}
