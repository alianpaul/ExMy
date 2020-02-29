#ifndef EXMY_H_
#define EXMY_H_

#include <cstdio>
#include <cstring>

using uint32 = unsigned int;

/* ExMy for X < 8
 * x: exponent bits, y: mantissa bits
 * Bias: (1 << (X - 1)) - 1
 *   Bias of float: 127
 * Denorm: (sign) * 2^(1 - Bias) * y(implicit leading 0)
 * Norm:   (sign) * 2^(x - Bias) * y(implicit leading 1)
 * Special: x is all 1s
 *   Inf: y is all zero
 *   NaN: y is nonzero
 *
 * Attention: This template can't handle E8My case. It assumes that X < 8, in which the denorm
 * range of ExMy lies in the norm range of float32(E8M23).
 *    
 */
template <int X, int Y>
struct ExMy {

  float data;
  static const int orig_X    = 8;
  static const int orig_Y    = 23;
  static const int orig_BIAS = 127;

  // X
  // Y
  static const int BIAS = (1 << (X - 1)) - 1;

  ExMy(float a) : data(a)
  {}

  ExMy() : data(0.0f)
  {}

  static float min_denorm()
  {
    /* 
     * Under the assumption that X < 8, All denorms of EXMY are actually norms of E8M23.
     * Be aware of the implicit 1.
     * E8M23                       EXMY
     * [1.M] * 2^(E - orig_BIAS) = [0.0..01] * 2^(1 - BIAS)
     *
     * [1.M]
     * --------- = 2 ^ (1 - BIAS + orig_BIAS - E)
     * [0.0..01]
     * 
     * Y = 1 - BIAS + orig_BIAS - E
     * 
     * E = 1 - BIAS + orig_BIAS - Y
     *
     */
    uint32 exp    = (orig_BIAS + 1 - BIAS - Y) << orig_Y;
    uint32 frac   = 0;
    uint32 result = exp | frac;
    float ret = 0;
    memcpy(&ret, &result, sizeof(float));
    return ret;
  }

  static float max_denorm()
  {
    return min_norm() - min_denorm();
  }

  static float min_norm()
  {
    uint32 exp    = (orig_BIAS + 1 - BIAS) << orig_Y;
    uint32 frac   = 0;
    uint32 result = exp | frac;
    float ret = 0;
    memcpy(&ret, &result, sizeof(float));
    return ret;
  }

  static float max_norm()
  {
    uint32 exp    = (orig_BIAS + ((1 << X) - 2) - BIAS) << orig_Y;
    uint32 frac   = ((1 << orig_Y) - 1) & ~((1 << (orig_Y - Y)) - 1);
    uint32 result = exp | frac;
    float ret = 0;
    memcpy(&ret, &result, sizeof(float));
    return ret;    
  }

  /* limitrange
   * limit the range of *data* to ExMy's
   */
  void limitrange()
  { 
    uint32 bits       = 0; memcpy(&bits, &data, sizeof(float));
    uint32 exp_bits   = (bits >> orig_Y) & 0xff;
    int    exp        = exp_bits - orig_BIAS;
    int    exp_denorm = (1 - BIAS) - Y;
    //The min_denorm's E of ExMy that E8M23 can represents
      
    if(exp >= (1 << X) - 1 - BIAS)
      {
	uint32 sign  = (data < 0) ? 1 << 31 : 0;
	uint32 exp   = ((1 << X) - 2 - BIAS + orig_BIAS) << orig_Y;
	uint32 frac  = ((1 << orig_Y) - 1) & ~((1 << (orig_Y - Y)) - 1);
	
	uint32 res   = sign | exp | frac;
	memcpy(&data, &res, sizeof(float));
      }
    else if(exp < exp_denorm) // -1??
      {
	data = 0.0f;
      }
  }

  float eps_spacing()
  {
    uint32 bits = 0; memcpy(&bits, &data, sizeof(float));
    bits &= 0x7fffffff; bits >>= orig_Y - Y;
    uint32 next = bits + 1;

    bits <<= orig_Y - Y;
    next <<= orig_Y - Y;

    float curr_f = 0; memcpy(&curr_f, &bits, sizeof(float));
    float next_f = 0; memcpy(&next_f, &next, sizeof(float));
    
    float eps_spacing = next_f - curr_f;
    
    if (eps_spacing < min_denorm()) eps_spacing = min_denorm();
    /* Why this is necessary?
     * min_denorm is the smallest eps_spacing for ExMy.
     */

    return eps_spacing;
  }
};

#endif
