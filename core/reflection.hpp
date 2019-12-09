#pragma once

#include <string_view>
#include "fixed_string.hpp"

namespace reflection {

template<size_t N>
struct FieldInfo {
   ctll::fixed_string<wchar_t, N> name;
};

template <size_t SIZE> FieldInfo(const wchar_t(&)[SIZE]) -> FieldInfo<SIZE>;


template<auto ...Args>
struct FieldsMetaInfo;

template<>
struct FieldsMetaInfo<> {
   using base = FieldsMetaInfo<>;
   static constexpr auto info = L"null";
   static constexpr auto f_ref = "null";
   static constexpr size_t field_count = 0;
};


template<auto F_REF, const auto* F_INFO>
struct FieldsMetaInfo<F_REF, F_INFO> {
   using base = FieldsMetaInfo<>;

   static constexpr auto info = *F_INFO;
   static constexpr auto f_ref = F_REF;
   static constexpr size_t field_count = 1;
};

template<auto F_REF, const auto* F_INFO, auto ...Args>
struct FieldsMetaInfo<F_REF, F_INFO, Args...> : public FieldsMetaInfo<Args...> {
   using base = FieldsMetaInfo<Args...>;
   static constexpr auto info = *F_INFO;
   static constexpr auto f_ref = F_REF;

   static constexpr size_t field_count = 1 + sizeof...(Args) / 2;

};

namespace details {
template<size_t POS, size_t N, typename FMI>
constexpr auto GetMetaInfo(const FMI& fmi) noexcept  {
   static_assert (N > 0, "");
   if constexpr (POS == 0) {
      return fmi;
   } else {
      (void)fmi;
      return GetMetaInfo<POS - 1, N - 1>(FMI::base());
   }
}
}

template<typename T>
struct ModelMetaInfo;

template<typename T, size_t POS>
constexpr decltype(auto) GetMetaInfo() {
   return details::GetMetaInfo<POS, ModelMetaInfo<T>::FieldsInfo::field_count>(typename ModelMetaInfo<T>::FieldsInfo());
}

}
