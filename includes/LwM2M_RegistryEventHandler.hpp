#ifndef __LWM2M_BUILDING_AND_REGISTRATION_FACADE_HPP
#define __LWM2M_BUILDING_AND_REGISTRATION_FACADE_HPP

#include "Information_Model/DeviceBuilderInterface.hpp"
#include "Technology_Adapter_Interface/ModelRegistryInterface.hpp"

class LwM2M_RegistryEventHandler {
public:
  using DeviceBuilderPtr =
      std::shared_ptr<Information_Model::DeviceBuilderInterface>;
  using ModelRegistryPtr =
      std::shared_ptr<Technology_Adapter::ModelRegistryInterface>;

  virtual ~LwM2M_RegistryEventHandler() = default;

  virtual void setBuilderAndRegistratyInterfaces(DeviceBuilderPtr builder,
                                                 ModelRegistryPtr registry) = 0;
};

#endif //__LWM2M_BUILDING_AND_REGISTRATION_FACADE_HPP