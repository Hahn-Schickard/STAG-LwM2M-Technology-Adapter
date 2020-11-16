#include "DeviceEventHandler.hpp"
#include "DataVariant.hpp"
#include "Variant_Visitor.hpp"

#include <functional>

using namespace std;
using namespace LwM2M;
using namespace Information_Model;
using namespace Technology_Adapter;
using namespace HaSLL;

struct DeviceNode {
  string name;
  string desc;
  ElementType element_type;
  Information_Model::DataType data_type;
  optional<ReadFunctor> read_cb;
  optional<WriteFunctor> write_cb;

  DeviceNode(ResourceDescriptorPtr descriptor,
             optional<ReadFunctor> read_callback,
             optional<WriteFunctor> write_callback) {
    name = descriptor->name_;
    desc = descriptor->description_;
    element_type = toElementType(descriptor->operations_);
    data_type = toDataType(descriptor->data_type_);
    read_cb = read_callback;
    write_cb = write_callback;
  }

private:
  ElementType toElementType(OperationsType operations) {
    switch (operations) {
    case OperationsType::READ: {
      return ElementType::READABLE;
    }
    case OperationsType::WRITE: {
      return ElementType::WRITABLE;
    }
    case OperationsType::READ_AND_WRITE: {
      return ElementType::WRITABLE;
    }
    case OperationsType::EXECUTE: {
      return ElementType::FUNCTION;
    }
    case OperationsType::NO_OPERATION: {
    }
    default: { return ElementType::UNDEFINED; }
    }
  }

  Information_Model::DataType toDataType(LwM2M::DataType data_type) {
    switch (data_type) {
    case LwM2M::DataType::BOOLEAN: {
      return Information_Model::DataType::BOOLEAN;
    }
    case LwM2M::DataType::INTEGER: {
      return Information_Model::DataType::INTEGER;
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
      // TODO: descide how to handle linking to other nodes
      return Information_Model::DataType::STRING;
    }
    case LwM2M::DataType::NONE: {
    }
    default: { return Information_Model::DataType::UNKNOWN; }
    }
  }
};

template <typename T>
void writeWrapper(shared_ptr<Resource<T>> resource,
                  Information_Model::DataVariant variant) {
  resource->write(get<T>(variant));
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
  default: { return "Null"; }
  }
}

template <typename T>
void bindCallbacks(optional<ReadFunctor> &read_cb,
                   optional<WriteFunctor> &write_cb,
                   shared_ptr<Resource<T>> resource) {
  switch (resource->getDescriptor()->operations_) {
  case OperationsType::READ: {
    read_cb = bind(static_cast<T (Resource<T>::*)(void)>(&Resource<T>::read),
                   resource);
    break;
  }
  case OperationsType::WRITE: {
    write_cb = bind(&writeWrapper<T>, resource, placeholders::_1);
    break;
  }
  case OperationsType::READ_AND_WRITE: {
    read_cb = bind(static_cast<T (Resource<T>::*)(void)>(&Resource<T>::read),
                   resource);
    write_cb = bind(&writeWrapper<T>, resource, placeholders::_1);
    break;
  }
  case OperationsType::EXECUTE:
  case OperationsType::NO_OPERATION:
  default: { break; }
  }
}

Information_Model::DevicePtr
DeviceEventHandler::buildDevice(LwM2M::DevicePtr device) {
  if (builder_) {
    logger_->log(SeverityLevel::TRACE, "Building device base for {}:{}",
                 device->getDeviceId(), device->getName());
    builder_->buildDeviceBase(device->getDeviceId(), device->getName(),
                              string());
    for (auto object_pair : device->getObjects()) {
      logger_->log(SeverityLevel::TRACE,
                   "Creating a Device Element group for {}:{} Object",
                   object_pair.second->getDescriptor()->id_,
                   object_pair.second->getDescriptor()->name_);
      auto object_id = builder_->addDeviceElementGroup(
          to_string(object_pair.first),
          object_pair.second->getDescriptor()->description_);
      for (auto instance_pair : object_pair.second->getInstances()) {
        logger_->log(
            SeverityLevel::TRACE,
            "Creating a Device Element Group for {} Object instance {}",
            object_id, to_string(instance_pair.first));
        auto instance_id = builder_->addDeviceElementGroup(
            object_id, to_string(instance_pair.first), string());
        for (auto resource_variant_pair :
             instance_pair.second->getResources()) {
          unique_ptr<DeviceNode> node;
          optional<ReadFunctor> read_cb;
          optional<WriteFunctor> write_cb;
          match(
              resource_variant_pair.second,
              [&](shared_ptr<Resource<bool>> resource) {
                logger_->log(SeverityLevel::TRACE,
                             "Binding {} callbacks for Boolean data type!",
                             toString(resource->getDescriptor()->operations_));
                bindCallbacks<bool>(read_cb, write_cb, resource);
                node = make_unique<DeviceNode>(resource->getDescriptor(),
                                               read_cb, write_cb);
              },
              [&](shared_ptr<Resource<int64_t>> resource) {
                logger_->log(
                    SeverityLevel::TRACE,
                    "Binding {} callbacks for Signed Integer data type!",
                    toString(resource->getDescriptor()->operations_));
                bindCallbacks<int64_t>(read_cb, write_cb, resource);
                node = make_unique<DeviceNode>(resource->getDescriptor(),
                                               read_cb, write_cb);
              },
              [&](shared_ptr<Resource<double>> resource) {
                logger_->log(SeverityLevel::TRACE,
                             "Binding {} callbacks for Double data type!",
                             toString(resource->getDescriptor()->operations_));
                bindCallbacks<double>(read_cb, write_cb, resource);
                node = make_unique<DeviceNode>(resource->getDescriptor(),
                                               read_cb, write_cb);
              },
              [&](shared_ptr<Resource<DateTime>> resource) {
                logger_->log(SeverityLevel::TRACE,
                             "Binding {} callbacks for DateTime data type!",
                             toString(resource->getDescriptor()->operations_));
                bindCallbacks<DateTime>(read_cb, write_cb, resource);
                node = make_unique<DeviceNode>(resource->getDescriptor(),
                                               read_cb, write_cb);
              },
              [&](shared_ptr<Resource<string>> resource) {
                logger_->log(SeverityLevel::TRACE,
                             "Binding {} callbacks for String data type!",
                             toString(resource->getDescriptor()->operations_));
                bindCallbacks<string>(read_cb, write_cb, resource);
                node = make_unique<DeviceNode>(resource->getDescriptor(),
                                               read_cb, write_cb);
              },
              [&](shared_ptr<Resource<uint64_t>> resource) {
                logger_->log(
                    SeverityLevel::TRACE,
                    "Binding {} callbacks for Unsigned Integer data type!",
                    toString(resource->getDescriptor()->operations_));
                bindCallbacks<uint64_t>(read_cb, write_cb, resource);
                node = make_unique<DeviceNode>(resource->getDescriptor(),
                                               read_cb, write_cb);
              },
              [&](shared_ptr<Resource<ObjectLink>> resource) {
                logger_->log(SeverityLevel::ERROR,
                             "Object link building is not supported. "
                             "Skipping resource {}",
                             resource->getDescriptor()->name_);
              },
              [&](shared_ptr<Resource<vector<uint8_t>>> resource) {
                logger_->log(SeverityLevel::TRACE,
                             "Binding {} callbacks for Byte array data type!",
                             toString(resource->getDescriptor()->operations_));
                bindCallbacks<vector<uint8_t>>(read_cb, write_cb, resource);
                node = make_unique<DeviceNode>(resource->getDescriptor(),
                                               read_cb, write_cb);
              });
          if (node) {
            logger_->log(SeverityLevel::TRACE,
                         "Creating a Device Element {} for Object instance {}",
                         node->name, instance_id);
            builder_->addDeviceElement(instance_id, node->name, node->desc,
                                       node->element_type, node->data_type,
                                       node->read_cb, node->write_cb);
          }
        }
      }
    }
    return builder_->getResult();
  }
  return Information_Model::DevicePtr();
}

void DeviceEventHandler::handleEvent(shared_ptr<RegistryEvent> event) {
  switch (event->type) {
  case RegistryEventType::REGISTERED:
  case RegistryEventType::UPDATED: {
    if (builder_ && registry_) {
      auto device = buildDevice(event->device);
      if (device)
        logger_->log(SeverityLevel::TRACE, "Registering device {}:{}",
                     device->getElementId(), device->getElementName());
      registry_->registerDevice(device);
    }
    break;
  }
  case RegistryEventType::DEREGISTERED: {
    if (registry_)
      logger_->log(SeverityLevel::TRACE, "Deregistering device {}",
                   event->identifier);
    registry_->deregisterDevice(event->identifier);
    break;
  }
  default: { break; }
  }
}

DeviceEventHandler::DeviceEventHandler(
    shared_ptr<Event_Model::EventSource<RegistryEvent>> event_source,
    shared_ptr<Logger> logger)
    : EventListener(event_source), logger_(logger) {}

void DeviceEventHandler::setBuilderAndRegistratyInterfaces(
    DeviceBuilderPtr builder, ModelRegistryPtr registry) {
  builder_ = builder;
  registry_ = registry;
}
