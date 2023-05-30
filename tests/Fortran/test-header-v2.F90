#define TA_FORTRAN_API_VERSION 2
#include "tagarray.fh"
#include "tagarray_assert.fh"

program test
  use tagarray
  implicit none
  TEST(check_container)
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
    if (container%get_status() /= TA_OK) then
      status = 1
      return
    end if
    TA_CONTAINER_ADD_VALUE(container, "INT32-value", value)
    if (container%get_status() /= TA_OK) then
      status = 2
      return
    end if
    TA_CONTAINER_ADD_DATA(container, "INT32-array", array)
    if (container%get_status() /= TA_OK) then
      status = 3
      return
    end if
    TA_CONTAINER_ADD_VALUE_OPTIONAL(container, "INT32-value-opt", value, comment = "value")
    if (container%get_status() /= TA_OK) then
      status = 4
      return
    end if
    TA_CONTAINER_ADD_DATA_OPTIONAL(container, "INT32-array-opt", array, comment = "array")
    if (container%get_status() /= TA_OK) then
      status = 5
      return
    end if
    stat = container%has_records((/ character(len=20) :: "INT32-value", "INT32-array", "INT32-value-opt", "INT32-array-opt" /))
    if (stat /= TA_OK) then
      status = 6
      return
    end if
    TA_GET_CONTAINER_VALUE(container, "INT32-value", p_value, stat)
    if (stat /= TA_OK) then
      status = 7
      return
    end if
    if (value /= p_value) then
      status = 8
      return
    end if
    TA_GET_CONTAINER_DATA(container, "INT32-array", p_array, stat)
    if (stat /= TA_OK) then
      status = 9
      return
    end if
    if (any(array /= p_array)) then
      status = 10
      return
    end if
    TA_GET_CONTAINER_VALUE(container, "INT32-value-opt", p_value, stat)
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
    TA_GET_CONTAINER_DATA(container, "INT32-array-opt", p_array, stat)
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
  integer function check_record() result(status)
    use, intrinsic :: iso_c_binding, only: c_int32_t
    type(record_t) :: record
    integer(c_int32_t) :: stat
    integer(c_int32_t), target, allocatable :: value, array(:)
    integer(c_int32_t), pointer :: p_value, p_array(:)
    status = -1
    allocate(value, source = 1_c_int32_t)
    allocate(array(10), source = 2_c_int32_t)
#warning "FIX ME: check_record: header v2 does not support values"
!    TA_RECORD_NEW(record, value)
    TA_RECORD_NEW(record, array)
    stat = record%get_status()
    if (stat /= TA_OK) then
      status = 1
      return
    end if
    TA_GET_RECORD_DATA(record, p_array, stat)
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
    TA_RECORD_NEW_OPTIONAL(record, array, comment = "INT32-array-opt")
    stat = record%get_status()
    if (stat /= TA_OK) then
      status = 5
      return
    end if
    TA_GET_RECORD_DATA(record, p_array, stat)
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
  end function check_record
end program test
