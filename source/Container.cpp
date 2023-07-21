#include <iostream>

#include "tagarray/Container.hpp"

namespace tagarray {

Container::~Container() noexcept { this->records_.clear(); }

int32_t Container::add_record(const std::string &tag, Record &record) noexcept {
  int32_t status = utils::check_tag(tag);
  if (status != TA_OK)
    return status;
  status = this->has_record(tag);
  if (status == TA_OK) {
    return TA_CONTAINER_RECORD_EXISTS;
  }
  this->records_.insert({tag, &record});
  return TA_OK;
}

int32_t Container::save(const std::filesystem::path &filename) noexcept {
  return TA_NOT_IMPLEMENTED;
}

int32_t Container::load(const std::filesystem::path &filename) noexcept {
  return TA_NOT_IMPLEMENTED;
}

void Container::dump(const int32_t level) const noexcept {
  std::cout << "Container v" << this->version_ << std::endl;
  std::cout << "  Comment len = " << this->comment_.length() << std::endl;
  std::cout << "  Comment `" << this->comment_ << "`" << std::endl;
  std::cout << "  Number of records: " << this->records_.size() << std::endl;
  for (const auto &[key, value] : this->records_) {
    std::cout << "  Record tag: `" << key << "`" << std::endl;
    if (level > 0)
      value->dump(level - 1);
  }
}

} // namespace tagarray
