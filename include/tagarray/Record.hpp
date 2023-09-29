#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "tagarray/defines.hpp"
#include "tagarray/info.h"

namespace tagarray {

using Dimensions = std::array<int64_t, defines::MAX_DIMENSIONS_LENGTH>;

class Record {
private:
  const int32_t type_id_;
  const int32_t itemsize_; // size of elements in bytes
  int64_t count_;          // number of elements
  int64_t n_dimensions_;
  Dimensions dimensions_;
  uint8_t *data_;
  std::string comment_;

public:
  Record(const int32_t type_id, const int32_t n_dimensions,
         const uint8_t *const &data, const int64_t count,
         const Dimensions &dimensions, const std::string &comment) noexcept;

  ~Record() noexcept;
  Record &operator=(const Record &) = delete;
  Record(const Record &) = delete;

  inline int32_t get_type_id() const noexcept { return this->type_id_; }

  inline int64_t get_n_dimensions() const noexcept {
    return this->n_dimensions_;
  }

  inline const Dimensions &get_dimensions() const noexcept {
    return this->dimensions_;
  }

  inline const std::string &get_comment() const noexcept {
    return this->comment_;
  }

  inline void
  update_comment(const std::string &comment = std::string()) noexcept {
    this->comment_ = comment;
  }

  inline int32_t get_itemsize() const noexcept { return this->itemsize_; }

  inline int64_t get_count() const noexcept { return this->count_; }

  inline uint8_t *get_data() const noexcept { return this->data_; }

  inline void set_data(const uint8_t *const &data = nullptr,
                       const int64_t count = 1,
                       const Dimensions &dimensions = {1}) noexcept {
    this->dimensions_ = dimensions;
    if (this->data_ != nullptr)
      delete[] this->data_;
    this->count_ = count;
    this->data_ = new (std::align_val_t(64), std::nothrow) uint8_t[this->count_ * this->itemsize_];
    if (this->data_ == nullptr) {
      return;
    }
    if (data == nullptr) {
      std::fill(this->data_, this->data_ + this->count_ * this->itemsize_, 0);
    } else {
      std::copy(data, data + this->count_ * this->itemsize_, this->data_);
    }
  }

  inline void free_data() noexcept {
    if (this->data_ != nullptr)
      delete[] this->data_;
    this->data_ = nullptr;
  }

  inline const Dimensions &get_shape() const noexcept {
    return this->dimensions_;
  }

  inline int32_t set_shape(const std::vector<int64_t> &dimensions) noexcept {
    if (dimensions.size() > defines::MAX_DIMENSIONS_LENGTH)
      return defines::DATA_TOO_MANY_DIMENSIONS;
    Dimensions dims{1};
    for (int32_t i = 0; i < static_cast<int32_t>(dimensions.size()); i++)
      dims[i] = dimensions[i];
    return set_shape(dims);
  }

  inline int32_t set_shape(const Dimensions &dimensions = {1}) noexcept {
    int64_t new_size = 1;
    for (const auto &dim : dimensions)
      new_size *= dim;
    if (new_size != this->count_)
      return defines::DATA_INSUFFICIENT_SIZE;
    this->dimensions_ = dimensions;
    return defines::OK;
  }

  inline bool is_allocated() const noexcept { return this->data_ != nullptr; }

  inline RecordInfo get_info() const noexcept {
    RecordInfo recordInfo = {
        this->type_id_, this->itemsize_, this->count_, this->n_dimensions_, {0},
        this->data_};
    std::copy(std::begin(this->dimensions_), std::end(this->dimensions_),
              std::begin(recordInfo.dimensions));
    return recordInfo;
  }

  void dump(const int32_t level) const noexcept;
};

} // namespace tagarray
