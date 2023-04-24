#ifndef _TAGARRAY_RECORD_HPP
#define _TAGARRAY_RECORD_HPP

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "tagarray/defines.h"
#include "tagarray/info.h"

namespace tagarray {

class Record {
private:
  const int32_t type_id;
  const int32_t n_dimensions;
  int64_t data_length;
  uint64_t data_size;
  uint8_t *data;
  int32_t status;
  std::array<uint64_t, TAGARRAY_DIMENSIONS_LENGTH> dimensions;
  std::array<int64_t, TAGARRAY_OPTIONS_LENGTH> options;
  std::string comment;

public:
  Record(const uint32_t type_id,
         const uint32_t n_dimensions, const uint8_t *data,
         const uint64_t data_length,
         const uint64_t dimensions[TAGARRAY_DIMENSIONS_LENGTH],
         const int64_t options[TAGARRAY_OPTIONS_LENGTH],
         const char *const comment) noexcept;
  Record(const uint32_t type_id,
         const uint32_t n_dimensions, const uint8_t *&data,
         const uint64_t data_length,
         const std::array<uint64_t, TAGARRAY_DIMENSIONS_LENGTH> &dimensions,
         const std::array<int64_t, TAGARRAY_OPTIONS_LENGTH> &options,
         const std::string &comment) noexcept;

  ~Record() noexcept;

  inline uint32_t get_type_id() const noexcept { return this->type_id; }

  inline uint32_t get_n_dimensions() const noexcept { return this->n_dimensions; }

  inline int32_t get_status() const noexcept { return this->status; }

  inline const std::array<uint64_t, TAGARRAY_DIMENSIONS_LENGTH> &
  get_dimensions() const noexcept {
    return this->dimensions;
  }

  inline const std::array<int64_t, TAGARRAY_OPTIONS_LENGTH> &
  get_options() const noexcept {
    return this->options;
  }

  inline const std::string &get_comment() const noexcept { return this->comment; }

  inline void update_comment(const std::string &comment) noexcept {
    this->comment = comment;
    this->status = TAGARRAY_OK;
  }
  inline void update_comment(const char *const comment_ptr) noexcept {
    std::string comment = std::string();
    if (comment_ptr != nullptr)
      comment = std::string(comment_ptr);
    update_comment(comment);
  }

  inline uint64_t get_data_length() const noexcept { return this->data_length; }

  inline uint8_t *get_data() const noexcept { return this->data; }

  void set_data(const int8_t *&data, const uint64_t data_length) noexcept;
  inline void
  set_data(const int8_t *&data, const uint64_t data_length,
           const std::array<uint64_t, TAGARRAY_DIMENSIONS_LENGTH> &dimensions) noexcept {
    this->dimensions = dimensions;
    this->set_data(data, data_length);
  }
  inline void
  set_data(const int8_t *&data, const uint64_t data_length,
           const int64_t (&dimensions)[TAGARRAY_DIMENSIONS_LENGTH]) noexcept {
    for(size_t i = 0; i < this->dimensions.size(); i++) {
      this->dimensions[i] = dimensions[i];
    }
    set_data(data, data_length);
  }

  inline void free_data() noexcept {
    if (this->data != nullptr)
      delete[] this->data;
    this->data = nullptr;
    this->status = TAGARRAY_OK;
  }

  inline void
  set_shape(const std::array<uint64_t, TAGARRAY_DIMENSIONS_LENGTH> &dimensions) noexcept {
    this->dimensions = dimensions;
    this->status = TAGARRAY_OK;
  }
  inline void
  set_shape(const int64_t (&dimensions)[TAGARRAY_DIMENSIONS_LENGTH]) noexcept {
    for(size_t i = 0; i < this->dimensions.size(); i++) {
      this->dimensions[i] = dimensions[i];
    }
    this->status = TAGARRAY_OK;
  }

  inline void set_option(const uint32_t index, const int64_t option_value) noexcept {
    if (index >= TAGARRAY_OPTIONS_LENGTH) {
      this->status = TAGARRAY_RECORD_OPTION_DOES_NOT_EXIST;
      return;
    }
    this->options[index] = option_value;
    this->status = TAGARRAY_OK;
  }
  inline void
  set_options(const std::array<int64_t, TAGARRAY_OPTIONS_LENGTH> &options) noexcept {
    this->options = options;
    this->status = TAGARRAY_OK;
  }
  inline void
  set_options(const int64_t (&options)[TAGARRAY_OPTIONS_LENGTH]) noexcept {
    for(size_t i = 0; i < this->options.size(); i++) {
      this->options[i] = options[i];
    }
    this->status = TAGARRAY_OK;
  }

  inline RecordInfo get_info() const noexcept {
    RecordInfo recordInfo = {this->type_id, this->n_dimensions, this->data, this->data_length};
    for(size_t i = 0; i < this->dimensions.size(); i++) {
      recordInfo.dimensions[i] = this->dimensions[i];
    }
    return recordInfo;
  }

  void dump(const int32_t level) const noexcept;
};

} // namespace tagarray

#endif
