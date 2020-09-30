#ifndef __LWM2M_BUILDING_AND_REGISTRATION_HPP
#define __LWM2M_BUILDING_AND_REGISTRATION_HPP

#include "BuildingAndRegistrationInterface.hpp"

class LwM2M_RegistryEventHandler {
public:
  virtual void setBuildingAndRegistrationInterface(
      std::shared_ptr<
          Information_Access_Manager::BuildingAndRegistrationInterface>
          building_and_registration_interface) = 0;
};

#endif //__LWM2M_BUILDING_AND_REGISTRATION_HPP