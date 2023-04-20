#ifndef _TAGARRAY_INFO_H
#define _TAGARRAY_INFO_H

#include "tagarray/defines.h"

#ifdef __cplusplus
#include <cstdint>
#else
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
  int64_t dimensions[TAGARRAY_DIMENSIONS_LENGTH];
} RecordInfo;

#ifdef __cplusplus
}
}
#endif

#endif
