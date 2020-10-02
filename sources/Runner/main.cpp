#include "BuildingAndRegistrationInterface_MOCK.hpp"
#include "LoggerRepository.hpp"
#include "LwM2M_Adapter.hpp"

#include <memory>
#include <thread>

using namespace HaSLL;
using namespace Technology_Adapter;
using namespace std;

int main(int argc, const char *argv[]) {
  try {
    LoggerRepository::initialise("loggerConfig.json");
    LoggerRepository::getInstance().configure(SeverityLevel::TRACE);
    auto adapter = make_shared<LwM2M_TechnologyAdapter>("serverConfig.json");
    if (adapter->setBuildingAndRegistrationInterface(
            make_shared<Information_Access_Manager::testing::MockBnR>())) {
      adapter->start();

      if (argc > 1) {
        int sleep_period = atoi(argv[1]);
        this_thread::sleep_for(chrono::seconds(sleep_period));
        adapter->stop();
      } else {
        for (;;) {
        }
      }
    }

  } catch (const exception &ex) {
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}