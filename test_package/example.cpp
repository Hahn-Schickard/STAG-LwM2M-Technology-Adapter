#include "HaSLL/LoggerManager.hpp"
#include "HaSLL/SPD_LoggerRepository.hpp"
#include "LwM2M_Technology_Adapter/LwM2M_Adapter.hpp"

#include <exception>
#include <iostream>
#include <memory>

using namespace HaSLL;
using namespace std;
using namespace Technology_Adapter;

int main() {
  try {
    auto repo = make_shared<SPD_LoggerRepository>("config/loggerConfig.json");
    LoggerManager::initialise(repo);

    make_shared<LwM2M_TechnologyAdapter>("config/serverConfig.json");

    cout << "Integration test successful." << endl;
    exit(EXIT_SUCCESS);
  } catch (const exception& ex) {
    cout << "An unhandled exception occurred during integration test. "
            "Exception: "
         << ex.what() << endl;
    cout << "Integration test failed" << endl;
    exit(EXIT_FAILURE);
  }
}
