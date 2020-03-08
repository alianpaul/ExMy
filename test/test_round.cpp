#include "exmy.hpp"
#include <iostream>
#include <iomanip>
#include <cassert>

// #define PRINT_ROUND(CASE, exmy, target) \
// { \
//  std::cout << CASE << "\n"; \
//  std::cout << exmy << "\n"; \
//  exmy.round(); \
//  assert(exmy.data == target.data); \
//  std::cout << exmy << "\n"; \
// }

#define PRINT_ROUND(CASE, exmy, target) \
{ \
 exmy.fixup(); \
 assert(exmy.data == target.data); \
}

template<int X, int Y>
void CHECK_ROUND_MIN_NORM()
{
  ExMy<X, Y> min_norm(ExMy<X, Y>::min_norm()); //even
  PRINT_ROUND("min_norm", min_norm, min_norm);
    
  ExMy<X, Y> min_norm_next(min_norm.data + min_norm.eps_spacing()); //odd
  PRINT_ROUND("min_norm_next", min_norm_next, min_norm_next);

  ExMy<X, Y> min_norm_mid(min_norm.data + min_norm.eps_spacing() / 2.f);
  PRINT_ROUND("min_norm_mid", min_norm_mid, min_norm);

  ExMy<X, Y> min_norm_quarter(min_norm.data + min_norm.eps_spacing() / 4.f);
  PRINT_ROUND("min_norm_quarter", min_norm_quarter, min_norm);

  ExMy<X, Y> min_norm_3quarter(min_norm.data + min_norm.eps_spacing() * 3.f / 4.f);
  PRINT_ROUND("min_norm_3quarter", min_norm_3quarter, min_norm_next);
}

template<int X, int Y>
void CHECK_ROUND_MAX_NORM()
{
  ExMy<X, Y> max_norm(ExMy<X, Y>::max_norm()); //odd
  PRINT_ROUND("max_norm", max_norm, max_norm);

  ExMy<X, Y> max_norm_before(max_norm.data); //even
  uint32 bits = 0; memcpy(&bits, &max_norm_before.data, sizeof(float));
  uint32 mask = 1 << (ExMy<X, Y>::orig_Y - Y);
  bits -= mask;
  memcpy(&max_norm_before.data, &bits, sizeof(float));
  PRINT_ROUND("max_norm_before", max_norm_before, max_norm_before);

  ExMy<X, Y> max_norm_before_2(max_norm.data); //odd
  bits = 0; memcpy(&bits, &max_norm_before_2.data, sizeof(float));
  mask = 1 << (ExMy<X, Y>::orig_Y - Y + 1);
  bits -= mask;
  memcpy(&max_norm_before_2.data, &bits, sizeof(float));
  PRINT_ROUND("max_norm_before_2", max_norm_before_2, max_norm_before_2);
  
  ExMy<X, Y> max_norm_mid(max_norm_before_2.data +
			  (max_norm_before_2.eps_spacing() / 2.f));
  PRINT_ROUND("max_norm_mid", max_norm_mid, max_norm_before);

  ExMy<X, Y> max_norm_quarter(max_norm_before_2.data +
			      (max_norm_before_2.eps_spacing() / 4.f));
  PRINT_ROUND("max_norm_quarter", max_norm_quarter, max_norm_before_2);

  ExMy<X, Y> max_norm_3quarter(max_norm_before_2.data +
			       (max_norm_before_2.eps_spacing() * 3.f / 4.f));
  PRINT_ROUND("max_norm_3quarter", max_norm_3quarter, max_norm_before);
}

template<int X, int Y>
void CHECK_ROUND_MIN_DENORM()
{
  ExMy<X, Y> min_denorm(ExMy<X, Y>::min_denorm());
  PRINT_ROUND("min_denorm", min_denorm, min_denorm);
  ExMy<X, Y> min_denorm_x2(ExMy<X, Y>::min_denorm() * 2.f);
  PRINT_ROUND("min_denorm_x2", min_denorm_x2, min_denorm_x2);

  ExMy<X, Y> min_denorm_pos_mid(min_denorm.data + min_denorm.eps_spacing() / 2.f);
  PRINT_ROUND("min_denorm_pos_mid", min_denorm_pos_mid, min_denorm_x2);
    
  ExMy<X, Y> min_denorm_neg_mid(min_denorm.data - min_denorm.eps_spacing() / 2.f);
  ExMy<X, Y> zero(0.f);
  PRINT_ROUND("min_denorm_neg_mid", min_denorm_neg_mid, zero);

  ExMy<X, Y> min_denorm_x2_pos_mid(min_denorm_x2.data + min_denorm.eps_spacing() / 2.f);
  PRINT_ROUND("min_denorm_x2_pos_mid", min_denorm_x2_pos_mid, min_denorm_x2);
}

template<int X, int Y>
void CHECK_ROUND_MAX_DENORM()
{
  ExMy<X, Y> max_denorm(ExMy<X, Y>::max_denorm());
  PRINT_ROUND("max_denorm", max_denorm, max_denorm);

  ExMy<X, Y> min_norm(ExMy<X, Y>::min_norm());
  PRINT_ROUND("min_norm", min_norm, min_norm);

  ExMy<X, Y> max_denorm_mid(max_denorm.data + max_denorm.eps_spacing() / 2.f);
  PRINT_ROUND("max_denorm_mid", max_denorm_mid, min_norm);

  ExMy<X, Y> max_denorm_quarter(max_denorm.data + max_denorm.eps_spacing() / 4.f);
  PRINT_ROUND("max_denorm_quarter", max_denorm_quarter, max_denorm);

  ExMy<X, Y> max_denorm_3quarter(max_denorm.data + max_denorm.eps_spacing() * 3.f / 4.f);
  PRINT_ROUND("max_denorm_3quarter", max_denorm_3quarter, min_norm);
}

template<int X, int Y>
void CHECK_ROUND()
{
  CHECK_ROUND_MIN_NORM<X, Y>();
  CHECK_ROUND_MAX_NORM<X, Y>();
  CHECK_ROUND_MAX_DENORM<X, Y>();
  CHECK_ROUND_MIN_DENORM<X, Y>();

  std::cout << X << " " << Y << " Pass \n";
}

int main(int argc, char **argv)
{
  CHECK_ROUND<3, 4>();
  CHECK_ROUND<4, 3>();
  CHECK_ROUND<5, 2>();
  CHECK_ROUND<6, 1>();
  
  CHECK_ROUND<5, 10>();
  CHECK_ROUND<6, 9>();
  CHECK_ROUND<7, 8>();
}
