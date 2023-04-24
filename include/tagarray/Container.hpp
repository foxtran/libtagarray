#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <map>

#include "tagarray/defines.h"
#include "tagarray/Record.hpp"

namespace tagarray {

class Container {
private:
  uint32_t version;
  int32_t status;
  std::string comment;
  std::map<std::string, Record *> records;

public:
  inline Container() noexcept : version(TA_CONTAINER_VERSION), status(TA_OK) {};
  inline Container(std::string &comment) noexcept : version(TA_CONTAINER_VERSION), status(TA_OK), comment(comment) {};
  inline Container(const char *const comment_ptr) noexcept : version(TA_CONTAINER_VERSION), status(TA_OK) {
    update_comment(comment_ptr);
  }
  ~Container() noexcept;

  inline uint32_t get_version() const noexcept { return this->version; }

  inline int32_t get_status() const noexcept { return this->status; }

  inline const std::string &get_comment() const noexcept { return this->comment; }

  inline void update_comment(const char *const comment_ptr) noexcept {
    std::string comment = std::string();
    if (comment_ptr != nullptr)
      comment = std::string(comment_ptr);
    update_comment(comment);
  }
  inline void update_comment(const std::string &comment) noexcept {
    this->comment = comment;
    this->status = TA_OK;
  }

  void add_record(const std::string &tag, Record &record) noexcept;
  inline void add_record(const char *const tag, Record &record) noexcept {
    if (tag == nullptr)
    {
      this->status = TA_EMPTY_TAG;
      return;
    }
    add_record(std::string(tag), record);
  }

  int32_t find_record(const std::string &tag) noexcept;
  inline int32_t find_record(const char *const tag) noexcept {
    if (tag == nullptr)
    {
      this->status = TA_EMPTY_TAG;
      return TA_CONTAINER_RECORD_NOT_FOUND;
    }
    return find_record(std::string(tag));
  }

  void remove_record(const std::string &tag) noexcept;
  inline void remove_record(const char *const tag) noexcept {
    if (tag == nullptr)
    {
      this->status = TA_EMPTY_TAG;
      return;
    }
    remove_record(std::string(tag));
  }

  Record *get_record(const std::string &tag) noexcept;
  inline Record *get_record(const char *const tag) noexcept {
    if (tag == nullptr)
    {
      this->status = TA_EMPTY_TAG;
      return nullptr;
    }
    return get_record(std::string(tag));
  }

  void save(const std::string &filename) noexcept;
  void save(const std::u32string &filename) noexcept;
  inline void save(const char *const filename_ptr) noexcept {
    if (filename_ptr == nullptr) {
      this->status = TA_FILENAME_NULLPTR;
      return;
    }
    save(std::string(filename_ptr));
  }
  inline void save(const char32_t *const filename_ptr) noexcept {
    if (filename_ptr == nullptr) {
      this->status = TA_FILENAME_NULLPTR;
      return;
    }
    save(std::u32string(filename_ptr));
  }

  void load(const std::string &filename) noexcept;
  void load(const std::u32string &filename) noexcept;
  inline void load(const char *const filename_ptr) noexcept {
    if (filename_ptr == nullptr) {
      this->status = TA_FILENAME_NULLPTR;
      return;
    }
    load(std::string(filename_ptr));
  }
  inline void load(const char32_t *const filename_ptr) noexcept {
    if (filename_ptr == nullptr) {
      this->status = TA_FILENAME_NULLPTR;
      return;
    }
    load(std::u32string(filename_ptr));
  }

  void dump(const int32_t level) const noexcept;
};

} // namespace tagarray
