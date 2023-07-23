#include "tagarray_assert.fh"

program test
  use tagarray
  use, intrinsic :: iso_c_binding, only: c_int32_t
  implicit none
  TEST(check_new_delete)
  TEST(check_add_record)
  TEST(check_add_record_data)
  TEST(check_reserve_data)
  TEST(check_has_record)
  TEST(check_get_record)
contains
  integer function check_new_delete() result(status)
    type(container_t) :: container
    status = -1
    call container%new()
    call container%delete()
    call container%new("Comment")
    call container%delete()
    status = 0
  end function check_new_delete
  integer(c_int32_t) function check_add_record() result(status)
    use, intrinsic :: iso_c_binding, only: c_null_ptr, c_int64_t
    type(container_t) :: container
    type(record_t) :: record
    status = -1
    call container%new()
    call record%new(TA_TYPE_INT32, c_null_ptr, 4_c_int64_t, 0_c_int64_t)
    status = container%add_record("EMPTY", record)
    if (status /= TA_OK) then
      status = 2
      return
    end if
    status = container%add_record("EMPTY", record)
    if (status /= TA_CONTAINER_RECORD_EXISTS) then
      status = 3
      return
    end if
    call record%delete()
    status = container%add_record("NULLPTR", record)
    if (status /= TA_OK) then
      status = 4
      return
    end if
    call container%delete()
    status = 0
  end function check_add_record
  integer(c_int32_t) function check_add_record_data() result(status)
    use, intrinsic :: iso_c_binding, only: c_null_ptr, c_int64_t
    type(container_t) :: container
    status = -1
    call container%new()
    status = container%add_record_data("TEST", TA_TYPE_INT32, c_null_ptr, 4_c_int64_t, 1_c_int64_t, (/ 1_c_int64_t, 1_c_int64_t /), comment = "comment")
    if (status /= TA_OK) then
      status = 2
      return
    end if
    status = container%add_record_data("TEST", TA_TYPE_INT32, c_null_ptr, 4_c_int64_t, 1_c_int64_t, (/ 1_c_int64_t, 1_c_int64_t /))
    if (status /= TA_CONTAINER_RECORD_EXISTS) then
      status = 3
      return
    end if
    call container%delete()
    status = 0
  end function check_add_record_data
  integer(c_int32_t) function check_reserve_data() result(status)
    use, intrinsic :: iso_c_binding, only: c_null_ptr, c_int64_t
    type(container_t) :: container
    status = -1
    call container%new()
    status = container%reserve_data("TEST", TA_TYPE_INT32, 1_c_int64_t, (/ 1_c_int64_t, 1_c_int64_t /), comment = "comment")
    if (status /= TA_OK) then
      status = 2
      return
    end if
    status = container%reserve_data("TEST", TA_TYPE_INT32, 1_c_int64_t, (/ 1_c_int64_t, 1_c_int64_t /))
    if (status /= TA_CONTAINER_RECORD_EXISTS) then
      status = 3
      return
    end if
    call container%delete()
    status = 0
  end function check_reserve_data
  integer(c_int32_t) function check_has_record() result(status)
    use, intrinsic :: iso_c_binding, only: c_int32_t, c_int64_t
    type(container_t) :: container
    type(record_t) :: record
    integer(c_int32_t) :: id
    character(len=*), parameter :: all_tags(4) = (/ character(len=4) :: "1", "02", "003", "0004" /)
    character(len=*), parameter :: wrong_tags(4) = (/ character(len=5) :: "01", "002", "0003", "00004" /)
    status = -1
    call record%reserve(TA_TYPE_INT32, 5_c_int64_t)
    call container%new()
    status = container%add_record("1", record)
    status = container%add_record("02", record)
    status = container%add_record("003", record)
    status = container%add_record("0004", record)
    status = container%has_record("")
    if (status /= TA_INCORRECT_TAG) then
      status = 2
      return
    end if
    status = container%has_record("1")
    if (status /= TA_OK) then
      status = 3
      return
    end if
    status = container%has_record("4")
    if (status /= TA_CONTAINER_RECORD_NOT_FOUND) then
      status = 4
      return
    end if
    status = container%has_records(all_tags)
    if (status /= TA_OK) then
      status = 5
      return
    end if
    status = container%has_records(wrong_tags)
    if (status /= TA_CONTAINER_RECORD_NOT_FOUND) then
      status = 6
      return
    end if
    status = container%has_records(all_tags, id)
    if (id /= size(all_tags, kind=c_int32_t)) then
      status = 7
      return
    end if
    status = container%has_records(wrong_tags, id)
    if (id /= 1_c_int32_t) then
      status = 8
      return
    end if
    call container%delete()
    status = 0
  end function check_has_record
  integer function check_get_record() result(status)
    use, intrinsic :: iso_c_binding, only: c_int64_t, c_null_ptr
    type(container_t) :: container
    type(record_t) :: record
    status = -1
    call container%new()
    status = container%reserve_data("1", TA_TYPE_INT32, 5_c_int64_t)
    if (status /= TA_OK) then
      status = 2
      return
    end if
    record = container%get_record("2")
    if (record%is_associated()) then
      status = 3
      return
    end if
    record = container%get_record("1")
    if (.not.record%is_allocated()) then
      status = 4
      return
    end if
    call container%delete()
    status = 0
  end function check_get_record
  integer function check_remove_record() result(status)
    use, intrinsic :: iso_c_binding, only: c_int64_t
    type(container_t) :: container
    type(recordinfo_t) :: recordinfo
    status = -1
    call container%new()
    status = container%reserve_data("1", TA_TYPE_INT32, 5_c_int64_t)
    if (status /= TA_OK) then
      status = 2
      return
    end if
    status = container%remove_record("1")
    if (status /= TA_OK) then
      status = 3
      return
    end if
#warning "FIX ME: check_remove_record: remove non-existing record"
!    call container%remove_record("1")
!    if (container%get_status() /= TA_CONTAINER_RECORD_NOT_FOUND) then
!      status = 4
!      return
!    end if
    status = container%reserve_data("1", TA_TYPE_INT32, 5_c_int64_t)
    if (status /= TA_OK) then
      status = 5
      return
    end if
    status = container%reserve_data("2", TA_TYPE_INT32, 5_c_int64_t)
    status = container%remove_records((/ character(len=2) :: "1", "2" /))
    if (status /= TA_OK) then
      status = 6
      return
    end if
#warning "FIX ME: check_remove_record: remove non-existing records"
!    call container%remove_records((/ character(len=2) :: "1", "2" /))
!    if (container%get_status() /= TA_CONTAINER_RECORD_NOT_FOUND) then
!      status = 7
!      return
!    end if
    call container%delete()
    status = 0
  end function check_remove_record
end program test
