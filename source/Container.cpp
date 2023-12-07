#include <fstream>
#include <iostream>

#include "tagarray/Container.hpp"

namespace tagarray {

struct Header {
  char header[4];     // TAGA
  char endianness[2]; // LE or BE or XE (XE - native encoding)
  int32_t version;    // container version
  int32_t nrecords;   // number of records
};

int32_t write_header(std::ofstream &dump, const Header &h) {
  dump.write(reinterpret_cast<const char *>(&h), sizeof(Header));
  return defines::OK;
}

static void read_header(std::istream &s, Header &h) {
  s.read(reinterpret_cast<char *>(&h), sizeof(Header));
}

struct chararray {
  int32_t len;
  char *data;

  int64_t size() const { return sizeof(int32_t) + len * sizeof(char); }

  void write(std::ofstream &dump) {
    dump.write(reinterpret_cast<const char *>(&len), sizeof(int32_t));
    dump.write(data, len * sizeof(char));
  }

  static chararray read(std::ifstream &s) {
    chararray ca;
    s.read(reinterpret_cast<char *>(&ca.len), sizeof(int32_t));
    if (ca.len > 0) {
      ca.data = new char[ca.len];
      s.read(ca.data, ca.len * sizeof(char));
    } else {
      ca.data = new char[1];
    }
    return ca;
  }
};

struct i64array {
  int64_t len;
  int64_t *data;

  int64_t size() const { return sizeof(int64_t) + len * sizeof(int64_t); }

  void write(std::ofstream &dump) {
    dump.write(reinterpret_cast<const char *>(&len), sizeof(int64_t));
    dump.write(reinterpret_cast<const char *>(data), len * sizeof(int64_t));
  }

  static i64array read(std::ifstream &s) {
    i64array i64a;
    s.read(reinterpret_cast<char *>(&i64a.len), sizeof(int64_t));
    i64a.data = new int64_t[i64a.len];
    s.read(reinterpret_cast<char *>(i64a.data), i64a.len * sizeof(int64_t));
    return i64a;
  }
};

struct v1_recordinfo {
  chararray tag;
  int32_t type_id;
  int32_t itemsize;
  int64_t count;
  i64array dims;     // dimensions
  chararray desc;    // description
  int64_t datashift; // shift in bytes to data position in file

  int64_t size() const {
    return tag.size() + 2 * sizeof(int32_t) + sizeof(int64_t) + dims.size() +
           desc.size() + sizeof(int64_t);
  }

  void write(std::ofstream &dump) {
    tag.write(dump);
    dump.write(reinterpret_cast<const char *>(&type_id), sizeof(int32_t));
    dump.write(reinterpret_cast<const char *>(&itemsize), sizeof(int32_t));
    dump.write(reinterpret_cast<const char *>(&count), sizeof(int64_t));
    dims.write(dump);
    desc.write(dump);
    dump.write(reinterpret_cast<const char *>(&datashift), sizeof(int64_t));
  }

  static v1_recordinfo read(std::ifstream &s) {
    v1_recordinfo ri;
    ri.tag = chararray::read(s);
    s.read(reinterpret_cast<char *>(&ri.type_id), sizeof(int32_t));
    s.read(reinterpret_cast<char *>(&ri.itemsize), sizeof(int32_t));
    s.read(reinterpret_cast<char *>(&ri.count), sizeof(int64_t));
    ri.dims = i64array::read(s);
    ri.desc = chararray::read(s);
    s.read(reinterpret_cast<char *>(&ri.datashift), sizeof(int64_t));
    return ri;
  }
};

int32_t save_v1(std::ofstream &dump, Container &cont) {
  auto keys = cont.keys();
  std::vector<std::pair<std::string, v1_recordinfo>> recs;
  chararray contdesc{static_cast<int32_t>(cont.description().size()),
                     const_cast<char *>(cont.description().data())};
  int64_t datashift = sizeof(Header) + contdesc.size();
  contdesc.write(dump);
  for (const auto &key : keys) {
    const auto rec = cont[key];
    const v1_recordinfo ri{
        {static_cast<int32_t>(key.size()), const_cast<char *>(key.data())},
        rec->type_id(),
        rec->itemsize(),
        rec->count(),
        {rec->ndims(), const_cast<int64_t *>(rec->shape().data())},
        {static_cast<int32_t>(rec->description().size()),
         rec->description().data()},
        0};
    recs.push_back(std::make_pair(key, ri));
    datashift += ri.size();
  }
  for (auto &pair : recs) {
    pair.second.datashift = datashift;
    pair.second.write(dump);
    datashift += cont[pair.first]->byte_count();
  }
  for (auto &pair : recs) {
    const auto rec = cont[pair.first];
    dump.write(rec->raw_data<char *>(), rec->byte_count());
  }
  return defines::OK;
}

int32_t load_v1(std::ifstream &s, Container &cont, const int32_t nrecs) {
  chararray contdesc = chararray::read(s);
  cont.description() = std::string(contdesc.data, contdesc.len);
  std::vector<v1_recordinfo> vri;
  for (int i = 0; i < nrecs; i++) {
    v1_recordinfo ri = v1_recordinfo::read(s);
    vri.push_back(ri);
  }
  for (const auto &ri : vri) {
    std::string tag(ri.tag.data, ri.tag.len);
    std::string desc(ri.desc.data, ri.desc.len);
    std::vector<int64_t> dims(ri.dims.data, ri.dims.data + ri.dims.len);
    cont.create(tag, ri.type_id, dims, nullptr, desc);
    auto rec = cont[tag];
    s.read(rec->raw_data<char *>(), rec->byte_count());
  }
  for (auto &ri : vri) {
    delete[] ri.tag.data;
    delete[] ri.desc.data;
    delete[] ri.dims.data;
  }
  delete[] contdesc.data;
  return defines::OK;
}

int32_t Container::save(const std::filesystem::path &filename,
                        const int32_t version) noexcept {
  std::ofstream dump(filename, std::ios::binary);
  if (dump.is_open()) {
    Header header{{'T', 'A', 'G', 'A'},
                  {'X', 'E'},
                  version,
                  static_cast<int32_t>(this->records_.size())};
    write_header(dump, header);
    switch (version) {
    case 1:
      save_v1(dump, *this);
      break;
    default:
      std::cout << "Wrong version!";
      return defines::NOT_IMPLEMENTED;
    }
    dump.close();
  } else {
    std::cout << "Can not open file '" << filename << "'!" << std::endl;
    return defines::OK;
  }
  return defines::OK;
}

Container Container::load(const std::filesystem::path &filename) noexcept {
  Container cont("");
  std::ifstream file(filename, std::ios::binary);
  if (file.is_open()) {
    Header h;
    read_header(file, h);
    switch (h.version) {
    case 1:
      load_v1(file, cont, h.nrecords);
      break;
    default:
      std::cout << "Wrong version!";
    }
  } else {
    std::cout << "Can not open file '" << filename << "'!" << std::endl;
  }
  return cont;
}

void Container::dump(const int32_t level) const noexcept {
  std::cout << "Container" << std::endl;
  std::cout << "  Comment len = " << this->description().length() << std::endl;
  std::cout << "  Comment `" << this->description() << "`" << std::endl;
  std::cout << "  Number of records: " << this->records_.size() << std::endl;
  for (const auto &[key, value] : this->records_) {
    std::cout << "  Record tag: `" << key << "`" << std::endl;
    if (level > 0)
      ;
    //      value->dump(level - 1);
  }
}

} // namespace tagarray
