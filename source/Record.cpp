#include <algorithm>
#include <iostream>

#include "tagarray/Record.hpp"

namespace tagarray {

Record::Record(const uint32_t type_id,
               const uint32_t n_dimensions, const uint8_t *data,
               const uint64_t data_length,
               const uint64_t dimensions_ptr[TAGARRAY_DIMENSIONS_LENGTH],
               const int64_t options_ptr[TAGARRAY_OPTIONS_LENGTH],
               const char *const comment_ptr) noexcept :
                 type_id(type_id), n_dimensions(n_dimensions) {
  this->data_length = data_length;
  this->data_size = sizeof(uint8_t) * this->data_length;
  this->data = new (std::nothrow) uint8_t[this->data_size];
  if (this->data == nullptr) {
    this->status = TAGARRAY_MEMORY_ALLOCATION_ERROR;
    return;
  }
  if (data == nullptr) {
    std::fill(this->data, this->data + this->data_size, 0);
  } else {
    std::copy(data, data + this->data_size, this->data);
  }
  for (size_t i = 0; i < this->dimensions.size(); i++) {
    this->dimensions[i] = dimensions_ptr[i];
  }
  for (size_t i = 0; i < this->options.size(); i++) {
    this->options[i] = options_ptr[i];
  }
  if (comment_ptr != nullptr)
    this->comment = std::string(comment_ptr);
  this->status = TAGARRAY_OK;
}

Record::Record(const uint32_t type_id,
               const uint32_t n_dimensions, const uint8_t *&data,
               const uint64_t data_length,
               const std::array<uint64_t, TAGARRAY_DIMENSIONS_LENGTH> &dimensions,
               const std::array<int64_t, TAGARRAY_OPTIONS_LENGTH> &options,
               const std::string &comment) noexcept
    : type_id(type_id), n_dimensions(n_dimensions), data_length(data_length),
      dimensions(dimensions), options(options), comment(comment) {
  this->data_size = sizeof(uint8_t) * this->data_length;
  this->data = new (std::nothrow) uint8_t[this->data_size];
  if (this->data == nullptr) {
    this->status = TAGARRAY_MEMORY_ALLOCATION_ERROR;
    return;
  }
  if (data == nullptr) {
    std::fill(this->data, this->data + this->data_size, 0);
  } else {
    std::copy(data, data + this->data_size, this->data);
  }
  this->status = TAGARRAY_OK;
}

Record::~Record() noexcept { if(this->data != nullptr) delete[] this->data; }

void Record::set_data(const int8_t *&data, uint64_t data_length) noexcept {
  if (this->data != nullptr)
    delete[] this->data;
  this->data_size = sizeof(uint8_t) * this->data_length;
  this->data = new (std::nothrow) uint8_t[this->data_size];
  if (this->data == nullptr) {
    this->status = TAGARRAY_MEMORY_ALLOCATION_ERROR;
    return;
  }
  if (data == nullptr) {
    std::fill(this->data, this->data + this->data_size, 0);
  } else {
    std::copy(data, data + this->data_size, this->data);
  }
  this->status = TAGARRAY_OK;
}

void Record::dump(const int32_t level) const noexcept {
  std::cout << "    type id: " << type_id << std::endl;
  std::cout << "    Dimensions: " << n_dimensions << std::endl;
  std::cout << "    Data length: " << data_length << std::endl;
  std::cout << "    Comment len = " << this->comment.length();
  std::cout << std::endl << "    dims = ";
  for (const auto& d: this->dimensions)
    std::cout << d << " ";
  std::cout << std::endl << "    opts = ";
  for (const auto& o: this->options)
    std::cout << o << " ";
  std::cout << std::endl;
  if (level > 0) {
    std::cout << "    len  = " << level << std::endl;
    std::cout << "    data = ";
    switch(type_id) {
      case 4:
        for (size_t i = 0; i < (data_length / sizeof(int32_t)) && i < level; i++) {
          std::cout << (int32_t)(reinterpret_cast<int32_t*>(data))[i] << " ";
        }
        break;
      default:
        for (size_t i = 0; i < (data_length / sizeof(int8_t)) && i < level; i++) {
          std::cout << std::hex << (int)reinterpret_cast<uint8_t*>(this->data)[i] << " ";
        }
        break;
    }
    std::cout << std::dec << std::endl;
  }
}

} // namespace tagarray
