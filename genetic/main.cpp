#include "genetic.h"

class Testing: public Screwdriver::Genetic<float>
{
  public:
    Testing();
};

Testing::Testing():
  Screwdriver::Genetic<float>()
{
  initialize_population(10, 10, 3);
}

int main(void)
{
  Testing a;
  return 0;
}
