#pragma once

#include <string>
#include <regex>

#include "tagarray/defines.h"

namespace tagarray {
namespace utils {

template <typename T>
int32_t check_ptr(const T *const ptr) noexcept {
  if (ptr == nullptr)
    return TA_NULLPTR;
  return TA_OK;
}

int32_t check_tag(const std::string &tag) noexcept {
  std::regex tag_regex("^\b[ -~]+\b$");
  if(std::regex_match(tag, tag_regex))
    return TA_OK;
  return TA_INCORRECT_TAG;
}

} // namespace utils
} // namespace tagarray
