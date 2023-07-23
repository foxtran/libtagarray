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
    procedure, public  :: has_record
    procedure, public  :: has_records
    procedure, public  :: get_record
    procedure, public  :: remove_record
    procedure, public  :: remove_records
    procedure, public  :: dump
    procedure, public  :: delete => container_t_delete
  end type
contains
  subroutine container_t_new(this, comment)
    class(container_t), intent(inout) :: this
    character(kind=TA_CHAR, len=*), optional :: comment
    character(kind=TA_CHAR, len=:), allocatable :: C_comment
    type(C_ptr) :: ptr
    if (present(comment)) then
      C_comment = to_Cstring(comment)
    else
      C_comment = to_Cstring(TA_CHAR_"")
    end if
    ptr = TA_Container_new(C_comment)
    this%container_ptr = ptr
  end subroutine container_t_new
  integer(c_int32_t) function add_record(this, tag, record) result(status)
    class(container_t), intent(inout) :: this
    character(kind=TA_CHAR, len=*), intent(in) :: tag
    type(record_t), intent(in) :: record
    character(kind=TA_CHAR, len=:), allocatable :: Ctag
    Ctag = to_Cstring(tag)
    status = TA_Container_add_record(this%container_ptr, Ctag, record%record_ptr)
  end function add_record
  integer(c_int32_t) function add_record_data(this, tag, type_id, data_ptr, data_el_size, array_size, array_shape, comment) &
                                                                                                               result(status)
    class(container_t), intent(inout) :: this
    character(kind=TA_CHAR, len=*),           intent(in) :: tag
    integer(c_int32_t),                             intent(in) :: type_id
    type(c_ptr),                                    intent(in) :: data_ptr
    integer(c_int64_t),                             intent(in) :: data_el_size
    integer(c_int64_t),                             intent(in) :: array_size
    integer(c_int64_t),                   optional, intent(in) :: array_shape(:)
    character(kind=TA_CHAR, len=*), optional, intent(in) :: comment
    !
    type(record_t) :: record
    character(kind=TA_CHAR, len=:), allocatable :: Ctag
    Ctag = to_Cstring(tag)
    call record%new(type_id, data_ptr, data_el_size, array_size, array_shape, comment)
    status = this%add_record(Ctag, record)
  end function add_record_data
  integer(c_int32_t) function reserve_data(this, tag, datatype, array_size, array_shape, comment, override) result(status)
    class(container_t), intent(inout) :: this
    character(kind=TA_CHAR, len=*),           intent(in) :: tag
    integer(c_int32_t),                             intent(in) :: datatype
    integer(c_int64_t),                             intent(in) :: array_size
    integer(c_int64_t),                   optional, intent(in) :: array_shape(:)
    character(kind=TA_CHAR, len=*), optional, intent(in) :: comment
    logical(1), optional, intent(in) :: override
    !
    type(record_t) :: record
    character(kind=TA_CHAR, len=:), allocatable :: Ctag
    Ctag = to_Cstring(tag)
    if (present(override)) then
      if (override) then
        status = this%remove_record(Ctag)
      end if
    end if
    call record%reserve(datatype, array_size, array_shape, comment)
    status = this%add_record(Ctag, record)
  end function reserve_data
  integer(c_int32_t) function has_record(this, tag) result(status)
    class(container_t),                   intent(inout) :: this
    character(kind=TA_CHAR, len=*), intent(in)    :: tag
    character(kind=TA_CHAR, len=:), allocatable   :: Ctag
    Ctag = to_Cstring(tag)
    status = TA_Container_has_record(this%container_ptr, Ctag)
  end function has_record
  integer(c_int32_t) function has_records(this, tags, id) result(status)
    class(container_t),                   intent(inout) :: this
    character(kind=TA_CHAR, len=*), intent(in)    :: tags(:)
    character(kind=TA_CHAR, len=:), allocatable   :: Ctag
    integer(c_int32_t), optional, intent(out) :: id
    integer :: i
    if (present(id)) id = 0
    do i = 1, size(tags)
      if (present(id)) id = i
      Ctag = to_Cstring(tags(i))
      status = TA_Container_has_record(this%container_ptr, Ctag)
      if (status /= TA_OK) return
    end do
  end function has_records
  type(record_t) function get_record(this, tag) result(record)
    use, intrinsic :: iso_c_binding, only: c_associated
    class(container_t),                   intent(inout) :: this
    character(kind=TA_CHAR, len=*), intent(in)    :: tag
    character(kind=TA_CHAR, len=:), allocatable   :: Ctag
    type(C_ptr) :: record_ptr
    Ctag = to_Cstring(tag)
    record_ptr = TA_Container_get_record(this%container_ptr, Ctag)
    print *, c_associated(record_ptr)
    record%record_ptr = record_ptr
  end function get_record
  integer(c_int32_t) function remove_record(this, tag) result(status)
    class(container_t),                   intent(inout) :: this
    character(kind=TA_CHAR, len=*), intent(in)    :: tag
    character(kind=TA_CHAR, len=:), allocatable   :: Ctag
    Ctag = to_Cstring(tag)
    status = TA_container_remove_record(this%container_ptr, Ctag)
  end function remove_record
  integer(c_int32_t) function remove_records(this, tags) result(status)
    class(container_t),                   intent(inout) :: this
    character(kind=TA_CHAR, len=*), intent(in)    :: tags(:)
    character(kind=TA_CHAR, len=:), allocatable   :: Ctag
    integer :: i, status_
    status = TA_OK
    do i = 1, size(tags)
      Ctag = to_Cstring(tags(i))
      status_ = TA_container_remove_record(this%container_ptr, Ctag)
      if (status_ /= TA_OK) status = status_
    end do
  end function remove_records
  subroutine dump(this, level)
    class(container_t), intent(inout) :: this
    integer(c_int32_t), intent(in) :: level
    call TA_Container_dump(this%container_ptr, level)
  end subroutine dump
  subroutine container_t_delete(this)
    class(container_t), intent(inout) :: this
    call TA_Container_delete(this%container_ptr)
    this%container_ptr = c_null_ptr
  end subroutine container_t_delete
end module tagarray_container
