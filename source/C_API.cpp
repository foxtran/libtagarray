#include "tagarray.h"
#include "tagarray/Record.hpp"
#include "tagarray/Container.hpp"

using namespace tagarray;

extern "C" void *new_container(const char *const comment) noexcept {
  return reinterpret_cast<void *>(new Container(comment));
}

extern "C" int32_t get_container_status(const void *const container) noexcept {
  return reinterpret_cast<const Container *const>(container)->get_status();
}

extern "C" void dump_container(const void *const container, const int32_t level) noexcept {
  reinterpret_cast<const Container *const>(container)->dump(level);
}

extern "C" void *load_container_c8(const char *const filename) noexcept {
  return nullptr;
}

extern "C" void *load_container_c32(const char32_t *const filename) noexcept {
  return nullptr;
}

extern "C" void save_container_c8(void *const container,
                                  const char *const filename) noexcept {
  reinterpret_cast<Container *const>(container)->save(filename);
}

extern "C" void save_container_c32(void *const container,
                                  const char32_t *const filename) noexcept {
  reinterpret_cast<Container *const>(container)->save(filename);
}

extern "C" void delete_container(void *const container) noexcept {
  delete reinterpret_cast<Container *const>(container);
}

extern "C" void *
new_record(const uint32_t type_id,
           const uint32_t n_dimensions, const uint8_t *const data,
           const uint64_t data_length,
           const uint64_t dimensions[TA_DIMENSIONS_LENGTH],
           const int64_t options[TA_OPTIONS_LENGTH],
           const char *const comment) noexcept {
  return reinterpret_cast<void *>(new Record(type_id, n_dimensions, data, data_length, dimensions, options, comment));
}

extern "C" int32_t get_record_status(const void *const record) noexcept {
  return reinterpret_cast<const Record *const>(record)->get_status();
}

extern "C" void dump_record(const void *const record, const int32_t level) noexcept {
  reinterpret_cast<const Record *const>(record)->dump(level);
}

extern "C" void delete_record(void *const record) noexcept {
  delete reinterpret_cast<Record *const>(record);
}

extern "C" void add_record(void *const container, const char *const tag, void *const record) noexcept {
  reinterpret_cast<Container *const>(container)->add_record(tag, *reinterpret_cast<Record *const>(record));
}

extern "C" void *get_record(void *const container, const char *const tag) noexcept {
  return reinterpret_cast<Container *const>(container)->get_record(tag);
}

extern "C" void container_remove_record(void *const container, const char *const tag) noexcept {
  return reinterpret_cast<Container *const>(container)->remove_record(tag);
}

extern "C" RecordInfo get_record_info(const void *const record) noexcept {
  return reinterpret_cast<const Record *const>(record)->get_info();
}
