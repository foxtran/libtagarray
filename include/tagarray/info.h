#pragma once

#ifdef __cplusplus
#include "tagarray/defines.hpp"
#include <cstdint>
#define TA_MAX_DIMENSIONS_LENGTH (defines::MAX_DIMENSIONS_LENGTH)
#else
#include "tagarray/defines.h"
#include <stdint.h>
#include <uchar.h>
#endif

#ifdef __cplusplus
namespace tagarray {

extern "C" {
#endif

typedef struct {
  int32_t type_id;
  int32_t itemsize;
  int64_t count;
  int64_t n_dimensions;
  int64_t dimensions[TA_MAX_DIMENSIONS_LENGTH];
  void *data;
} RecordInfo;

#ifdef __cplusplus
#undef TA_DIMENSIONS_LENGTH
}
} // namespace tagarray
#endif
