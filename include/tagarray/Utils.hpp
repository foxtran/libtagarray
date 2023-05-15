#pragma once

#include <regex>
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
  std::regex whitespace_regex("\\s");
  std::string f(tag[0], 1);
  std::string l(tag.back(), 1);
  if (std::regex_match(f, whitespace_regex))
    return TA_INCORRECT_TAG;
  if (std::regex_match(l, whitespace_regex))
    return TA_INCORRECT_TAG;
  std::regex tag_regex("^[ -~]+$");
  if (std::regex_match(tag, tag_regex))
    return TA_OK;
  return TA_INCORRECT_TAG;
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
