#include <iostream>

#include "tagarray/Container.hpp"

namespace tagarray {

Container::~Container() {
  this->records.clear();
}

void Container::add_record(const std::string &tag, Record &record) {
  this->find_record(tag);
  if (this->status == TAGARRAY_OK) {
    this->status = TAGARRAY_CONTAINER_RECORD_EXISTS;
    return;
  }
  this->records.insert({tag, &record});
  this->status = TAGARRAY_OK;
}

int32_t Container::find_record(const std::string &tag) {
  if (auto search = this->records.find(tag); search != this->records.end()) {
    this->status = TAGARRAY_OK;
  } else {
    this->status = TAGARRAY_CONTAINER_RECORD_NOT_FOUND;
  }
  return this->status;
}

void Container::remove_record(const std::string &tag) {
  this->find_record(tag);
  if (this->status != TAGARRAY_OK) return;
  delete this->records[tag];
  this->records.erase(tag);
  this->status = TAGARRAY_OK;
}

Record *Container::get_record(const std::string &tag) {
  this->find_record(tag);
  if (this->status != TAGARRAY_OK) return nullptr;
  return this->records[tag];
}

void Container::save(const std::string &filename) {
  this->status = TAGARRAY_NOT_IMPLEMENTED;
}
void Container::save(const std::u32string &filename) {
  this->status = TAGARRAY_NOT_IMPLEMENTED;
}

void Container::load(const std::string &filename) {
  this->status = TAGARRAY_NOT_IMPLEMENTED;
}
void Container::load(const std::u32string &filename) {
  this->status = TAGARRAY_NOT_IMPLEMENTED;
}

void Container::dump(const int32_t level) const {
  std::cout << "Container v" << this->version << std::endl;
  std::cout << "  Comment len = " << this->comment.length() << std::endl;
  std::cout << "  Comment `";
  for (const auto& c: this->comment)
    std::cout << static_cast<char>(c);
  std::cout << "`" << std::endl;
  std::cout << "  Number of records: " << this->records.size() << std::endl;
  for (const auto& pair: this->records) {
    std::cout << "  Record tag: `" << pair.first << "`" << std::endl;
    if (level > 0) pair.second->dump(level-1);
  }
}

} // namespace tagarray
