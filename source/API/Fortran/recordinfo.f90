module tagarray_recordinfo
  use, intrinsic :: iso_c_binding, only: c_ptr, c_int32_t, c_int64_t
  use tagarray_defines
  implicit none
  private
  type, bind(C) :: C_RecordInfo_t
    integer(c_int32_t) :: type_id
    integer(c_int32_t) :: n_dimensions
    type(c_ptr)        :: data
    integer(c_int64_t) :: data_length
    integer(c_int64_t) :: dimensions(TA_DIMENSIONS_LENGTH)
  end type C_RecordInfo_t
  type :: RecordInfo_t
    integer(c_int32_t) :: type_id
    integer(c_int32_t) :: n_dimensions
    type(c_ptr)        :: data
    integer(c_int64_t) :: data_length
    integer(c_int64_t) :: dimensions(TA_DIMENSIONS_LENGTH)
  contains
    procedure, private :: recordinfo_assign
    procedure, public  :: get_status
    generic,   public  :: assignment(=) => recordinfo_assign
  end type RecordInfo_t
!  interface assignment(=)
!    module procedure recordinfo_assign
!  end interface assignment(=)
  public :: C_RecordInfo_t, RecordInfo_t
contains
  subroutine recordinfo_assign(this, recordinfo)
    class(RecordInfo_t),   intent(out) :: this
    type(C_RecordInfo_t), intent(in)  :: recordinfo
    this%type_id = recordinfo%type_id
    this%n_dimensions = recordinfo%n_dimensions
    this%data = recordinfo%data
    this%data_length = recordinfo%data_length
    this%dimensions = recordinfo%dimensions
  end subroutine recordinfo_assign
  integer(c_int32_t) function get_status(this, ext_type_id, ext_ndim) result(status)
    class(RecordInfo_t), intent(in) :: this
    integer(c_int32_t), intent(in) :: ext_type_id, ext_ndim
    integer(c_int32_t), parameter :: WRONG_TYPE_ID     =   1
    integer(c_int32_t), parameter :: WRONG_DIMENSIONS  =  10
    if (ext_ndim > TA_DIMENSIONS_LENGTH) then
      status = TA_DATA_TOO_MANY_DIMENSIONS
      return
    end if
    if (any(this%dimensions == 0)) then
      status = TA_DATA_ZERO_LENGTH
      return
    end if
    status = TA_OK
    if (this%type_id /= ext_type_id) status = status + WRONG_TYPE_ID
    if (this%n_dimensions /= ext_ndim) status = status + WRONG_DIMENSIONS
    if (status == WRONG_TYPE_ID) then
      status = TA_DATA_INCORRECT_TYPE
    else if (status == WRONG_DIMENSIONS) then
      status = TA_DATA_INCORRECT_DIMENSIONS
    else if (status /= TA_OK) then
      status = TA_DATA_INCORRECT
    end if
  end function get_status
end module tagarray_recordinfo
