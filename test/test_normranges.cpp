#include "exmy.hpp"
#include "test_utils.hpp"

#define PRINT_RANGES(X, Y) \
{ \
  ExMy<X, Y> exmy; \
  printf("e%dm%d\t", X, Y); \
  printf("min_denorm: %.2e\t", exmy.min_denorm()); \
  printf("max_denorm: %.2e\t", exmy.max_denorm()); \
  printf("min_norm:  %.2e\t", exmy.min_norm()); \
  printf("max_norm:  %.2e\n", exmy.max_norm()); \
}
  

int main(int argc, char** argv)
{
  printf("== 8 bits ==\n");
  PRINT_RANGES(3, 4);
  PRINT_RANGES(4, 3);
  PRINT_RANGES(5, 2);
  PRINT_RANGES(6, 1);

  printf("== 16 bits ==\n");
  PRINT_RANGES(5, 10);
  PRINT_RANGES(6, 9);
  PRINT_RANGES(7, 8);
}
