#include <algorithm>
#include <iostream>

#include "tagarray/Record.hpp"

namespace tagarray {

Record::Record(const uint32_t type_id,
               const uint32_t n_dimensions, const uint8_t *data,
               const uint64_t data_length,
               const uint64_t dimensions_ptr[TA_DIMENSIONS_LENGTH],
               const int64_t options_ptr[TA_OPTIONS_LENGTH],
               const char *const comment_ptr) noexcept :
                 type_id(type_id), n_dimensions(n_dimensions) {
  this->data_length = data_length;
  this->data_size = sizeof(uint8_t) * this->data_length;
  this->data = new (std::nothrow) uint8_t[this->data_size];
  if (this->data == nullptr) {
    this->status = TA_MEMORY_ALLOCATION_ERROR;
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
  this->status = TA_OK;
}

Record::Record(const uint32_t type_id,
               const uint32_t n_dimensions, const uint8_t *&data,
               const uint64_t data_length,
               const std::array<uint64_t, TA_DIMENSIONS_LENGTH> &dimensions,
               const std::array<int64_t, TA_OPTIONS_LENGTH> &options,
               const std::string &comment) noexcept
    : type_id(type_id), n_dimensions(n_dimensions), data_length(data_length),
      dimensions(dimensions), options(options), comment(comment) {
  this->data_size = sizeof(uint8_t) * this->data_length;
  this->data = new (std::nothrow) uint8_t[this->data_size];
  if (this->data == nullptr) {
    this->status = TA_MEMORY_ALLOCATION_ERROR;
    return;
  }
  if (data == nullptr) {
    std::fill(this->data, this->data + this->data_size, 0);
  } else {
    std::copy(data, data + this->data_size, this->data);
  }
  this->status = TA_OK;
}

Record::~Record() noexcept { if(this->data != nullptr) delete[] this->data; }

void Record::set_data(const int8_t *&data, const uint64_t data_length) noexcept {
  if (this->data != nullptr)
    delete[] this->data;
  this->data_length = data_length;
  this->data_size = sizeof(uint8_t) * this->data_length;
  this->data = new (std::nothrow) uint8_t[this->data_size];
  if (this->data == nullptr) {
    this->status = TA_MEMORY_ALLOCATION_ERROR;
    return;
  }
  if (data == nullptr) {
    std::fill(this->data, this->data + this->data_size, 0);
  } else {
    std::copy(data, data + this->data_size, this->data);
  }
  this->status = TA_OK;
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
      case TA_TYPE_CHAR8:
        for (size_t i = 0; i < (data_length / sizeof(char)) && i < (size_t)level; i++) {
          std::cout << (char)(reinterpret_cast<char*>(data))[i] << " ";
        }
        break;
      case TA_TYPE_INT8:
        for (size_t i = 0; i < (data_length / sizeof(int8_t)) && i < (size_t)level; i++) {
          std::cout << (int8_t)(reinterpret_cast<int8_t*>(data))[i] << " ";
        }
        break;
      case TA_TYPE_INT16:
        for (size_t i = 0; i < (data_length / sizeof(int16_t)) && i < (size_t)level; i++) {
          std::cout << (int16_t)(reinterpret_cast<int16_t*>(data))[i] << " ";
        }
        break;
      case TA_TYPE_INT32:
        for (size_t i = 0; i < (data_length / sizeof(int32_t)) && i < (size_t)level; i++) {
          std::cout << (int32_t)(reinterpret_cast<int32_t*>(data))[i] << " ";
        }
        break;
      case TA_TYPE_INT64:
        for (size_t i = 0; i < (data_length / sizeof(int64_t)) && i < (size_t)level; i++) {
          std::cout << (int64_t)(reinterpret_cast<int64_t*>(data))[i] << " ";
        }
        break;
      case TA_TYPE_UINT8:
        for (size_t i = 0; i < (data_length / sizeof(uint8_t)) && i < (size_t)level; i++) {
          std::cout << (uint8_t)(reinterpret_cast<uint8_t*>(data))[i] << " ";
        }
        break;
      case TA_TYPE_UINT16:
        for (size_t i = 0; i < (data_length / sizeof(uint16_t)) && i < (size_t)level; i++) {
          std::cout << (uint16_t)(reinterpret_cast<uint16_t*>(data))[i] << " ";
        }
        break;
      case TA_TYPE_UINT32:
        for (size_t i = 0; i < (data_length / sizeof(uint32_t)) && i < (size_t)level; i++) {
          std::cout << (uint32_t)(reinterpret_cast<uint32_t*>(data))[i] << " ";
        }
        break;
      case TA_TYPE_UINT64:
        for (size_t i = 0; i < (data_length / sizeof(uint64_t)) && i < (size_t)level; i++) {
          std::cout << (uint64_t)(reinterpret_cast<uint64_t*>(data))[i] << " ";
        }
        break;
      case TA_TYPE_REAL32:
        for (size_t i = 0; i < (data_length / sizeof(float)) && i < (size_t)level; i++) {
          std::cout << (float)(reinterpret_cast<float*>(data))[i] << " ";
        }
        break;
      case TA_TYPE_REAL64:
        for (size_t i = 0; i < (data_length / sizeof(double)) && i < (size_t)level; i++) {
          std::cout << (double)(reinterpret_cast<double*>(data))[i] << " ";
        }
        break;
      case TA_TYPE_UNKNOWN:
      default:
        for (size_t i = 0; i < (data_length / sizeof(int8_t)) && i < (size_t)level; i++) {
          std::cout << std::hex << (int)reinterpret_cast<uint8_t*>(this->data)[i] << " ";
        }
        std::cout << std::dec;
        break;
    }
    std::cout << std::endl;
  }
}

} // namespace tagarray
