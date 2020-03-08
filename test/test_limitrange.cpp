#include "exmy.hpp"
#include "test_utils.hpp"

#include <float.h>


using namespace std;

#define LIMIT_RANGE(X, Y) \
{ \
  ExMy<X, Y> exmy_min(FLT_MIN); \
  ExMy<X, Y> exmy_max(FLT_MAX); \
  exmy_min.limitrange(); \
  exmy_max.limitrange(); \
  printf("e%dm%d\t low %.6e up %.6e\n", X, Y, exmy_min.data, exmy_max.data); \
}

int main(int argc, char **argv)
{
  printf("e8m23\t low %.6e up %.6e\n", FLT_MIN, FLT_MAX);
  
  printf("== 8 bits ==\n");
  LIMIT_RANGE(3, 4);
  LIMIT_RANGE(4, 3);
  LIMIT_RANGE(5, 2);
  LIMIT_RANGE(6, 1);

  printf("== 16 bits ==\n");
  LIMIT_RANGE(5, 10);
  LIMIT_RANGE(6, 9);
  LIMIT_RANGE(7, 8);
  LIMIT_RANGE(8, 7);
}
