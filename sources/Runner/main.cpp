#include "HaSLL/LoggerManager.hpp"
#include "HaSLL/SPD_LoggerRepository.hpp"
#include "Information_Model/Metric.hpp"
#include "Information_Model/WritableMetric.hpp"
#include "Information_Model/mocks/DeviceMockBuilder.hpp"
#include "Technology_Adapter_Interface/mocks/ModelRegistryInterface_MOCK.hpp"

#include "LwM2M_Adapter.hpp"

#include <memory>
#include <thread>

using namespace HaSLL;
using namespace Technology_Adapter::testing;
using namespace Information_Model;
using namespace std;

using ::testing::NiceMock;

void print(const DevicePtr& device);
void print(NonemptyDeviceElementPtr element, size_t offset);
void print(NonemptyWritableMetricPtr element, size_t offset);
void print(NonemptyMetricPtr element, size_t offset);
void print(NonemptyDeviceElementGroupPtr elements, size_t offset);

static constexpr size_t BASE_OFFSET = 160;
static constexpr size_t ELEMENT_OFFSET = 3;

bool registrationHandler(const DevicePtr& device) {
  cout << "Registering new Device: " << device->getElementName() << endl;
  thread(
      [](const DevicePtr& device) {
        print(device);
        cout << string(BASE_OFFSET, '=') << endl;
      },
      device)
      .detach();
  return true;
}

bool deregistrationHandler(const std::string& device_id) {
  cout << endl;
  cout << "Device: " << device_id << " was deregistered!" << endl;
  return true;
}

int main(int argc, const char* argv[]) {
  try {
    auto repo = make_shared<SPD_LoggerRepository>("config/loggerConfig.json");
    LoggerManager::initialise(repo);

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
      int sleep_period = atoi(argv[1]); // NOLINT
      this_thread::sleep_for(chrono::seconds(sleep_period));
      adapter->stop();
    } else {
      for (;;) {
      }
    }

  } catch (const exception& ex) {
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}

void print(NonemptyDeviceElementGroupPtr elements, size_t offset) {
  cout << string(offset, ' ') << "Group contains elements:" << endl;
  for (auto element : elements->getSubelements()) {
    print(element, offset + ELEMENT_OFFSET);
  }
}

void print(NonemptyMetricPtr element, size_t offset) {
  cout << string(offset, ' ') << "Reads " << toString(element->getDataType())
       << " value: " << toString(element->getMetricValue()) << endl;
  cout << endl;
}

void print(NonemptyWritableMetricPtr element, size_t offset) {
  cout << string(offset, ' ') << "Reads " << toString(element->getDataType())
       << " value: " << toString(element->getMetricValue()) << endl;
  cout << string(offset, ' ') << "Writes " << toString(element->getDataType())
       << " value type" << endl;
  cout << endl;
}

void print(NonemptyDeviceElementPtr element, size_t offset) {
  cout << string(offset, ' ') << "Element name: " << element->getElementName()
       << endl;
  cout << string(offset, ' ') << "Element id: " << element->getElementId()
       << endl;
  cout << string(offset, ' ')
       << "Described as: " << element->getElementDescription() << endl;

  match(
      element->specific_interface,
      [offset](NonemptyDeviceElementGroupPtr interface) {
        print(interface, offset);
      },
      [offset](NonemptyMetricPtr interface) { print(interface, offset); },
      [offset](
          NonemptyWritableMetricPtr interface) { print(interface, offset); });
}

void print(const DevicePtr& device) {
  cout << "Device name: " << device->getElementName() << endl;
  cout << "Device id: " << device->getElementId() << endl;
  cout << "Described as: " << device->getElementDescription() << endl;
  cout << endl;
  print(device->getDeviceElementGroup(), ELEMENT_OFFSET);
}
