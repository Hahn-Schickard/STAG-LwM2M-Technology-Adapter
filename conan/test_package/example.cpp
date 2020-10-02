#include "BuildingAndRegistrationInterface_MOCK.hpp"
#include "LoggerRepository.hpp"
#include "LwM2M_Adapter.hpp"

#include <memory>
#include <thread>

using namespace HaSLL;
using namespace std;
using namespace Technology_Adapter;

int main() {
  try {
    LoggerRepository::initialise("loggerConfig.json");
    LoggerRepository::getInstance().configure(SeverityLevel::TRACE);
    auto adapter = make_shared<LwM2M_TechnologyAdapter>("serverConfig.json");
    if (adapter->setBuildingAndRegistrationInterface(
            make_shared<Information_Access_Manager::testing::MockBnR>())) {
      adapter->start();
      this_thread::sleep_for(chrono::seconds(1));
      adapter->stop();
    }
  } catch (const exception &ex) {
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}