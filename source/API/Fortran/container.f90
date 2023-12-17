module tagarray_container
  use, intrinsic :: iso_c_binding, only: c_int32_t, c_ptr, c_f_pointer, c_null_ptr, c_associated
  use tagarray_defines
  use tagarray_CAPI
  use tagarray_utils
  use tagarray_recordinfo, only: recordinfo_t
  implicit none
  private
  type, public :: container_t
    type(c_ptr) :: container_ptr = c_null_ptr
  contains
    procedure, public  :: new => container_t_new
    procedure, public  :: delete => container_t_delete
    procedure, public  :: is_allocated => container_t_is_allocated
    procedure, public  :: create => container_t_create
    procedure, public  :: get => container_t_get
    procedure, private :: container_t_contains_s
    procedure, private :: container_t_contains_m
    procedure, private :: container_t_erase_s
    procedure, private :: container_t_erase_m
    generic,   public  :: contains => container_t_contains_s, container_t_contains_m
    generic,   public  :: erase => container_t_erase_s, container_t_erase_m
    procedure, public  :: clear => container_t_clear
    procedure, public  :: load => container_t_load
    procedure, public  :: save => container_t_save
  end type
contains
  subroutine container_t_new(this, description)
    class(container_t), intent(inout) :: this
    character(kind=TA_CHAR, len=*), optional :: description
    character(kind=TA_CHAR, len=:), allocatable :: C_desc
    if (c_associated(this%container_ptr)) call this%delete()
    C_desc = to_Cstring(description)
    this%container_ptr = TA_Container_new(C_desc)
  end subroutine container_t_new
  subroutine container_t_load(this, path)
    class(container_t), intent(inout) :: this
    character(kind=TA_CHAR, len=*), intent(in) :: path
    character(kind=TA_CHAR, len=:), allocatable :: C_path
    if (c_associated(this%container_ptr)) call this%delete()
    C_path = to_Cstring(path)
    this%container_ptr = TA_Container_load(C_path)
  end subroutine container_t_load
  integer(c_int32_t) function container_t_save(this, path) result(status)
    class(container_t), intent(inout) :: this
    character(kind=TA_CHAR, len=*), intent(in) :: path
    character(kind=TA_CHAR, len=:), allocatable :: C_path
    C_path = to_Cstring(path)
    status = TA_Container_save(this%container_ptr, C_path)
  end function container_t_save
  subroutine container_t_delete(this)
    class(container_t), intent(inout) :: this
    call TA_Container_delete(this%container_ptr)
    this%container_ptr = c_null_ptr
  end subroutine container_t_delete
  logical(c_bool) function container_t_is_allocated(this) result(status)
    class(container_t), intent(inout) :: this
    status = c_associated(this%container_ptr)
  end function container_t_is_allocated
  integer(c_int32_t) function container_t_create(this, tag, type_id, shape, data_ptr, description, override) result(status)
    class(container_t),                       intent(inout) :: this
    character(kind=TA_CHAR, len=*),           intent(in)    :: tag
    integer(c_int32_t),                       intent(in)    :: type_id
    integer(c_int64_t),                       intent(in)    :: shape(:)
    type(c_ptr),                    optional, intent(in)    :: data_ptr
    character(kind=TA_CHAR, len=*), optional, intent(in)    :: description
    logical,                        optional, intent(in)    :: override
    !
    character(kind=TA_CHAR, len=:), allocatable :: Ctag, Cdesc
    Ctag = to_Cstring(tag)
    Cdesc = to_Cstring(description)
    if (present(override)) then
      if (override) then
        call this%erase(Ctag)
      end if
    end if
    if (present(data_ptr)) then
      status = TA_Container_create(this%container_ptr, Ctag, type_id, size(shape, kind=4), shape, data_ptr, Cdesc)
    else
      status = TA_Container_create(this%container_ptr, Ctag, type_id, size(shape, kind=4), shape, c_null_ptr, Cdesc)
    end if
  end function container_t_create
  type(RecordInfo_t) function container_t_get(this, tag) result(ri)
    class(container_t),                       intent(inout) :: this
    character(kind=TA_CHAR, len=*),           intent(in)    :: tag
    character(kind=TA_CHAR, len=:), allocatable :: Ctag
    Ctag = to_Cstring(tag)
    ri = TA_Container_get(this%container_ptr, Ctag)
  end function container_t_get
  logical(c_bool) function container_t_contains_s(this, tag) result(status)
    class(container_t),                   intent(inout) :: this
    character(kind=TA_CHAR, len=*), intent(in)    :: tag
    character(kind=TA_CHAR, len=:), allocatable   :: Ctag
    Ctag = to_Cstring(tag)
    status = TA_Container_contains(this%container_ptr, Ctag)
  end function container_t_contains_s
  logical(c_bool) function container_t_contains_m(this, tags, id) result(status)
    class(container_t),                   intent(inout) :: this
    character(kind=TA_CHAR, len=*), intent(in)    :: tags(:)
    character(kind=TA_CHAR, len=:), allocatable   :: Ctag
    integer(c_int32_t), optional, intent(out) :: id
    integer :: i
    status = .true.
    if (present(id)) id = 0
    do i = 1, size(tags)
      if (present(id)) id = i
      Ctag = to_Cstring(tags(i))
      status = TA_Container_contains(this%container_ptr, Ctag)
      if (.not.status) return
    end do
  end function container_t_contains_m
  subroutine container_t_erase_s(this, tag)
    class(container_t),             intent(inout) :: this
    character(kind=TA_CHAR, len=*), intent(in)    :: tag
    character(kind=TA_CHAR, len=:), allocatable   :: Ctag
    Ctag = to_Cstring(tag)
    call TA_container_erase(this%container_ptr, Ctag)
  end subroutine container_t_erase_s
  subroutine container_t_erase_m(this, tags)
    class(container_t),             intent(inout) :: this
    character(kind=TA_CHAR, len=*), intent(in)    :: tags(:)
    character(kind=TA_CHAR, len=:), allocatable   :: Ctag
    integer :: i
    do i = 1, size(tags)
      Ctag = to_Cstring(tags(i))
      call TA_container_erase(this%container_ptr, Ctag)
    end do
  end subroutine container_t_erase_m
  subroutine container_t_clear(this)
    class(container_t),             intent(inout) :: this
    call TA_container_clear(this%container_ptr)
  end subroutine container_t_clear
end module tagarray_container
