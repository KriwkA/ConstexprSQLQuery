#pragma once

#include <utility>
#include <cstddef>
#include <string_view>
#include <cstdint>

namespace ctll {

template <typename CharT, size_t N>
class fixed_string {
   CharT content[N] = {};
   size_t real_size{0};

public:
   static constexpr size_t content_size = N;
   using value_type = CharT;

   constexpr fixed_string() = default;

   //concat value_type of all args must be same
   template<typename ...Args>
   constexpr fixed_string(const Args& ...args) noexcept {
      size_t pos = 0;
      auto builder = [&](const auto& str) {
         for(size_t i = 0; i < str.size(); ++i) {
            content[pos++] = str[i];
         }
         real_size += str.size();
      };
      (builder(args), ...);
   }

   constexpr fixed_string(const CharT (&input)[N]) noexcept {
      for (size_t i{0}; i < N; ++i) {
         content[i] = input[i];
         if ((i == (N-1)) && (input[i] == 0)) {
            break;
         }
         real_size++;
      }
   }

   template<size_t M>
   constexpr fixed_string(const fixed_string<CharT, M>& other) noexcept {
      for (size_t i = 0; i < other.size(); ++i) {
         content[i] = other[i];
      }
      real_size = other.size();
   }
   constexpr size_t size() const noexcept {
      return real_size;
   }
   constexpr const CharT * begin() const noexcept {
      return content;
   }
   constexpr const CharT * end() const noexcept {
      return content + size();
   }
   constexpr CharT operator[](size_t i) const noexcept {
      return content[i];
   }


   template<size_t M>
   constexpr fixed_string<CharT, N + M> operator+(const fixed_string<CharT, M>& fs) const noexcept {
      return fixed_string<CharT, N + M>(*this, fs);
   }

   template<size_t M>
   constexpr fixed_string<CharT, N + M> operator+(const CharT (&input)[M]) const noexcept {
      return fixed_string<CharT, N + M>(*this, fixed_string<CharT, M>(input));
   }

   constexpr fixed_string<CharT, N + 1> operator+(CharT c) const noexcept {
      CharT arr[] = {c};
      return *this + arr;
   }

   template <size_t M>
   constexpr bool is_same_as(const fixed_string<CharT, M> & rhs) const noexcept {
      if (real_size != rhs.size()) return false;
      for (size_t i{0}; i != real_size; ++i) {
         if (content[i] != rhs[i]) {
            return false;
         }
      }
      return true;
   }
};

template <typename CharT> class fixed_string<CharT, 0> {
   static constexpr CharT __empty[1] = {0};
public:
   constexpr fixed_string() noexcept {}
   constexpr fixed_string(const CharT *) noexcept {

   }
   constexpr fixed_string(std::initializer_list<CharT>) noexcept {

   }
   constexpr fixed_string(const fixed_string &) noexcept {

   }
   constexpr size_t size() const noexcept {
      return 0;
   }
   constexpr const CharT * begin() const noexcept {
      return __empty;
   }
   constexpr const CharT * end() const noexcept {
      return __empty + size();
   }
   constexpr CharT operator[](size_t) const noexcept {
      return 0;
   }

   template<size_t M>
   constexpr fixed_string<CharT,M> operator+(const fixed_string<CharT, M>& fs) const noexcept {
      return fs;
   }

   constexpr fixed_string<CharT, 1> operator+(CharT c) const noexcept {
      CharT arr[1] = {c};
      return fixed_string<CharT, 1>(arr);
   }

};

template <typename CharT, size_t N> fixed_string(const CharT (&)[N]) -> fixed_string<CharT, N>;

}

template <typename CharT, size_t N, size_t M>
constexpr decltype(auto) operator+(const CharT (&left)[N], const ctll::fixed_string<CharT, M>& right) noexcept {
   return ctll::fixed_string(left) + right;
}

template <typename CharT, size_t N>
constexpr decltype(auto) operator+(CharT c, const ctll::fixed_string<CharT, N>& right) noexcept {
   CharT arr[1] = {c};
   return arr + right;
}



template <size_t N>
std::ostream& operator<<(std::ostream& os, const ctll::fixed_string<char, N>& str) {
   os.write(str.begin(), str.size());
   return os;
}

template <size_t N>
std::wostream& operator<<(std::wostream& os, const ctll::fixed_string<wchar_t, N>& str) {
   os.write(str.begin(), str.size());
   return os;
}
