#ifndef _LWM2M_TECHNOLOGY_ADAPTER_HPP
#define _LWM2M_TECHNOLOGY_ADAPTER_HPP

#include "LwM2M_RegistryEventHandler.hpp"
#include "LwM2M_Server.hpp"
#include "TechnologyAdapter.hpp"

namespace Technology_Adapter {
class LwM2M_TechnologyAdapter : public TechnologyAdapter {
  std::unique_ptr<LwM2M::Server> server_;
  std::unique_ptr<LwM2M_RegistryEventHandler> event_handler_;

  void run() final;
  void interfaceSet() final;

public:
  LwM2M_TechnologyAdapter();
  LwM2M_TechnologyAdapter(const std::string filepath);

  void start();
  void stop();
};
} // namespace Technology_Adapter

#endif //_LWM2M_TECHNOLOGY_ADAPTER_HPP