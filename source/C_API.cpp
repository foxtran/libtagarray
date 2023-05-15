#include "tagarray.h"
#include "tagarray/Container.hpp"
#include "tagarray/Record.hpp"
#include "tagarray/Utils.hpp"

using namespace tagarray;

extern "C" void *TA_Container_new(const char *const comment) noexcept {
  return static_cast<void *>(new (std::nothrow) Container(comment));
}

extern "C" int32_t
TA_Container_get_status(const void *const container) noexcept {
  return static_cast<const Container *>(container)->get_status();
}

extern "C" void TA_Container_dump(const void *const container,
                                  const int32_t level) noexcept {
  static_cast<const Container *>(container)->dump(level);
}

extern "C" void *TA_Container_load(const char *const filename) noexcept {
  Container *container = new (std::nothrow) Container();
  container->load(filename);
  return static_cast<void *>(container);
}

extern "C" void TA_Container_save(void *const container,
                                  const char *const filename) noexcept {
  static_cast<Container *>(container)->save(filename);
}

extern "C" void TA_Container_delete(void *const container) noexcept {
  delete static_cast<Container *>(container);
}

extern "C" void *TA_Record_new(const int32_t type_id,
                               const int32_t n_dimensions,
                               const uint8_t *const data,
                               const int64_t data_length,
                               const int64_t dimensions[TA_DIMENSIONS_LENGTH],
                               const int64_t options[TA_OPTIONS_LENGTH],
                               const char *const comment) noexcept {
  return static_cast<void *>(new (std::nothrow) Record(
      type_id, n_dimensions, data, data_length, dimensions, options, comment));
}

extern "C" int32_t TA_Record_get_status(const void *const record) noexcept {
  return static_cast<const Record *>(record)->get_status();
}

extern "C" void TA_Record_dump(const void *const record,
                               const int32_t level) noexcept {
  static_cast<const Record *>(record)->dump(level);
}

extern "C" void TA_Record_delete(void *const record) noexcept {
  delete static_cast<Record *>(record);
}

extern "C" void TA_Container_add_record(void *const container,
                                        const char *const tag,
                                        void *const record) noexcept {
  static_cast<Container *>(container)->add_record(
      tag, *static_cast<Record *>(record));
}

extern "C" int32_t TA_Container_has_record(void *const container,
                                           const char *const tag) noexcept {
  return static_cast<Container *>(container)->has_record(tag);
}

extern "C" void *TA_Container_get_record(void *const container,
                                         const char *const tag) noexcept {
  return static_cast<Container *>(container)->get_record(tag);
}

extern "C" void TA_Container_remove_record(void *const container,
                                           const char *const tag) noexcept {
  return static_cast<Container *>(container)->remove_record(tag);
}

extern "C" RecordInfo
TA_Record_get_record_info(const void *const record) noexcept {
  return static_cast<const Record *>(record)->get_info();
}

extern "C" const char *TA_get_status_message(const int32_t status,
                                             const char *const tag) noexcept {
  std::string message = tagarray::utils::get_status_message(status, tag);
  char *message_ptr = new (std::nothrow) char[message.size() + 1];
  std::copy(message.begin(), message.end(), message_ptr);
  message_ptr[message.size()] = '\0';
  return message_ptr;
}

extern "C" void TA_string_delete(void *const string_ptr) noexcept {
  delete static_cast<char *>(string_ptr);
}
