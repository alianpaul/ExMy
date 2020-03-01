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
     * [1.00..0] * 2^(E - orig_BIAS) = [0.0..01] * 2^(1 - BIAS)
     *
     * [1.00..0]
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
      
    if(exp >= (1 << X) - 1 - BIAS){
   
      uint32 sign  = (data < 0) ? 1 << 31 : 0;
      uint32 exp   = ((1 << X) - 2 - BIAS + orig_BIAS) << orig_Y;
      uint32 frac  = ((1 << orig_Y) - 1) & ~((1 << (orig_Y - Y)) - 1);
	
      uint32 res   = sign | exp | frac;
      memcpy(&data, &res, sizeof(float));
    }
    else if(exp < exp_denorm - 1) {
	/* The min_denorm  of exmy in e8m23: 2^(-Y) * 2^(1 - bias) = 1.00..0 * 2^(1 - bias -Y)
	 * The min_denorm / 2 would be:      1.00..0 * 2^((1 - bias - Y) - 1)
	 *
	 * The e8m23 numbers in (min_denorm / 2, min_denorm) should be
	 * be rounded to min_denorm. *Round* will take the job. Only
	 * numbers in [0, min_denorm / 2] can be safely set to 0.
	 */
      data = 0.0f;
    }
  }

  float eps_spacing()
  {
    uint32 bits = 0; memcpy(&bits, &data, sizeof(float));
    bits &= 0x7fffffff; bits >>= orig_Y - Y;
    uint32 next = bits + 1;
    /* Whay +1 is enough?  First of all, we are operatig on e8m23
     * norm. e8m23 denorms will round to 0(by limitrange). e8m23 has
     * its own eps_spacing. We can get that by +1 without shifting.
     *
     * e8m23: 
     *
     * dnorm + 1 -> dnorm: easy one, no mantissa bit overflow.
     *
     * max_dnorm + 1 -> min_norm: mantissa overflows to the exponent
     * bit and when this happens, the result is exactly min_norm.
     *
     * norm + 1 -> norm. Also should be fine when +1 overflows to
     * exponent bit because we just got the next norm.
     * 
     * How about exmy? This is what we really want. We are using e8m23
     * to represent exmy's denorm and norm. If e8m23 falls into
     * [min_norm, max_norm] of exmy. Then there is no implicit 1
     * inconsistency.  We can safely round(to zero) the e8m23 to the
     * nearest exmy by truncating the last orig_Y - Y bits and find
     * the exmy' corresponding eps.  But this rule is not correct when
     * e8m23 falls into exmy' [min_denorm/2, min_norm) range because
     * there is no implicit 1 there.  The result after truncating
     * contains the exmy's denorm, but it also contains some real
     * number that is not exmy. The eps_spacing we got could smaller
     * than what we want.  But the eps_spacing in this range is
     * actually fixed and is actually min_denorm. So a detection of
     * the eps_spacing underflow at last should be enough.
     */

    bits <<= orig_Y - Y;
    next <<= orig_Y - Y;

    float curr_f = 0; memcpy(&curr_f, &bits, sizeof(float));
    float next_f = 0; memcpy(&next_f, &next, sizeof(float));
    
    float eps_spacing = next_f - curr_f;
    
    if (eps_spacing < min_denorm()) eps_spacing = min_denorm();
    /* Why this is necessary?  min_denorm is the smallest eps_spacing
     * for ExMy.  And we are operating on the e8m23 data and we didn't
     * consider the factor that its range is bigger than exmy's. So it
     * could be possible that we got an eps_spacing of e8m23 that
     * smaller than exmy
     */
    return eps_spacing;
  }

  void round()
  {
    float bias = eps_spacing() / 2;

    //round-to-even
    if(data < min_denorm() + min_denorm()){
      /* bit that identifying even/odd(the least significant bit after
       * truncating) is in exp.
       */
      
      /* if data == min_denorm + bias, tied to even by rounding to min_denorm * 2.
       * So no need to set bias to 0
       */
      
      //min_denorm()
      if(data <= bias){
	data = bias = 0;
      }
    }
    else{
      /* bit/
      uint32 bits   = 0; memcpy(&bits, &data, sizeof(float));      
      uint32 eo_bit = 1 << (orig_Y - Y); //even odd bit pos

      if (data < min_norm()){
	/*exmy dnorm -> e8m23 norm. add the implicit 1 explicitly*/
	uint32 exp      = (bits >> orig_Y) & 0xff - orig_BIAS;
	uint32 exp_exmy = 1 - BIAS;
	eo_bit <<= exp_exmy - exp;
      }

      if(!(bits & eo_bit) &&
	 (bits & (eo_bit - 1) & (eo_bit >> 1))){
	/*even after trunc, and data is at the halfway*/
	bias = 0;
      }
    }

    data += bias;

    //Now we can truncate safely
    trunc()
  }

  void trunc()
  {
    
  }
};

#endif
