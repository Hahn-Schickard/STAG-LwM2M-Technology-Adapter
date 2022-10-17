#include "DataVariant.hpp"

#include <functional>
#include <iostream>
#include <memory>
#include <string>

using namespace std;
using namespace Information_Model;

using Read_Callback = function<DataVariant()>;
using Write_Callback = function<void(DataVariant)>;

template <typename T> class CB_Test {
  T value_;

public:
  T getInt() { return value_; }
  void setInt(T value) { value_ = value; }
};

template <typename T> class CB_Wrapper {
  function<void(T)> cb_;

public:
  CB_Wrapper(function<void(T)> cb) : cb_(cb) {}

  void writeWrapper(DataVariant variant) { cb_(get<T>(variant)); }
};

void print(DataVariant variant) {
  match(
      variant,
      [&](bool value) { cout << "Boolean: " << to_string(value) << endl; },
      [&](uint8_t value) { cout << "Byte: " << to_string(value) << endl; },
      [&](int16_t value) { cout << "Short: " << to_string(value) << endl; },
      [&](int32_t value) { cout << "Word: " << to_string(value) << endl; },
      [&](int64_t value) { cout << "Long: " << to_string(value) << endl; },
      [&](float value) { cout << "Float: " << to_string(value) << endl; },
      [&](double value) { cout << "Double: " << to_string(value) << endl; },
      [&](string value) { cout << "String: " << value << endl; });
}

int main() {
  auto int_obj = CB_Test<int>();
  Read_Callback int_read_cb = bind(&CB_Test<int>::getInt, &int_obj);
  auto int_wrapper =
      CB_Wrapper<int>(bind(&CB_Test<int>::setInt, &int_obj, placeholders::_1));
  Write_Callback int_write_cb =
      bind(&CB_Wrapper<int>::writeWrapper, &int_wrapper, placeholders::_1);

  int_obj.setInt(10);
  print(int_read_cb());
  int_write_cb(23);
  print(int_read_cb());

  auto double_obj = CB_Test<double>();
  Read_Callback double_read_cb = bind(&CB_Test<double>::getInt, &double_obj);
  auto double_wrapper = CB_Wrapper<double>(
      bind(&CB_Test<double>::setInt, &double_obj, placeholders::_1));
  Write_Callback double_write_cb = bind(
      &CB_Wrapper<double>::writeWrapper, &double_wrapper, placeholders::_1);

  double_obj.setInt(10.1);
  print(double_read_cb());
  double_write_cb(23.2);
  print(double_read_cb());

  auto shared_obj = make_shared<CB_Test<string>>();
  Read_Callback shared_read_cb = bind(&CB_Test<string>::getInt, shared_obj);
  auto shared_wrapper = CB_Wrapper<string>(
      bind(&CB_Test<string>::setInt, shared_obj, placeholders::_1));
  Write_Callback shared_write_cb = bind(
      &CB_Wrapper<string>::writeWrapper, &shared_wrapper, placeholders::_1);

  shared_obj->setInt(string("Hello"));
  print(shared_read_cb());
  shared_write_cb(string("Bye bye"));
  print(shared_read_cb());

  return 0;
}
