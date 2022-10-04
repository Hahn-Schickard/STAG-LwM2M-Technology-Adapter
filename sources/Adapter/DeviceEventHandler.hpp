#include "Event_Model/EventListenerInterface.hpp"
#include "HaSLL/Logger.hpp"
#include "LwM2M/DeviceRegistry.hpp"
#include "LwM2M/RegistryEvent.hpp"

#include "LwM2M_RegistryEventHandler.hpp"

#include <memory>

class DeviceEventHandler
    : public Event_Model::EventListenerInterface<LwM2M::RegistryEvent>,
      public LwM2M_RegistryEventHandler {
  DeviceBuilderPtr builder_;
  ModelRegistryPtr registry_;
  HaSLI::LoggerPtr logger_;

  void addSubelements(std::string instance_id, LwM2M::Resources resources);
  void populateRootElementGroup(LwM2M::ObjectsMap objects);
  Information_Model::DevicePtr buildDevice(LwM2M::DevicePtr device);
  void handleEvent(LwM2M::RegistryEventPtr event) override;

public:
  DeviceEventHandler(
      LwM2M::EventSourcePtr event_source, HaSLI::LoggerPtr logger);

  void setBuilderAndRegistryInterfaces(
      DeviceBuilderPtr builder, ModelRegistryPtr registry) override;
};
