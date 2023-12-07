module tagarray_Crecordinfo
  use, intrinsic :: iso_c_binding, only: c_ptr, c_int32_t, c_int64_t, c_null_ptr
  use tagarray_defines
  implicit none
  private
  type, bind(C) :: C_RecordInfo_t
    integer(c_int32_t) :: type_id = 0
    integer(c_int32_t) :: itemsize = 0
    integer(c_int64_t) :: count = 0
    integer(c_int64_t) :: ndims = 0
    type(c_ptr)        :: dims = c_null_ptr
    type(c_ptr)        :: data = c_null_ptr
    type(c_ptr)        :: description = c_null_ptr
  end type C_RecordInfo_t
  public :: C_RecordInfo_t
end module tagarray_Crecordinfo
