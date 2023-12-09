#include <cassert>
#include <cstdint>
#include <numeric>
#include <string>
#include <vector>

#include "tagarray/hidden/Record.hpp"

namespace tagarray {

class PyRecordInfo {
private:
  std::shared_ptr<hidden::Record> ptr;

public:
  PyRecordInfo(const std::shared_ptr<hidden::Record> &ptr) noexcept {
    this->ptr = ptr;
  }
  PyRecordInfo(const PyRecordInfo &pyrec) noexcept { this->ptr = pyrec.ptr; }
  const std::shared_ptr<hidden::Record> &get() const noexcept {
    return this->ptr;
  }
};
} // namespace tagarray