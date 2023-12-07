module tagarray_Crecordinfo
  use, intrinsic :: iso_c_binding, only: c_ptr, c_int32_t, c_int64_t
  use tagarray_defines
  implicit none
  private
  type, bind(C) :: C_RecordInfo_t
    integer(c_int32_t) :: type_id
    integer(c_int32_t) :: itemsize
    integer(c_int64_t) :: count
    integer(c_int64_t) :: ndims
    type(c_ptr)        :: dims
    type(c_ptr)        :: data
    type(c_ptr)        :: description
  end type C_RecordInfo_t
  public :: C_RecordInfo_t
end module tagarray_Crecordinfo
