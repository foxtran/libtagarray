#pragma once

#include "tagarray/defines.h"
#include "tagarray/info.h"

#ifdef __cplusplus
#include <cstdint>
#define TA_NOEXCEPT noexcept
#else
#include <stdint.h>
#include <uchar.h>
#define TA_NOEXCEPT
#endif

#ifdef __cplusplus
namespace tagarray {
namespace CAPI {

extern "C" {
#endif

void *TA_Container_new(const char *const comment) TA_NOEXCEPT;

int32_t TA_Container_get_status(const void *const container) TA_NOEXCEPT;

void TA_Container_dump(const void *const container,
                       const int32_t level) TA_NOEXCEPT;

void *TA_Container_load(const char *const filename) TA_NOEXCEPT;

void TA_Container_save(void *const container,
                       const char *const filename) TA_NOEXCEPT;

void TA_Container_delete(void *const container) TA_NOEXCEPT;

void *TA_Record_new(const int32_t type_id, const int32_t n_dimensions,
                    const uint8_t *data, const int64_t data_length,
                    const int64_t dimensions[TA_DIMENSIONS_LENGTH],
                    const int64_t options[TA_OPTIONS_LENGTH],
                    const char *const comment) TA_NOEXCEPT;

int32_t TA_Record_get_status(const void *const record) TA_NOEXCEPT;

void TA_Record_dump(const void *const record, const int32_t level) TA_NOEXCEPT;

void TA_Record_delete(void *const record) TA_NOEXCEPT;

void TA_Container_add_record(void *const container, const char *const tag,
                             void *const record) TA_NOEXCEPT;

int32_t TA_Container_has_record(void *const container,
                                const char *const tag) TA_NOEXCEPT;

void *TA_Container_get_record(void *const container,
                              const char *const tag) TA_NOEXCEPT;

void TA_Container_remove_record(void *const container,
                                const char *const tag) TA_NOEXCEPT;

RecordInfo TA_Record_get_record_info(const void *const record) TA_NOEXCEPT;

const char *TA_get_status_message(const int32_t status,
                                  const char *const tag) TA_NOEXCEPT;

void TA_string_delete(void *const string_ptr) TA_NOEXCEPT;

#ifdef __cplusplus
}
} // namespace CAPI
} // namespace tagarray
#endif
