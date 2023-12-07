#include "tagarray.hpp"

using namespace tagarray;

extern "C" void *TA_Container_new(const char *const description) noexcept {
  std::string desc("");
  if (description != nullptr)
    desc = std::string(description);
  return static_cast<void *>(new (std::nothrow) Container(desc));
}

extern "C" void TA_Container_delete(void *const container) noexcept {
  if (container == nullptr)
    return;
  delete static_cast<Container *>(container);
}

extern "C" int32_t
TA_Container_create(void *const container, const char *const ctag,
                    const int32_t type_id, const int32_t ndims,
                    const int64_t *const cdims, const uint8_t *const data,
                    const char *const description) noexcept {
  if (container == nullptr)
    return defines::NOT_IMPLEMENTED;
  if (ctag == nullptr)
    return defines::NOT_IMPLEMENTED;
  if (cdims == nullptr)
    return defines::NOT_IMPLEMENTED;
  std::string tag(ctag), desc("");
  if (description != nullptr)
    desc = std::string(description);
  std::vector<int64_t> dims;
  dims.assign(cdims, cdims + ndims);
  return static_cast<Container *>(container)->create(tag, type_id, dims, data,
                                                     desc);
}

extern "C" RecordInfo TA_Container_get(void *const container,
                                       const char *const ctag) noexcept {
  if (container == nullptr || ctag == nullptr)
    return RecordInfo{
        defines::TYPE_UNKNOWN, 1, -1, 0, nullptr, nullptr, nullptr};
  return static_cast<Container *>(container)->get(ctag)->info();
}

extern "C" bool TA_Container_contains(void *const container,
                                      const char *const ctag) noexcept {
  if (container == nullptr || ctag == nullptr)
    return false;
  return static_cast<Container *>(container)->contains(ctag);
}

extern "C" void TA_Container_clear(void *const container) noexcept {
  if (container == nullptr)
    return;
  static_cast<Container *>(container)->clear();
}

extern "C" void TA_Container_erase(void *const container,
                                   const char *const ctag) noexcept {
  if (container == nullptr || ctag == nullptr)
    return;
  static_cast<Container *>(container)->erase(ctag);
}

extern "C" void *TA_Container_load(const char *const filename) noexcept {
  if (filename == nullptr)
    return nullptr;
  std::string path(filename);
  return static_cast<void *>(new (std::nothrow)
                                 Container(Container::load(path)));
}

extern "C" int32_t TA_Container_save(void *const container,
                                     const char *const filename) noexcept {
  if (container == nullptr || filename == nullptr)
    return defines::NOT_IMPLEMENTED;
  std::string path(filename);
  return static_cast<Container *>(container)->save(path);
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

extern "C" int32_t TA_get_storage_size(const int32_t datatype) noexcept {
  return tagarray::utils::get_storage_size(datatype);
}
