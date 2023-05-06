#include "tagarray.h"
#include "tagarray/Container.hpp"
#include "tagarray/Record.hpp"

using namespace tagarray;

extern "C" void *new_container(const char *const comment) noexcept {
  return static_cast<void *>(new Container(comment));
}

extern "C" int32_t get_container_status(const void *const container) noexcept {
  return static_cast<const Container *>(container)->get_status();
}

extern "C" void dump_container(const void *const container,
                               const int32_t level) noexcept {
  static_cast<const Container *>(container)->dump(level);
}

extern "C" void *load_container_c8(const char *const filename) noexcept {
  return nullptr;
}

extern "C" void *load_container_c32(const char32_t *const filename) noexcept {
  return nullptr;
}

extern "C" void save_container_c8(void *const container,
                                  const char *const filename) noexcept {
  static_cast<Container *>(container)->save(filename);
}

extern "C" void save_container_c32(void *const container,
                                   const char32_t *const filename) noexcept {
  static_cast<Container *>(container)->save(filename);
}

extern "C" void delete_container(void *const container) noexcept {
  delete static_cast<Container *>(container);
}

extern "C" void *new_record(const uint32_t type_id, const uint32_t n_dimensions,
                            const uint8_t *const data,
                            const uint64_t data_length,
                            const uint64_t dimensions[TA_DIMENSIONS_LENGTH],
                            const int64_t options[TA_OPTIONS_LENGTH],
                            const char *const comment) noexcept {
  return static_cast<void *>(new Record(
      type_id, n_dimensions, data, data_length, dimensions, options, comment));
}

extern "C" int32_t get_record_status(const void *const record) noexcept {
  return static_cast<const Record *>(record)->get_status();
}

extern "C" void dump_record(const void *const record,
                            const int32_t level) noexcept {
  static_cast<const Record *>(record)->dump(level);
}

extern "C" void delete_record(void *const record) noexcept {
  delete static_cast<Record *>(record);
}

extern "C" void add_record(void *const container, const char *const tag,
                           void *const record) noexcept {
  static_cast<Container *>(container)->add_record(
      tag, *static_cast<Record *>(record));
}

extern "C" void *get_record(void *const container,
                            const char *const tag) noexcept {
  return static_cast<Container *>(container)->get_record(tag);
}

extern "C" void container_remove_record(void *const container,
                                        const char *const tag) noexcept {
  return static_cast<Container *>(container)->remove_record(tag);
}

extern "C" RecordInfo get_record_info(const void *const record) noexcept {
  return static_cast<const Record *>(record)->get_info();
}
