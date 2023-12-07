module tagarray_recordinfo
  use, intrinsic :: iso_c_binding, only: c_ptr, c_int32_t, c_int64_t, c_null_ptr, c_associated
  use tagarray_defines
  use tagarray_Crecordinfo
  implicit none
  private
  type :: RecordInfo_t
    integer(c_int32_t)          :: type_id = 0
    integer(c_int32_t)          :: itemsize = 0
    integer(c_int64_t)          :: count = 0
    integer(c_int64_t)          :: ndims = 0
    integer(c_int64_t), pointer :: dims(:) => null()
    type(c_ptr)                 :: data = c_null_ptr
    character(kind=TA_CHAR, len=:), allocatable :: description
  contains
    procedure, private :: recordinfo_assign
    generic,   public  :: assignment(=) => recordinfo_assign
    procedure, public  :: get_status
    procedure, public  :: is_associated
  end type RecordInfo_t
  public :: RecordInfo_t
contains
  subroutine recordinfo_assign(this, recordinfo)
    use tagarray_utils, only: from_Cstring
    use, intrinsic :: iso_c_binding, only: c_f_pointer
    class(RecordInfo_t),   intent(out) :: this
    type(C_RecordInfo_t),  intent(in)  :: recordinfo
    this%type_id = recordinfo%type_id
    this%itemsize = recordinfo%itemsize
    this%count = recordinfo%count
    this%ndims = recordinfo%ndims
    if (c_associated(recordinfo%dims)) &
      call c_f_pointer(recordinfo%dims, this%dims, [this%ndims])
    this%data = recordinfo%data
    this%description = from_Cstring(recordinfo%description)
  end subroutine recordinfo_assign
  logical function is_associated(this) result(lstat)
    class(RecordInfo_t),   intent(out) :: this
    lstat = c_associated(this%data)
  end function is_associated
  integer(c_int32_t) function get_status(this, ext_type_id, ext_ndim) result(status)
    class(RecordInfo_t),   intent(in) :: this
    integer(c_int32_t),    intent(in) :: ext_type_id, ext_ndim
    integer(c_int32_t), parameter :: WRONG_TYPE_ID     =   1
    integer(c_int32_t), parameter :: WRONG_DIMENSIONS  =  10
    if (ext_ndim > TA_MAX_DIMENSIONS_LENGTH) then
      status = TA_DATA_TOO_MANY_DIMENSIONS
      return
    end if
    if (any(this%dims == 0)) then
      status = TA_DATA_ZERO_LENGTH
      return
    end if
    status = TA_OK
    if (this%type_id /= ext_type_id) status = status + WRONG_TYPE_ID
    if (this%ndims /= ext_ndim) status = status + WRONG_DIMENSIONS
    if (status == WRONG_TYPE_ID) then
      status = TA_DATA_INCORRECT_TYPE
    else if (status == WRONG_DIMENSIONS) then
      status = TA_DATA_INCORRECT_DIMENSIONS
    else if (status /= TA_OK) then
      status = TA_DATA_INCORRECT
    end if
  end function get_status
end module tagarray_recordinfo
