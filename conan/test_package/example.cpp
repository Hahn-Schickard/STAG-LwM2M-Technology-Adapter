#include "HaSLL/LoggerManager.hpp"
#include "HaSLL/SPD_LoggerRepository.hpp"
#include "LwM2M_Technology_Adapter/LwM2M_Adapter.hpp"

#include <memory>

using namespace HaSLL;
using namespace std;
using namespace Technology_Adapter;

int main() {
  auto repo = make_shared<SPD_LoggerRepository>("config/loggerConfig.json");
  LoggerManager::initialise(repo);
  try {
    make_shared<LwM2M_TechnologyAdapter>("config/serverConfig.json");
  } catch (const exception& ex) {
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}