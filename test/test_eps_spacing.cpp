#include "exmy.hpp"
#include <iostream>
#include <cassert>

template<int X, int Y>
void CHECK_EPS_SPACING()
{
  ExMy<X, Y> exmy_min_norm(ExMy<X, Y>::min_norm());

  float next_min_norm = 0;
  uint32 exp  = (ExMy<X, Y>::orig_BIAS + 1 - ExMy<X, Y>::BIAS + 1) << ExMy<X, Y>::orig_Y;
  uint32 frac = 0;
  uint32 result = exp | frac;
  memcpy(&next_min_norm, &result, sizeof(float));
  ExMy<X, Y> exmy_next_min_norm(next_min_norm);
  
  // std::cout << exmy_next_min_norm.data / exmy_min_norm.data << std::endl;
  assert(int(exmy_next_min_norm.data / exmy_min_norm.data) == 2);
  printf("e%dm%d \t pass\n", X, Y);
}

int main(int argc, char **argv)
{
  CHECK_EPS_SPACING<3, 4>();
  CHECK_EPS_SPACING<4, 3>();
  CHECK_EPS_SPACING<5, 2>();
  CHECK_EPS_SPACING<6, 1>();

  CHECK_EPS_SPACING<5, 10>();
  CHECK_EPS_SPACING<6, 9>();
  CHECK_EPS_SPACING<7, 8>();
  CHECK_EPS_SPACING<8, 7>();
}
