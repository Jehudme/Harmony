#pragma once
#include "Object.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

namespace Harmony
{	
	class Configuration;

	template<typename Type>
	class Resource : public Object
	{
	public:
		Resource(std::shared_ptr<Configuration> configuration);

		Resource(const std::string& FILENAME);

		Type& getResource();

		bool isValid() const;

	private:
		Type resource_;
		bool isValid_;
	};

	using Texture = Resource<sf::Texture>;
	using Font = Resource<sf::Font>;

	template<typename Type>
	inline Resource<Type>::Resource(const std::string& FILENAME)
		: Object(NULL, FILENAME), isValid_(resource_.loadFromFile(FILENAME))
	{
	}

	template<typename Type>
	inline Type& Resource<Type>::getResource()
	{
		return resource_;
	}

	template<typename Type>
	inline bool Resource<Type>::isValid() const
	{
		return isValid_;
	}

	template<>
	inline std::shared_ptr<Texture> find(const std::string& name) {
		if (Object::registeredByName_.contains(name)) {
			if (auto object = std::dynamic_pointer_cast<Texture>(Object::registeredByName_[name].lock())) {
				return object;
			}
			throw std::runtime_error("Dynamic pointer cast failed");
		}
		return create<Texture>(name);
	}

	template<>
	inline std::shared_ptr<Font> find(const std::string& name) {
		if (Object::registeredByName_.contains(name)) {
			if (auto object = std::dynamic_pointer_cast<Font>(Object::registeredByName_[name].lock())) {
				return object;
			}
			throw std::runtime_error("Dynamic pointer cast failed");
		}
		return create<Font>(name);
	}
}


