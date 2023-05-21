#pragma once

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

} // namespace utils
} // namespace tagarray
