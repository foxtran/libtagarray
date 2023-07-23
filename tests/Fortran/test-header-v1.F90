#define TA_FORTRAN_API_VERSION 1
#include "tagarray.fh"
#include "tagarray_assert.fh"

program test
  use tagarray
  implicit none
  TEST(check_container)
  TEST(check_record_value)
  TEST(check_record_data)
contains
  integer function check_container() result(status)
    use, intrinsic :: iso_c_binding, only: c_int32_t
    type(container_t) :: container
    integer(c_int32_t) :: stat
    integer(c_int32_t), target, allocatable :: value, array(:)
    integer(c_int32_t), pointer :: p_value, p_array(:)
    status = -1
    allocate(value, source = 1_c_int32_t)
    allocate(array(10), source = 2_c_int32_t)
    call container%new()
    TA_CONTAINER_ADD_VALUE(stat, container, "INT32-value", TA_TYPE_INT32, value)
    if (stat /= TA_OK) then
      status = 2
      return
    end if
    TA_CONTAINER_ADD_DATA(stat, container, "INT32-array", TA_TYPE_INT32, array)
    if (stat /= TA_OK) then
      status = 3
      return
    end if
    TA_CONTAINER_ADD_VALUE_OPTIONAL(stat, container, "INT32-value-opt", TA_TYPE_INT32, value, comment = "value")
    if (stat /= TA_OK) then
      status = 4
      return
    end if
    TA_CONTAINER_ADD_DATA_OPTIONAL(stat, container, "INT32-array-opt", TA_TYPE_INT32, array, comment = "array")
    if (stat /= TA_OK) then
      status = 5
      return
    end if
    stat = container%has_records((/ character(len=20) :: "INT32-value", "INT32-array", "INT32-value-opt", "INT32-array-opt" /))
    if (stat /= TA_OK) then
      status = 6
      return
    end if
    TA_CONTAINER_GET_VALUE(container, "INT32-value", TA_TYPE_INT32, p_value, stat)
    if (stat /= TA_OK) then
      status = 7
      return
    end if
    if (value /= p_value) then
      status = 8
      return
    end if
    TA_CONTAINER_GET_DATA(container, "INT32-array", TA_TYPE_INT32, p_array, stat)
    if (stat /= TA_OK) then
      status = 9
      return
    end if
    if (any(array /= p_array)) then
      status = 10
      return
    end if
    TA_CONTAINER_GET_VALUE(container, "INT32-value-opt", TA_TYPE_INT32, p_value, stat)
    if (stat /= TA_OK) then
      status = 11
      return
    end if
    if (value /= p_value) then
      status = 12
      return
    end if
    value = 4_c_int32_t
    if (value == p_value) then
      status = 13
      return
    end if
    TA_CONTAINER_GET_DATA(container, "INT32-array-opt", TA_TYPE_INT32, p_array, stat)
    if (stat /= TA_OK) then
      status = 14
      return
    end if
    if (any(array /= p_array)) then
      status = 15
      return
    end if
    array = 4_c_int32_t
    if (.not. any(array /= p_array)) then
      status = 16
      return
    end if
    call container%delete()
    status = 0
  end function check_container
  integer function check_record_value() result(status)
    use, intrinsic :: iso_c_binding, only: c_int32_t
    type(record_t) :: record
    integer(c_int32_t) :: stat
    integer(c_int32_t), target, allocatable :: value
    integer(c_int32_t), pointer :: p_value
    status = -1
    allocate(value, source = 1_c_int32_t)
    TA_RECORD_NEW_VALUE(record, TA_TYPE_INT32, value)
    if (.not.record%is_allocated()) then
      status = 1
      return
    end if
    TA_RECORD_GET_VALUE(record, TA_TYPE_INT32, p_value, stat)
    if (stat /= TA_OK) then
      status = 2
      return
    end if
    if (value /= p_value) then
      status = 3
      return
    end if
    value = 2_c_int32_t
    if (value == p_value) then
      status = 4
      return
    end if
    call record%delete()
    deallocate(value)
    allocate(value, source = 1_c_int32_t)
    TA_RECORD_NEW_VALUE_OPTIONAL(record, TA_TYPE_INT32, value, comment = "INT32-array-opt")
    if (.not.record%is_allocated()) then
      status = 5
      return
    end if
    TA_RECORD_GET_VALUE(record, TA_TYPE_INT32, p_value, stat)
    if (stat /= TA_OK) then
      status = 6
      return
    end if
    if (value /= p_value) then
      status = 7
      return
    end if
    value = 2_c_int32_t
    if (value == p_value) then
      status = 8
      return
    end if
    status = 0
  end function check_record_value
  integer function check_record_data() result(status)
    use, intrinsic :: iso_c_binding, only: c_int32_t
    type(record_t) :: record
    integer(c_int32_t) :: stat
    integer(c_int32_t), target, allocatable :: array(:)
    integer(c_int32_t), pointer :: p_array(:)
    status = -1
    allocate(array(10), source = 2_c_int32_t)
    TA_RECORD_NEW_DATA(record, TA_TYPE_INT32, array)
    if (.not.record%is_allocated()) then
      status = 1
      return
    end if
    TA_RECORD_GET_DATA(record, TA_TYPE_INT32, p_array, stat)
    if (stat /= TA_OK) then
      status = 2
      return
    end if
    if (any(array /= p_array)) then
      status = 3
      return
    end if
    array = 4_c_int32_t
    if (.not. any(array /= p_array)) then
      status = 4
      return
    end if
    call record%delete()
    deallocate(array)
    allocate(array(10), source = 2_c_int32_t)
    TA_RECORD_NEW_DATA_OPTIONAL(record, TA_TYPE_INT32, array, comment = "INT32-array-opt")
    if (.not.record%is_allocated()) then
      status = 5
      return
    end if
    TA_RECORD_GET_DATA(record, TA_TYPE_INT32, p_array, stat)
    if (stat /= TA_OK) then
      status = 6
      return
    end if
    if (any(array /= p_array)) then
      status = 7
      return
    end if
    array = 4_c_int32_t
    if (.not. any(array /= p_array)) then
      status = 8
      return
    end if
    status = 0
  end function check_record_data
end program test
