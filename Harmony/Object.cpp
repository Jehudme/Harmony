#include "pch.h"
#include "Object.h"
#include "Utilities.h"
#include "Configuration.h"
#include <cstdint>
#include <string>

namespace harmony::core {

    // Helper function to generate or retrieve the unique ID
    uint64_t getID(const uint64_t& uniqueId) {
        return uniqueId ? uniqueId : utilities::generateRandomNumber<uint64_t>();
    }

    uint64_t getID(const Configuration& configuration) {
        return getID(configuration.getData<uint64_t>({ "UniqueId" }).value_or(utilities::generateRandomNumber<uint64_t>()));
    }

    // Constructors
    Object::Object(const uint64_t uniqueId)
        : uniqueId(getID(uniqueId)), name(std::to_string(uniqueId)) {
    }

    Object::Object(const Configuration& configuration)
        : uniqueId(getID(configuration)), name(configuration.getData({ "Name" }).value_or(std::to_string(uniqueId))) {
    }

    // Destructor
    Object::~Object() {
        if (m_registers[uniqueId].expired()) {
            m_registers.erase(uniqueId);
        }
    }

    // Retain and release methods
    void Object::retainSelf() {
        m_retained[this] = shared_from_this();
    }

    void Object::releaseSelf() {
        m_retained.erase(this);
    }
}

