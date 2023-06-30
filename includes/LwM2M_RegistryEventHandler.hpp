#ifndef __LWM2M_BUILDING_AND_REGISTRATION_FACADE_HPP
#define __LWM2M_BUILDING_AND_REGISTRATION_FACADE_HPP

#include "Information_Model/DeviceBuilderInterface.hpp"
#include "Technology_Adapter_Interface/TechnologyAdapterInterface.hpp"

struct LwM2M_RegistryEventHandler {
  virtual ~LwM2M_RegistryEventHandler() = default;

  virtual void setBuilderAndRegistryInterfaces(
      Information_Model::NonemptyDeviceBuilderInterfacePtr builder,
      Technology_Adapter::NonemptyDeviceRegistryPtr registry) = 0;
};

#endif //__LWM2M_BUILDING_AND_REGISTRATION_FACADE_HPP
