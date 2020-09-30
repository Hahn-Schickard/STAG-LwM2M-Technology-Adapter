#include "LoggerRepository.hpp"
#include "LwM2M_Adapter.hpp"

#include <iostream>

using namespace HaSLL;
using namespace std;
using namespace Technology_Adapter;

int main() {
  auto config = HaSLL::Configuration(
      "./log", "logfile.log", "[%Y-%m-%d-%H:%M:%S:%F %z][%n]%^[%l]: %v%$",
      HaSLL::SeverityLevel::TRACE, false, 8192, 2, 25, 100, 1);
  LoggerRepository::initialise(config);
  LoggerRepository::getInstance().configure(SeverityLevel::TRACE);
  LwM2M_TechnologyAdapter();
  exit(EXIT_SUCCESS);
}