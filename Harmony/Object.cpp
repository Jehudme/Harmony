#include "pch.h"
#include "Object.h"
#include "Logger.h"
#include "Utilities.h"

harmony::core::Object::Object(const uint64_t uniqueId)
    : uniqueId(uniqueId ? uniqueId : utilities::generateRandomNumber<uint64_t>()) {
    LOG_TRACE(Logger::core, "Initialized object with ID: {}", this->uniqueId);
}

harmony::core::Object::~Object() {
    if (m_registers[uniqueId].expired()) {
        m_registers.erase(uniqueId);;
    }
    LOG_TRACE(Logger::core, "Destroyed object of type '{}' with ID: {}", objectType, uniqueId);
}

void harmony::core::Object::retainSelf() {
    m_retained[this] = shared_from_this();
    LOG_TRACE(Logger::core, "Retained object with ID: {}", uniqueId);
}

void harmony::core::Object::releaseSelf() {
    if (m_retained.contains(this)) {
        m_retained.erase(this);
        LOG_TRACE(Logger::core, "Released object with ID: {}", uniqueId);
    }
    else {
        LOG_WARN(Logger::core, "Attempted to release unretained object with ID: {}", uniqueId);
    }
}
