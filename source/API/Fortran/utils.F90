module tagarray_utils
  use, intrinsic :: iso_c_binding, only: c_int8_t, c_int16_t, c_int32_t, c_int64_t, &
                                         c_float, c_double, c_float_complex, c_double_complex, &
                                         c_ptr, c_loc
  use tagarray_defines
  use tagarray_CAPI, only: TA_get_status_message, TA_string_delete
  implicit none
  private
  interface to_Cstring
    module procedure :: str_to_Cstr
  end interface to_Cstring
  interface from_Cstring
    module procedure :: Cstr_to_str
  end interface from_Cstring
  interface get_type_id
    module procedure :: get_type_id_scalar
#if TA_FORTRAN_API_VERSION_AVAILABLE >= 2
    module procedure :: get_type_id_array
#endif
  end interface get_type_id
  public to_Cstring, from_Cstring, get_type_id, get_storage_size, get_status_message
contains
  function str_to_Cstr(string) result(Cstring)
    character(kind=TA_CHAR, len=*), intent(in) :: string
    character(kind=TA_CHAR, len=:), allocatable :: Cstring
    integer :: last
    Cstring = trim(adjustl(string))
    last = len(Cstring)
    if (last == 0) then
      deallocate(Cstring)
      allocate(character(kind=TA_CHAR, len=1) :: Cstring)
      Cstring(1:1) = char(0, kind=TA_CHAR)
      last = 1
    end if
    if (Cstring(last:last) /= char(0, kind=TA_CHAR)) then
      Cstring = Cstring // char(0, kind=TA_CHAR)
    end if
  end function str_to_Cstr
  function Cstr_to_str(Cstring) result(string)
    use, intrinsic :: iso_c_binding, only: c_char, c_f_pointer, c_null_char
    type(c_ptr), intent(in) :: Cstring
    character(kind=TA_CHAR, len=:), allocatable :: string
    character(len=1, kind=c_char), pointer :: chars(:)
    integer :: i, n
    call c_f_pointer(Cstring, chars, [huge(0)])
    i = 1
    do
      if (chars(i) == C_NULL_CHAR) exit
      i = i + 1
    end do
    n = i - 1
    allocate(character(len=n) :: string)
    do i = 1, n
      string(i:i) = chars(i)
    end do
  end function Cstr_to_str
#if TA_FORTRAN_API_VERSION_AVAILABLE >= 2
  integer(c_int32_t) function get_type_id_array(values) result(type_id)
    class(*), target, intent(in) :: values(*)
    class(*), pointer :: value
    value => values(1)
    type_id = get_type_id_scalar(value)
  end function get_type_id_array
#endif
  integer(c_int32_t) function get_type_id_scalar(value) result(type_id)
    class(*), target, intent(in) :: value
    type_id = TA_TYPE_UNKNOWN
    select type (value)
      type is (integer(c_int8_t))
        type_id = TA_TYPE_INT8
      type is (integer(c_int16_t))
        type_id = TA_TYPE_INT16
      type is (integer(c_int32_t))
        type_id = TA_TYPE_INT32
      type is (integer(c_int64_t))
        type_id = TA_TYPE_INT64
      type is (real(c_float))
        type_id = TA_TYPE_REAL32
      type is (real(c_double))
        type_id = TA_TYPE_REAL64
      type is (complex(c_float_complex))
        type_id = TA_TYPE_CMPLX32
      type is (complex(c_double_complex))
        type_id = TA_TYPE_CMPLX64
      type is (character(kind=TA_CHAR, len=*))
        type_id = TA_TYPE_CHAR8
      class default
        type_id = TA_TYPE_UNKNOWN
    end select
  end function get_type_id_scalar
  integer(c_int64_t) function get_storage_size(datatype) result(size)
    integer(c_int32_t) :: datatype
    select case(datatype)
      case (TA_TYPE_UNKNOWN)
        size = 1
      case (TA_TYPE_INT8)
        size = 1
      case (TA_TYPE_INT16)
        size = 2
      case (TA_TYPE_INT32)
        size = 4
      case (TA_TYPE_INT64)
        size = 8
      case (TA_TYPE_UINT8)
        size = 1
      case (TA_TYPE_UINT16)
        size = 2
      case (TA_TYPE_UINT32)
        size = 4
      case (TA_TYPE_UINT64)
        size = 8
      case (TA_TYPE_REAL32)
        size = 4
      case (TA_TYPE_REAL64)
        size = 8
      case (TA_TYPE_CMPLX32)
        size = 8
      case (TA_TYPE_CMPLX64)
        size = 16
      case default
        size = 1
    end select
  end function get_storage_size
  function get_status_message(status, tag) result(message)
    integer(c_int32_t), intent(in) :: status
    character(kind=TA_CHAR, len=*), optional, intent(in) :: tag
    character(kind=TA_CHAR, len=:), allocatable :: Ctag, message
    type(c_ptr) :: message_ptr
    Ctag = to_Cstring("")
    if (present(tag)) Ctag = to_Cstring(tag)
    message_ptr = TA_get_status_message(status, Ctag)
    message = from_Cstring(message_ptr)
    call TA_string_delete(message_ptr)
  end function get_status_message
end module tagarray_utils
