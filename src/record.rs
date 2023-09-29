/// Single multi-dimensional record with a type tag.
use pyo3::pyclass;

#[pyclass]
pub struct Record {
  type_id: i32,
  item_size: isize, // size of one element in bytes
  count: isize, // number of elements
  n_dimensions: isize, // not needed! use dimensions.count()
  dimensions: Vec<isize>,
  data: Vec<u8>,
  description: String,
}

// Public Python interface
  py::class_<Record>(m, "Record", py::buffer_protocol())
      .def(py::init([](py::buffer b, const std::string &description) {
             py::buffer_info info = b.request();
             if (info.ndim > defines::MAX_DIMENSIONS_LENGTH)
               throw std::runtime_error("Too many dimensions");
             int32_t type = py_utils::get_type_from_pyformat(info.format);
             std::vector<int64_t> dims;
             transform(info.shape.begin(), info.shape.end(), dims.begin(),
                       py_utils::static_cast_TO<int64_t, ssize_t>());
             return new Record(type, static_cast<int32_t>(info.ndim),
                               static_cast<uint8_t *>(info.ptr), info.size,
                               dims, description);
           }),
           py::arg("buf"), py::arg("description") = std::string(""))
      .def_property("description", &Record::description,
                    &Record::set_description)
      .def_property_readonly("typeid", &Record::type_id)
      .def_property_readonly("ndim", &Record::ndim)
      .def_property_readonly("size", &Record::count)
      .def_property_readonly("itemsize", &Record::itemsize)
      .def_property("shape", &Record::shape,
                    [](Record &rec, const std::vector<int64_t> &dims) {
                      int32_t status = rec.set_shape(dims);
                      switch (status) {
                      case defines::OK:
                        break;
                      case defines::DATA_TOO_MANY_DIMENSIONS:
                        throw std::runtime_error("Too many dimensions");
                        break;
                      case defines::DATA_INSUFFICIENT_SIZE:
                        throw std::runtime_error(
                            "New size is not the same as previous");
                        break;
                      default:
                        throw std::runtime_error("Unknown error");
                        break;
                      }
                    })
      .def_property_readonly("allocated", &Record::is_allocated)
      .def_property(
          "data",
          [](Record &rec) {
            auto numpy_array =
                py_utils::get_numpy_array<py_utils::supported_types>(rec);
            if (numpy_array)
              return *numpy_array;
            throw std::runtime_error("Type is not supported");
          },
          [](Record &rec, py::buffer b) {
            py::buffer_info info = b.request();
            if (std::string(info.format) !=
                py_utils::type_format(rec.type_id()))
              throw std::runtime_error("Types are different");
            if (info.ndim > defines::MAX_DIMENSIONS_LENGTH)
              throw std::runtime_error("Too many dimensions");
            std::vector<int64_t> dims;
            transform(info.shape.begin(), info.shape.end(), dims.begin(),
                      py_utils::static_cast_TO<int64_t, ssize_t>());
            rec.set_data(static_cast<uint8_t *>(info.ptr), dims);
          })
      .def("free_data", &Record::free_data);



#[pymethods]
impl Record {
  #[new]
  fn Record(const int32_t type_id, const int32_t n_dimensions,
         const uint8_t *const &data, const int64_t count,
         const std::vector<int64_t> &dimensions,
         const std::string &description) noexcept;

  ~Record() noexcept;

  inline int32_t type_id() const noexcept { return this->type_id_; }

  inline int64_t ndim() const noexcept { return this->n_dimensions_; }

  inline const std::string &description() const noexcept {
    return this->description_;
  }

  inline void set_description(const std::string &str = std::string()) noexcept {
    this->description_ = str;
  }

  inline int32_t itemsize() const noexcept { return this->itemsize_; }

  inline int64_t count() const noexcept { return this->count_; }

  inline int64_t byte_count() const noexcept {
    return this->count_ * this->itemsize_;
  }

  template <typename T> inline auto raw_data() const noexcept {
    return reinterpret_cast<T>(this->data_.get());
  }

  inline void set_data(const uint8_t *const &data = nullptr,
                       const std::vector<int64_t> &dimensions =
                           std::vector<int64_t>(1, 1)) noexcept {
    this->dimensions_ = dimensions;
    this->free_data();
    this->count_ = std::accumulate(dimensions.begin(), dimensions.end(), 1,
                                   std::multiplies<int64_t>());
    std::shared_ptr<uint8_t[]> tmp(new (std::align_val_t(64), std::nothrow)
                                       uint8_t[this->byte_count()],
                                   std::default_delete<uint8_t[]>());
    this->data_ = tmp;
    if (this->data_.get() == nullptr) {
      return;
    }
    if (data == nullptr) {
      std::fill(this->data_.get(), this->data_.get() + this->byte_count(), 0);
    } else {
      std::copy(data, data + this->byte_count(), this->data_.get());
    }
  }

  inline void free_data() noexcept {
    this->data_.reset();
  }

  inline const std::vector<int64_t> &shape() const noexcept {
    return this->dimensions_;
  }

  inline int32_t set_shape(const std::vector<int64_t> &dimensions) noexcept {
    if (dimensions.size() > defines::MAX_DIMENSIONS_LENGTH)
      return defines::DATA_TOO_MANY_DIMENSIONS;
    if (std::accumulate(dimensions.begin(), dimensions.end(), 1,
                        std::multiplies<int64_t>()) != this->count_)
      return defines::DATA_INSUFFICIENT_SIZE;
    this->dimensions_ = dimensions;
    return defines::OK;
  }

  inline bool is_allocated() const noexcept { return this->data_ != nullptr; }

  inline RecordInfo info() const noexcept {
    RecordInfo recordInfo = {this->type_id_,
                             this->itemsize_,
                             this->count_,
                             this->n_dimensions_,
                             {1},
                             this->data_.get()};
    std::copy(std::begin(this->dimensions_), std::end(this->dimensions_),
              std::begin(recordInfo.dimensions));
    return recordInfo;
  }

  void dump(const int32_t level) const noexcept;

}
