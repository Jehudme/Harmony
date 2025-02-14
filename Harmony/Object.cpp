#include "pch.h"
#include "Object.h"
#include "Configuration.h"
#include "Utilities.h"

namespace Harmony {

    Object::Object(uint64_t uniqueId, const std::string& name)
        : uniqueId_(uniqueId ? uniqueId : Utilities::generateRandomNumber<uint64_t>()),
        name_(DEFAULT_OBJECT_NAME) {
    }

    Object::Object(std::shared_ptr<Configuration> configuration)
        : uniqueId_(configuration->get<uint64_t>({ CONFIG_KEY_UNIQUE_ID }).value_or(Utilities::generateRandomNumber<uint64_t>())),
        name_(configuration->get<std::string>({ CONFIG_KEY_NAME }).value_or(DEFAULT_OBJECT_NAME)) {
    }

    Object::~Object() {
        if (registeredById_[uniqueId_].expired()) {
            registeredById_.erase(uniqueId_);
        }
        if (registeredByName_[name_].expired()) {
            registeredByName_.erase(name_);
        }
    }

    void Object::retain() {
        retainedObjects_[this] = shared_from_this();
    }

    void Object::release() {
        if (retainedObjects_.contains(this)) {
            retainedObjects_.erase(this);
        }
    }

    void Object::setName(const std::string& name) {
        if (registeredByName_.contains(name)) {
            throw std::runtime_error("Name already in use");
        }

        if (registeredByName_[name].lock().get() == this) {
            registeredByName_.erase(name);
        }

        registeredByName_[name] = weak_from_this();
        name_ = name;
    }

    const std::string& Object::getName() const {
        return name_;
    }

    uint64_t Object::getUniqueId() const {
        return uniqueId_;
    }
}