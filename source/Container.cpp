#include <iostream>

#include "tagarray/Container.hpp"

namespace tagarray {

Container::~Container() noexcept { this->records_.clear(); }

void Container::add_record(const std::string &tag, Record &record) noexcept {
  this->status_ = utils::check_tag(tag);
  if (this->status_ != TA_OK)
    return;
  this->has_record(tag);
  if (this->status_ == TA_OK) {
    this->status_ = TA_CONTAINER_RECORD_EXISTS;
    return;
  }
  this->records_.insert({tag, &record});
  this->status_ = TA_OK;
}

void Container::save(const std::filesystem::path &filename) noexcept {
  this->status_ = TA_NOT_IMPLEMENTED;
}

void Container::load(const std::filesystem::path &filename) noexcept {
  this->status_ = TA_NOT_IMPLEMENTED;
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
