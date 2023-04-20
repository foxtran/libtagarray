module tagarray_container
  use, intrinsic :: iso_c_binding, only: c_int32_t, c_ptr, c_f_pointer, c_null_ptr
  use tagarray_defines
  use tagarray_CAPI
  use tagarray_utils
  use tagarray_recordinfo, only: recordinfo_t
  use tagarray_record
  implicit none
  private
  type, public :: container_t
    type(c_ptr) :: container_ptr = c_null_ptr
  contains
    procedure, public  :: new => container_t_new
    procedure, public  :: add_record
    procedure, public  :: add_record_data
    procedure, public  :: reserve_data
    procedure, public  :: get_record
    procedure, public  :: get_record_info
    procedure, public  :: remove_record
    procedure, public  :: dump
    procedure, public  :: get_status => container_t_get_status
    procedure, public  :: delete => container_t_delete
  end type
contains
  subroutine container_t_new(this, comment)
    class(container_t), intent(inout) :: this
    character(kind=TAGARRAY_CHAR, len=*), optional :: comment
    character(kind=TAGARRAY_CHAR, len=:), allocatable :: C_comment
    type(C_ptr) :: ptr
    if (present(comment)) then
      C_comment = to_Cstring(comment)
    else
      C_comment = to_Cstring(TAGARRAY_CHAR_"")
    end if
    ptr = TAGARRAY_new_container(C_comment)
    this%container_ptr = ptr
  end subroutine container_t_new
  subroutine add_record(this, record)
    class(container_t), intent(inout) :: this
    type(record_t), intent(in) :: record
    call TAGARRAY_add_record(this%container_ptr, record%record_ptr)
  end subroutine add_record
  subroutine add_record_data(this, tag, type_id, data_ptr, data_el_size, array_size, array_shape, options, comment)
    class(container_t), intent(inout) :: this
    character(kind=TAGARRAY_CHAR, len=*),           intent(in) :: tag
    integer(c_int32_t),                             intent(in) :: type_id
    type(c_ptr),                                    intent(in) :: data_ptr
    integer(c_int64_t),                             intent(in) :: data_el_size
    integer(c_int64_t),                             intent(in) :: array_size
    integer(c_int64_t),                   optional, intent(in) :: array_shape(:)
    integer(c_int64_t),                   optional, intent(in) :: options(TAGARRAY_OPTIONS_LENGTH)
    character(kind=TAGARRAY_CHAR, len=*), optional, intent(in) :: comment
    !
    type(record_t) :: record
    character(kind=TAGARRAY_CHAR, len=:), allocatable :: Ctag
    Ctag = to_Cstring(tag)
    call record%new(Ctag, type_id, data_ptr, data_el_size, array_size, array_shape, options, comment)
    call this%add_record(record)
  end subroutine add_record_data
  subroutine reserve_data(this, tag, datatype, array_size, array_shape, options, comment)
    class(container_t), intent(inout) :: this
    character(kind=TAGARRAY_CHAR, len=*),           intent(in) :: tag
    integer(c_int32_t),                             intent(in) :: datatype
    integer(c_int64_t),                             intent(in) :: array_size
    integer(c_int64_t),                   optional, intent(in) :: array_shape(:)
    integer(c_int64_t),                   optional, intent(in) :: options(TAGARRAY_OPTIONS_LENGTH)
    character(kind=TAGARRAY_CHAR, len=*), optional, intent(in) :: comment
    !
    type(record_t) :: record
    character(kind=TAGARRAY_CHAR, len=:), allocatable :: Ctag
    Ctag = to_Cstring(tag)
    call record%reserve(Ctag, datatype, array_size, array_shape, options, comment)
    call this%add_record(record)
  end subroutine reserve_data
  type(record_t) function get_record(this, tag) result(record)
    class(container_t),                   intent(inout) :: this
    character(kind=TAGARRAY_CHAR, len=*), intent(in)    :: tag
    character(kind=TAGARRAY_CHAR, len=:), allocatable   :: Ctag
    type(C_ptr) :: record_ptr
    Ctag = to_Cstring(tag)
    record_ptr = TAGARRAY_get_record(this%container_ptr, Ctag)
    record%record_ptr = record_ptr
  end function get_record
  type(recordinfo_t) function get_record_info(this, tag) result(recordinfo)
    class(container_t),                   intent(inout) :: this
    character(kind=TAGARRAY_CHAR, len=*), intent(in)    :: tag
    character(kind=TAGARRAY_CHAR, len=:), allocatable   :: Ctag
    type(record_t) :: record
    integer :: status
    Ctag = to_Cstring(tag)
    record = this%get_record(Ctag)
    status = this%get_status()
    if (status /= TAGARRAY_OK) return
    recordinfo = record%get_info()
  end function get_record_info
  subroutine remove_record(this, tag)
    class(container_t),                   intent(inout) :: this
    character(kind=TAGARRAY_CHAR, len=*), intent(in)    :: tag
    character(kind=TAGARRAY_CHAR, len=:), allocatable   :: Ctag
    Ctag = to_Cstring(tag)
    call TAGARRAY_container_remove_record(this%container_ptr, Ctag)
  end subroutine remove_record
  integer(c_int32_t) function container_t_get_status(this) result(status)
    class(container_t), intent(inout) :: this
    status = TAGARRAY_get_container_status(this%container_ptr)
  end function container_t_get_status
  subroutine dump(this, level)
    class(container_t), intent(inout) :: this
    integer(c_int32_t), intent(in) :: level
    call TAGARRAY_dump_container(this%container_ptr, level)
  end subroutine dump
  subroutine container_t_delete(this)
    class(container_t), intent(inout) :: this
    call TAGARRAY_delete_container(this%container_ptr)
    this%container_ptr = c_null_ptr
  end subroutine container_t_delete
end module tagarray_container
