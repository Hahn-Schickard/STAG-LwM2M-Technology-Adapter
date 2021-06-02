#include "Information_Model/Metric.hpp"
#include "Information_Model/WritableMetric.hpp"
#include "Information_Model/mocks/DeviceMockBuilder.hpp"
#include "LoggerRepository.hpp"
#include "LwM2M_Adapter.hpp"
#include "Technology_Adapter_Interface/mocks/ModelRegistryInterface_MOCK.hpp"

#include <memory>
#include <thread>

using namespace HaSLL;
using namespace Technology_Adapter::testing;
using namespace Information_Model;
using namespace std;

using ::testing::NiceMock;

void print(DevicePtr device);
void print(DeviceElementPtr element, size_t offset);
void print(WritableMetricPtr element, size_t offset);
void print(MetricPtr element, size_t offset);
void print(DeviceElementGroupPtr elements, size_t offset);

bool registrationHandler(DevicePtr device) {
  thread([](DevicePtr device) { print(device); }, device).detach();
  return true;
}

bool deregistrationHandler(const std::string &device_id) {
  cout << endl;
  cout << "Device: " << device_id << " was deregistered!" << endl;
  return true;
}

int main(int argc, const char *argv[]) {
  try {
    LoggerRepository::initialise("config/loggerConfig.json");
    LoggerRepository::getInstance().configure(SeverityLevel::TRACE);
    auto adapter = make_shared<Technology_Adapter::LwM2M_TechnologyAdapter>(
        "config/serverConfig.json");
    adapter->setInterfaces(
        make_shared<Information_Model::testing::DeviceMockBuilder>(),
        make_shared<NiceMock<ModelRegistryMock>>(
            make_shared<RegistrationHandler>(
                bind(&registrationHandler, placeholders::_1)),
            make_shared<DeregistrationHandler>(
                bind(&deregistrationHandler, placeholders::_1))));
    adapter->start();

    if (argc > 1) {
      int sleep_period = atoi(argv[1]);
      this_thread::sleep_for(chrono::seconds(sleep_period));
      adapter->stop();
    } else {
      for (;;) {
      }
    }

  } catch (const exception &ex) {
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}

void print(DeviceElementGroupPtr elements, size_t offset) {
  cout << string(offset, ' ') << "Group contains elements:" << endl;
  for (auto element : elements->getSubelements()) {
    print(element, offset + 3);
  }
}

void print(MetricPtr element, size_t offset) {
  cout << string(offset, ' ') << "Reads " << toString(element->getDataType())
       << " value: " << toString(element->getMetricValue()) << endl;
  cout << endl;
}

void print(WritableMetricPtr element, size_t offset) {
  auto value = element->getMetricValue();
  auto value_string = toString(value);
  cout << string(offset, ' ') << "Reads " << toString(element->getDataType())
       << " value: " << value_string << endl;
  cout << string(offset, ' ') << "Writes " << toString(element->getDataType())
       << " value type" << endl;
  cout << endl;
}

void print(DeviceElementPtr element, size_t offset) {
  cout << string(offset, ' ') << "Element name: " << element->getElementName()
       << endl;
  cout << string(offset, ' ') << "Element id: " << element->getElementId()
       << endl;
  cout << string(offset, ' ')
       << "Described as: " << element->getElementDescription() << endl;
  cout << string(offset, ' ')
       << "Element type: " << toString(element->getElementType()) << endl;

  switch (element->getElementType()) {
  case ElementType::GROUP: {
    print(static_pointer_cast<DeviceElementGroup>(element), offset);
    break;
  }
  case ElementType::READABLE: {
    print(static_pointer_cast<Metric>(element), offset);
    break;
  }
  case ElementType::WRITABLE: {
    print(static_pointer_cast<WritableMetric>(element), offset);
    break;
  }
  case ElementType::FUNCTION: {
    cerr << string(offset, ' ') << "Function element types are not implemented!"
         << endl;
    break;
  }
  case ElementType::OBSERVABLE: {
    cerr << string(offset, ' ')
         << "Observable elements types are not implemented!" << endl;
    break;
  }
  case ElementType::UNDEFINED:
  default: {
    cerr << string(offset, ' ') << "Is not a valid element type!" << endl;
    break;
  }
  }
}

void print(DevicePtr device) {
  if (device) {
    cout << "Device name: " << device->getElementName() << endl;
    cout << "Device id: " << device->getElementId() << endl;
    cout << "Described as: " << device->getElementDescription() << endl;
    cout << endl;
    print(device->getDeviceElementGroup(), 3);
  } else {
    cerr << "Received a nullptr for Device!" << endl;
  }
}
