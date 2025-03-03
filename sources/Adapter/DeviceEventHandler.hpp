#include "Event_Model/EventListenerInterface.hpp"
#include "HaSLL/Logger.hpp"
#include "LwM2M_Server/DeviceRegistry.hpp"
#include "LwM2M_Server/RegistryEvent.hpp"

#include "LwM2M_RegistryEventHandler.hpp"

#include <memory>
#include <mutex>
#include <unordered_map>

struct LwM2M_ExecuteRequestsMap {
  Information_Model::DeviceBuilderInterface::ExecutorResult call(
      LwM2M::Executable::Result&& execute_future) {
    auto result_getter = LwM2M_ExecuteResultGetter(std::move(execute_future));
    auto result_future = result_getter.getFuture();

    std::lock_guard lock(mutex);
    auto caller_id = requests_.size();
    requests_.emplace(caller_id, std::move(result_getter));
    return std::make_pair(caller_id, std::move(result_future));
  }

  void cancel(uintmax_t caller_id) {
    auto request = requests_.find(caller_id);
    if (request != requests_.end()) {
      request->second.cancel();
      {
        std::lock_guard lock(mutex);
        requests_.erase(request);
      }
    }
  }

private:
  struct LwM2M_ExecuteResultGetter {
    LwM2M_ExecuteResultGetter(LwM2M::Executable::Result&& result)
        : result_(std::move(result)) {}

    std::future<Information_Model::DataVariant> getFuture() {
      return std::async(
          std::launch::deferred, [this]() -> Information_Model::DataVariant {
            bool executed = result_.get();
            return Information_Model::DataVariant(executed);
          });
    }

    void cancel() { result_.cancel(); }

  private:
    LwM2M::Executable::Result result_;
  };

  std::mutex mutex;
  std::unordered_map<uintmax_t, LwM2M_ExecuteResultGetter> requests_;
};

using LwM2M_ExecuteRequestsMapPtr = std::shared_ptr<LwM2M_ExecuteRequestsMap>;

struct DeviceEventHandler
    : public Event_Model::EventListenerInterface<LwM2M::RegistryEvent>,
      public LwM2M_RegistryEventHandler {
  DeviceEventHandler(
      LwM2M::EventSourcePtr event_source, HaSLL::LoggerPtr logger);

  void setBuilderAndRegistryInterfaces(
      Information_Model::NonemptyDeviceBuilderInterfacePtr builder,
      Technology_Adapter::NonemptyDeviceRegistryPtr registry) override;

private:
  void addSubelements(
      std::string instance_id, const LwM2M::Resources& resources);
  void populateRootElementGroup(const LwM2M::ObjectsMap& objects);
  Information_Model::NonemptyDevicePtr buildDevice(LwM2M::DevicePtr device);
  void handleEvent(LwM2M::RegistryEventPtr event) override;

  LwM2M_ExecuteRequestsMapPtr execute_requests_;
  Information_Model::DeviceBuilderInterfacePtr builder_;
  Technology_Adapter::DeviceRegistryPtr registry_;
  HaSLL::LoggerPtr logger_;
};
