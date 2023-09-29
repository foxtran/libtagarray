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
  int32_t n_dimensions;
  void *data;
  int64_t data_length;
  int64_t dimensions[TA_MAX_DIMENSIONS_LENGTH];
} RecordInfo;

#ifdef __cplusplus
#undef TA_DIMENSIONS_LENGTH
}
} // namespace tagarray
#endif
