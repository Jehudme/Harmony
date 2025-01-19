#include "pch.h"
#include "Object.h"
#include "Logger.h"

Harmony::Core::Object::Object(const uint64_t uniqueId)
    : uniqueId(uniqueId ? uniqueId : Utilities::generateRandomNumber<uint64_t>()) {
    LOG_TRACE(Logger::core, "[Object] Initialized object with ID: {}", this->uniqueId);
}

Harmony::Core::Object::~Object() {
    if (m_registers[uniqueId].expired()) {
        m_registers.erase(uniqueId);;
    }
    LOG_TRACE(Logger::core, "[Object] Destroyed object of type '{}' with ID: {}", objectType, uniqueId);
}

void Harmony::Core::Object::retainSelf() {
    m_retained[this] = shared_from_this();
    LOG_TRACE(Logger::core, "[Object] Retained object with ID: {}", uniqueId);
}

void Harmony::Core::Object::releaseSelf() {
    if (m_retained.contains(this)) {
        m_retained.erase(this);
        LOG_TRACE(Logger::core, "[Object] Released object with ID: {}", uniqueId);
    }
    else {
        LOG_WARN(Logger::core, "[Object] Attempted to release unretained object with ID: {}", uniqueId);
    }
}
