#ifndef MIDPOINT_HPP
#define MIDPOINT_HPP

#include <cmath>
#include <cstdint>

namespace std {
  template<typename _Int>
    constexpr std::enable_if_t<std::is_integral_v<_Int>
			   && !std::is_same_v<_Int, bool>, _Int>
    midpoint(_Int __a, _Int __b) noexcept {
      // +2 is enough for [INT_MIN*2, UINT_MAX*2+1]
      // According to standard, when a+b is odd,
      // round towards a, aka up if a>b
      
      // _BitInt only exist on clang, so this won't
      // compile on gcc or msvc currently.
      return (_BitInt(8*sizeof(__a)+2)(__a)+__b+(__a>__b)) >> 1;
    }
  template<typename _Float>
    constexpr std::enable_if_t<std::is_floating_point_v<_Float>, _Float>
    midpoint(_Float __a, _Float __b) noexcept {
      // This function may result in multiple inaccurate operations,
      // but provide correct result. Proof is below.
      
      // If t is finite, then it's either(or both)
      // normal or accurate (subnormal, constant eps)
      // and dividing by two provides correct result.
      
      // If a is infinite, then result is either
      // infinite or NaN, and *.5 won't matter.
      
      // If both a and b are finite but t is infinite,
      // then a has same sign to b, say both positive.
      // This algorithm differs from [llvm version](https://github.com/llvm/llvm-project/blob/main/libcxx/include/__numeric/midpoint.h#L78)
      // only when either a or b is tiny, say b.
      // On round-to-nearest, b is almost ignored.
      // On round-up, b*.5 would at least remain positive.
      // On round-down/round-towards-zero,
      // b*.5 would also get ignored.
      _Float __t = __a + __b;
      return isfinite(__t)?__t*.5:__a*.5+__b*.5;
    }

  template<typename _Tp>
    _Tp* midpoint(_Tp* __a, _Tp* __b) noexcept {
      // On modern computer, pointer differences are far from UINT_MAX
      return __a + (__b - __a) / 2;
    }

} // namespace std

#endif // MIDPOINT_HPP
