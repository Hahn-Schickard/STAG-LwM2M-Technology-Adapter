#include "Event_Model/EventListenerInterface.hpp"
#include "HaSLL/Logger.hpp"
#include "LwM2M_Server/DeviceRegistry.hpp"
#include "LwM2M_Server/RegistryEvent.hpp"

#include "LwM2M_RegistryEventHandler.hpp"

#include <memory>

struct DeviceEventHandler
    : public Event_Model::EventListenerInterface<LwM2M::RegistryEvent>,
      public LwM2M_RegistryEventHandler {
  DeviceEventHandler(
      LwM2M::EventSourcePtr event_source, HaSLI::LoggerPtr logger);

  void setBuilderAndRegistryInterfaces(
      Information_Model::NonemptyDeviceBuilderInterfacePtr builder,
      Technology_Adapter::NonemptyDeviceRegistryPtr registry) override;

private:
  void addSubelements(
      std::string instance_id, const LwM2M::Resources& resources);
  void populateRootElementGroup(const LwM2M::ObjectsMap& objects);
  Information_Model::NonemptyDevicePtr buildDevice(LwM2M::DevicePtr device);
  void handleEvent(LwM2M::RegistryEventPtr event) override;

  Information_Model::DeviceBuilderInterfacePtr builder_;
  Technology_Adapter::DeviceRegistryPtr registry_;
  HaSLI::LoggerPtr logger_;
};
