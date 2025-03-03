#include "HSCUL/Integer.hpp"
#include "Information_Model/DataVariant.hpp"

#include "DeviceEventHandler.hpp"
#include "Variant_Visitor.hpp"

#include <functional>
#include <future>
#include <regex>

using namespace std;
using namespace LwM2M;
using namespace Information_Model;
using namespace Technology_Adapter;
using namespace HaSLL;

static constexpr uint8_t SECURITY_OBJECT = 0;
static constexpr uint8_t SERVER_OBJECT = 1;

ElementType toElementType(OperationsType operations) {
  switch (operations) {
  case OperationsType::READ: {
    return ElementType::READABLE;
  }
  case OperationsType::EXECUTE: {
    return ElementType::FUNCTION;
  }
  default:
    return ElementType::WRITABLE;
  }
}

Information_Model::DataType toDataType(LwM2M::DataType data_type) {
  switch (data_type) {
  case LwM2M::DataType::BOOLEAN: {
    return Information_Model::DataType::BOOLEAN;
  }
  case LwM2M::DataType::SIGNED_INTEGER: {
    return Information_Model::DataType::INTEGER;
  }
  case LwM2M::DataType::UNSIGNED_INTEGER: {
    return Information_Model::DataType::UNSIGNED_INTEGER;
  }
  case LwM2M::DataType::FLOAT: {
    return Information_Model::DataType::DOUBLE;
  }
  case LwM2M::DataType::STRING: {
    return Information_Model::DataType::STRING;
  }
  case LwM2M::DataType::OPAQUE: {
    return Information_Model::DataType::OPAQUE;
  }
  case LwM2M::DataType::TIME: {
    return Information_Model::DataType::TIME;
  }
  case LwM2M::DataType::OBJECT_LINK: {
    // TODO: deicide how to handle linking to other nodes
    return Information_Model::DataType::STRING;
  }
  case LwM2M::DataType::NONE: {
    return Information_Model::DataType::NONE;
  }
  default: {
    return Information_Model::DataType::UNKNOWN;
  }
  }
}

string toString(OperationsType type) {
  switch (type) {
  case OperationsType::READ: {
    return "Read";
  }
  case OperationsType::WRITE: {
    return "Write";
  }
  case OperationsType::READ_AND_WRITE: {
    return "Read and Write";
  }
  case OperationsType::EXECUTE: {
    return "Execute";
  }
  case OperationsType::NO_OPERATION: {
  }
  default: {
    return "Null";
  }
  }
}

Information_Model::DataVariant readWrapper(ReadablePtr resource) {
  auto response = resource->read();
  auto response_future = response.asyncGet();
  if (auto status = response_future.wait_for(2s);
      status != future_status::ready) {
    response.cancel();
  }
  auto value = response_future.get();
  Information_Model::DataVariant result;
  match(
      value,
      [&](TimeStamp timestamp) {
        auto date_time = Information_Model::DateTime(timestamp.getValue());
        result = Information_Model::DataVariant(date_time);
      },
      [&](ObjectLink object_link) {
        result = Information_Model::DataVariant(object_link.toString());
      },
      [&](auto type) { result = Information_Model::DataVariant(type); });

  return result;
}

void writeWrapper(
    WritablePtr resource, const Information_Model::DataVariant& variant) {
  match(
      variant,
      [&](DateTime date_time) {
        auto value = TimeStamp(date_time.getValue());
        resource->write(value);
      },
      [&](string value) {
        // potential optimization as a static
        regex checker("[\\d]{1,5}:[\\d]{1,5}");
        // if provided string value is between (2;12) characters long, it is
        // possible that it is not a human readable value, but instead an
        // ObjectLink, check it with the regex pattern to make sure
        if (value.size() > 2 && value.size() < 12 && // NOLINT
            regex_search(value, checker)) {
          auto object_id = value.substr(0, value.find_first_of(':') - 1);
          auto instance_id = value.substr(value.find_first_of(':') + 1);
          auto object_link = ObjectLink(HSCUL::toUnsignedInteger(object_id),
              HSCUL::toUnsignedInteger(instance_id));
          resource->write(object_link);
        } else {
          resource->write(value);
        }
      },
      [&](auto value) { resource->write(value); });
}

DeviceBuilderInterface::ExecutorResult executeWrapper(ExecutablePtr resource,
    // NOLINTNEXTLINE(performance-unnecessary-value-param)
    LwM2M_ExecuteRequestsMapPtr execute_requests,
    const Function::Parameters& parameters) {
  string execute_args =
      std::accumulate(std::next(parameters.begin()), parameters.end(), string(),
          [](std::string& result,
              const pair<uintmax_t, Function::Parameter>& parameter) {
            if (parameter.second.has_value()) {
              return result + "," + toString(parameter.second.value());
            } else {
              return result;
            }
          });
  auto execute_future = resource->execute(execute_args);
  return execute_requests->call(move(execute_future));
}

void cancelWrapper( // NOLINTNEXTLINE(performance-unnecessary-value-param)
    LwM2M_ExecuteRequestsMapPtr execute_requests, uintmax_t call_id) {
  execute_requests->cancel(call_id);
}

void DeviceEventHandler::addSubelements(
    string instance_id, const LwM2M::Resources& resources) {
  for (const auto& resource_variant_pair : resources) {
    auto descriptor = resource_variant_pair.second->getDescriptor();
    auto name = descriptor->name_;
    auto description = descriptor->description_;
    auto data_type = toDataType(descriptor->data_type_);
    auto resource_instances =
        resource_variant_pair.second->getResourceInstances();
    for (const auto& resource_instance : resource_instances) {
      match(
          resource_instance.second,
          [&](const ReadablePtr& instance) {
            DeviceBuilderInterface::Reader read_cb =
                bind(&readWrapper, instance);
            logger_->trace(
                "Binding LwM2M Resource {} with Read access to a {} data "
                "reader.",
                resource_instance.first.toString(),
                LwM2M::toString(descriptor->data_type_));
            builder_->addReadableMetric(
                instance_id, name, description, data_type, read_cb);
          },
          [&](const WritablePtr& instance) {
            DeviceBuilderInterface::Writer write_cb =
                std::bind(&writeWrapper, instance, placeholders::_1);
            logger_->trace(
                "Binding LwM2M Resource {} with Write access to a {} data "
                "writer.",
                resource_instance.first.toString(),
                LwM2M::toString(descriptor->data_type_));
            builder_->addWritableMetric(
                instance_id, name, description, data_type, write_cb);
          },
          [&](const ReadAndWritablePtr& instance) {
            DeviceBuilderInterface::Reader read_cb =
                std::bind(&readWrapper, instance);
            DeviceBuilderInterface::Writer write_cb =
                std::bind(&writeWrapper, instance, placeholders::_1);
            logger_->trace(
                "Binding LwM2M Resource {} with Read ahd Write access to a {} "
                "data reader and writer.",
                resource_instance.first.toString(),
                LwM2M::toString(descriptor->data_type_));
            builder_->addWritableMetric(
                instance_id, name, description, data_type, write_cb, read_cb);
          },
          [&](const ExecutablePtr& instance) {
            DeviceBuilderInterface::Executor execute_cb = std::bind(
                &executeWrapper, instance, execute_requests_, placeholders::_1);
            DeviceBuilderInterface::Canceler cancel_cb =
                std::bind(&cancelWrapper, execute_requests_, placeholders::_1);
            logger_->trace(
                "Binding LwM2M Resource {} with Execute access to a {} "
                "executor and canceler.",
                resource_instance.first.toString(),
                LwM2M::toString(descriptor->data_type_));
            builder_->addFunction(instance_id, name, description,
                Information_Model::DataType::BOOLEAN, // LwM2M supports only
                                                      // boolean returns
                execute_cb, cancel_cb,
                Function::ParameterTypes{
                    {0, // all parameters are encoded in a single string
                        Function::ParameterType{
                            Information_Model::DataType::STRING, true}}});
          },
          [&](auto) {
            logger_->warning(
                "Ignoring unsupported Resource {} with access type {}",
                resource_instance.first.toString(),
                LwM2M::toString(descriptor->operations_));
          });
    }
  }
}

void DeviceEventHandler::populateRootElementGroup(
    const LwM2M::ObjectsMap& objects) {
  for (auto object_pair : objects) {
    if ((object_pair.first != SECURITY_OBJECT) &&
        (object_pair.first != SERVER_OBJECT)) {
      auto instances = object_pair.second->getObjectInstances();
      for (auto instance_pair : instances) {
        logger_->trace("Creating a Device Element group for Object {}:{}",
            object_pair.second->getDescriptor()->name_, instance_pair.first);

        auto instance_name = object_pair.second->getDescriptor()->name_ + " " +
            to_string(instance_pair.first);
        auto instance_id = builder_->addDeviceElementGroup(
            instance_name, object_pair.second->getDescriptor()->description_);
        addSubelements(instance_id, instance_pair.second->getResources());
      }
    } else {
      logger_->trace("Excluding {} Object {} from Information Model",
          (object_pair.first == SERVER_OBJECT
                  ? "Server"
                  : (object_pair.first == SECURITY_OBJECT
                            ? "Security"
                            : "THIS SHOULD NEVER GET HERE")),
          object_pair.second->getDescriptor()->name_);
    }
  }
}

NonemptyDevicePtr DeviceEventHandler::buildDevice(LwM2M::DevicePtr device) {
  if (builder_) {
    logger_->trace("Building device base for {}:{}", device->getDeviceId(),
        device->getName());
    builder_->buildDeviceBase(
        device->getDeviceId(), device->getName(), string());
    populateRootElementGroup(device->getObjects());

    return NonemptyDevicePtr(builder_->getResult());
  }
  throw runtime_error("Device builder is not set");
}

void DeviceEventHandler::handleEvent(shared_ptr<RegistryEvent> event) {
  switch (event->type_) {
  case RegistryEventType::REGISTERED:
  case RegistryEventType::UPDATED: {
    if (builder_ && registry_) {
      if (event->device_.has_value()) {
        // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
        auto device = buildDevice(event->device_.value());
        logger_->trace("Registering device {}:{}", device->getElementId(),
            device->getElementName());
        registry_->registrate(device);
      } else {
        logger_->warning("Received Device Updated event without Device value");
      }
    }
    break;
  }
  case RegistryEventType::DEREGISTERED: {
    if (registry_) {
      logger_->trace("Deregistering device {}", event->identifier_);
      registry_->deregistrate(event->identifier_);
    }
    break;
  }
  default: {
    break;
  }
  }
}

// NEVER move the logger into the DeviceEventHandler, otherwise the provider
// WILL loose it
DeviceEventHandler::DeviceEventHandler(
    EventSourcePtr event_source, LoggerPtr logger) // NOLINT
    : EventListenerInterface(event_source), logger_(logger) {} // NOLINT

void DeviceEventHandler::setBuilderAndRegistryInterfaces(
    Information_Model::NonemptyDeviceBuilderInterfacePtr builder,
    Technology_Adapter::NonemptyDeviceRegistryPtr registry) {
  builder_ = builder.base();
  registry_ = registry.base();
}
