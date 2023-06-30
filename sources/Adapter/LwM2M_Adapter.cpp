#include "LwM2M_Adapter.hpp"
#include "DeviceEventHandler.hpp"

using namespace std;
using namespace LwM2M;

namespace Technology_Adapter {
LwM2M_TechnologyAdapter::LwM2M_TechnologyAdapter(const string& filepath)
    : TechnologyAdapterInterface("LwM2M Technology Adapter"),
      server_(make_unique<Server>(filepath)),
      event_handler_(
          make_unique<DeviceEventHandler>(server_->getEventSource(), logger)) {}

void LwM2M_TechnologyAdapter::interfaceSet() {
  event_handler_->setBuilderAndRegistryInterfaces(
      getDeviceBuilder(), getDeviceRegistry());
}

void LwM2M_TechnologyAdapter::start() {
  server_->start();
  TechnologyAdapterInterface::start();
}

void LwM2M_TechnologyAdapter::stop() {
  server_->stop();
  TechnologyAdapterInterface::stop();
}
} // namespace Technology_Adapter
