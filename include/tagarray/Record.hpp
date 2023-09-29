#pragma once

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <string>
#include <vector>

#include "tagarray/defines.hpp"
#include "tagarray/info.h"

namespace tagarray {

class Record {
private:
  const int32_t type_id_;
  const int32_t itemsize_; // size of one element in bytes
  int64_t count_;          // number of elements
  int64_t n_dimensions_;
  std::vector<int64_t> dimensions_;
  uint8_t *data_;
  std::string description_;

public:
  Record(const int32_t type_id, const int32_t n_dimensions,
         const uint8_t *const &data, const int64_t count,
         const std::vector<int64_t> &dimensions,
         const std::string &description) noexcept;

  ~Record() noexcept;
  Record &operator=(const Record &) = delete;
  Record(const Record &) = delete;

  inline int32_t type_id() const noexcept { return this->type_id_; }

  inline int64_t ndim() const noexcept { return this->n_dimensions_; }

  inline const std::string &description() const noexcept {
    return this->description_;
  }

  inline void set_description(const std::string &str = std::string()) noexcept {
    this->description_ = str;
  }

  inline int32_t itemsize() const noexcept { return this->itemsize_; }

  inline int64_t count() const noexcept { return this->count_; }

  inline int64_t byte_count() const noexcept {
    return this->count_ * this->itemsize_;
  }

  inline uint8_t *data() const noexcept { return this->data_; }

  inline void set_data(const uint8_t *const &data = nullptr,
                       const std::vector<int64_t> &dimensions =
                           std::vector<int64_t>(1, 1)) noexcept {
    this->dimensions_ = dimensions;
    if (this->data_ != nullptr)
      delete[] this->data_;
    this->count_ = std::accumulate(dimensions.begin(), dimensions.end(), 1,
                                   std::multiplies<int64_t>());
    this->data_ =
        new (std::align_val_t(64), std::nothrow) uint8_t[this->byte_count()];
    if (this->data_ == nullptr) {
      return;
    }
    if (data == nullptr) {
      std::fill(this->data_, this->data_ + this->byte_count(), 0);
    } else {
      std::copy(data, data + this->byte_count(), this->data_);
    }
  }

  inline void free_data() noexcept {
    if (this->data_ != nullptr)
      delete[] this->data_;
    this->data_ = nullptr;
  }

  inline const std::vector<int64_t> &shape() const noexcept {
    return this->dimensions_;
  }

  inline int32_t set_shape(const std::vector<int64_t> &dimensions) noexcept {
    if (dimensions.size() > defines::MAX_DIMENSIONS_LENGTH)
      return defines::DATA_TOO_MANY_DIMENSIONS;
    if (std::accumulate(dimensions.begin(), dimensions.end(), 1,
                        std::multiplies<int64_t>()) != this->count_)
      return defines::DATA_INSUFFICIENT_SIZE;
    this->dimensions_ = dimensions;
    return defines::OK;
  }

  inline bool is_allocated() const noexcept { return this->data_ != nullptr; }

  inline RecordInfo info() const noexcept {
    RecordInfo recordInfo = {
        this->type_id_, this->itemsize_, this->count_, this->n_dimensions_, {1},
        this->data_};
    std::copy(std::begin(this->dimensions_), std::end(this->dimensions_),
              std::begin(recordInfo.dimensions));
    return recordInfo;
  }

  void dump(const int32_t level) const noexcept;
};

} // namespace tagarray
