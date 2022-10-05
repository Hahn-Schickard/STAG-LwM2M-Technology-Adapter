#ifndef _LWM2M_TECHNOLOGY_ADAPTER_HPP
#define _LWM2M_TECHNOLOGY_ADAPTER_HPP

#include "LwM2M_Server/Server.hpp"
#include "Technology_Adapter_Interface/TechnologyAdapter.hpp"

#include "LwM2M_RegistryEventHandler.hpp"

namespace Technology_Adapter {
class LwM2M_TechnologyAdapter : public TechnologyAdapter {
  std::unique_ptr<LwM2M::Server> server_;
  std::unique_ptr<LwM2M_RegistryEventHandler> event_handler_;

  void interfaceSet() final;

public:
  LwM2M_TechnologyAdapter(const std::string& filepath);

  void start() override;
  void stop() override;
};
} // namespace Technology_Adapter

#endif //_LWM2M_TECHNOLOGY_ADAPTER_HPP