#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <string>
#include <unordered_map>

#include "tagarray/Record.hpp"
#include "tagarray/Utils.hpp"
#include "tagarray/defines.hpp"

namespace tagarray {

class Container {
private:
  int32_t version_;
  std::string comment_;
  std::unordered_map<std::string, Record *> records_;

public:
  inline Container() noexcept
      : version_(defines::CONTAINER_VERSION), comment_(""){};
  inline Container(const std::string &comment) noexcept
      : version_(defines::CONTAINER_VERSION), comment_(comment){};
  inline Container(const char *const comment_ptr) noexcept
      : version_(defines::CONTAINER_VERSION), comment_("") {
    this->update_comment(comment_ptr);
  }
  ~Container() noexcept;
  Container &operator=(const Container &) = delete;
  Container(const Container &) = delete;

  inline int32_t get_version() const noexcept { return this->version_; }

  inline const std::string &get_comment() const noexcept {
    return this->comment_;
  }

  inline int32_t update_comment(const char *const comment_ptr) noexcept {
    if (int32_t status = utils::check_ptr(comment_ptr); status != defines::OK)
      return status;
    return this->update_comment(std::string(comment_ptr));
  }
  inline int32_t update_comment(const std::string &comment) noexcept {
    this->comment_ = comment;
    return defines::OK;
  }

  int32_t add_record(const std::string &tag, Record &record) noexcept;
  inline int32_t add_record(const char *const tag, Record &record) noexcept {
    if (int32_t status = utils::check_ptr(tag); status != defines::OK)
      return status;
    return this->add_record(std::string(tag), record);
  }

  inline int32_t has_record(const std::string &tag) noexcept {
    if (int32_t status = utils::check_tag(tag); status != defines::OK)
      return status;
    int32_t status = defines::CONTAINER_RECORD_NOT_FOUND;
    if (auto search = this->records_.find(tag); search != this->records_.end())
      status = defines::OK;
    return status;
  }
  inline int32_t has_record(const char *const tag) noexcept {
    if (int32_t status = utils::check_ptr(tag); status != defines::OK)
      return status;
    return this->has_record(std::string(tag));
  }

  inline int32_t has_records(const std::vector<std::string> &tags) noexcept {
    for (const auto &tag : tags) {
      if (int32_t status = has_record(tag); status != defines::OK)
        return status;
    }
    return defines::OK;
  }

  inline int32_t remove_record(const std::string &tag) noexcept {
    if (int32_t status = utils::check_tag(tag); status != defines::OK)
      return status;
    if (int32_t status = this->has_record(tag); status != defines::OK)
      return status;
    delete this->records_[tag];
    this->records_.erase(tag);
    return defines::OK;
  }
  inline int32_t remove_record(const char *const tag) noexcept {
    if (int32_t status = utils::check_ptr(tag); status != defines::OK)
      return status;
    return this->remove_record(std::string(tag));
  }

  inline void remove_records(const std::vector<std::string> &tags) noexcept {
    for (const auto &tag : tags)
      remove_record(tag);
  }

  inline Record *get_record(const std::string &tag) noexcept {
    if (utils::check_tag(tag) != defines::OK)
      return nullptr;
    if (this->has_record(tag) != defines::OK)
      return nullptr;
    return this->records_[tag];
  }
  inline Record *get_record(const char *const tag) noexcept {
    if (utils::check_ptr(tag) != defines::OK)
      return nullptr;
    return this->get_record(std::string(tag));
  }

  int32_t save(const std::filesystem::path &filename) noexcept;
  inline int32_t save(const std::string &filename) noexcept {
    return this->save(std::filesystem::path(filename));
  }
  inline int32_t save(const char *const filename_ptr) noexcept {
    if (utils::check_ptr(filename_ptr) != defines::OK)
      return defines::NULLPTR;
    return this->save(std::string(filename_ptr));
  }

  int32_t load(const std::filesystem::path &filename) noexcept;
  inline int32_t load(const std::string &filename) noexcept {
    return this->load(std::filesystem::path(filename));
  }
  inline int32_t load(const char *const filename_ptr) noexcept {
    if (int32_t status = utils::check_ptr(filename_ptr); status != defines::OK)
      return status;
    return this->load(std::string(filename_ptr));
  }

  void dump(const int32_t level) const noexcept;
};

} // namespace tagarray
