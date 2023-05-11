#include <iostream>

#include "tagarray/Container.hpp"

namespace tagarray {

Container::~Container() noexcept { this->_records.clear(); }

void Container::add_record(const std::string &tag, Record &record) noexcept {
  this->_status = utils::check_tag(tag);
  if (this->_status != TA_OK)
    return;
  this->find_record(tag);
  if (this->_status == TA_OK) {
    this->_status = TA_CONTAINER_RECORD_EXISTS;
    return;
  }
  this->_records.insert({tag, &record});
  this->_status = TA_OK;
}

void Container::save(const std::string &filename) noexcept {
  this->_status = TA_NOT_IMPLEMENTED;
}
void Container::save(const std::u32string &filename) noexcept {
  this->_status = TA_NOT_IMPLEMENTED;
}

void Container::load(const std::string &filename) noexcept {
  this->_status = TA_NOT_IMPLEMENTED;
}
void Container::load(const std::u32string &filename) noexcept {
  this->_status = TA_NOT_IMPLEMENTED;
}

void Container::dump(const int32_t level) const noexcept {
  std::cout << "Container v" << this->_version << std::endl;
  std::cout << "  Comment len = " << this->_comment.length() << std::endl;
  std::cout << "  Comment `" << this->_comment << "`" << std::endl;
  std::cout << "  Number of records: " << this->_records.size() << std::endl;
  for (const auto &[key, value] : this->_records) {
    std::cout << "  Record tag: `" << key << "`" << std::endl;
    if (level > 0)
      value->dump(level - 1);
  }
}

} // namespace tagarray
