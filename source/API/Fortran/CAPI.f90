module tagarray_CAPI
  use, intrinsic :: iso_c_binding, only: c_ptr, c_int32_t, c_int64_t, c_bool
  use tagarray_defines, only: TA_CHAR, TA_DIMENSIONS_LENGTH
  use tagarray_recordinfo, only: C_RecordInfo_t
  implicit none
  interface
    type(C_ptr) function TA_Container_new(Ccomment) bind(C, name="TA_Container_new")
      import
      implicit none
      character(kind=TA_CHAR), intent(in) :: Ccomment(*)
    end function TA_Container_new
    integer(c_int32_t) function TA_Container_get_status(container) bind(C, name="TA_Container_get_status")
      import
      type(c_ptr),        value, intent(in) :: container
    end function TA_Container_get_status
    subroutine TA_Container_dump(container, level) bind(C, name="TA_Container_dump")
      import
      implicit none
      type(c_ptr),        value, intent(in) :: container
      integer(c_int32_t), value, intent(in) :: level
    end subroutine TA_Container_dump
    subroutine TA_Container_delete(container) bind(C, name="TA_Container_delete")
      import
      implicit none
      type(c_ptr),        value, intent(in) :: container
    end subroutine TA_Container_delete
    type(C_ptr) function TA_Record_new(type_id, n_dimensions, data, data_length, dimensions, Ccomment) &
                                                                                                bind(C, name="TA_Record_new")
      import
      implicit none
      integer(c_int32_t), value,     intent(in) :: type_id
      integer(c_int32_t), value,     intent(in) :: n_dimensions
      type(c_ptr),        value,     intent(in) :: data
      integer(c_int64_t), value,     intent(in) :: data_length
      integer(c_int64_t),            intent(in) :: dimensions(TA_DIMENSIONS_LENGTH)
      character(kind=TA_CHAR), intent(in) :: Ccomment(*)
    end function TA_Record_new
    logical(C_bool) function TA_Record_is_allocated(record) bind(C, name="TA_Record_is_allocated")
      import
      implicit none
      type(c_ptr),        value, intent(in) :: record
    end function TA_Record_is_allocated
    subroutine TA_Record_dump(record, level) bind(C, name="TA_Record_dump")
      import
      implicit none
      type(c_ptr),        value, intent(in) :: record
      integer(c_int32_t), value, intent(in) :: level
    end subroutine TA_Record_dump
    subroutine TA_Record_delete(record) bind(C, name="TA_Record_delete")
      import
      implicit none
      type(c_ptr),        value, intent(in) :: record
    end subroutine TA_Record_delete
    subroutine TA_Container_add_record(container, Ctag, record) bind(C, name="TA_Container_add_record")
      import
      implicit none
      type(c_ptr), value, intent(in) :: container
      character(kind=TA_CHAR), intent(in) :: Ctag(*)
      type(c_ptr), value, intent(in) :: record
    end subroutine TA_Container_add_record
    integer(c_int32_t) function TA_Container_has_record(container, Ctag) bind(C, name="TA_Container_has_record")
      import
      implicit none
      type(c_ptr), value,            intent(in) :: container
      character(kind=TA_CHAR), intent(in) :: Ctag(*)
    end function TA_Container_has_record
    type(C_ptr) function TA_Container_get_record(container, Ctag) bind(C, name="TA_Container_get_record")
      import
      implicit none
      type(c_ptr), value,            intent(in) :: container
      character(kind=TA_CHAR), intent(in) :: Ctag(*)
    end function TA_Container_get_record
    subroutine TA_Container_remove_record(container, Ctag) bind(C, name="TA_Container_remove_record")
      import
      implicit none
      type(c_ptr), value,            intent(in) :: container
      character(kind=TA_CHAR), intent(in) :: Ctag(*)
    end subroutine TA_Container_remove_record
    type(C_RecordInfo_t) function TA_Record_get_record_info(record) bind(C, name="TA_Record_get_record_info")
      import
      implicit none
      type(c_ptr), value,            intent(in) :: record
    end function TA_Record_get_record_info
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
  end interface
end module tagarray_CAPI
