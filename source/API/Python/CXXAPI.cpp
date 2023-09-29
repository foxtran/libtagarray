#include "tagarray.hpp"

#include <pybind11/pybind11.h>

using namespace tagarray;
namespace py = pybind11;

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
}
