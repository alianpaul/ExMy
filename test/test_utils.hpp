//For debug
#include "exmy.hpp"
#include <iostream>
#include <bitset>

template<int X, int Y>
std::ostream& operator<<(std::ostream& os, ExMy<X, Y> exmy)
{
  uint32 bits = 0; memcpy(&bits, &exmy.data, sizeof(float));
  if(bits >> 31)
    os << "1 ";
  else
    os << "0 ";

  //E
  uint32 E = (bits >> 23) & 0xff;
  os << std::bitset<8>(E) << ' ';

  uint32 M = bits & ((1 << (23 + 1)) - 1);
  os << std::bitset<23>(M) << ' ';

  int exp = (int) E - 127;
  os << E << " -127 = "<< exp << " ";

  int E_exmy = exp + (int) ExMy<X, Y>::BIAS; //Can be -1
  os << "+" << ExMy<X, Y>::BIAS << " = " << E_exmy << " " \
     << std::bitset<X>(E_exmy < 0 ? 0 : E_exmy);

  os << " " <<exmy.data;
  return os;
}
