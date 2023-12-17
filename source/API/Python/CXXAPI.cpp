#include <complex>
#include <unordered_map>
#include <variant>

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "tagarray.hpp"
#include "tagarray/PyRecordInfo.hpp"

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
std::optional<py::object> get_numpy_array(const PyRecordInfo &pyrec) {
  if constexpr (I < std::variant_size_v<V>) {
    using T = std::variant_alternative_t<I, V>;
    if (std::type_index(typeid(T)) ==
        py_utils::get_type(pyrec.get()->type_id())) {
      return py::cast(new py::array_t<T, py::array::f_style>(
          pyrec.get()->shape(), pyrec.get()->raw_data<T *>(),
          py::capsule(new auto(pyrec.get()->raw_data<void *>()),
                      [](void *) { /* no deallocation */ })));
    }
    return get_numpy_array<V, I + 1>(pyrec);
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

  py::class_<PyRecordInfo>(m, "RecordInfo")
      .def(py::init([](const PyRecordInfo &pyrec) {
             return new PyRecordInfo(pyrec);
           }),
           py::return_value_policy::take_ownership)
      .def("data", [](PyRecordInfo &pyrec) {
        auto numpy_array =
            py_utils::get_numpy_array<py_utils::supported_types>(pyrec);
        if (numpy_array)
          return *numpy_array;
        throw std::runtime_error("Type is not supported");
      });

  py::class_<Container>(m, "Container")
      .def(py::init([](const std::string &description) {
             return new Container(description);
           }),
           py::arg("description") = std::string(""),
           py::return_value_policy::take_ownership)
      .def_property_readonly(
          "size", [](const Container &cont) { return cont.keys().size(); })
      .def_property(
          "description",
          [](const Container &cont) { return cont.description(); },
          [](Container &cont, const std::string &desc) {
            cont.description() = desc;
          })
      .def("create", &Container::create) // returns some status
      .def("keys", &Container::keys)
      .def("clear", &Container::clear)
      .def("contains", py::overload_cast<const std::string &>(
                           &Container::contains, py::const_))
      .def("contains", py::overload_cast<const std::vector<std::string> &>(
                           &Container::contains, py::const_))
      .def("erase", py::overload_cast<const std::string &>(&Container::erase))
      .def("erase", py::overload_cast<const std::vector<std::string> &>(
                        &Container::erase))
      .def("save",
           py::overload_cast<const std::string &, const int32_t>(
               &Container::save),
           py::arg("path"), py::arg("version") = defines::CONTAINER_VERSION)
      .def_static("load",
                  py::overload_cast<const std::string &>(&Container::load))
      .def("__len__", [](const Container &cont) { return cont.keys().size(); })
      .def("__delitem__",
           py::overload_cast<const std::string &>(&Container::erase))
      .def("__contains__", py::overload_cast<const std::string &>(
                               &Container::contains, py::const_))
      .def("copy", py::overload_cast<>(&Container::copy, py::const_))
      .def("copy", py::overload_cast<const std::vector<std::string> &>(
                       &Container::copy))
      .def("deepcopy", py::overload_cast<>(&Container::deepcopy, py::const_))
      .def("deepcopy", py::overload_cast<const std::vector<std::string> &>(
                           &Container::deepcopy))
      .def("get",
           [](Container &cont, const std::string &key) {
             if (!cont.contains(key))
               throw std::runtime_error(std::string("Key '") + key +
                                        std::string("' does not exist!"));
             return new PyRecordInfo(cont.get(key));
           })
      .def("__getitem__",
           [](Container &cont, const std::string &key) {
             if (!cont.contains(key))
               throw std::runtime_error(std::string("Key '") + key +
                                        std::string("' does not exist!"));
             return new PyRecordInfo(cont.get(key));
           })
      .def("__setitem__",
           [](Container &cont, const std::string &key, py::buffer value) {
             py::buffer_info info = value.request();
             int32_t type_id = py_utils::get_type_from_pyformat(info.format);
             std::vector<int64_t> dims;
             for (const auto &val : info.shape)
               dims.push_back(static_cast<int64_t>(val));
             cont.create(key, type_id, dims, static_cast<uint8_t *>(info.ptr));
           })
      .def("__setitem__",
           [](Container &cont, const std::string &key,
              const PyRecordInfo &value) { cont[key] = value.get(); })
      .def("update",
           [](Container &cont, const std::string &key, py::buffer value) {
             if (cont.contains(key))
               cont.erase(key);
             py::buffer_info info = value.request();
             int32_t type_id = py_utils::get_type_from_pyformat(info.format);
             std::vector<int64_t> dims;
             for (const auto &val : info.shape)
               dims.push_back(static_cast<int64_t>(val));
             cont.create(key, type_id, dims, static_cast<uint8_t *>(info.ptr));
           })
      .def("update",
           [](Container &cont, const std::string &key,
              const PyRecordInfo &value) {
             if (cont.contains(key))
               cont.erase(key);
             cont[key] = value.get();
           })
      .def("values",
           [](Container &cont) {
             std::vector<PyRecordInfo> values;
             for (const auto &key : cont.keys()) {
               values.push_back(PyRecordInfo(cont.get(key)));
             }
             return values;
           })
      .def("items", [](Container &cont) {
        std::vector<std::pair<std::string, PyRecordInfo>> items;
        for (const auto &key : cont.keys()) {
          items.push_back(std::make_pair(key, PyRecordInfo(cont.get(key))));
        }
        return items;
      });
}
