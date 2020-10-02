#include "DeviceRegistry.hpp"
#include "Event_Model.hpp"
#include "Logger.hpp"
#include "LwM2M_RegistryEventHandler.hpp"
#include "RegistryEvent.hpp"

#include <memory>

class DeviceEventHandler
    : public Event_Model::EventListener<LwM2M::RegistryEvent>,
      public LwM2M_RegistryEventHandler {
  std::shared_ptr<Information_Access_Manager::BuildingAndRegistrationInterface>
      bnr_;
  std::shared_ptr<HaSLL::Logger> logger_;

  void buildAndRegisterDevice(LwM2M::DevicePtr device);
  void handleEvent(std::shared_ptr<LwM2M::RegistryEvent> event) override;

public:
  DeviceEventHandler(
      std::shared_ptr<Event_Model::EventSource<LwM2M::RegistryEvent>>
          event_source,
      std::shared_ptr<HaSLL::Logger> logger);

  void setBuildingAndRegistrationInterface(
      std::shared_ptr<
          Information_Access_Manager::BuildingAndRegistrationInterface>
          building_and_registration_interface) override;
};
