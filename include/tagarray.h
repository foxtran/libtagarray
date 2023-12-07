#pragma once

#include "tagarray/RecordInfo.h"
#include "tagarray/defines.h"

#ifdef __cplusplus
#include <cstdint>
#define TA_NOEXCEPT noexcept
#else
#include <stdbool.h>
#include <stdint.h>
#define TA_NOEXCEPT
#endif

#ifdef __cplusplus
namespace tagarray {
namespace CAPI {

extern "C" {
#endif

void *TA_Container_new(const char *const description) TA_NOEXCEPT;

void *TA_Container_load(const char *const filename) TA_NOEXCEPT;

void TA_Container_delete(void *const container) TA_NOEXCEPT;

int32_t TA_Container_create(void *const container, const char *const ctag,
                            const int32_t type_id, const int32_t ndims,
                            const int64_t *const cdims,
                            const uint8_t *const data,
                            const char *const description) TA_NOEXCEPT;

RecordInfo TA_Container_get(void *const container,
                            const char *const ctag) TA_NOEXCEPT;

bool TA_Container_contains(void *const container,
                           const char *const ctag) TA_NOEXCEPT;

void TA_Container_clear(void *const container) TA_NOEXCEPT;

void TA_Container_erase(void *const container,
                        const char *const ctag) TA_NOEXCEPT;

int32_t TA_Container_save(void *const container,
                          const char *const filename) TA_NOEXCEPT;

const char *TA_get_status_message(const int32_t status,
                                  const char *const tag) TA_NOEXCEPT;

void TA_string_delete(void *const string_ptr) TA_NOEXCEPT;

int64_t TA_get_storage_size(const int32_t datatype) TA_NOEXCEPT;

#ifdef __cplusplus
}
} // namespace CAPI
} // namespace tagarray
#endif
