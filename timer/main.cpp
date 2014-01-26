#include "timer.h"
#include <vector>

int main(void)
{
  Timer::start("0");
  Timer::start("inner");
  std::vector<double> a;
  for(unsigned int i = 0 ; i < 100000 ; i++)
  {
    for(unsigned int j = 0 ; j < 1000 ; j++)
    {
      a.push_back((double)i);
      a.erase(a.begin());
    }
    Timer::tick("inner");
  }
  Timer::printLiteralMean("inner");
  Timer::printLiteral("0");
  return 0;
}
