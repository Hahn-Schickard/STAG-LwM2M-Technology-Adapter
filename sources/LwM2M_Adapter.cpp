#include "LwM2M_Adapter.hpp"
#include "DeviceEventHandler.hpp"

using namespace std;
using namespace LwM2M;

namespace Technology_Adapter {

LwM2M_TechnologyAdapter::LwM2M_TechnologyAdapter()
    : TechnologyAdapter("LwM2M Technology Adapter"),
      server_(make_unique<Server>(Configuration{string("model/descriptors.xml"),
                                                string("0.0.0.0"), 5683, 5})),
      event_handler_(make_unique<DeviceEventHandler>(
          server_->getEventSource(), TechnologyAdapter::adapter_logger_)) {}

LwM2M_TechnologyAdapter::LwM2M_TechnologyAdapter(const string filepath)
    : TechnologyAdapter("LwM2M Technology Adapter"),
      server_(make_unique<Server>(filepath)),
      event_handler_(make_unique<DeviceEventHandler>(
          server_->getEventSource(), TechnologyAdapter::adapter_logger_)) {}

void LwM2M_TechnologyAdapter::run() {
  // nothing to do
}

void LwM2M_TechnologyAdapter::interfaceSet() {
  event_handler_->setBuildingAndRegistrationInterface(
      getBuildingAndRegistrationInterface());
}

void LwM2M_TechnologyAdapter::start() {
  if (event_handler_) {
    TechnologyAdapter::start();
    server_->start();
  } else {
    throw runtime_error("Building and Registration Interface is not "
                        "instantiated, aborting start!");
  }
}

void LwM2M_TechnologyAdapter::stop() {
  TechnologyAdapter::stop();
  server_->stop();
}
} // namespace Technology_Adapter
