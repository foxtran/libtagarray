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
    logical :: lstat
    status = -1
    allocate(value, source = 1_c_int32_t)
    allocate(array(10), source = 2_c_int32_t)
    call container%new()
    TA_CONTAINER_CREATE(stat, container, "INT32-value", value)
    if (stat /= TA_OK) then
      status = 2
      return
    end if
    TA_CONTAINER_CREATE(stat, container, "INT32-array", array)
    if (stat /= TA_OK) then
      status = 3
      return
    end if
    TA_CONTAINER_CREATE_OPTIONAL(stat, container, "INT32-value-opt", value, description = "value")
    if (stat /= TA_OK) then
      status = 4
      return
    end if
    TA_CONTAINER_CREATE_OPTIONAL(stat, container, "INT32-array-opt", array, description = "array")
    if (stat /= TA_OK) then
      status = 5
      return
    end if
    lstat = container%contains((/ character(len=20) :: "INT32-value", "INT32-array", "INT32-value-opt", "INT32-array-opt" /))
    if (.not.lstat) then
      status = 6
      return
    end if
    TA_CONTAINER_GET_VALUE(container, "INT32-value", p_value, stat)
    if (stat /= TA_OK) then
      status = 7
      return
    end if
    if (value /= p_value) then
      status = 8
      return
    end if
    TA_CONTAINER_GET_ARRAY(container, "INT32-array", p_array, stat)
    if (stat /= TA_OK) then
      status = 9
      return
    end if
    if (any(array /= p_array)) then
      status = 10
      return
    end if
    TA_CONTAINER_GET_VALUE(container, "INT32-value-opt", p_value, stat)
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
    TA_CONTAINER_GET_ARRAY(container, "INT32-array-opt", p_array, stat)
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
end program test
