#include "pch.h"
#include "Object.h"
#include "Logger.h"
#include "Utilities.h"
#include "Configuration.h"

const uint64_t getID(const uint64_t& uniqueId) {
    return uniqueId ? uniqueId : harmony::utilities::generateRandomNumber<uint64_t>();
}

const uint64_t getID(const std::shared_ptr<harmony::Configuration> configuration) {
    return getID(configuration->getData<uint64_t>({ "UniqueId" }).value_or(harmony::utilities::generateRandomNumber<uint64_t>()));
}

harmony::core::Object::Object(const uint64_t uniqueId)
    : uniqueId(getID(uniqueId)), name(std::to_string(uniqueId)) {
    LOG_TRACE(Logger::core, "[Object] Initialized object with ID: {}", this->uniqueId);
}

harmony::core::Object::Object(const std::shared_ptr<Configuration> configuration) : 
    uniqueId(getID(configuration)), name(configuration->getData({"Name"}).value_or(std::to_string(uniqueId))) {
    LOG_TRACE(Logger::core, "[Object] Initialized object with ID: {}", this->uniqueId);
}

harmony::core::Object::~Object() {
    if (m_registers[uniqueId].expired()) {
        m_registers.erase(uniqueId);
    }
    LOG_TRACE(Logger::core, "[Object] Destroyed object of type '{}' with ID: {}", objectType, this->uniqueId);
}

void harmony::core::Object::retainSelf() {
    m_retained[this] = shared_from_this();
    LOG_TRACE(Logger::core, "[Object] Retained object with ID: {}", this->uniqueId);
}

void harmony::core::Object::releaseSelf() {
    if (m_retained.contains(this)) {
        m_retained.erase(this);
        LOG_TRACE(Logger::core, "[Object] Released object with ID: {}", this->uniqueId);
    }
    else {
        LOG_WARN(Logger::core, "[Object] Attempted to release unretained object with ID: {}", this->uniqueId);
    }
}
