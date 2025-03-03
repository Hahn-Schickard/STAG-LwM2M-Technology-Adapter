#include "HaSLL/LoggerManager.hpp"
#include "Information_Model/Metric.hpp"
#include "Information_Model/WritableMetric.hpp"
#include "Information_Model/mocks/DeviceMockBuilder.hpp"
#include "Technology_Adapter_Interface/mocks/ModelRepositoryInterface_MOCK.hpp"

#include "LwM2M_Adapter.hpp"

#include <memory>
#include <thread>

using namespace HaSLL;
using namespace Technology_Adapter::testing;
using namespace Information_Model;
using namespace std;

using ::testing::NiceMock;

bool registrationHandler(const NonemptyDevicePtr& device);
bool deregistrationHandler(const string& device_id);

int main(int argc, const char* argv[]) {
  auto status = EXIT_SUCCESS;
  try {
    LoggerManager::initialise(
        makeDefaultRepository("config/loggerConfig.json"));

    try {
      auto adapter = make_shared<Technology_Adapter::LwM2M_TechnologyAdapter>(
          "config/serverConfig.json");
      adapter->setInterfaces(
          []() {
            return make_unique<Information_Model::testing::DeviceMockBuilder>();
          },
          Nonempty::make_shared<NiceMock<ModelRepositoryMock>>(
              bind(&registrationHandler, placeholders::_1),
              bind(&deregistrationHandler, placeholders::_1)));
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
      cerr << "An exception occurred during program execution. Exception: "
           << ex.what() << endl;
      status = EXIT_FAILURE;
    }
    LoggerManager::terminate();
  } catch (...) {
    cerr << "An unhandled exception has occurred during program execution or "
            "logger acquisition"
         << endl;
    status = EXIT_FAILURE;
  }

  exit(status);
}

static constexpr size_t BASE_OFFSET = 160;
static constexpr size_t ELEMENT_OFFSET = 3;

void print(const NonemptyMetricPtr& element, size_t offset) {
  cout << string(offset, ' ') << "Reads " << toString(element->getDataType())
       << " value: " << toString(element->getMetricValue()) << endl;
  cout << endl;
}

void print(const NonemptyObservableMetricPtr& element, size_t offset) {
  cout << string(offset, ' ') << "Observes " << toString(element->getDataType())
       << " value: " << toString(element->getMetricValue()) << endl;
  cout << endl;
}

void print(const NonemptyWritableMetricPtr& element, size_t offset) {
  cout << string(offset, ' ') << "Reads " << toString(element->getDataType())
       << " value: " << toString(element->getMetricValue()) << endl;
  cout << string(offset, ' ') << "Writes " << toString(element->getDataType())
       << " value type" << endl;
  cout << endl;
}

void print(const NonemptyFunctionPtr& element, size_t offset) {
  cout << string(offset, ' ') << "Executes " << toString(element->resultType())
       << " (" << toString(element->parameterTypes()) << ")" << endl;
}

void print(const NonemptyDeviceElementGroupPtr& elements, size_t offset);

void print(const NonemptyDeviceElementPtr& element, size_t offset) {
  cout << string(offset, ' ') << "Element name: " << element->getElementName()
       << endl;
  cout << string(offset, ' ') << "Element id: " << element->getElementId()
       << endl;
  cout << string(offset, ' ')
       << "Described as: " << element->getElementDescription() << endl;

  match(
      element->functionality,
      [offset](const NonemptyDeviceElementGroupPtr& interface) {
        print(interface, offset);
      },
      [offset](
          const NonemptyMetricPtr& interface) { print(interface, offset); },
      [offset](const NonemptyObservableMetricPtr& interface) {
        print(interface, offset);
      },
      [offset](const NonemptyWritableMetricPtr& interface) {
        print(interface, offset);
      },
      [offset](
          const NonemptyFunctionPtr& interface) { print(interface, offset); });
}

void print(const NonemptyDeviceElementGroupPtr& elements, size_t offset) {
  cout << string(offset, ' ') << "Group contains elements:" << endl;
  for (const auto& element : elements->getSubelements()) {
    print(element, offset + ELEMENT_OFFSET);
  }
}

void print(const NonemptyDevicePtr& device) {
  cout << "Device name: " << device->getElementName() << endl;
  cout << "Device id: " << device->getElementId() << endl;
  cout << "Described as: " << device->getElementDescription() << endl;
  cout << endl;
  print(device->getDeviceElementGroup(), ELEMENT_OFFSET);
}

bool registrationHandler(const NonemptyDevicePtr& device) {
  cout << "Registering new Device: " << device->getElementName() << endl;
  thread(
      [](const NonemptyDevicePtr& device) {
        print(device);
        cout << string(BASE_OFFSET, '=') << endl;
      },
      device)
      .detach();
  return true;
}

bool deregistrationHandler(const string& device_id) {
  cout << endl;
  cout << "Device: " << device_id << " was deregistered!" << endl;
  return true;
}