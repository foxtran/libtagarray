#include <iostream>

#include <tagarray.hpp>

using namespace tagarray;

int main() {
  // create containers hest (c1) and test (c2)
  Container c1 = Container();
  c1.description("hest");
  Container c2 = Container("test");
  // output: "hest test"
  std::cout << c1.description() << " " << c2.description() << std::endl;

  // create record test in test container
  c2.create("test", defines::TYPE_INT32, {1}, nullptr, "test");

  // copy container test from test to hest
  c1["hest"] = c2["test"]->deepcopy();
  // output: "test test"
  std::cout << c1["hest"]->description() << " " << c2["test"]->description() << std::endl;

  // update test's desc in hest to hest
  c1["hest"]->description("hest");
  // output: "hest hest"
  std::cout << c1["hest"]->description() << " " << c2["test"]->description() << std::endl;

  // output: "0 0"
  std::cout << c1["hest"]->raw_data<int32_t*>()[0] << " " << c2["test"]->raw_data<int32_t*>()[0] << std::endl;
  c1["hest"]->raw_data<int32_t*>()[0] = 1;
  c2["test"]->raw_data<int32_t*>()[0] = 2;
  // output: "2 2"
  std::cout << c1["hest"]->raw_data<int32_t*>()[0] << " " << c2["test"]->raw_data<int32_t*>()[0] << std::endl;

  c2.create("test-double", defines::TYPE_REAL64, { 3, 3, 3 }, nullptr);
  for(int i = 0; i < c2["test-double"]->count(); i++)
    c2["test-double"]->raw_data<double*>()[i] = i;

  std::string path("test.dmp");
  c2.save(path);
  Container c3 = Container::load(path);
  std::cout << "DESC: " << c2.description() << " " << c3.description() << std::endl;
  for(const auto key : c3.keys()) {
    std::cout << key << std::endl;
  }
  for(int i = 0; i < c3["test-double"]->count(); i++)
    std::cout << c3["test-double"]->raw_data<double*>()[i] << " ";
  std::cout << std::endl;
  return 0;
}
