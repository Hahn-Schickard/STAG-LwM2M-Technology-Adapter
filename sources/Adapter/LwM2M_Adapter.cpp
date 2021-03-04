#include "LwM2M_Adapter.hpp"
#include "DeviceEventHandler.hpp"

using namespace std;
using namespace LwM2M;

namespace Technology_Adapter {
LwM2M_TechnologyAdapter::LwM2M_TechnologyAdapter(const string filepath)
    : TechnologyAdapter("LwM2M Technology Adapter"),
      server_(make_unique<Server>(filepath)),
      event_handler_(make_unique<DeviceEventHandler>(server_->getEventSource(),
                                                     getLogger())) {}

void LwM2M_TechnologyAdapter::interfaceSet() {
  event_handler_->setBuilderAndRegistratyInterfaces(getDeviceBuilder(),
                                                    getModelRegistry());
}

void LwM2M_TechnologyAdapter::start() {
  server_->start();
  TechnologyAdapter::start();
}

void LwM2M_TechnologyAdapter::stop() {
  server_->stop();
  TechnologyAdapter::stop();
}
} // namespace Technology_Adapter
