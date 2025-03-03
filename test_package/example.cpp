#include "HaSLL/LoggerManager.hpp"
#include "LwM2M_Technology_Adapter/LwM2M_Adapter.hpp"

#include <exception>
#include <iostream>
#include <memory>

using namespace HaSLL;
using namespace std;
using namespace Technology_Adapter;

int main() {
  auto status = EXIT_SUCCESS;
  try {
    LoggerManager::initialise(
        makeDefaultRepository("config/loggerConfig.json"));

    try {
      auto adapter =
          make_shared<LwM2M_TechnologyAdapter>("config/serverConfig.json");
      adapter->start();
      cerr << "Adapter started without interfaces being set" << endl;
      status = EXIT_FAILURE;
    } catch (const exception& ex) {
      cout << "Integration test successful." << endl;
    }
    LoggerManager::terminate();
  } catch (...) {
    cerr << "An unknown unhandled exception occurred during integration test. "
         << endl;
    cerr << "Integration test failed" << endl;
    status = EXIT_FAILURE;
  }
  exit(status);
}
