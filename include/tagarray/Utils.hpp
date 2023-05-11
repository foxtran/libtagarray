#pragma once

#include "tagarray/defines.h"

namespace tagarray {
namespace utils {

template <typename T>
int32_t check_ptr(const T *const ptr) noexcept {
  if (ptr == nullptr)
    return TA_NULLPTR;
  return TA_OK;
}

} // namespace utils
} // namespace tagarray
