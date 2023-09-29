#include <iostream>

#include "tagarray/Record.hpp"
#include "tagarray/Utils.hpp"

namespace tagarray {

Record::Record(const int32_t type_id, const int32_t n_dimensions,
               const uint8_t *const &data, const int64_t count,
               const std::vector<int64_t> &dimensions,
               const std::string &description) noexcept
    : type_id_(type_id), itemsize_(utils::get_storage_size(type_id)),
      count_(count), n_dimensions_(n_dimensions), dimensions_(dimensions),
      description_(description) {
  std::shared_ptr<uint8_t[]> tmp(new (std::align_val_t(64), std::nothrow)
                                     uint8_t[this->byte_count()],
                                 std::default_delete<uint8_t[]>());
  this->data_ = tmp;
  if (this->data_.get() == nullptr) {
    return;
  }
  if (data == nullptr) {
    std::fill(this->data_.get(), this->data_.get() + this->byte_count(), 0);
  } else {
    std::copy(data, data + this->byte_count(), this->data_.get());
  }
}

Record::~Record() noexcept { this->free_data(); }

template <typename T> void dump_helper(const Record &rec, const int32_t level) {
  for (size_t i = 0; i < static_cast<size_t>(rec.count()) && i < (size_t)level;
       i++) {
    std::cout << (T)rec.raw_data<T *>()[i] << " ";
  }
}

void Record::dump(const int32_t level) const noexcept {
  std::cout << "    type id: " << this->type_id_ << std::endl;
  std::cout << "    Dimensions: " << this->n_dimensions_ << std::endl;
  std::cout << "    Data length: " << this->itemsize_ * this->count_
            << std::endl;
  std::cout << "    Comment len = " << this->description_.length();
  std::cout << std::endl << "    dims = ";
  for (const auto &d : this->dimensions_)
    std::cout << d << " ";
  std::cout << std::endl;
  if (level > 0) {
    std::cout << "    len  = " << level << std::endl;
    std::cout << "    data = ";
    switch (this->type_id_) {
    case defines::TYPE_CHAR8:
      dump_helper<char>(*this, level);
      break;
    case defines::TYPE_INT8:
      dump_helper<int8_t>(*this, level);
      break;
    case defines::TYPE_INT16:
      dump_helper<int16_t>(*this, level);
      break;
    case defines::TYPE_INT32:
      dump_helper<int32_t>(*this, level);
      break;
    case defines::TYPE_INT64:
      dump_helper<int64_t>(*this, level);
      break;
    case defines::TYPE_UINT8:
      dump_helper<uint8_t>(*this, level);
      break;
    case defines::TYPE_UINT16:
      dump_helper<uint16_t>(*this, level);
      break;
    case defines::TYPE_UINT32:
      dump_helper<uint32_t>(*this, level);
      break;
    case defines::TYPE_UINT64:
      dump_helper<uint64_t>(*this, level);
      break;
    case defines::TYPE_REAL32:
      dump_helper<float>(*this, level);
      break;
    case defines::TYPE_REAL64:
      dump_helper<double>(*this, level);
      break;
    case defines::TYPE_UNKNOWN:
    default:
      std::cout << std::hex;
      dump_helper<int8_t>(*this, level);
      std::cout << std::dec;
      break;
    }
    std::cout << std::endl;
  }
}

} // namespace tagarray
