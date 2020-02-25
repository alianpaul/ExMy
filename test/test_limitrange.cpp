
#include "exmy.hpp"

using namespace std;

int main(int argc, char **argv)
{
  ExMy<5,10> v(65514.f);
  printf("%f\n", v.data);
  v.limitrange();
  printf("%f\n", v.data);
}
