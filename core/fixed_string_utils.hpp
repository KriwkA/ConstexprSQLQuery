#pragma once

#include "fixed_string.hpp"

#include <array>

namespace ctll {

namespace details
{
   constexpr wchar_t wditigs_arr[] = L"0123456789";

   constexpr size_t length(uint64_t val) noexcept {
      size_t res = val == 0 ? 1 : 0;
      while (val > 0) {
         val /= 10;
         res += 1;
      }
      return res;
   }


   template<uint64_t val>
   struct to_fstring {
      static constexpr fixed_string<wchar_t, length(val)> value =
            val < 10 ? fixed_string<wchar_t, length(val)>(fixed_string<wchar_t, 0>() +  wditigs_arr[val])
                     : fixed_string<wchar_t, length(val)>(to_fstring<val / 10>::value + wditigs_arr[val % 10]);
   };

   template<>
   struct to_fstring<0> {
      static constexpr fixed_string value = L"0";
   };
}



template<uint64_t val>
struct to_wfixed_string {
   static constexpr auto value = details::to_fstring<val>::value;
};

}
