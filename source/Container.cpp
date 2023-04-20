#include <iostream>

#include "tagarray/Container.hpp"

namespace tagarray {

Container::~Container() {
  for (auto record : this->records) {
    delete record;
  }
}

void Container::add_record(Record &record) {
  ssize_t index = this->find_record(record.get_tag());
  if (this->status != TAGARRAY_CONTAINER_RECORD_NOT_FOUND) {
    this->status = TAGARRAY_CONTAINER_RECORD_EXISTS;
    return;
  }
  this->records.push_back(&record);
  this->status = TAGARRAY_OK;
}

ssize_t Container::find_record(const std::string &tag) {
  for (ssize_t index = 0; index < static_cast<ssize_t>(this->records.size());
       index++) {
    if (this->records[index]->get_tag() == tag) {
      this->status = TAGARRAY_OK;
      return index;
    }
  }
  this->status = TAGARRAY_CONTAINER_RECORD_NOT_FOUND;
  return TAGARRAY_CONTAINER_RECORD_NOT_FOUND;
}

void Container::remove_record(const std::string &tag) {
  ssize_t index = this->find_record(tag);
  if (this->status != TAGARRAY_OK) {
    return;
  }
  for (auto it = this->records.begin(); it != this->records.end(); ++it) {
    if ((it - this->records.begin()) == index) {
      it = this->records.erase(it);
      break;
    }
  }
  this->status = TAGARRAY_OK;
  return;
}

Record *Container::get_record(const std::string &tag) {
  ssize_t index = this->find_record(tag);
  if (this->status != TAGARRAY_OK) {
    return nullptr;
  }
  return this->records[index];
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
  if (level > 0) {
    int i = 0;
    for (const auto& record: this->records) {
      i++;
      std::cout << "  Record = " << i << std::endl;
      record->dump(level-1);
    }
  }
}

} // namespace tagarray
