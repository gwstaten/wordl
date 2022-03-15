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
        std::cout << "Find best (y) or another guess (n)? " << std::endl;
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
