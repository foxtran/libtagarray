#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "tagarray/defines.h"
#include "tagarray/info.h"

namespace tagarray {

class Record {
private:
  const int32_t type_id_;
  const int32_t n_dimensions_;
  int64_t data_length_;
  int64_t data_size_;
  uint8_t *data_;
  std::array<int64_t, TA_DIMENSIONS_LENGTH> dimensions_;
  std::array<int64_t, TA_OPTIONS_LENGTH> options_;
  std::string comment_;

public:
  Record(const int32_t type_id, const int32_t n_dimensions,
         const uint8_t *data, const int64_t data_length,
         const int64_t dimensions[TA_DIMENSIONS_LENGTH],
         const int64_t options[TA_OPTIONS_LENGTH],
         const char *const comment) noexcept;
  Record(const int32_t type_id, const int32_t n_dimensions,
         const uint8_t *&data, const int64_t data_length,
         const std::array<int64_t, TA_DIMENSIONS_LENGTH> &dimensions,
         const std::array<int64_t, TA_OPTIONS_LENGTH> &options,
         const std::string &comment) noexcept;

  ~Record() noexcept;
  Record &operator=(const Record &) = delete;
  Record(const Record &) = delete;

  inline int32_t get_type_id() const noexcept { return this->type_id_; }

  inline int32_t get_n_dimensions() const noexcept {
    return this->n_dimensions_;
  }

  inline const std::array<int64_t, TA_DIMENSIONS_LENGTH> &
  get_dimensions() const noexcept {
    return this->dimensions_;
  }

  inline const std::array<int64_t, TA_OPTIONS_LENGTH> &
  get_options() const noexcept {
    return this->options_;
  }

  inline const std::string &get_comment() const noexcept {
    return this->comment_;
  }

  inline void update_comment(const std::string &comment) noexcept {
    this->comment_ = comment;
  }
  inline void update_comment(const char *const comment_ptr) noexcept {
    std::string comment = std::string();
    if (comment_ptr != nullptr)
      comment = std::string(comment_ptr);
    update_comment(comment);
  }

  inline int64_t get_data_length() const noexcept {
    return this->data_length_;
  }

  inline uint8_t *get_data() const noexcept { return this->data_; }

  void set_data(const int8_t *&data, const int64_t data_length) noexcept;
  inline void set_data(
      const int8_t *&data, const int64_t data_length,
      const std::array<int64_t, TA_DIMENSIONS_LENGTH> &dimensions) noexcept {
    this->dimensions_ = dimensions;
    this->set_data(data, data_length);
  }
  inline void
  set_data(const int8_t *&data, const int64_t data_length,
           const int64_t (&dimensions)[TA_DIMENSIONS_LENGTH]) noexcept {
    std::copy(dimensions, dimensions + this->dimensions_.size(),
              this->dimensions_.begin());
    set_data(data, data_length);
  }

  inline void free_data() noexcept {
    if (this->data_ != nullptr)
      delete[] this->data_;
    this->data_ = nullptr;
  }

  inline void set_shape(
      const std::array<int64_t, TA_DIMENSIONS_LENGTH> &dimensions) noexcept {
    this->dimensions_ = dimensions;
  }
  inline void
  set_shape(const int64_t (&dimensions)[TA_DIMENSIONS_LENGTH]) noexcept {
    std::copy(dimensions, dimensions + this->dimensions_.size(),
              this->dimensions_.begin());
  }

  inline void set_option(const int32_t index,
                         const int64_t option_value) noexcept {
    if (index >= TA_OPTIONS_LENGTH) {
      return;
    }
    this->options_[index] = option_value;
  }
  inline void
  set_options(const std::array<int64_t, TA_OPTIONS_LENGTH> &options) noexcept {
    this->options_ = options;
  }
  inline void
  set_options(const int64_t (&options)[TA_OPTIONS_LENGTH]) noexcept {
    std::copy(options, options + this->options_.size(), this->options_.begin());
  }

  inline RecordInfo get_info() const noexcept {
    RecordInfo recordInfo = {this->type_id_,
                             this->n_dimensions_,
                             this->data_,
                             this->data_length_,
                             {0}};
    std::copy(std::begin(this->dimensions_), std::end(this->dimensions_),
              std::begin(recordInfo.dimensions));
    return recordInfo;
  }

  void dump(const int32_t level) const noexcept;
};

} // namespace tagarray
