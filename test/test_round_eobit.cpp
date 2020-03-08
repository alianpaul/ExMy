#include "exmy.hpp"
#include "test_utils.hpp"
#include <cassert>

template<int X, int Y>
void CHECK_ROUND_EO_BIT()
{
  ExMy<X, Y> exmy;
  
  /* Check #1 [0, min_denorm * 2)
   */
  std::cout << "==[0, min_norm * 2)==\n";
  exmy.data = ExMy<X, Y>::min_denorm() / 2;
  std::cout << exmy << "\n";
  exmy.round();
  std::cout << "\n";

  exmy.data = ExMy<X, Y>::min_denorm();
  std::cout << exmy << "\n";
  exmy.round();
  std::cout << "\n";

  /* Check #2 [min_denorm * 2, min_norm)
   */
  std::cout << "==[min_denorm * 2, min_norm)==\n";
  exmy.data = ExMy<X, Y>::min_denorm() * 2;
  std::cout << exmy << "\n";
  exmy.round();
  std::cout << "\n";
  
  exmy.data = ExMy<X, Y>::min_denorm() * 3;
  std::cout << exmy << "\n";
  exmy.round();
  std::cout << "\n";  

  exmy.data = ExMy<X, Y>::min_denorm() * 4;
  std::cout << exmy << "\n";
  exmy.round();
  std::cout << "\n";

  exmy.data = ExMy<X, Y>::max_denorm();
  std::cout << exmy << "\n";
  exmy.round();
  std::cout << "\n";  

  /* Check #3 [min_norm, max_norm]
   */
  std::cout << "==[min_norm, max_norm]==\n";
  
  exmy.data = ExMy<X, Y>::min_norm();
  std::cout << exmy << "\n";
  exmy.round();
  std::cout << "\n";  

  exmy.data = ExMy<X, Y>::max_norm();
  std::cout << exmy << "\n";
  exmy.round();
  std::cout << "\n";  

  exmy.data = ExMy<X, Y>::min_norm() + ExMy<X, Y>::max_norm() / 3;
  std::cout << exmy << "\n";
  exmy.round();
  std::cout << "\n";
}



int main(int argc, char **argv)
{
  CHECK_ROUND_EO_BIT<5, 10>();
}
