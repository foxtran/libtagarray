#pragma once

#include "tagarray/filesystem.hpp" // #include <filesystem>
#include <cassert>
#include <cstdint>
#include <memory>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

#include "tagarray/RecordInfo.h"
#include "tagarray/Utils.hpp"
#include "tagarray/defines.hpp"

namespace tagarray {

class Container {
private:
#include "tagarray/private/HRecord.hpp"
  std::string description_;
  std::unordered_map<std::string, std::shared_ptr<HRecord>> records_;

public:
  inline Container(const std::string &description = std::string("")) noexcept
      : description_(description){};
  ~Container() noexcept { this->records_.clear(); }
  Container &operator=(const Container &) = delete;

  inline const std::string &description() const noexcept {
    return this->description_;
  }

  inline std::string &description() noexcept { return this->description_; }

  inline void description(const std::string &description) noexcept {
    this->description_ = description;
  }

  inline int32_t
  create(const std::string &tag, const int32_t type_id,
         std::vector<int64_t> dimensions, const uint8_t *const data = nullptr,
         const std::string &description = std::string("")) noexcept {
    if (auto res = utils::check_tag(tag); res != defines::OK)
      return res;
    if (this->contains(tag))
      return defines::CONTAINER_RECORD_EXISTS;
    this->records_.insert(std::make_pair(
        tag, new HRecord(type_id, dimensions, data, description)));
    return defines::OK;
  }

  inline std::shared_ptr<HRecord> get(const std::string &tag) noexcept {
    if (this->contains(tag))
      return this->records_[tag];
    return std::make_shared<HRecord>(HRecord::invalid());
  }

  inline std::shared_ptr<HRecord> &operator[](const std::string &tag) noexcept {
    if (!this->contains(tag))
      this->records_[tag] = std::make_shared<HRecord>(HRecord::invalid());
    return this->records_[tag];
  }

  inline bool contains(const std::string &tag) const noexcept {
    if (auto search = this->records_.find(tag); search != this->records_.end())
      return true;
    return false;
  }
  inline bool contains(const std::vector<std::string> &tags) const noexcept {
    for (const auto &tag : tags)
      if (!this->contains(tag))
        return false;
    return true;
  }

  inline std::vector<std::string> keys() const noexcept {
    std::vector<std::string> keys;
    for (const auto &item : this->records_)
      keys.push_back(item.first);
    return keys;
  }

  inline void erase(const std::string &tag) noexcept {
    this->records_[tag].reset();
    this->records_.erase(tag);
  }

  inline void erase(const std::vector<std::string> &tags) noexcept {
    for (const auto &tag : tags)
      this->erase(tag);
  }

  inline void clear() noexcept {
    for (auto &item : this->records_)
      item.second.reset();
    this->records_.clear();
  }

  inline void append(const Container &other) noexcept {
    for (const auto &item : other.records_)
      this->records_.insert(item);
  }

  // TODO: fold expression
  inline static Container
  merge(Container &c1, Container &c2,
        std::string description = std::string("")) noexcept {
    Container c = Container(description);
    c.append(c1);
    c.append(c2);
    return c;
  }

  inline static Container
  merge(const std::vector<Container> &cvec,
        std::string description = std::string("")) noexcept {
    Container c = Container(description);
    for (const auto &cont : cvec)
      c.append(cont);
    return c;
  }

  inline Container copy() const noexcept {
    Container copy = Container(this->description());
    for (const auto &item : this->records_) {
      copy.records_.insert(item);
    }
    return copy;
  }

  inline Container deepcopy() const noexcept {
    Container dcopy = Container(this->description());
    for (const auto &item : this->records_) {
      dcopy.records_.insert(
          std::make_pair(item.first, item.second->deepcopy()));
    }
    return dcopy;
  }

  inline Container copy(const std::vector<std::string> &tags) noexcept {
    Container select = Container(this->description());
    for (const auto &tag : tags)
      if (this->contains(tag))
        select.records_.insert(std::make_pair(tag, this->records_[tag]));
    return select;
  }

  inline Container deepcopy(const std::vector<std::string> &tags) noexcept {
    Container dselect = Container(this->description());
    for (const auto &tag : tags)
      if (this->contains(tag))
        dselect.records_.insert(
            std::make_pair(tag, this->records_[tag]->deepcopy()));
    return dselect;
  }

  int32_t save(const std::filesystem::path &filename,
               const int32_t version = defines::CONTAINER_VERSION) noexcept;
  inline int32_t
  save(const std::string &filename,
       const int32_t version = defines::CONTAINER_VERSION) noexcept {
    return this->save(std::filesystem::path(filename), version);
  }

  static Container load(const std::filesystem::path &filename) noexcept;
  inline static Container load(const std::string &filename) noexcept {
    return Container::load(std::filesystem::path(filename));
  }

  void dump() const noexcept;
};

} // namespace tagarray
