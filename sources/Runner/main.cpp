#include "Information_Model/mocks/DeviceBuilderInterface_MOCK.hpp"
#include "LoggerRepository.hpp"
#include "LwM2M_Adapter.hpp"
#include "Technology_Adapter_Interface/mocks/ModelRegistryInterface_MOCK.hpp"

#include <memory>
#include <thread>

using namespace HaSLL;
using namespace Technology_Adapter::testing;
using namespace Information_Model::testing;
using namespace std;

using ::testing::NiceMock;

int main(int argc, const char *argv[]) {
  try {
    LoggerRepository::initialise("config/loggerConfig.json");
    LoggerRepository::getInstance().configure(SeverityLevel::TRACE);
    auto adapter = make_shared<Technology_Adapter::LwM2M_TechnologyAdapter>(
        "config/serverConfig.json");
    adapter->setInterfaces(make_shared<NiceMock<DeviceBuilderInterfaceMock>>(),
                           make_shared<NiceMock<ModelRegistryMock>>());
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