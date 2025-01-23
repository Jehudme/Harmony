#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "Object.h"


namespace harmony 
{
	class Configuration : public  core::Object
	{
	public:
		using Key = const std::string;

	public:
		Configuration(const std::string path, const uint64_t& uniqueId = NULL);

		void load();
		void save();

		template<typename Type>
		std::optional<Type> getData(const std::initializer_list<Key> keys);

		template<typename Type>
		void setData(const std::initializer_list<Key> keys, Type& data);

	private:
		bool contain(const std::initializer_list<Key> keys);

	public:
		std::string path;
		nlohmann::json data;
	};

    extern std::shared_ptr<Configuration> configuration;
}


namespace harmony
{
    template<typename Type>
    std::optional<Type> Configuration::getData(const std::initializer_list<Key> keys)
    {
        LOG_TRACE(Logger::core, "Getting data with keys");
        auto current = this->data;
        for (auto key : keys)
        {
            if (!current.contains(key))
            {
                return std::nullopt;
            }
            current = current[key];
        }

        Type value = current.get<Type>();
        LOG_TRACE(Logger::core, "Data retrieved successfully");
        return std::make_optional<Type>(value);
    }

    template<typename Type>
    void Configuration::setData(const std::initializer_list<Key> keys, Type& data)
    {
        LOG_TRACE(Logger::core, "Setting data with keys");
        auto& current = this->data;
        for (auto key : keys)
        {
            if (current.find(key) == current.end())
            {
                current[key] = nlohmann::json::object();
            }
            current = current[key];
        }

        current = data;
    }
}
