#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <string>
#include <unordered_map>

#include "tagarray/Record.hpp"
#include "tagarray/Utils.hpp"
#include "tagarray/defines.h"

namespace tagarray {

class Container {
private:
  int32_t _version;
  int32_t _status;
  std::string _comment;
  std::unordered_map<std::string, Record *> _records;

public:
  inline Container() noexcept
      : _version(TA_CONTAINER_VERSION), _status(TA_OK), _comment(""){};
  inline Container(std::string &comment) noexcept
      : _version(TA_CONTAINER_VERSION), _status(TA_OK), _comment(comment){};
  inline Container(const char *const comment_ptr) noexcept
      : _version(TA_CONTAINER_VERSION), _status(TA_OK), _comment("") {
    this->update_comment(comment_ptr);
  }
  ~Container() noexcept;
  Container &operator=(const Container &) = delete;
  Container(const Container &) = delete;

  inline int32_t get_version() const noexcept { return this->_version; }

  inline int32_t get_status() const noexcept { return this->_status; }

  inline const std::string &get_comment() const noexcept {
    return this->_comment;
  }

  inline void update_comment(const char *const comment_ptr) noexcept {
    this->_status = utils::check_ptr(comment_ptr);
    if (this->_status != TA_OK)
      return;
    this->update_comment(std::string(comment_ptr));
  }
  inline void update_comment(const std::string &comment) noexcept {
    this->_comment = comment;
    this->_status = TA_OK;
  }

  void add_record(const std::string &tag, Record &record) noexcept;
  inline void add_record(const char *const tag, Record &record) noexcept {
    this->_status = utils::check_ptr(tag);
    if (this->_status != TA_OK)
      return;
    this->add_record(std::string(tag), record);
  }

  inline int32_t has_record(const std::string &tag) noexcept {
    this->_status = utils::check_tag(tag);
    if (this->_status != TA_OK)
      return this->_status;
    this->_status = TA_CONTAINER_RECORD_NOT_FOUND;
    if (auto search = this->_records.find(tag); search != this->_records.end())
      this->_status = TA_OK;
    return this->_status;
  }
  inline int32_t has_record(const char *const tag) noexcept {
    this->_status = utils::check_ptr(tag);
    if (this->_status != TA_OK)
      return this->_status;
    return this->has_record(std::string(tag));
  }

  inline int32_t has_records(const std::vector<std::string> &tags) noexcept {
    for (const auto &tag : tags) {
      if (has_record(tag) != TA_OK)
        return this->_status;
    }
    return TA_OK;
  }

  inline void remove_record(const std::string &tag) noexcept {
    this->_status = utils::check_tag(tag);
    if (this->_status != TA_OK)
      return;
    this->has_record(tag);
    if (this->_status != TA_OK)
      return;
    delete this->_records[tag];
    this->_records.erase(tag);
    this->_status = TA_OK;
  }
  inline void remove_record(const char *const tag) noexcept {
    this->_status = utils::check_ptr(tag);
    if (this->_status != TA_OK)
      return;
    this->remove_record(std::string(tag));
  }

  inline void remove_records(const std::vector<std::string> &tags) noexcept {
    for (const auto &tag : tags)
      remove_record(tag);
  }

  inline Record *get_record(const std::string &tag) noexcept {
    this->_status = utils::check_tag(tag);
    if (this->_status != TA_OK)
      return nullptr;
    this->has_record(tag);
    if (this->_status != TA_OK)
      return nullptr;
    return this->_records[tag];
  }
  inline Record *get_record(const char *const tag) noexcept {
    this->_status = utils::check_ptr(tag);
    if (this->_status != TA_OK)
      return nullptr;
    return this->get_record(std::string(tag));
  }

  void save(const std::filesystem::path &filename) noexcept;
  inline void save(const std::string &filename) noexcept {
    this->save(std::filesystem::path(filename));
  }
  inline void save(const char *const filename_ptr) noexcept {
    this->_status = utils::check_ptr(filename_ptr);
    if (this->_status != TA_OK)
      return;
    this->save(std::string(filename_ptr));
  }

  void load(const std::filesystem::path &filename) noexcept;
  inline void load(const std::string &filename) noexcept {
    this->load(std::filesystem::path(filename));
  }
  inline void load(const char *const filename_ptr) noexcept {
    this->_status = utils::check_ptr(filename_ptr);
    if (this->_status != TA_OK)
      return;
    this->load(std::string(filename_ptr));
  }

  void dump(const int32_t level) const noexcept;
};

} // namespace tagarray
