#ifndef __LWM2M_BUILDING_AND_REGISTRATION_FACADE_HPP
#define __LWM2M_BUILDING_AND_REGISTRATION_FACADE_HPP

#include "DeviceBuilderInterface.hpp"
#include "ModelRegistryInterface.hpp"

class LwM2M_RegistryEventHandler {
public:
  using DeviceBuilderPtr =
      std::shared_ptr<Technology_Adapter::DeviceBuilderInterface>;
  using ModelRegistryPtr =
      std::shared_ptr<Technology_Adapter::ModelRegistryInterface>;

  virtual void setBuilderAndRegistratyInterfaces(DeviceBuilderPtr builder,
                                                 ModelRegistryPtr registry) = 0;
};

#endif //__LWM2M_BUILDING_AND_REGISTRATION_FACADE_HPP