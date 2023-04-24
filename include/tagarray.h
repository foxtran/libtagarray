#ifndef _TAGARRAY_H
#define _TAGARRAY_H

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

void *new_container(const char *const comment) TA_NOEXCEPT;

int32_t get_container_status(const void *const container) TA_NOEXCEPT;

void dump_container(const void *const container, const int32_t level) TA_NOEXCEPT;

void *load_container_c8(const char *const filename) TA_NOEXCEPT;
void *load_container_c32(const char32_t *const filename) TA_NOEXCEPT;

void save_container_c8(void *const container, const char *const filename) TA_NOEXCEPT;
void save_container_c32(void *const container, const char32_t *const filename) TA_NOEXCEPT;

void delete_container(void *const container) TA_NOEXCEPT;

void *new_record(const uint32_t type_id,
                 const uint32_t n_dimensions, const uint8_t *data,
                 const uint64_t data_length,
                 const uint64_t dimensions[TAGARRAY_DIMENSIONS_LENGTH],
                 const int64_t options[TAGARRAY_OPTIONS_LENGTH],
                 const char *const comment) TA_NOEXCEPT;

int32_t get_record_status(const void *const record) TA_NOEXCEPT;

void dump_record(const void *const record, const int32_t level) TA_NOEXCEPT;

void delete_record(void *const record) TA_NOEXCEPT;

void add_record(void *const container, const char *const tag, void *const record) TA_NOEXCEPT;

void *get_record(void *const container, const char *const tag) TA_NOEXCEPT;

void container_remove_record(void *const container, const char *const tag) TA_NOEXCEPT;

RecordInfo get_record_info(const void* const record) TA_NOEXCEPT;

#ifdef __cplusplus
}
}
}
#endif

#endif
