#include "tagarray_assert.fh"

program test
  use tagarray
  implicit none
  TEST(check_new_delete)
  TEST(check_reserve_delete)
  TEST(check_get_status)
contains
  integer function check_new_delete() result(status)
    use, intrinsic :: iso_c_binding, only: c_loc, c_sizeof, c_associated, c_f_pointer, c_int64_t
    type(record_t) :: record
    type(recordinfo_t) :: recordinfo
    integer(4), target, allocatable :: iarr_1D(:), iarr_2D(:,:)
    integer(4), pointer :: iarr_1D_test(:), iarr_2D_test(:,:)
    integer i, j
    allocate(iarr_1D(10), iarr_2D(3,3), source = 0_4)
    do i = 10, 1
      iarr_1D(i) = 10-i
    end do
    do i = 1, 3
      do j = 1, 3
        iarr_2D(j,i) = j + i * 3
      end do
    end do
    status = -1
    call record%new(TA_TYPE_INT32, c_loc(iarr_1D), get_storage_size(TA_TYPE_INT32), size(iarr_1D, kind=c_int64_t), shape(iarr_1D, kind=c_int64_t), comment="test 1D intrinsic")
    recordinfo = record%get_info()
    if (recordinfo%type_id /= TA_TYPE_INT32) then
      status = 101
      return
    end if
    if (recordinfo%n_dimensions /= 1) then
      status = 102
      return
    end if
    if (recordinfo%data_length /= c_sizeof(iarr_1D(1)) * size(iarr_1D)) then
      status = 103
      return
    end if
    if (recordinfo%dimensions(1) /= 10_c_int64_t) then
      status = 104
      return
    end if
    if (product(recordinfo%dimensions) /= 10_c_int64_t) then
      status = 105
    end if
    if (.not.c_associated(recordinfo%data)) then
      status = 106
      return
    end if
    call c_f_pointer(recordinfo%data, iarr_1D_test, shape=(/ recordinfo%dimensions(1) /))
    if (any(iarr_1D /= iarr_1D_test)) then
      status = 107
      return
    end if
    call record%delete()
    call record%new(TA_TYPE_INT32, c_loc(iarr_1D), get_storage_size(TA_TYPE_INT32), 10_c_int64_t, (/ 10_c_int64_t /), comment="test 1D manual")
    recordinfo = record%get_info()
    if (recordinfo%type_id /= TA_TYPE_INT32) then
      status = 201
      return
    end if
    if (recordinfo%n_dimensions /= 1) then
      status = 202
      return
    end if
    if (recordinfo%data_length /= c_sizeof(iarr_1D(1)) * size(iarr_1D)) then
      status = 203
      return
    end if
    if (recordinfo%dimensions(1) /= 10_c_int64_t) then
      status = 204
      return
    end if
    if (product(recordinfo%dimensions) /= 10_c_int64_t) then
      status = 205
    end if
    if (.not.c_associated(recordinfo%data)) then
      status = 206
      return
    end if
    call c_f_pointer(recordinfo%data, iarr_1D_test, shape=(/ recordinfo%dimensions(1) /))
    if (any(iarr_1D /= iarr_1D_test)) then
      status = 207
      return
    end if
    call record%delete()
    call record%new(TA_TYPE_INT32, c_loc(iarr_1D), get_storage_size(TA_TYPE_INT32), 10_c_int64_t, comment="test 1D size")
    recordinfo = record%get_info()
    if (recordinfo%type_id /= TA_TYPE_INT32) then
      status = 301
      return
    end if
    if (recordinfo%n_dimensions /= 1) then
      status = 302
      return
    end if
    if (recordinfo%data_length /= c_sizeof(iarr_1D(1)) * size(iarr_1D)) then
      status = 303
      return
    end if
    if (recordinfo%dimensions(1) /= 10_c_int64_t) then
      status = 304
      return
    end if
    if (product(recordinfo%dimensions) /= 10_c_int64_t) then
      status = 305
    end if
    if (.not.c_associated(recordinfo%data)) then
      status = 306
      return
    end if
    call c_f_pointer(recordinfo%data, iarr_1D_test, shape=(/ recordinfo%dimensions(1) /))
    if (any(iarr_1D /= iarr_1D_test)) then
      status = 307
      return
    end if
    call record%delete()
    call record%new(TA_TYPE_INT32, c_loc(iarr_2D), get_storage_size(TA_TYPE_INT32), size(iarr_2D, kind=c_int64_t), shape(iarr_2D, kind=c_int64_t), comment="test 2D intrinsic")
    recordinfo = record%get_info()
    if (recordinfo%type_id /= TA_TYPE_INT32) then
      status = 401
      return
    end if
    if (recordinfo%n_dimensions /= 2) then
      status = 402
      return
    end if
    if (recordinfo%data_length /= c_sizeof(iarr_2D(1,1)) * size(iarr_2D)) then
      status = 403
      return
    end if
    if ((recordinfo%dimensions(1) /= 3_c_int64_t) .or. &
        (recordinfo%dimensions(2) /= 3_c_int64_t)) then
      status = 404
      return
    end if
    if (product(recordinfo%dimensions) /= 9_c_int64_t) then
      status = 405
    end if
    if (.not.c_associated(recordinfo%data)) then
      status = 406
      return
    end if
    call c_f_pointer(recordinfo%data, iarr_2D_test, shape=(/ recordinfo%dimensions(1:2) /))
    if (any(iarr_2D /= iarr_2D_test)) then
      status = 407
      return
    end if
    call record%delete()
    call record%new(TA_TYPE_INT32, c_loc(iarr_2D), get_storage_size(TA_TYPE_INT32), 9_c_int64_t, (/ 3_c_int64_t, 3_c_int64_t /), comment="test 2D manual")
    recordinfo = record%get_info()
    if (recordinfo%type_id /= TA_TYPE_INT32) then
      status = 501
      return
    end if
    if (recordinfo%n_dimensions /= 2) then
      status = 502
      return
    end if
    if (recordinfo%data_length /= c_sizeof(iarr_2D(1,1)) * size(iarr_2D)) then
      status = 503
      return
    end if
    if ((recordinfo%dimensions(1) /= 3_c_int64_t) .or. &
        (recordinfo%dimensions(2) /= 3_c_int64_t)) then
      status = 504
      return
    end if
    if (product(recordinfo%dimensions) /= 9_c_int64_t) then
      status = 505
    end if
    if (.not.c_associated(recordinfo%data)) then
      status = 506
      return
    end if
    call c_f_pointer(recordinfo%data, iarr_2D_test, shape=(/ recordinfo%dimensions(1:2) /))
    if (any(iarr_2D /= iarr_2D_test)) then
      status = 507
      return
    end if
    call record%delete()
    call record%new(TA_TYPE_INT32, c_loc(iarr_2D), get_storage_size(TA_TYPE_INT32), 8_c_int64_t, (/ 3_c_int64_t, 3_c_int64_t /)) ! comment="test 2D wrong size
    recordinfo = record%get_info()
    if (recordinfo%type_id /= TA_TYPE_INT32) then
      status = 601
      return
    end if
    if (recordinfo%n_dimensions /= 2) then
      status = 602
      return
    end if
    if (recordinfo%data_length /= c_sizeof(iarr_2D(1,1)) * size(iarr_2D)) then
      status = 603
      return
    end if
    if ((recordinfo%dimensions(1) /= 3_c_int64_t) .or. &
        (recordinfo%dimensions(2) /= 3_c_int64_t)) then
      status = 604
      return
    end if
    if (product(recordinfo%dimensions) /= 9_c_int64_t) then
      status = 605
    end if
    if (.not.c_associated(recordinfo%data)) then
      status = 606
      return
    end if
    call c_f_pointer(recordinfo%data, iarr_2D_test, shape=(/ recordinfo%dimensions(1:2) /))
    if (any(iarr_2D /= iarr_2D_test)) then
      status = 607
      return
    end if
    call record%delete()
    status = 0
  end function check_new_delete
  integer function check_reserve_delete() result(status)
    use, intrinsic :: iso_c_binding, only: c_int64_t, c_sizeof, c_associated
    type(record_t) :: record
    type(recordinfo_t) :: recordinfo
    integer(4), pointer :: iarr_1D(:), iarr_3D(:,:,:)
    status = -1
    call record%reserve(TA_TYPE_INT32, 10_c_int64_t, (/ 10_c_int64_t /), comment = "reserve 1D array")
    recordinfo = record%get_info()
    if (recordinfo%type_id /= TA_TYPE_INT32) then
      status = 101
      return
    end if
    if (recordinfo%n_dimensions /= 1) then
      status = 102
      return
    end if
    if (recordinfo%data_length /= c_sizeof(iarr_1D(1)) * 10) then
      status = 103
      return
    end if
    if ((recordinfo%dimensions(1) /= 10_c_int64_t)) then
      status = 104
      return
    end if
    if (product(recordinfo%dimensions) /= 10_c_int64_t) then
      status = 105
    end if
    if (.not.c_associated(recordinfo%data)) then
      status = 106
      return
    end if
    call c_f_pointer(recordinfo%data, iarr_1D, shape=(/ recordinfo%dimensions(1) /))
    if (any(iarr_1D /= 0_4)) then
      status = 107
      return
    end if
    call record%delete()
    call record%reserve(TA_TYPE_INT32, 10_c_int64_t, comment = "reserve 1D array")
    recordinfo = record%get_info()
    if (recordinfo%type_id /= TA_TYPE_INT32) then
      status = 201
      return
    end if
    if (recordinfo%n_dimensions /= 1) then
      status = 202
      return
    end if
    if (recordinfo%data_length /= c_sizeof(iarr_1D(1)) * 10) then
      status = 203
      return
    end if
    if ((recordinfo%dimensions(1) /= 10_c_int64_t)) then
      status = 204
      return
    end if
    if (product(recordinfo%dimensions) /= 10_c_int64_t) then
      status = 205
    end if
    if (.not.c_associated(recordinfo%data)) then
      status = 206
      return
    end if
    call c_f_pointer(recordinfo%data, iarr_1D, shape=(/ recordinfo%dimensions(1) /))
    if (any(iarr_1D /= 0_4)) then
      status = 207
      return
    end if
    call record%delete()
    call record%reserve(TA_TYPE_INT32, 8_c_int64_t, (/ 2_c_int64_t, 2_c_int64_t, 2_c_int64_t /), comment = "reserve 3D array")
    recordinfo = record%get_info()
    if (recordinfo%type_id /= TA_TYPE_INT32) then
      status = 301
      return
    end if
    if (recordinfo%n_dimensions /= 3) then
      status = 302
      return
    end if
    if (recordinfo%data_length /= c_sizeof(iarr_3D(1,1,1)) * 8) then
      status = 303
      return
    end if
    if ((recordinfo%dimensions(1) /= 2_c_int64_t) .or. &
        (recordinfo%dimensions(2) /= 2_c_int64_t) .or. &
        (recordinfo%dimensions(3) /= 2_c_int64_t)) then
      status = 304
      return
    end if
    if (product(recordinfo%dimensions) /= 8_c_int64_t) then
      status = 305
    end if
    if (.not.c_associated(recordinfo%data)) then
      status = 306
      return
    end if
    call c_f_pointer(recordinfo%data, iarr_3D, shape=(/ recordinfo%dimensions(1:3) /))
    if (any(iarr_3D /= 0_4)) then
      status = 307
      return
    end if
    call record%delete()
    call record%reserve(TA_TYPE_INT8, 0_c_int64_t)
    recordinfo = record%get_info()
    if (recordinfo%type_id /= TA_TYPE_INT8) then
      status = 401
      return
    end if
    if (recordinfo%n_dimensions /= 1) then
      status = 402
      return
    end if
    if (recordinfo%data_length /= 0) then
      status = 403
      return
    end if
    if ((recordinfo%dimensions(1) /= 0_c_int64_t)) then
      status = 404
      return
    end if
    if (.not.c_associated(recordinfo%data)) then
      status = 406
      return
    end if
    status = 0
  end function check_reserve_delete
  integer function check_get_status() result(status)
    use, intrinsic :: iso_c_binding, only: c_size_t, c_int64_t
    integer(c_size_t), parameter :: max_size = huge(max_size)
    type(record_t) :: record
    status = -1
    call record%reserve(TA_TYPE_INT8, 0_c_int64_t)
    if (.not.record%is_allocated()) then
      status = 1
      return
    end if
    call record%delete()
    call record%reserve(TA_TYPE_INT8, int(max_size, kind=c_int64_t))
    if (record%is_allocated()) then
      status = 2
      return
    end if
    call record%delete()
    status = 0
  end function check_get_status
end program test
