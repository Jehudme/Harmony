#include "pch.h"
#include "Configuration.h"

namespace Harmony {

    Object::Object(const uint64_t uuid)
        : uniqueID_(uuid == INVALID_UNIQUE_ID ? generateRandomUniqueID() : uuid) {
        initialize();
    }

    Object::Object(const std::shared_ptr<Configuration>& configuration)
        : uniqueID_(configuration->getParameterOrDefault("unique_id", INVALID_UNIQUE_ID)) {
        initialize();
    }

    Object::~Object() {
        try {
            if (auto it = objectRegistry.find(uniqueID_); it != objectRegistry.end() && it->second.expired()) {
                objectRegistry.erase(it);
            }
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Error during Object destruction with ID [{}] [{}]: {}", typeId_, uniqueID_, e.what());
        }
    }

    void Object::setID(uint64_t uuid) {
        try {
            if (auto it = objectRegistry.find(uniqueID_); it != objectRegistry.end() && it->second.lock() == shared_from_this()) {
                objectRegistry.erase(it);
            }

            uniqueID_ = uuid == INVALID_UNIQUE_ID ? generateRandomUniqueID() : uuid;
            objectRegistry[uniqueID_] = weak_from_this();
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Error while setting ID for Object with ID [{}] [{}]: {}", typeId_, uniqueID_, e.what());
            throw;

        }
    }

    uint64_t Object::getID() const {
        try {
            return uniqueID_;
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Error retrieving Object ID [{}] [{}]: {}", typeId_, uniqueID_, e.what());
            throw;
        }
    }

    void Object::retainSelf() {
        try {
            retainedObjects[this] = shared_from_this();
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Error retaining Object with ID [{}] [{}]: {}", typeId_, uniqueID_, e.what());
            throw;
        }
    }

    void Object::releaseSelf() {
        try {
            if (retainedObjects.contains(this)) {
                retainedObjects.erase(this);
            }
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Error releasing Object with ID [{}] [{}]: {}", typeId_, uniqueID_, e.what());
            throw;
        }
    }

    bool Object::isSelfRetained() const {
        try {
            return retainedObjects.contains(this);
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Error checking self-retained status for Object with ID [{}] [{}]: {}", typeId_, uniqueID_, e.what());
            throw;
        }
    }

    uint64_t Object::generateRandomUniqueID() {
        static std::random_device rd;
        static std::mt19937_64 engine(rd());
        static std::uniform_int_distribution<uint64_t> dist;

        return dist(engine);
    }

    void Object::initialize() {
        // Initialization logic, if necessary.
    }

}
