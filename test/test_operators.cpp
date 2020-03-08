#include "exmy.hpp"
#include "test_utils.hpp"

int main(int argc, char **argv)
{
  ExMy<5, 10> min_norm_a(ExMy<5, 10>::min_norm());
  ExMy<5, 10> min_norm_b(ExMy<5, 10>::min_norm());
  
  std::cout << min_norm_a << "\n";
  std::cout << min_norm_b << "\n";

  ExMy<5, 10> min_norm_c = min_norm_a + min_norm_b;
  
  std::cout << min_norm_c << "\n";
}
