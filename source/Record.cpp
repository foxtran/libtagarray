#include <iostream>

#include "tagarray/Record.hpp"

namespace tagarray {

Record::Record(const uint32_t type_id, const uint32_t n_dimensions,
               const uint8_t *data, const uint64_t data_length,
               const uint64_t dimensions_ptr[TA_DIMENSIONS_LENGTH],
               const int64_t options_ptr[TA_OPTIONS_LENGTH],
               const char *const comment_ptr) noexcept
    : _type_id(type_id), _n_dimensions(n_dimensions) {
  this->_data_length = data_length;
  this->_data_size = sizeof(uint8_t) * this->_data_length;
  this->_data = new (std::align_val_t(64), std::nothrow) uint8_t[this->_data_size];
  if (this->_data == nullptr) {
    this->_status = TA_MEMORY_ALLOCATION_ERROR;
    return;
  }
  if (data == nullptr) {
    std::fill(this->_data, this->_data + this->_data_size, 0);
  } else {
    std::copy(data, data + this->_data_size, this->_data);
  }
  std::copy(dimensions_ptr, dimensions_ptr + this->_dimensions.size(),
            this->_dimensions.begin());
  std::copy(options_ptr, options_ptr + this->_options.size(),
            this->_options.begin());
  if (comment_ptr != nullptr)
    this->_comment = std::string(comment_ptr);
  this->_status = TA_OK;
}

Record::Record(const uint32_t type_id, const uint32_t n_dimensions,
               const uint8_t *&data, const uint64_t data_length,
               const std::array<uint64_t, TA_DIMENSIONS_LENGTH> &dimensions,
               const std::array<int64_t, TA_OPTIONS_LENGTH> &options,
               const std::string &comment) noexcept
    : _type_id(type_id), _n_dimensions(n_dimensions), _data_length(data_length),
      _dimensions(dimensions), _options(options), _comment(comment) {
  this->_data_size = sizeof(uint8_t) * this->_data_length;
  this->_data = new (std::align_val_t(64), std::nothrow) uint8_t[this->_data_size];
  if (this->_data == nullptr) {
    this->_status = TA_MEMORY_ALLOCATION_ERROR;
    return;
  }
  if (data == nullptr) {
    std::fill(this->_data, this->_data + this->_data_size, 0);
  } else {
    std::copy(data, data + this->_data_size, this->_data);
  }
  this->_status = TA_OK;
}

Record::~Record() noexcept {
  if (this->_data != nullptr)
    delete[] this->_data;
}

void Record::set_data(const int8_t *&data,
                      const uint64_t data_length) noexcept {
  if (this->_data != nullptr)
    delete[] this->_data;
  this->_data_length = data_length;
  this->_data_size = sizeof(uint8_t) * this->_data_length;
  this->_data = new (std::nothrow) uint8_t[this->_data_size];
  if (this->_data == nullptr) {
    this->_status = TA_MEMORY_ALLOCATION_ERROR;
    return;
  }
  if (data == nullptr) {
    std::fill(this->_data, this->_data + this->_data_size, 0);
  } else {
    std::copy(data, data + this->_data_size, this->_data);
  }
  this->_status = TA_OK;
}

void Record::dump(const int32_t level) const noexcept {
  std::cout << "    type id: " << this->_type_id << std::endl;
  std::cout << "    Dimensions: " << this->_n_dimensions << std::endl;
  std::cout << "    Data length: " << this->_data_length << std::endl;
  std::cout << "    Comment len = " << this->_comment.length();
  std::cout << std::endl << "    dims = ";
  for (const auto &d : this->_dimensions)
    std::cout << d << " ";
  std::cout << std::endl << "    opts = ";
  for (const auto &o : this->_options)
    std::cout << o << " ";
  std::cout << std::endl;
  if (level > 0) {
    std::cout << "    len  = " << level << std::endl;
    std::cout << "    data = ";
    switch (this->_type_id) {
    case TA_TYPE_CHAR8:
      for (size_t i = 0;
           i < (this->_data_length / sizeof(char)) && i < (size_t)level; i++) {
        std::cout << (char)(reinterpret_cast<char *>(this->_data))[i] << " ";
      }
      break;
    case TA_TYPE_INT8:
      for (size_t i = 0;
           i < (this->_data_length / sizeof(int8_t)) && i < (size_t)level;
           i++) {
        std::cout << (int8_t)(reinterpret_cast<int8_t *>(this->_data))[i]
                  << " ";
      }
      break;
    case TA_TYPE_INT16:
      for (size_t i = 0;
           i < (this->_data_length / sizeof(int16_t)) && i < (size_t)level;
           i++) {
        std::cout << (int16_t)(reinterpret_cast<int16_t *>(this->_data))[i]
                  << " ";
      }
      break;
    case TA_TYPE_INT32:
      for (size_t i = 0;
           i < (this->_data_length / sizeof(int32_t)) && i < (size_t)level;
           i++) {
        std::cout << (int32_t)(reinterpret_cast<int32_t *>(this->_data))[i]
                  << " ";
      }
      break;
    case TA_TYPE_INT64:
      for (size_t i = 0;
           i < (this->_data_length / sizeof(int64_t)) && i < (size_t)level;
           i++) {
        std::cout << (int64_t)(reinterpret_cast<int64_t *>(this->_data))[i]
                  << " ";
      }
      break;
    case TA_TYPE_UINT8:
      for (size_t i = 0;
           i < (this->_data_length / sizeof(uint8_t)) && i < (size_t)level;
           i++) {
        std::cout << (uint8_t)(reinterpret_cast<uint8_t *>(this->_data))[i]
                  << " ";
      }
      break;
    case TA_TYPE_UINT16:
      for (size_t i = 0;
           i < (this->_data_length / sizeof(uint16_t)) && i < (size_t)level;
           i++) {
        std::cout << (uint16_t)(reinterpret_cast<uint16_t *>(this->_data))[i]
                  << " ";
      }
      break;
    case TA_TYPE_UINT32:
      for (size_t i = 0;
           i < (this->_data_length / sizeof(uint32_t)) && i < (size_t)level;
           i++) {
        std::cout << (uint32_t)(reinterpret_cast<uint32_t *>(this->_data))[i]
                  << " ";
      }
      break;
    case TA_TYPE_UINT64:
      for (size_t i = 0;
           i < (this->_data_length / sizeof(uint64_t)) && i < (size_t)level;
           i++) {
        std::cout << (uint64_t)(reinterpret_cast<uint64_t *>(this->_data))[i]
                  << " ";
      }
      break;
    case TA_TYPE_REAL32:
      for (size_t i = 0;
           i < (this->_data_length / sizeof(float)) && i < (size_t)level; i++) {
        std::cout << (float)(reinterpret_cast<float *>(this->_data))[i] << " ";
      }
      break;
    case TA_TYPE_REAL64:
      for (size_t i = 0;
           i < (this->_data_length / sizeof(double)) && i < (size_t)level;
           i++) {
        std::cout << (double)(reinterpret_cast<double *>(this->_data))[i]
                  << " ";
      }
      break;
    case TA_TYPE_UNKNOWN:
    default:
      for (size_t i = 0;
           i < (this->_data_length / sizeof(int8_t)) && i < (size_t)level;
           i++) {
        std::cout << std::hex
                  << (int)reinterpret_cast<uint8_t *>(this->_data)[i] << " ";
      }
      std::cout << std::dec;
      break;
    }
    std::cout << std::endl;
  }
}

} // namespace tagarray
