#include "pch.h"
#include "Object.h"

Harmony::Core::Object::Object(const uint64_t uniqueId) :
	uniqueId(uniqueId ? uniqueId : Utilities::generateRandomId()) {}

Harmony::Core::Object::~Object()
{
    if (m_registers[uniqueId].expired())
        m_registers.erase(uniqueId);
}

void Harmony::Core::Object::retainSelf()
{
    m_retained[this] = shared_from_this();
}

void Harmony::Core::Object::releaseSelf()
{
    if (m_retained.contains(this))
        m_retained.erase(this);
}

uint64_t Harmony::Utilities::generateRandomId()
{
    static std::random_device rd;
    static std::mt19937_64 engine(rd());
    static std::uniform_int_distribution<uint64_t> dist;

    return dist(engine);
}
