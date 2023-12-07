#pragma once

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
//#include <uchar.h>
#endif

#ifdef __cplusplus
namespace tagarray {

extern "C" {
#endif

typedef struct {
  int32_t type_id;
  int32_t itemsize; // item size in bytes
  int64_t count; // item count
  int64_t ndims; // number of dimensions
  int64_t *dims;
  void *data;
  char *desc;
} RecordInfo;

#ifdef __cplusplus
}
} // namespace tagarray
#endif
