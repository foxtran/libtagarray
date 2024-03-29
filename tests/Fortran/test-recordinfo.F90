#include "tagarray_assert.fh"

program test
  use tagarray
  implicit none
  TEST(check_assignment)
  TEST(check_get_status)
contains
  integer function check_assignment() result(status)
    use tagarray_Crecordinfo
    type(C_recordinfo_t) :: crecinfo
    type(recordinfo_t) :: recinfo
    status = -1
    recinfo = crecinfo
    status = 0
  end function check_assignment
  integer function check_get_status() result(status)
    use, intrinsic :: iso_c_binding, only: c_null_ptr, c_int32_t, c_int64_t
    type(recordinfo_t) :: recinfo
    integer(c_int32_t), pointer :: int32_2D(:,:), int32_3D(:,:,:)
    real(4), pointer :: real_2D(:,:), real_3D(:,:,:)
    integer(c_int32_t), pointer :: int32_12D(:,:,:,:,:, :,:,:,:,:, :,:)
    integer(c_int32_t), pointer :: int32_13D(:,:,:,:,:, :,:,:,:,:, :,:,:)
    integer(c_int32_t), pointer :: int32_14D(:,:,:,:,:, :,:,:,:,:, :,:,:,:)
    integer(c_int32_t), pointer :: int32_15D(:,:,:,:,:, :,:,:,:,:, :,:,:,:,:)
    integer(c_int64_t), target :: dims(3) = (/ 3_8, 2_8, 4_8 /)
    status = -1
    ! assume 3-dimensional int32 array with dims [3, 2, 4], data is not provided
    recinfo%type_id = TA_TYPE_INT32
    recinfo%itemsize = 4
    recinfo%count = 3*2*4
    recinfo%ndims = 3
    recinfo%dims => dims
    recinfo%data = c_null_ptr
    recinfo%description = ""
    ! check with hand-written type discovering
    if (recinfo%get_status(TA_TYPE_INT32, size(shape(int32_3D), kind=8)) /= TA_OK) then
      status = 1
      return
    end if
    if (recinfo%get_status(TA_TYPE_INT32, size(shape(int32_2D), kind=8)) /= TA_DATA_INCORRECT_DIMENSIONS) then
      status = 2
      return
    end if
    if (recinfo%get_status(TA_TYPE_REAL32, size(shape(real_3D), kind=8)) /= TA_DATA_INCORRECT_TYPE) then
      status = 3
      return
    end if
    if (recinfo%get_status(TA_TYPE_REAL32, size(shape(real_2D), kind=8)) /= TA_DATA_INCORRECT) then
      status = 4
      return
    end if
    ! check with automatic type discovering
#if TA_FORTRAN_API_VERSION_AVAILABLE >= 2
    if (recinfo%get_status(get_type_id(int32_3D), size(shape(int32_3D), kind=8)) /= TA_OK) then
      status = 5
      return
    end if
    if (recinfo%get_status(get_type_id(int32_2D), size(shape(int32_2D), kind=8)) /= TA_DATA_INCORRECT_DIMENSIONS) then
      status = 6
      return
    end if
    if (recinfo%get_status(get_type_id(real_3D), size(shape(real_3D), kind=8)) /= TA_DATA_INCORRECT_TYPE) then
      status = 7
      return
    end if
    if (recinfo%get_status(get_type_id(real_2D), size(shape(real_2D), kind=8)) /= TA_DATA_INCORRECT) then
      status = 8
      return
    end if
#endif
    ! check maximum supported dimensions
    if (recinfo%get_status(TA_TYPE_INT32, size(shape(int32_12D), kind=8)) /= TA_DATA_INCORRECT_DIMENSIONS) then
      status = 9
      return
    end if
    if (recinfo%get_status(TA_TYPE_INT32, size(shape(int32_13D), kind=8)) /= TA_DATA_TOO_MANY_DIMENSIONS) then
      status = 10
      return
    end if
    if (recinfo%get_status(TA_TYPE_INT32, size(shape(int32_14D), kind=8)) /= TA_DATA_TOO_MANY_DIMENSIONS) then
      status = 11
      return
    end if
    if (recinfo%get_status(TA_TYPE_INT32, size(shape(int32_15D), kind=8)) /= TA_DATA_TOO_MANY_DIMENSIONS) then
      status = 12
      return
    end if
    ! check data correctness
    recinfo%dims(3) = 0
    if (recinfo%get_status(TA_TYPE_INT32, size(shape(int32_3D), kind=8)) /= TA_DATA_ZERO_LENGTH) then
      status = 13
      return
    end if
    status = 0
  end function check_get_status
end program test
