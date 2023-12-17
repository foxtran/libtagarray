#include <cassert>
#include <cstdint>
#include <numeric>
#include <string>
#include <vector>

#include "tagarray/hidden/Record.hpp"

namespace tagarray {

class PyRecordInfo {
private:
  std::shared_ptr<hidden::Record> ptr_;

public:
  PyRecordInfo(const std::shared_ptr<hidden::Record> &ptr) noexcept {
    this->ptr_ = ptr;
  }
  PyRecordInfo(const PyRecordInfo &pyrec) noexcept { this->ptr_ = pyrec.ptr_; }
  const std::shared_ptr<hidden::Record> &get() const noexcept {
    return this->ptr_;
  }
};
} // namespace tagarray