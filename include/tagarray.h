#ifndef _TAGARRAY_H
#define _TAGARRAY_H

#include "tagarray/defines.h"
#include "tagarray/info.h"

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#include <uchar.h>
#endif

#ifdef __cplusplus
namespace tagarray {
namespace CAPI {

extern "C" {
#endif

void *new_container(const char *const comment);

int32_t get_container_status(const void *const container);

void dump_container(const void *const container, const int32_t level);

void *load_container_c8(const char *const filename);
void *load_container_c32(const char32_t *const filename);

void save_container_c8(void *container, const char *const filename);
void save_container_c32(void *container, const char32_t *const filename);

void delete_container(void *container);

void *new_record(const char *const tag, const uint32_t type_id,
                 const uint32_t n_dimensions, const uint8_t *data,
                 const uint64_t data_length,
                 const uint64_t dimensions[TAGARRAY_DIMENSIONS_LENGTH],
                 const int64_t options[TAGARRAY_OPTIONS_LENGTH],
                 const char *const comment);

int32_t get_record_status(const void *const record);

void dump_record(const void *const record, const int32_t level);

void delete_record(void *record);

void add_record(void *container, void *record);

void *get_record(void *const container, const char *const tag);

void container_remove_record(void *const container, const char *const tag);

RecordInfo get_record_info(const void* const record);

#ifdef __cplusplus
}
}
}
#endif

#endif
