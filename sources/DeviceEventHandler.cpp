#include "DeviceEventHandler.hpp"
#include "DataVariant.hpp"

#include <functional>

using namespace std;
using namespace LwM2M;
using namespace Information_Model;

using Read_Callback = function<DataVariant()>;
using Write_Callback = function<void(DataVariant)>;

struct DeviceNode {
  string name;
  string desc;
  ElementType element_type;
  Information_Model::DataType data_type;
  Read_Callback read_cb;
  Write_Callback write_cb;

  DeviceNode(ResourceDescriptorPtr descriptor, Read_Callback read_cb,
             Write_Callback write_cb) {
    name = descriptor->name_;
    desc = descriptor->description_;
    element_type = toElementType(descriptor->operations_);
    data_type = toDataType(descriptor->data_type_);
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
      // TODO:  implement array data type to information model
      return Information_Model::DataType::UNKNOWN;
    }
    case LwM2M::DataType::TIME: {
      return Information_Model::DataType::LONG;
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

void DeviceEventHandler::buildAndRegisterDevice(DevicePtr device) {
  if (bnr_) {
    bnr_->buildDeviceBase(device->getDeviceId(), device->getName(), string());
    for (auto object_pair : device->getObjects()) {
      auto object_id = bnr_->addDeviceElementGroup(
          to_string(object_pair.first),
          object_pair.second->getDescriptor()->description_);
      for (auto instance_pair : object_pair.second->getInstances()) {
        auto instance_id = bnr_->addDeviceElementGroup(
            object_id, to_string(instance_pair.first), string());
        for (auto resource_variant_pair :
             instance_pair.second->getResources()) {
          unique_ptr<DeviceNode> node;
          Read_Callback read_cb;
          Write_Callback write_cb;
          match(resource_variant_pair.second,
                [&](shared_ptr<Resource<bool>> resource) {
                  if (resource->getDescriptor()->data_type_ !=
                      LwM2M::DataType::BOOLEAN) {
                    throw runtime_error("Set data type does not match the "
                                        "descriptors data type!");
                  }
                  node = make_unique<DeviceNode>(resource->getDescriptor(),
                                                 read_cb, write_cb);
                },
                [&](shared_ptr<Resource<int64_t>> resource) {
                  if (resource->getDescriptor()->data_type_ !=
                      LwM2M::DataType::INTEGER) {
                    throw runtime_error("Set data type does not match the "
                                        "descriptors data type!");
                  }
                  node = make_unique<DeviceNode>(resource->getDescriptor(),
                                                 read_cb, write_cb);
                },
                [&](shared_ptr<Resource<double>> resource) {
                  if (resource->getDescriptor()->data_type_ !=
                      LwM2M::DataType::FLOAT) {
                    throw runtime_error("Set data type does not match the "
                                        "descriptors data type!");
                  }
                  node = make_unique<DeviceNode>(resource->getDescriptor(),
                                                 read_cb, write_cb);
                },
                [&](shared_ptr<Resource<string>> resource) {
                  if (resource->getDescriptor()->data_type_ !=
                      LwM2M::DataType::STRING) {
                    throw runtime_error("Set data type does not match the "
                                        "descriptors data type!");
                  }
                  node = make_unique<DeviceNode>(resource->getDescriptor(),
                                                 read_cb, write_cb);
                },
                [&](shared_ptr<Resource<uint64_t>> resource) {
                  if (resource->getDescriptor()->data_type_ !=
                      LwM2M::DataType::TIME) {
                    throw runtime_error("Set data type does not match the "
                                        "descriptors data type!");
                  }
                  node = make_unique<DeviceNode>(resource->getDescriptor(),
                                                 read_cb, write_cb);
                },
                [&](shared_ptr<Resource<ObjectLink>> resource) {
                  if (resource->getDescriptor()->data_type_ !=
                      LwM2M::DataType::OBJECT_LINK) {
                    throw runtime_error("Set data type does not match the "
                                        "descriptors data type!");
                  }
                  node = make_unique<DeviceNode>(resource->getDescriptor(),
                                                 read_cb, write_cb);
                },
                [&](shared_ptr<Resource<vector<uint8_t>>> resource) {
                  if (resource->getDescriptor()->data_type_ !=
                      LwM2M::DataType::OPAQUE) {
                    throw runtime_error("Set data type does not match the "
                                        "descriptors data type!");
                  }
                  node = make_unique<DeviceNode>(resource->getDescriptor(),
                                                 read_cb, write_cb);
                });
          if (node) {
            switch (node->element_type) {
            case Information_Model::ElementType::READABLE: {
              bnr_->addReadableMetric(instance_id, node->name, node->desc,
                                      node->data_type, node->read_cb);
              break;
            }
            case Information_Model::ElementType::WRITABLE: {
              bnr_->addWritableMetric(instance_id, node->name, node->desc,
                                      node->data_type, node->read_cb,
                                      node->write_cb);
              break;
            }
            case Information_Model::ElementType::FUNCTION: {
            }
            case Information_Model::ElementType::UNDEFINED: {
            }
            default: {
              throw runtime_error("Received an unhandeled element type!");
            }
            }
          }
        }
      }
    }
    bnr_->registerDevice(bnr_->getResult());
  }
}

void DeviceEventHandler::handleEvent(shared_ptr<RegistryEvent> event) {
  switch (event->type) {
  case RegistryEventType::REGISTERED:
  case RegistryEventType::UPDATED: {
    buildAndRegisterDevice(event->device);
    break;
  }
  case RegistryEventType::DEREGISTERED: {
    if (bnr_)
      bnr_->deregisterDevice(event->identifier);
    break;
  }
  default: { break; }
  }
}

DeviceEventHandler::DeviceEventHandler(
    shared_ptr<Event_Model::EventSource<RegistryEvent>> event_source)
    : EventListener(event_source) {}

void DeviceEventHandler::setBuildingAndRegistrationInterface(
    shared_ptr<Information_Access_Manager::BuildingAndRegistrationInterface>
        building_and_registration_interface) {
  bnr_ = building_and_registration_interface;
}
