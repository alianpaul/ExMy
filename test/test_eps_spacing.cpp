#include "exmy.hpp"
#include "test_utils.hpp"

#include <cassert>

template<int X, int Y>
void CHECK_EPS_SPACING()
{
  ExMy<X, Y> exmy_min_norm(ExMy<X, Y>::min_norm());

  /* Check #1
   * The spacing changes at the numbers that are perfect power of 2*/
  float  next_pow2_min_norm = 0;
  uint32 exp  = (ExMy<X, Y>::orig_BIAS + 1 - ExMy<X, Y>::BIAS + 1) << ExMy<X, Y>::orig_Y;
  uint32 frac = 0;
  uint32 result = exp | frac;
  memcpy(&next_pow2_min_norm, &result, sizeof(float));
  ExMy<X, Y> exmy_next_pow2_min_norm(next_pow2_min_norm);
  assert(int(exmy_next_pow2_min_norm.eps_spacing() / exmy_min_norm.eps_spacing()) == 2);

  /* Check #2
   * The eps spacing in [0, max_denorm] is the same in [min_norm, next_pow2_min_norm]*/
  ExMy<X, Y> exmy_zero;
  assert(exmy_zero.eps_spacing() == exmy_min_norm.eps_spacing());

  /* Check #3
   * The eps spacing is the same in the power of 2 range
   */
  float  next_min_norm = 0;
  exp  = (ExMy<X, Y>::orig_BIAS + 1 - ExMy<X, Y>::BIAS) << ExMy<X, Y>::orig_Y;
  frac = 1;
  result = exp | frac;
  memcpy(&next_min_norm, &result, sizeof(float));
  ExMy<X, Y> exmy_next_min_norm(next_min_norm);
  assert(exmy_min_norm.eps_spacing() == exmy_next_min_norm.eps_spacing());
  printf("e%dm%d \t pass\n", X, Y);

  /* Check #4
   * eps in denorm == min_denorm
   */
  assert(exmy_zero.eps_spacing() == (ExMy<X, Y>::min_denorm()));

  /* Check #5
   * eps in denorm == min_norm - max_denorm
   */
  assert(exmy_zero.eps_spacing() == (ExMy<X, Y>::min_norm() - ExMy<X, Y>::max_denorm()));
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
}
