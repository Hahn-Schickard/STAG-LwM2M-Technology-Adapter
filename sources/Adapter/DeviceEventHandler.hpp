#include "Event_Model/EventListenerInterface.hpp"
#include "Logger.hpp"
#include "LwM2M/DeviceRegistry.hpp"
#include "LwM2M/RegistryEvent.hpp"
#include "LwM2M_RegistryEventHandler.hpp"

#include <memory>

class DeviceEventHandler
    : public Event_Model::EventListenerInterface<LwM2M::RegistryEvent>,
      public LwM2M_RegistryEventHandler {
  DeviceBuilderPtr builder_;
  ModelRegistryPtr registry_;
  std::shared_ptr<HaSLL::Logger> logger_;

  std::shared_ptr<Information_Model::Device>
  buildDevice(LwM2M::DevicePtr device);
  void handleEvent(LwM2M::RegistryEventPtr event) override;

public:
  DeviceEventHandler(LwM2M::EventSourcePtr event_source,
                     std::shared_ptr<HaSLL::Logger> logger);

  void setBuilderAndRegistratyInterfaces(DeviceBuilderPtr builder,
                                         ModelRegistryPtr registry) override;
};
