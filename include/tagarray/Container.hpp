#ifndef _TAGARRAY_CONTAINER_HPP
#define _TAGARRAY_CONTAINER_HPP

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "tagarray/defines.h"
#include "tagarray/Record.hpp"

namespace tagarray {

class Container {
private:
  uint32_t version;
  int32_t status;
  std::string comment;
  std::vector<Record *> records;

public:
  inline Container() : version(TAGARRAY_CONTAINER_VERSION), status(TAGARRAY_OK) {};
  inline Container(std::string &comment) : version(TAGARRAY_CONTAINER_VERSION), status(TAGARRAY_OK), comment(comment) {};
  inline Container(const char *const comment_ptr) : version(TAGARRAY_CONTAINER_VERSION), status(TAGARRAY_OK) {
    update_comment(comment_ptr);
  }
  ~Container();

  inline uint32_t get_version() const { return this->version; }

  inline int32_t get_status() const { return this->status; }

  inline const std::string &get_comment() const { return this->comment; }

  inline void update_comment(const char *const comment_ptr) {
    std::string comment = std::string();
    if (comment_ptr != nullptr)
      comment = std::string(comment_ptr);
    update_comment(comment);
  }
  inline void update_comment(const std::string &comment) {
    this->comment = comment;
    this->status = TAGARRAY_OK;
  }

  void add_record(Record &record);

  ssize_t find_record(const std::string &tag);
  inline ssize_t find_record(const char *const tag) {
    if (tag == nullptr)
    {
      this->status = TAGARRAY_EMPTY_TAG;
      return TAGARRAY_CONTAINER_RECORD_NOT_FOUND;
    }
    return find_record(std::string(tag));
  }

  void remove_record(const std::string &tag);
  inline void remove_record(const char *const tag) {
    if (tag == nullptr)
    {
      this->status = TAGARRAY_EMPTY_TAG;
      return;
    }
    remove_record(std::string(tag));
  }

  Record *get_record(const std::string &tag);
  inline Record *get_record(const char *const tag) {
    if (tag == nullptr)
    {
      this->status = TAGARRAY_EMPTY_TAG;
      return nullptr;
    }
    return get_record(std::string(tag));
  }

  void save(const std::string &filename);
  void save(const std::u32string &filename);
  inline void save(const char *const filename_ptr) {
    if (filename_ptr == nullptr) {
      this->status = TAGARRAY_FILENAME_NULLPTR;
      return;
    }
    save(std::string(filename_ptr));
  }
  inline void save(const char32_t *const filename_ptr) {
    if (filename_ptr == nullptr) {
      this->status = TAGARRAY_FILENAME_NULLPTR;
      return;
    }
    save(std::u32string(filename_ptr));
  }

  void load(const std::string &filename);
  void load(const std::u32string &filename);
  inline void load(const char *const filename_ptr) {
    if (filename_ptr == nullptr) {
      this->status = TAGARRAY_FILENAME_NULLPTR;
      return;
    }
    load(std::string(filename_ptr));
  }
  inline void load(const char32_t *const filename_ptr) {
    if (filename_ptr == nullptr) {
      this->status = TAGARRAY_FILENAME_NULLPTR;
      return;
    }
    load(std::u32string(filename_ptr));
  }

  void dump(const int32_t level) const;
};

} // namespace tagarray

#endif
