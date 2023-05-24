module tagarray_record
  use, intrinsic :: iso_c_binding, only: c_ptr, c_loc, c_sizeof, c_f_pointer, c_null_ptr
  use tagarray_defines
  use tagarray_recordinfo
  use tagarray_CAPI
  use tagarray_utils
  implicit none
  private
  type, public :: record_t
    type(c_ptr) :: record_ptr
  contains
    procedure, public :: new => record_t_new
    procedure, public :: reserve => record_t_reserve
    procedure, public :: get_info
    procedure, public :: get_status
    procedure, public :: dump
    procedure, public :: delete => record_t_delete
  end type
contains
  subroutine record_t_new(this, type_id, data_ptr, data_el_size, array_size, array_shape, options, comment)
    class(record_t), intent(inout) :: this
    integer(c_int32_t),                             intent(in) :: type_id
    type(c_ptr),                                    intent(in) :: data_ptr
    integer(c_int64_t),                             intent(in) :: data_el_size
    integer(c_int64_t),                             intent(in) :: array_size
    integer(c_int64_t),                   optional, intent(in) :: array_shape(:)
    integer(c_int64_t),                   optional, intent(in) :: options(TA_OPTIONS_LENGTH)
    character(kind=TA_CHAR, len=*), optional, intent(in) :: comment
    !
    integer(c_int64_t) :: data_length, options_(TA_OPTIONS_LENGTH), dimensions_(TA_DIMENSIONS_LENGTH)
    integer(c_int32_t) :: n_dimensions
    character(kind=TA_CHAR, len=:), allocatable :: Ccomment
    if (present(array_shape)) then
      n_dimensions = size(array_shape)
      data_length = product(array_shape)
!      if (data_length /= array_size)
    else
      n_dimensions = 1
      data_length = array_size
    end if
    data_length = data_length * data_el_size
    if (present(options)) then
      options_ = options
    else
      options_ = TA_OPTIONS_ZERO
    end if
    dimensions_ = TA_DIMENSIONS_ZERO
    if (n_dimensions > 1) then
      dimensions_(1:n_dimensions) = array_shape
    else
      dimensions_(1) = array_size
    end if
    if (present(comment)) then
      Ccomment = to_Cstring(comment)
    else
      Ccomment = to_Cstring(TA_CHAR_"")
    end if
    this%record_ptr = TA_Record_new(type_id, n_dimensions, data_ptr, data_length, dimensions_, options_, Ccomment)
  end subroutine record_t_new
  subroutine record_t_reserve(this, datatype, array_size, array_shape, options, comment)
    class(record_t), intent(inout) :: this
    integer(c_int32_t),                             intent(in) :: datatype
    integer(c_int64_t),                             intent(in) :: array_size
    integer(c_int64_t),                   optional, intent(in) :: array_shape(:)
    integer(c_int64_t),                   optional, intent(in) :: options(TA_OPTIONS_LENGTH)
    character(kind=TA_CHAR, len=*), optional, intent(in) :: comment
    !
    integer(c_int64_t) :: data_length, options_(TA_OPTIONS_LENGTH), dimensions_(TA_DIMENSIONS_LENGTH)
    integer(c_int32_t) :: n_dimensions
    character(kind=TA_CHAR, len=:), allocatable :: Ccomment
    if (present(array_shape)) then
      n_dimensions = size(array_shape)
    else
      n_dimensions = 1
    end if
    data_length = array_size * get_storage_size(datatype)
    if (present(options)) then
      options_ = options
    else
      options_ = TA_OPTIONS_ZERO
    end if
    dimensions_ = TA_DIMENSIONS_ZERO
    if (n_dimensions > 1) then
      dimensions_(1:n_dimensions) = array_shape
    else
      dimensions_(1) = array_size
    end if
    if (present(comment)) then
      Ccomment = to_Cstring(comment)
    else
      Ccomment = to_Cstring(TA_CHAR_"")
    end if
    this%record_ptr = TA_Record_new(datatype, n_dimensions, c_null_ptr, data_length, dimensions_, options_, Ccomment)
  end subroutine record_t_reserve
  type(RecordInfo_t) function get_info(this) result(recordinfo)
    class(record_t), intent(inout) :: this
    recordinfo =  TA_Record_get_record_info(this%record_ptr)
  end function get_info
  integer(c_int32_t) function get_status(this) result(status)
    class(record_t), intent(inout) :: this
    status = TA_Record_get_status(this%record_ptr)
  end function get_status
  subroutine dump(this, level)
    class(record_t), intent(inout) :: this
    integer(c_int32_t), intent(in) :: level
    call TA_record_dump(this%record_ptr, level)
  end subroutine dump
  subroutine record_t_delete(this)
    class(record_t), intent(inout) :: this
    call TA_Record_delete(this%record_ptr)
    this%record_ptr = c_null_ptr
  end subroutine record_t_delete
end module tagarray_record
