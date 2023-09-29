#include "tagarray.hpp"

#include <complex>
#include <unordered_map>
#include <variant>

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

using namespace tagarray;
namespace py = pybind11;

namespace tagarray {
namespace py_utils {

using supported_types =
    std::variant<int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t,
                 uint64_t, float, double, std::complex<float>,
                 std::complex<double>, char>;

const std::unordered_map<int32_t, std::type_index> type_index_map{
    {defines::TYPE_INT8, std::type_index(typeid(int8_t))},
    {defines::TYPE_INT16, std::type_index(typeid(int16_t))},
    {defines::TYPE_INT32, std::type_index(typeid(int32_t))},
    {defines::TYPE_INT64, std::type_index(typeid(int64_t))},
    {defines::TYPE_UINT8, std::type_index(typeid(uint8_t))},
    {defines::TYPE_UINT16, std::type_index(typeid(uint16_t))},
    {defines::TYPE_UINT32, std::type_index(typeid(uint32_t))},
    {defines::TYPE_UINT64, std::type_index(typeid(uint64_t))},
    {defines::TYPE_REAL32, std::type_index(typeid(float))},
    {defines::TYPE_REAL64, std::type_index(typeid(double))},
    {defines::TYPE_CMPLX32, std::type_index(typeid(std::complex<float>))},
    {defines::TYPE_CMPLX64, std::type_index(typeid(std::complex<double>))},
    {defines::TYPE_CHAR8, std::type_index(typeid(char))}};

const std::unordered_map<int32_t, std::string> pyformat_string_map{
    {defines::TYPE_INT8, py::format_descriptor<int8_t>::format()},
    {defines::TYPE_INT16, py::format_descriptor<int16_t>::format()},
    {defines::TYPE_INT32, py::format_descriptor<int32_t>::format()},
    {defines::TYPE_INT64, py::format_descriptor<int64_t>::format()},
    {defines::TYPE_UINT8, py::format_descriptor<uint8_t>::format()},
    {defines::TYPE_UINT16, py::format_descriptor<uint16_t>::format()},
    {defines::TYPE_UINT32, py::format_descriptor<uint32_t>::format()},
    {defines::TYPE_UINT64, py::format_descriptor<uint64_t>::format()},
    {defines::TYPE_REAL32, py::format_descriptor<float>::format()},
    {defines::TYPE_REAL64, py::format_descriptor<double>::format()},
    {defines::TYPE_CMPLX32,
     py::format_descriptor<std::complex<float>>::format()},
    {defines::TYPE_CMPLX64,
     py::format_descriptor<std::complex<double>>::format()},
    {defines::TYPE_CHAR8, py::format_descriptor<char>::format()}};

const std::unordered_map<std::string, int32_t> from_pyformat_string_map{
    {py::format_descriptor<int8_t>::format(), defines::TYPE_INT8},
    {py::format_descriptor<int16_t>::format(), defines::TYPE_INT16},
    {py::format_descriptor<int32_t>::format(), defines::TYPE_INT32},
    {py::format_descriptor<int64_t>::format(), defines::TYPE_INT64},
    {py::format_descriptor<uint8_t>::format(), defines::TYPE_UINT8},
    {py::format_descriptor<uint16_t>::format(), defines::TYPE_UINT16},
    {py::format_descriptor<uint32_t>::format(), defines::TYPE_UINT32},
    {py::format_descriptor<uint64_t>::format(), defines::TYPE_UINT64},
    {py::format_descriptor<float>::format(), defines::TYPE_REAL32},
    {py::format_descriptor<double>::format(), defines::TYPE_REAL64},
    {py::format_descriptor<std::complex<float>>::format(),
     defines::TYPE_CMPLX32},
    {py::format_descriptor<std::complex<double>>::format(),
     defines::TYPE_CMPLX64},
    {py::format_descriptor<char>::format(), defines::TYPE_CHAR8}};

int32_t get_type_from_pyformat(std::string pyformat) {
  auto pair = from_pyformat_string_map.find(pyformat);
  if (pair != from_pyformat_string_map.end()) {
    return pair->second;
  }
  throw std::runtime_error("Type not found");
}

const std::type_index &get_type(const int32_t type_id) {
  auto pair = type_index_map.find(type_id);
  if (pair != type_index_map.end()) {
    return pair->second;
  }
  throw std::runtime_error("Type not found");
}

const std::string &type_format(const int32_t type_id) {
  auto pair = pyformat_string_map.find(type_id);
  if (pair != pyformat_string_map.end()) {
    return pair->second;
  }
  throw std::runtime_error("Type not found");
}

template <typename V, std::size_t I = 0>
std::optional<py::object> get_numpy_array(const Record &rec) {
  if constexpr (I < std::variant_size_v<V>) {
    using T = std::variant_alternative_t<I, V>;
    if (std::type_index(typeid(T)) == py_utils::get_type(rec.get_type_id())) {
      Dimensions dims_arr = rec.get_shape();
      std::vector<int64_t> shape(rec.get_n_dimensions(), 0);
      for (int32_t i = 0; i < static_cast<int32_t>(shape.size()); i++) {
        shape[i] = dims_arr[i];
      }
      return py::cast(new py::array_t<T, py::array::f_style>(
          shape, reinterpret_cast<T *>(rec.get_data())));
    }

    return get_numpy_array<V, I + 1>(rec);
  }
  return {};
}

} // namespace py_utils
} // namespace tagarray

PYBIND11_MODULE(tagarray, m) {

  auto m_types = m.def_submodule("types");
  m_types.attr("unknown") = defines::TYPE_UNKNOWN;
  m_types.attr("int8") = defines::TYPE_INT8;
  m_types.attr("int16") = defines::TYPE_INT16;
  m_types.attr("int32") = defines::TYPE_INT32;
  m_types.attr("int64") = defines::TYPE_INT64;
  m_types.attr("uint8") = defines::TYPE_UINT8;
  m_types.attr("uint16") = defines::TYPE_UINT16;
  m_types.attr("uint32") = defines::TYPE_UINT32;
  m_types.attr("uint64") = defines::TYPE_UINT64;
  m_types.attr("real32") = defines::TYPE_REAL32;
  m_types.attr("real64") = defines::TYPE_REAL64;
  m_types.attr("complex32") = defines::TYPE_CMPLX32;
  m_types.attr("complex64") = defines::TYPE_CMPLX64;
  m_types.attr("char8") = defines::TYPE_CHAR8;

  m.attr("__version__") = defines::VERSION;
  m.attr("__version_major__") = defines::VERSION_MAJOR;
  m.attr("__version_minor__") = defines::VERSION_MINOR;
  m.attr("__version_patch__") = defines::VERSION_PATCH;

  py::class_<Record>(m, "Record", py::buffer_protocol())
      .def(py::init([](py::buffer b, const std::string &description) {
             py::buffer_info info = b.request();
             if (info.ndim > defines::DIMENSIONS_LENGTH)
               throw std::runtime_error("Too many dimensions");
             int32_t type = py_utils::get_type_from_pyformat(info.format);
             int64_t data_length = info.itemsize * info.size;
             Dimensions dims{1};
             for (auto i = 0; i < info.ndim; i++) {
               dims[i] = static_cast<int64_t>(info.shape[i]);
             }
             return new Record(type, static_cast<int32_t>(info.ndim),
                               static_cast<uint8_t *>(info.ptr), data_length,
                               dims, description);
           }),
           py::arg("buf"), py::arg("description") = std::string(""))
      .def_property(
          "description", &Record::get_comment,
          py::overload_cast<const std::string &>(&Record::update_comment))
      .def_property_readonly("typeid", &Record::get_type_id)
      .def_property_readonly("ndim", &Record::get_n_dimensions)
      .def_property_readonly("size", &Record::get_data_size)
      .def_property_readonly(
          "itemsize",
          [](const Record &rec) { return utils::get_storage_size(rec.get_type_id()); })
      .def_property(
          "shape",
          [](const Record &rec) {
            Dimensions dims_arr = rec.get_shape();
            std::vector<int64_t> dims(rec.get_n_dimensions(), 0);
            for (decltype(dims.size()) i = 0; i < dims.size(); i++) {
              dims[i] = dims_arr[i];
            }
            return dims;
          },
          [](Record &rec, const std::vector<int64_t> &dims) {
            int32_t status = rec.set_shape(dims);
            switch (status) {
            case defines::OK:
              break;
            case defines::DATA_TOO_MANY_DIMENSIONS:
              throw std::runtime_error("Too many dimensions");
              break;
            case defines::DATA_INSUFFICIENT_SIZE:
              throw std::runtime_error("New size is not the same as previous");
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
                py_utils::type_format(rec.get_type_id()))
              throw std::runtime_error("Types are different");
            if (info.ndim > defines::DIMENSIONS_LENGTH)
              throw std::runtime_error("Too many dimensions");
            int64_t data_length = info.itemsize * info.size;
            Dimensions dims{1};
            for (auto i = 0; i < info.ndim; i++) {
              dims[i] = static_cast<int64_t>(info.shape[i]);
            }
            rec.set_data(static_cast<uint8_t *>(info.ptr), data_length, dims);
          })
      .def("free_data", &Record::free_data);
}
