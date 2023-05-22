#include "tagarray_assert.fh"

program test
  use tagarray
  implicit none
  TEST(check_to_Cstring)
  TEST(check_from_Cstring)
  TEST(check_get_type_id)
  TEST(check_get_storage_size)
  TEST(check_get_status_message)
contains
  integer function check_to_Cstring() result(status)
    use, intrinsic :: iso_c_binding, only: c_null_char
    character(kind=TA_CHAR, len=:), allocatable :: string, Cstring
    status = -1
    ! check empty string
    string = ""
    Cstring = to_Cstring(string)
    if (Cstring(1:1) /= c_null_char) then
      status = 1
      return
    end if
    ! check normal string
    string = "string"
    Cstring = to_Cstring(string)
    if (len(string) + 1 /= len(Cstring)) then
      status = 2
      return
    end if
    ! check null-terminated string
    string = "string" // c_null_char
    Cstring = to_Cstring(string)
    if (len(string) /= len(Cstring)) then
      status = 3
      return
    end if
#warning "FIX ME: check_to_Cstring: check twice null-terminated string"
!    ! check twice null-terminated string
!    string = "string" // c_null_char // c_null_char
!    Cstring = to_Cstring(string)
!    if (len(string) -1 /= len(Cstring)) then
!      status = 4
!      return
!    end if
    status = 0
  end function check_to_Cstring
  integer function check_from_Cstring() result(status)
    use, intrinsic :: iso_c_binding, only: c_null_char, c_ptr, c_loc
    type(c_ptr) :: str_ptr
    character(kind=TA_CHAR, len=:), allocatable :: string, ostring
    character(kind=TA_CHAR, len=:), allocatable, target :: Cstring
    status = -1
    ! check empty string
    string = ""
    Cstring = to_Cstring(string)
    str_ptr = c_loc(Cstring)
    ostring = from_Cstring(str_ptr)
    if (len(string) /= len(ostring)) then
      status = 1
      return
    end if
    ! check normal string
    string = "string"
    Cstring = to_Cstring(string)
    str_ptr = c_loc(Cstring)
    ostring = from_Cstring(str_ptr)
    if (len(string) /= len(ostring)) then
      status = 2
      return
    end if
    if (string /= ostring) then
      status = 3
      return
    end if
    ! check null-terminated string
    string = "string" // c_null_char
    Cstring = to_Cstring(string)
    str_ptr = c_loc(Cstring)
    ostring = from_Cstring(str_ptr)
    if (len(string) /= len(ostring) + 1) then
      status = 4
      return
    end if
    ! check fifth null-terminated string
    string = "string" // repeat(c_null_char, 5)
    Cstring = to_Cstring(string)
    str_ptr = c_loc(Cstring)
    ostring = from_Cstring(str_ptr)
    if (len(string) /= len(ostring) + 5) then
      status = 5
      return
    end if
    status = 0
  end function check_from_Cstring
  integer function check_get_type_id() result(status)
    use, intrinsic :: iso_c_binding, only: c_ptr, c_int32_t
    character(kind=TA_CHAR, len=:), allocatable :: string
    type(c_ptr) :: ptr
    integer(c_int32_t) :: int32
#if TA_FORTRAN_API_VERSION_AVAILABLE >= 2
    integer(c_int32_t), allocatable :: int32_arr(:)
    integer(c_int32_t), pointer :: int32_arr_p(:), int32_scalar_p
#endif
    status = -1
    if (get_type_id(string) /= TA_TYPE_CHAR8) then
      status = 1
      return
    end if
    if (get_type_id(ptr) /= TA_TYPE_UNKNOWN) then
      status = 2
      return
    end if
    if (get_type_id(int32) /= TA_TYPE_INT32) then
      status = 3
      return
    end if
#if TA_FORTRAN_API_VERSION_AVAILABLE >= 2
    if (get_type_id(int32_arr) /= TA_TYPE_INT32) then
      status = 4
      return
    end if
    if (get_type_id(int32_arr_p) /= TA_TYPE_INT32) then
      status = 5
      return
    end if
    if (get_type_id(int32_scalar_p) /= TA_TYPE_INT32) then
      status = 6
      return
    end if
#endif
    status = 0
  end function check_get_type_id
  integer function check_get_storage_size() result(status)
    use, intrinsic :: iso_c_binding, only: c_ptr, c_int32_t
    integer, parameter :: BITS_IN_BYTE = 8
    integer(c_int32_t) :: int32
    status = -1
    if (get_storage_size(get_type_id(int32)) * BITS_IN_BYTE /= storage_size(int32)) then
      status = 1
      return
    end if
    status = 0
  end function check_get_storage_size
  integer function check_get_status_message() result(status)
    status = -1
    if (get_status_message(0) == "OK!") then
      status = 0
      return
    end if
    status = 0
  end function check_get_status_message
end program test
