#pragma once

#include <algorithm>
#include <array>
#include <string>

#include "tagarray/defines.h"

namespace tagarray {
namespace utils {

template <typename T> inline int32_t check_ptr(const T *const ptr) noexcept {
  return ptr == nullptr ? TA_NULLPTR : TA_OK;
}

inline int32_t check_tag(const std::string &tag) noexcept {
  if (tag.empty())
    return TA_INCORRECT_TAG;
  // detect leading whitespace(s)
  if (tag[0] == ' ')
    return TA_INCORRECT_TAG;
  // detect trailing whitespace(s)
  if (tag.back() == ' ')
    return TA_INCORRECT_TAG;
  // all symbols should be visible and presented in ASCII table
  for (const auto &c : tag)
    if (c < ' ' || c > '~')
      return TA_INCORRECT_TAG;
  return TA_OK;
}

std::string
get_status_message(const int32_t status,
                   const std::string &tag = std::string("")) noexcept;
inline std::string get_status_message(const int32_t status,
                                      const char *const tag) noexcept {
  if (check_ptr(tag) == TA_OK) {
    return get_status_message(status, std::string(tag));
  }
  return get_status_message(status);
}

inline std::string to_string(const char *const str_ptr) noexcept {
  std::string str = std::string();
  if (str_ptr != nullptr)
    str = std::string(str_ptr);
  return str;
}

template <size_t N, typename T>
inline std::array<T, N> to_array(const T (&arr_in)[N]) noexcept {
  std::array<T, N> arr_out;
  std::copy(arr_in, arr_in + arr_out.size(), arr_out.begin());
  return arr_out;
}

template <size_t N, typename T>
inline std::array<T, N> to_array(const T *const &arr_in) noexcept {
  std::array<T, N> arr_out;
  std::copy(arr_in, arr_in + arr_out.size(), arr_out.begin());
  return arr_out;
}

int64_t get_storage_size(const int32_t datatype) noexcept;

} // namespace utils
} // namespace tagarray
