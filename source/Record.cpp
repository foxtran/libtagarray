#include <iostream>

#include "tagarray/Record.hpp"
#include "tagarray/Utils.hpp"

namespace tagarray {

Record::Record(const int32_t type_id, const int32_t n_dimensions,
               const uint8_t *const &data, const int64_t count,
               const std::vector<int64_t> &dimensions,
               const std::string &comment) noexcept
    : type_id_(type_id), itemsize_(utils::get_storage_size(type_id)),
      count_(count), n_dimensions_(n_dimensions), dimensions_(dimensions),
      comment_(comment) {
  this->data_ = new (std::align_val_t(64), std::nothrow)
      uint8_t[this->itemsize_ * this->count_];
  if (this->data_ == nullptr) {
    return;
  }
  if (data == nullptr) {
    std::fill(this->data_, this->data_ + this->itemsize_ * this->count_, 0);
  } else {
    std::copy(data, data + this->itemsize_ * this->count_, this->data_);
  }
}

Record::~Record() noexcept {
  if (this->data_ != nullptr)
    delete[] this->data_;
}

void Record::dump(const int32_t level) const noexcept {
  std::cout << "    type id: " << this->type_id_ << std::endl;
  std::cout << "    Dimensions: " << this->n_dimensions_ << std::endl;
  std::cout << "    Data length: " << this->itemsize_ * this->count_
            << std::endl;
  std::cout << "    Comment len = " << this->comment_.length();
  std::cout << std::endl << "    dims = ";
  for (const auto &d : this->dimensions_)
    std::cout << d << " ";
  std::cout << std::endl;
  if (level > 0) {
    std::cout << "    len  = " << level << std::endl;
    std::cout << "    data = ";
    switch (this->type_id_) {
    case defines::TYPE_CHAR8:
      for (size_t i = 0;
           i < static_cast<size_t>(this->count_) && i < (size_t)level; i++) {
        std::cout << (char)(reinterpret_cast<char *>(this->data_))[i] << " ";
      }
      break;
    case defines::TYPE_INT8:
      for (size_t i = 0;
           i < static_cast<size_t>(this->count_) && i < (size_t)level; i++) {
        std::cout << (int8_t)(reinterpret_cast<int8_t *>(this->data_))[i]
                  << " ";
      }
      break;
    case defines::TYPE_INT16:
      for (size_t i = 0;
           i < static_cast<size_t>(this->count_) && i < (size_t)level; i++) {
        std::cout << (int16_t)(reinterpret_cast<int16_t *>(this->data_))[i]
                  << " ";
      }
      break;
    case defines::TYPE_INT32:
      for (size_t i = 0;
           i < static_cast<size_t>(this->count_) && i < (size_t)level; i++) {
        std::cout << (int32_t)(reinterpret_cast<int32_t *>(this->data_))[i]
                  << " ";
      }
      break;
    case defines::TYPE_INT64:
      for (size_t i = 0;
           i < static_cast<size_t>(this->count_) && i < (size_t)level; i++) {
        std::cout << (int64_t)(reinterpret_cast<int64_t *>(this->data_))[i]
                  << " ";
      }
      break;
    case defines::TYPE_UINT8:
      for (size_t i = 0;
           i < static_cast<size_t>(this->count_) && i < (size_t)level; i++) {
        std::cout << (uint8_t)(reinterpret_cast<uint8_t *>(this->data_))[i]
                  << " ";
      }
      break;
    case defines::TYPE_UINT16:
      for (size_t i = 0;
           i < static_cast<size_t>(this->count_) && i < (size_t)level; i++) {
        std::cout << (uint16_t)(reinterpret_cast<uint16_t *>(this->data_))[i]
                  << " ";
      }
      break;
    case defines::TYPE_UINT32:
      for (size_t i = 0;
           i < static_cast<size_t>(this->count_) && i < (size_t)level; i++) {
        std::cout << (uint32_t)(reinterpret_cast<uint32_t *>(this->data_))[i]
                  << " ";
      }
      break;
    case defines::TYPE_UINT64:
      for (size_t i = 0;
           i < static_cast<size_t>(this->count_) && i < (size_t)level; i++) {
        std::cout << (uint64_t)(reinterpret_cast<uint64_t *>(this->data_))[i]
                  << " ";
      }
      break;
    case defines::TYPE_REAL32:
      for (size_t i = 0;
           i < static_cast<size_t>(this->count_) && i < (size_t)level; i++) {
        std::cout << (float)(reinterpret_cast<float *>(this->data_))[i] << " ";
      }
      break;
    case defines::TYPE_REAL64:
      for (size_t i = 0;
           i < static_cast<size_t>(this->count_) && i < (size_t)level; i++) {
        std::cout << (double)(reinterpret_cast<double *>(this->data_))[i]
                  << " ";
      }
      break;
    case defines::TYPE_UNKNOWN:
    default:
      for (size_t i = 0;
           i < static_cast<size_t>(this->count_) && i < (size_t)level; i++) {
        std::cout << std::hex
                  << (int)reinterpret_cast<uint8_t *>(this->data_)[i] << " ";
      }
      std::cout << std::dec;
      break;
    }
    std::cout << std::endl;
  }
}

} // namespace tagarray
