#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>

#include "tagarray/defines.h"
#include "tagarray/info.h"

namespace tagarray {

class Record {
private:
  const int32_t _type_id;
  const int32_t _n_dimensions;
  int64_t _data_length;
  int64_t _data_size;
  uint8_t *_data;
  int32_t _status;
  std::array<uint64_t, TA_DIMENSIONS_LENGTH> _dimensions;
  std::array<int64_t, TA_OPTIONS_LENGTH> _options;
  std::string _comment;

public:
  Record(const uint32_t type_id,
         const uint32_t n_dimensions, const uint8_t *data,
         const uint64_t data_length,
         const uint64_t dimensions[TA_DIMENSIONS_LENGTH],
         const int64_t options[TA_OPTIONS_LENGTH],
         const char *const comment) noexcept;
  Record(const uint32_t type_id,
         const uint32_t n_dimensions, const uint8_t *&data,
         const uint64_t data_length,
         const std::array<uint64_t, TA_DIMENSIONS_LENGTH> &dimensions,
         const std::array<int64_t, TA_OPTIONS_LENGTH> &options,
         const std::string &comment) noexcept;

  ~Record() noexcept;

  inline uint32_t get_type_id() const noexcept { return this->_type_id; }

  inline uint32_t get_n_dimensions() const noexcept { return this->_n_dimensions; }

  inline int32_t get_status() const noexcept { return this->_status; }

  inline const std::array<uint64_t, TA_DIMENSIONS_LENGTH> &
  get_dimensions() const noexcept {
    return this->_dimensions;
  }

  inline const std::array<int64_t, TA_OPTIONS_LENGTH> &
  get_options() const noexcept {
    return this->_options;
  }

  inline const std::string &get_comment() const noexcept { return this->_comment; }

  inline void update_comment(const std::string &comment) noexcept {
    this->_comment = comment;
    this->_status = TA_OK;
  }
  inline void update_comment(const char *const comment_ptr) noexcept {
    std::string comment = std::string();
    if (comment_ptr != nullptr)
      comment = std::string(comment_ptr);
    update_comment(comment);
  }

  inline uint64_t get_data_length() const noexcept { return this->_data_length; }

  inline uint8_t *get_data() const noexcept { return this->_data; }

  void set_data(const int8_t *&data, const uint64_t data_length) noexcept;
  inline void
  set_data(const int8_t *&data, const uint64_t data_length,
           const std::array<uint64_t, TA_DIMENSIONS_LENGTH> &dimensions) noexcept {
    this->_dimensions = dimensions;
    this->set_data(data, data_length);
  }
  inline void
  set_data(const int8_t *&data, const uint64_t data_length,
           const int64_t (&dimensions)[TA_DIMENSIONS_LENGTH]) noexcept {
    std::copy(dimensions, dimensions + this->_dimensions.size(), this->_dimensions.begin());
    set_data(data, data_length);
  }

  inline void free_data() noexcept {
    if (this->_data != nullptr)
      delete[] this->_data;
    this->_data = nullptr;
    this->_status = TA_OK;
  }

  inline void
  set_shape(const std::array<uint64_t, TA_DIMENSIONS_LENGTH> &dimensions) noexcept {
    this->_dimensions = dimensions;
    this->_status = TA_OK;
  }
  inline void
  set_shape(const int64_t (&dimensions)[TA_DIMENSIONS_LENGTH]) noexcept {
    std::copy(dimensions, dimensions + this->_dimensions.size(), this->_dimensions.begin());
    this->_status = TA_OK;
  }

  inline void set_option(const uint32_t index, const int64_t option_value) noexcept {
    if (index >= TA_OPTIONS_LENGTH) {
      this->_status = TA_RECORD_OPTION_DOES_NOT_EXIST;
      return;
    }
    this->_options[index] = option_value;
    this->_status = TA_OK;
  }
  inline void
  set_options(const std::array<int64_t, TA_OPTIONS_LENGTH> &options) noexcept {
    this->_options = options;
    this->_status = TA_OK;
  }
  inline void
  set_options(const int64_t (&options)[TA_OPTIONS_LENGTH]) noexcept {
    std::copy(options, options + this->_options.size(), this->_options.begin());
    this->_status = TA_OK;
  }

  inline RecordInfo get_info() const noexcept {
    RecordInfo recordInfo = {this->_type_id, this->_n_dimensions, this->_data, this->_data_length, {0}};
    std::copy(std::begin(this->_dimensions), std::end(this->_dimensions), std::begin(recordInfo.dimensions));
    return recordInfo;
  }

  void dump(const int32_t level) const noexcept;
};

} // namespace tagarray
