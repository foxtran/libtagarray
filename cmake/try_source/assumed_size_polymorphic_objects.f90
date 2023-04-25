program test
  implicit none
contains
  subroutine UP(arr)
    class(*), intent(inout) :: arr(*)
  end subroutine UP
end program test

