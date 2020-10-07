
#include "LoggerRepository.hpp"
#include "LwM2M_Adapter.hpp"

#include <memory>

using namespace HaSLL;
using namespace std;
using namespace Technology_Adapter;

int main() {
  try {
    LoggerRepository::initialise("loggerConfig.json");
    LoggerRepository::getInstance().configure(SeverityLevel::TRACE);
    make_shared<LwM2M_TechnologyAdapter>("serverConfig.json");

  } catch (const exception &ex) {
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}