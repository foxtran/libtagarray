#include <iostream>

#include "tagarray/Container.hpp"

namespace tagarray {

Container::~Container() noexcept {
  this->records.clear();
}

void Container::add_record(const std::string &tag, Record &record) noexcept {
  this->find_record(tag);
  if (this->status == TA_OK) {
    this->status = TA_CONTAINER_RECORD_EXISTS;
    return;
  }
  this->records.insert({tag, &record});
  this->status = TA_OK;
}

void Container::save(const std::string &filename) noexcept {
  this->status = TA_NOT_IMPLEMENTED;
}
void Container::save(const std::u32string &filename) noexcept {
  this->status = TA_NOT_IMPLEMENTED;
}

void Container::load(const std::string &filename) noexcept {
  this->status = TA_NOT_IMPLEMENTED;
}
void Container::load(const std::u32string &filename) noexcept {
  this->status = TA_NOT_IMPLEMENTED;
}

void Container::dump(const int32_t level) const noexcept {
  std::cout << "Container v" << this->version << std::endl;
  std::cout << "  Comment len = " << this->comment.length() << std::endl;
  std::cout << "  Comment `" << this->comment << "`" << std::endl;
  std::cout << "  Number of records: " << this->records.size() << std::endl;
  for (const auto& [key, value] : this->records) {
    std::cout << "  Record tag: `" << key << "`" << std::endl;
    if (level > 0) value->dump(level-1);
  }
}

} // namespace tagarray
