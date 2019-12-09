#pragma once

#include <string_view>
#include <string>
#include "reflection.hpp"
#include "fixed_string_utils.hpp"

#include <iostream>


namespace db {

namespace details {

template<typename T, size_t POS, size_t N>
constexpr decltype(auto) JoinFieldsToInsert() {
   constexpr auto f = reflection::GetMetaInfo<T, POS>().info.name;
   if constexpr(POS == 0) {
      if constexpr (N != 1) {
         return L'[' + f + L']' + JoinFieldsToInsert<T, POS + 1, N>();
      } else {
         return L'[' + f + L']';
      }
   } else if constexpr(POS < N - 1) {
      return L", [" + f + L']' + JoinFieldsToInsert<T, POS + 1, N>();
   } else if constexpr(POS == N - 1)  {
      return L",[" + f + L']';
   }
}

template<typename T>
constexpr decltype(auto) JoinFieldsToInsert() {
   constexpr auto field_count = reflection::ModelMetaInfo<T>::FieldsInfo::field_count;
   static_assert (field_count> 0, "");
   return  JoinFieldsToInsert<T, 0, field_count>();
}

template<size_t POS, size_t N>
constexpr decltype(auto) JoinInsValuesPlaceholders() {
   if constexpr(POS == 1) {
      if constexpr (N != 1) {
         return L"$1" + JoinInsValuesPlaceholders<POS + 1, N>();
      } else {
         return L"$1";
      }
   } else if constexpr(POS < N) {
      return L", $" + ctll::to_wfixed_string<POS>::value + JoinInsValuesPlaceholders<POS + 1, N>();
   } else if constexpr(POS == N)  {
      return L", $" + ctll::to_wfixed_string<POS>::value;
   }
}

template<typename T>
constexpr decltype(auto) JoinInsValuesPlaceholders() {
   return JoinInsValuesPlaceholders<1, reflection::ModelMetaInfo<T>::FieldsInfo::field_count>();
}

template<typename T, size_t POS, size_t N>
constexpr decltype(auto) JoinFieldsToUpdate() {
   constexpr auto f = reflection::GetMetaInfo<T, POS>().info.name;
   if constexpr(POS == 0) {
      if constexpr (N != 1) {
         return L"[" + f + L"]=$1" + JoinFieldsToUpdate<T, POS + 1, N>();
      } else {
         return L"[" + f + L"]=$1";
      }
   } else if constexpr(POS < N - 1) {
      return L", [" + f + L"]=$" + ctll::to_wfixed_string<POS + 1>::value + JoinFieldsToUpdate<T, POS + 1, N>();
   } else if constexpr(POS == N - 1)  {
      return L", [" + f + L"]=$" + ctll::to_wfixed_string<POS + 1>::value;
   }
}

template<typename T>
constexpr decltype(auto) JoinFieldsToUpdate() {
   constexpr auto field_count = reflection::ModelMetaInfo<T>::FieldsInfo::field_count;
   static_assert (field_count> 0, "");
   return  JoinFieldsToUpdate<T, 0, field_count>();
}

}

template<typename T, typename TABLE_CLASS>
constexpr auto InsertQuery() {
   constexpr auto res = L"INSERT INTO [" + TABLE_CLASS::TABLE_NAME + L"] (" +
                        details::JoinFieldsToInsert<T>() + L") VALUES (" + details::JoinInsValuesPlaceholders<T>() + L')';
   return res;
}

template<typename T, typename TABLE_CLASS>
constexpr auto UpdateQuery() {

    constexpr auto res = L"UPDATE [" + TABLE_CLASS::TABLE_NAME +
          L"] SET " + details::JoinFieldsToUpdate<T>();
   return res;
}


}
