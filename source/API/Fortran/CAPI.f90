module tagarray_CAPI
  use, intrinsic :: iso_c_binding, only: c_ptr, c_int32_t, c_int64_t
  use tagarray_defines, only: TAGARRAY_CHAR, TAGARRAY_DIMENSIONS_LENGTH, TAGARRAY_OPTIONS_LENGTH
  use tagarray_recordinfo, only: C_RecordInfo_t
  implicit none
  interface
    type(C_ptr) function TAGARRAY_new_container(Ccomment) bind(C, name="new_container")
      import
      implicit none
      character(kind=TAGARRAY_CHAR), intent(in) :: Ccomment(*)
    end function TAGARRAY_new_container
    integer(c_int32_t) function TAGARRAY_get_container_status(container) bind(C, name="get_container_status")
      import
      type(c_ptr),        value, intent(in) :: container
    end function TAGARRAY_get_container_status
    subroutine TAGARRAY_dump_container(container, level) bind(C, name="dump_container")
      import
      implicit none
      type(c_ptr),        value, intent(in) :: container
      integer(c_int32_t), value, intent(in) :: level
    end subroutine TAGARRAY_dump_container
    subroutine TAGARRAY_delete_container(container) bind(C, name="delete_container")
      import
      implicit none
      type(c_ptr),        value, intent(in) :: container
    end subroutine TAGARRAY_delete_container
    type(C_ptr) function TAGARRAY_new_record(Ctag, type_id, n_dimensions, data, data_length, dimensions, options, Ccomment) &
                                                                                                          bind(C, name="new_record")
      import
      implicit none
      character(kind=TAGARRAY_CHAR), intent(in) :: Ctag(*)
      integer(c_int32_t), value,     intent(in) :: type_id
      integer(c_int32_t), value,     intent(in) :: n_dimensions
      type(c_ptr),        value,     intent(in) :: data
      integer(c_int64_t), value,     intent(in) :: data_length
      integer(c_int64_t),            intent(in) :: dimensions(TAGARRAY_DIMENSIONS_LENGTH)
      integer(c_int64_t),            intent(in) :: options(TAGARRAY_OPTIONS_LENGTH)
      character(kind=TAGARRAY_CHAR), intent(in) :: Ccomment(*)
    end function TAGARRAY_new_record
    integer(c_int32_t) function TAGARRAY_get_record_status(record) bind(C, name="get_record_status")
      import
      type(c_ptr),        value, intent(in) :: record
    end function TAGARRAY_get_record_status
    subroutine TAGARRAY_dump_record(record, level) bind(C, name="dump_record")
      import
      implicit none
      type(c_ptr),        value, intent(in) :: record
      integer(c_int32_t), value, intent(in) :: level
    end subroutine TAGARRAY_dump_record
    subroutine TAGARRAY_delete_record(record) bind(C, name="delete_record")
      import
      implicit none
      type(c_ptr),        value, intent(in) :: record
    end subroutine TAGARRAY_delete_record
    subroutine TAGARRAY_add_record(container, record) bind(C, name="add_record")
      import
      implicit none
      type(c_ptr), value, intent(in) :: container
      type(c_ptr), value, intent(in) :: record
    end subroutine TAGARRAY_add_record
    type(C_ptr) function TAGARRAY_get_record(container, Ctag) bind(C, name="get_record")
      import
      implicit none
      type(c_ptr), value,            intent(in) :: container
      character(kind=TAGARRAY_CHAR), intent(in) :: Ctag(*)
    end function TAGARRAY_get_record
    subroutine TAGARRAY_container_remove_record(container, Ctag) bind(C, name="container_remove_record")
      import
      implicit none
      type(c_ptr), value,            intent(in) :: container
      character(kind=TAGARRAY_CHAR), intent(in) :: Ctag(*)
    end subroutine TAGARRAY_container_remove_record
    type(C_RecordInfo_t) function TAGARRAY_get_record_info(record) bind(C, name="get_record_info")
      import
      implicit none
      type(c_ptr), value,            intent(in) :: record
    end function TAGARRAY_get_record_info
  end interface
end module tagarray_CAPI
