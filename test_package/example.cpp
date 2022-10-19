#include "HaSLL/LoggerManager.hpp"
#include "HaSLL/SPD_LoggerRepository.hpp"
#include "LwM2M_Technology_Adapter/LwM2M_Adapter.hpp"

#include <memory>

using namespace HaSLL;
using namespace std;
using namespace Technology_Adapter;

int main() {
  try {
    auto repo = make_shared<SPD_LoggerRepository>("config/loggerConfig.json");
    LoggerManager::initialise(repo);
    auto logger = LoggerManager::registerLogger("main");

    auto adapter =
        make_shared<LwM2M_TechnologyAdapter>("config/serverConfig.json");

    try {
      adapter->start();
      logger->error("Adapter started without interfaces being set");
      exit(EXIT_FAILURE);
    } catch (const exception& ex) {
      logger->info(
          "Adapter did not start due to: {}. INTEGRATION TEST SUCCESSFUL.",
          ex.what());
      exit(EXIT_SUCCESS);
    }
  } catch (const exception& ex) {
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
