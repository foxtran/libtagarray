module tagarray_CAPI
  use, intrinsic :: iso_c_binding, only: c_ptr, c_int32_t, c_int64_t, c_bool
  use tagarray_defines, only: TA_CHAR
  use tagarray_Crecordinfo, only: C_RecordInfo_t
  implicit none
  interface
    type(C_ptr) function TA_Container_new(Ccomment) bind(C, name="TA_Container_new")
      import
      implicit none
      character(kind=TA_CHAR), intent(in) :: Ccomment(*)
    end function TA_Container_new
    type(C_ptr) function TA_Container_load(Cpath) bind(C, name="TA_Container_load")
      import
      implicit none
      character(kind=TA_CHAR), intent(in) :: Cpath(*)
    end function TA_Container_load
    subroutine TA_Container_delete(container) bind(C, name="TA_Container_delete")
      import
      implicit none
      type(c_ptr),        value, intent(in) :: container
    end subroutine TA_Container_delete
    integer(c_int32_t) function TA_Container_create(container, Ctag, type_id, ndims, dims, data_ptr, Cdesc) &
                                              bind(C, name="TA_Container_create")
      import
      implicit none
      type(c_ptr),        value, intent(in) :: container
      character(kind=TA_CHAR),   intent(in) :: Ctag(*)
      integer(c_int32_t), value, intent(in) :: type_id
      integer(c_int32_t), value, intent(in) :: ndims
      integer(c_int64_t),        intent(in) :: dims(*)
      type(c_ptr),        value, intent(in) :: data_ptr
      character(kind=TA_CHAR),   intent(in) :: Cdesc(*)
    end function TA_Container_create
    logical(c_bool) function TA_Container_contains(container, Ctag) bind(C, name="TA_Container_contains")
      import
      implicit none
      type(c_ptr), value,            intent(in) :: container
      character(kind=TA_CHAR), intent(in) :: Ctag(*)
    end function TA_Container_contains
    type(C_RecordInfo_t) function TA_Container_get(container, Ctag) bind(C, name="TA_Container_get")
      import
      implicit none
      type(c_ptr), value,            intent(in) :: container
      character(kind=TA_CHAR), intent(in) :: Ctag(*)
    end function TA_Container_get
    integer(c_int32_t) function TA_Container_append(container, Ctag, ndims, dims, data_ptr) &
                                              bind(C, name="TA_Container_append")
      import
      implicit none
      type(c_ptr),        value, intent(in) :: container
      character(kind=TA_CHAR),   intent(in) :: Ctag(*)
      integer(c_int32_t), value, intent(in) :: ndims
      integer(c_int64_t),        intent(in) :: dims(*)
      type(c_ptr),        value, intent(in) :: data_ptr
    end function TA_Container_append
    subroutine TA_Container_erase(container, Ctag) bind(C, name="TA_Container_erase")
      import
      implicit none
      type(c_ptr), value,            intent(in) :: container
      character(kind=TA_CHAR), intent(in) :: Ctag(*)
    end subroutine TA_Container_erase
    subroutine TA_Container_clear(container) bind(C, name="TA_Container_clear")
      import
      implicit none
      type(c_ptr), value,            intent(in) :: container
    end subroutine TA_Container_clear
    integer(c_int32_t) function TA_Container_save(container, Cpath) bind(C, name="TA_Container_save")
      import
      implicit none
      type(c_ptr), value,            intent(in) :: container
      character(kind=TA_CHAR), intent(in) :: Cpath(*)
    end function TA_Container_save
    type(C_ptr) function TA_get_status_message(status, Ctag) bind(C, name="TA_get_status_message")
      import
      implicit none
      integer(c_int32_t), value, intent(in) :: status
      character(kind=TA_CHAR), intent(in) :: Ctag(*)
    end function TA_get_status_message
    subroutine TA_string_delete(ptr) bind(C, name="TA_string_delete")
      import
      implicit none
      type(c_ptr), value,            intent(in) :: ptr
    end subroutine TA_string_delete
    integer(c_int32_t) function TA_get_storage_size(datatype) bind(C, name="TA_get_storage_size")
      import
      implicit none
      integer(c_int32_t), value :: datatype
    end function TA_get_storage_size
  end interface
end module tagarray_CAPI
