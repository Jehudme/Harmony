#include "pch.h"
#include "Object.h"
#include "Utilities.h"
#include "Configuration.h"
#include "Utilities.h"

constexpr const char* INITIAL_OBJECT_NAME = "Unkow";


namespace Harmony
{
	Object::Object(const uint64_t& uniqueId)
		: m_uniqueId(uniqueId ? uniqueId : Utilities::generateRandomNumber<uint64_t>()), m_name(INITIAL_OBJECT_NAME), m_isFoundByNameEnable(false) {
	}

	static inline uint64_t setUniqueId(std::shared_ptr<Configuration> configuration) {
		return configuration->get<uint64_t>({ "UniqueId" }).value_or(Utilities::generateRandomNumber<uint64_t>());
	}

	Object::Object(std::shared_ptr<Configuration> configuration)
		: m_uniqueId(setUniqueId(configuration)), m_name(INITIAL_OBJECT_NAME), m_isFoundByNameEnable(false) {
	}

	Object::~Object()
	{
		if (m_registersById[m_uniqueId].expired()) {
			m_registersById.erase(m_uniqueId);
		} 
		if (m_isFoundByNameEnable && m_registersByName[m_name].expired()) {
			m_registersByName.erase(m_name);
		}
	}

	void Object::retainSelf() {
		m_retained[this] = shared_from_this();
	}

	void Object::releaseSelf() {
		if (m_retained.contains(this)) {
			m_retained.erase(this);
		}
	}

	void Object::setName(const std::string& name)
	{
		if(Object::m_registersByName.contains(name)) {
			throw std::runtime_error("Object Not Find");
		}

		if (Object::m_registersByName[name].lock().get() == this) {
			Object::m_registersByName.erase(name);
		}

		m_registersByName[name] = weak_from_this();
		m_name = name;
		m_isFoundByNameEnable = true;
	}

	const std::string& Object::getName() const {
		return m_name;
	}
	const uint64_t& Object::getUniqueId() const {
		return m_uniqueId;
	}
}
