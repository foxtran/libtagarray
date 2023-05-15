#include "tagarray/Utils.hpp"

namespace tagarray {
namespace utils {

std::string get_status_message(const int32_t status,
                               const std::string &tag) noexcept {
  std::string message("");
  switch (status) {
  case TA_OK:
    message = "OK!";
    break;
  case TA_MEMORY_ALLOCATION_ERROR:
    message = "Can not allocate memory";
    if (tag.empty()) {
      message += ".";
    } else {
      message += " for tag " + tag + ".";
    }
    break;
  case TA_CONTAINER_NULLPTR:
    message = "Container is not initialized.";
    break;
  case TA_CONTAINER_RECORD_NOT_FOUND:
    message = "Record ";
    if (!tag.empty()) {
      message += "`" + tag + "` ";
    }
    message + "not found!";
    break;
  case TA_CONTAINER_RECORD_EXISTS:
    message = "Record ";
    if (!tag.empty()) {
      message += "`" + tag + "` ";
    }
    message + "already exists!";
    break;
  case TA_RECORD_OPTION_DOES_NOT_EXIST:
    message = "Option index is wrong";
    if (tag.empty()) {
      message += ".";
    } else {
      message += " for tag " + tag + ".";
    }
    break;
  case TA_DATA_ZERO_LENGTH:
    if (!tag.empty()) {
      message += "For tag `" + tag + "`, ";
    }
    message += "Data has zero length.";
    break;
  case TA_DATA_TOO_MANY_DIMENSIONS:
    message += "Too many dimensions were requested.\n"
               "  Maximum allowed number of dimensions is " +
               std::to_string(TA_DIMENSIONS_LENGTH) + ".";
    break;
  case TA_DATA_INCORRECT_TYPE:
    if (!tag.empty()) {
      message += "For tag `" + tag + "`, ";
    }
    message += "Warning: Incompatible types in casting between internal data "
               "and user-defined data";
    break;
  case TA_DATA_INCORRECT_DIMENSIONS:
    if (!tag.empty()) {
      message += "For tag `" + tag + "`, ";
    }
    message += "Warning: Incompatible number of dimensions in casting between "
               "internal data and user-defined data";
    break;
  case TA_DATA_INCORRECT:
    if (!tag.empty()) {
      message += "For tag `" + tag + "`, ";
    }
    message += "Warning: Data error (check types and number of dimensions).";
    break;
  case TA_RECORD_NULLPTR:
    message = "Record is not initialized.";
    break;
  case TA_NULLPTR:
    message = "A null pointer was passed as a parameter.";
    break;
  case TA_INCORRECT_TAG:
    if (!tag.empty()) {
      message += "Tag `" + tag + "` is invalid.\n";
    }
    message +=
        "Tags can only contain Latin letters, digits, and special characters "
        "(see ASCII table). Leading and trailing white spaces are not allowed.";
    break;
  case TA_UNKNOWN_ERROR:
    message += "Unknown error.";
    break;
  case TA_NOT_IMPLEMENTED:
    message += "Feature is not implemented.";
    break;
  default:
    message = "Unknown status: " + std::to_string(status);
    break;
  }
  return message;
}

} // namespace utils
} // namespace tagarray
