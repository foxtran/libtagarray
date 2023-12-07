#include <stdio.h>

#include <tagarray.h>

int main() {
  int64_t a[1] = { 1 };
  void *c = TA_Container_new(NULL);
  TA_Container_create(c, "test", TA_TYPE_UNKNOWN, 1, a, NULL, NULL);
  RecordInfo ri = TA_Container_get(c, "test");
  TA_Container_delete(c);
/*
  c = TA_Container_load("test.dmp");
  RecordInfo ri2 = TA_Container_get(c, "test-double");
  printf("%lld\n", ri2.ndims);
  printf("%lld %lld %lld\n", ri2.dims[0], ri2.dims[1], ri2.dims[2]);
  TA_Container_delete(c);
*/
  return 0;
}
