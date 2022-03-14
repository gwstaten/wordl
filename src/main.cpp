#include "search.h"
std::vector<std::string> validWords;

int main()
{
  std::string in;
  int number;
  std::vector<std::vector<std::string>> valids;
  std::cout << "Word list? ";
  std::cin >> in;
  std::ifstream fin("wordlists/" + in);
  std::string temp;
  while(!fin.eof())
  {
    fin >> temp;
    validWords.push_back(temp);
  }
  std::cout << "Number of parallel wordls? ";
  std::cin >> number;
  for(int i = 0; i < number; i++)
  {
    valids.push_back(validWords);
  }

  std::vector<std::string> fileLocation(number,"log/" + in + "/");

  int loops = 0;
  while(true)
  {
    loops++;
    char temp;
    if(loops != 1)
    {
      std::cout << "Find best (y) or another guess (n)? " << std::endl;
      std::cin >> temp;
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
              std::ofstream fout(fileLocation[j]);
              std::pair<std::string, double> best = findBest1(valids[j]);
              std::pair<std::string, double> best2 = findBestDiff(valids[j], validWords);
              if(best2.second == best.second)
              {
                fout << best.first << " " << best.second;
                std::cout << best.first << std::endl;
                std::cout << "Narrows down to " << best.second << " possibilities on average" << std::endl << std::endl;
              }
              else
              {
                fout << best2.first << " " << best2.second;
                std::cout << best2.first << std::endl;
                std::cout << "Narrows down to " << best2.second << " possibilities on average" << std::endl << std::endl;
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
              std::cout << " " << valids[j][1];
            }
            std::cout << std::endl << std::endl;
          }
        }
      }
    }
    bool stillGood = true;
    for(unsigned int k = 0; k < valids.size() && stillGood; k++)
    {
      if(valids[k].size() > 2)
      {
        stillGood = false;
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
/*  std::ifstream fin("unlimitedOut");
  std::ofstream fout("unlimitedOut3");
  std::string data;
  std::vector<std::string> list;
  while(!fin.eof())
  {
    fin >> data;
    int uniqueLetters = 1;
    for(int i = 1; i < data.length(); i++)
    {
      uniqueLetters++;
      bool done = false;
      for(int j = 0; j < i && !done; j++)
      {
        if(data.at(i) == data.at(j))
        {
          done = true;
          uniqueLetters--;
        }
      }
    }
    if(uniqueLetters == 3)
    {
      std::cout << data << std::endl;
      fout << data << std::endl;
    }
  }*/
  //std::cout << findBest1(validWords) << std::endl;in = "5";
  /*std::ifstream fin("wordlists/11");
  std::string temp;
  while(!fin.eof())
  {
    fin >> temp;
    validWords.push_back(temp);
  }
  double guesses = 0;
  int max = 0;
  int priorGuesses = 0;
  for(double i = 0; i < validWords.size(); i++)
  {
    std::string fileLocation = "log/11/";
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
        guess = "disturbance";
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
            std::pair<std::string, double> best2 = findBestDiff(wordsLeft);
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
      if(sum != 22)
      {
        wordsLeft = filter(wordsLeft, std::make_pair(guess, rating));
        std::cout << "    " << wordsLeft.size() << std::endl;
      }
      else
      {
        std::cout << std::endl << answer << " " << i << " " << (i+1)/3022 << std::endl;
        done = true;
      }
    }
    if(guesses - priorGuesses > max)
    {
      max = guesses - priorGuesses;
    }
    if(guesses - priorGuesses == 6)
    {
      std::cout << validWords[i] << std::endl;
    }
    priorGuesses = guesses;
    std::cout << guesses / (i+1) << "       " << max << std::endl;
    std::cout << std::endl;
  }*/
}
