#define TA_FORTRAN_API_VERSION 1
#include "tagarray.fh"
#include "tagarray_assert.fh"

program test
  use tagarray
  use, intrinsic :: iso_c_binding, only: c_int32_t
  implicit none
  TEST(check_new_delete)
  TEST(check_create)
  TEST(check_contains)
  TEST(check_get)
  TEST(check_append)
  TEST(check_erase)
  TEST(check_saveload)
contains
  integer function check_new_delete() result(status)
    type(container_t) :: container
    status = -1
    call container%new()
    call container%delete()
    call container%new("Comment")
    call container%delete()
    call container%new()
    call container%new()
    call container%delete()
    call container%delete()
    status = 0
  end function check_new_delete
  integer(c_int32_t) function check_create() result(status)
    use, intrinsic :: iso_c_binding, only: c_null_ptr, c_int64_t
    type(container_t) :: container
    status = -1
    call container%new()
    status = container%create("TEST", TA_TYPE_INT32, (/ 1_c_int64_t, 1_c_int64_t /), description = "comment")
    if (status /= TA_OK) then
      status = 2
      return
    end if
    status = container%create("TEST", TA_TYPE_INT32, (/ 1_c_int64_t, 1_c_int64_t /))
    if (status /= TA_CONTAINER_RECORD_EXISTS) then
      status = 3
      return
    end if
    call container%delete()
    status = 0
  end function check_create
  integer(c_int32_t) function check_contains() result(status)
    use, intrinsic :: iso_c_binding, only: c_int32_t, c_int64_t
    type(container_t) :: container
    type(recordinfo_t) :: record
    integer(c_int32_t) :: id
    character(len=*), parameter :: all_tags(4) = (/ character(len=4) :: "1", "02", "003", "0004" /)
    character(len=*), parameter :: wrong_tags(4) = (/ character(len=5) :: "01", "002", "0003", "00004" /)
    logical :: lstat
    status = -1
    call container%new()
    status = container%create("1", TA_TYPE_INT32, (/ 5_c_int64_t /))
    status = container%create("02", TA_TYPE_INT32, (/ 5_c_int64_t /))
    status = container%create("003", TA_TYPE_INT32, (/ 5_c_int64_t /))
    status = container%create("0004", TA_TYPE_INT32, (/ 5_c_int64_t /))
    lstat = container%contains("")
    if (lstat) then
      status = 2
      return
    end if
    lstat = container%contains("1")
    if (.not.lstat) then
      status = 3
      return
    end if
    lstat = container%contains("4")
    if (lstat) then
      status = 4
      return
    end if
    lstat = container%contains(all_tags)
    if (.not.lstat) then
      status = 5
      return
    end if
    lstat = container%contains(wrong_tags)
    if (lstat) then
      status = 6
      return
    end if
    lstat = container%contains(all_tags, id)
    if (id /= size(all_tags, kind=c_int32_t)) then
      status = 7
      return
    end if
    lstat = container%contains(wrong_tags, id)
    if (id /= 1_c_int32_t) then
      status = 8
      return
    end if
    call container%delete()
    status = 0
  end function check_contains
  integer function check_get() result(status)
    use, intrinsic :: iso_c_binding, only: c_int64_t, c_null_ptr
    type(container_t) :: container
    type(recordinfo_t) :: record
    status = -1
    call container%new()
    status = container%create("1", TA_TYPE_INT32, (/ 5_c_int64_t /))
    if (status /= TA_OK) then
      status = 2
      return
    end if
    record = container%get("2")
    if (record%is_associated()) then
      status = 3
      return
    end if
#warning "FIX ME: no check status of allocation!"
!    record = container%get("1")
!    if (.not.record%is_allocated()) then
!      status = 4
!      return
!    end if
    call container%delete()
    status = 0
  end function check_get
  integer(c_int32_t) function check_append() result(status)
    use, intrinsic :: iso_c_binding, only: c_null_ptr, c_int64_t, c_loc
    type(container_t) :: container
    type(recordinfo_t) :: record
    integer(c_int64_t), pointer :: arr(:)
    integer(c_int64_t), parameter :: arr_ref(3) = (/ 1_c_int64_t, 1_c_int64_t, 2_c_int64_t /)
    integer(c_int64_t), target :: ival1(1) = 2_c_int64_t, ival2(2) = 1_c_int64_t
    status = -1
    ival2 = 1_c_int64_t
    ival1 = 2_c_int64_t
    call container%new()
    status = container%create("TEST", TA_TYPE_INT64, shape(ival2, kind=c_int64_t), c_loc(ival2), description = "comment")
    if (status /= TA_OK) then
      status = 2
      return
    end if
    status = container%append("TEST", shape(ival1, kind=c_int64_t), c_loc(ival1))
    if (status /= TA_OK) then
      status = 3
      return
    end if
    record = container%get("TEST")
    call c_f_pointer(record%data, arr, record%dims)
    if (any(arr_ref /= arr)) then
      print *, arr
      print *, record%dims
      status = 4
      return
    end if
    call container%delete()
    status = 0
  end function check_append
  integer function check_erase() result(status)
    use, intrinsic :: iso_c_binding, only: c_int64_t
    type(container_t) :: container
    type(recordinfo_t) :: recordinfo
    logical :: lstat
    status = -1
    call container%new()
    status = container%create("1", TA_TYPE_INT32, (/ 5_c_int64_t /))
    if (status /= TA_OK) then
      status = 2
      return
    end if
    call container%erase("1")
    status = container%create("1", TA_TYPE_INT32, (/ 5_c_int64_t /))
    if (status /= TA_OK) then
      status = 5
      return
    end if
    status = container%create("2", TA_TYPE_INT32, (/ 5_c_int64_t /))
    call container%erase((/ character(len=2) :: "1", "2" /))
    call container%delete()
    status = 0
  end function check_erase
  integer function check_saveload() result(status)
    use, intrinsic :: iso_c_binding, only: c_int32_t, c_int64_t
    type(container_t) :: c1, c2
    integer(c_int32_t), pointer :: p_iarr(:)
    real(8), pointer :: p_darr(:)
    status = -1
    call c1%new()
    status = c1%create("int", TA_TYPE_INT32, (/ 5_c_int64_t /))
    status = c1%create("double", TA_TYPE_REAL64, (/ 5_c_int64_t /))
    TA_CONTAINER_GET_ARRAY(c1, "int", TA_TYPE_INT32, p_iarr, status)
    TA_CONTAINER_GET_ARRAY(c1, "double", TA_TYPE_REAL64, p_darr, status)
    p_iarr = 1
    p_iarr(2) = 2
    p_iarr(4) = 4
    p_darr = (/ 1._8, 3._8, 5._8, 7._8, 9._8 /)
    p_iarr => null()
    p_darr => null()
    status = c1%save("test.ta")
    if (status /= TA_OK) then
      status = 1
      return
    end if
    call c1%delete()
    call c2%load("test.ta")
    TA_CONTAINER_GET_ARRAY(c2, "int", TA_TYPE_INT32, p_iarr, status)
    if (status /= TA_OK) then
      status = 2
      return
    end if
    TA_CONTAINER_GET_ARRAY(c2, "double", TA_TYPE_REAL64, p_darr, status)
    if (status /= TA_OK) then
      status = 3
      return
    end if
    if (any(p_iarr /= (/ 1_4, 2_4, 1_4, 4_4, 1_4 /))) then
      status = 4
      return
    end if
    if (any(p_darr /= (/ 1._8, 3._8, 5._8, 7._8, 9._8 /))) then
      status = 5
      return
    end if
    call c2%delete()
    status = 0
  end function check_saveload
end program test
