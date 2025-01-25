#pragma once

#include "Event.h"
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#define UPDATE_SCRIPT_ARGUMENTS_TYPES std::shared_ptr<harmony::core::Object>, const sf::Time&, harmony::core::EventQueue&
#define DRAW_SCRIPT_ARGUMENTS_TYPES const std::shared_ptr<harmony::core::Object>, sf::RenderTarget&, sf::RenderStates

#define CREATE_SCRIPT_ARGUMENTS_TYPES harmony::core::Object*
#define DESTROY_SCRIPT_ARGUMENTS_TYPES std::shared_ptr<harmony::core::Object> 

#define ENTER_SCRIPT_ARGUMENTS_TYPES std::shared_ptr<harmony::core::Object> 
#define EXIT_SCRIPT_ARGUMENTS_TYPES std::shared_ptr<harmony::core::Object>


#define UPDATE_SCRIPT_ARGUMENTS std::shared_ptr<harmony::core::Object> target, const sf::Time&, harmony::core::EventQueue& eventQueue
#define DRAW_SCRIPT_ARGUMENTS const std::shared_ptr<harmony::core::Object> target, sf::RenderTarget& renderTarget, sf::RenderStates states

#define CREATE_SCRIPT_ARGUMENTS harmony::core::Object* target
#define DESTROY_SCRIPT_ARGUMENTS std::shared_ptr<harmony::core::Object> target

#define ENTER_SCRIPT_ARGUMENTS std::shared_ptr<harmony::core::Object> target
#define EXIT_SCRIPT_ARGUMENTS std::shared_ptr<harmony::core::Object> target


#define REGISTER_UPDATE_SCRIPT(function) void function(UPDATE_SCRIPT_ARGUMENTS); inline static bool _REGISTER_UPDATE_SCRIPT_##function = []() { harmony::CreateScript::addScript(#function, function); return true; }();
#define REGISTER_DRAW_SCRIPT(function) void function(DRAW_SCRIPT_ARGUMENTS); inline static bool _REGISTER_UPDATE_SCRIPT_##function = []() {harmony::DrawScript::addScript(#function, function); return true; }();

#define REGISTER_CREATE_SCRIPT(function) void function(CREATE_SCRIPT_ARGUMENTS); inline static bool _REGISTER_UPDATE_SCRIPT_##function = []() {harmony::CreateScript::addScript(#function, function); return true; }();
#define REGISTER_DESTROY_SCRIPT(function) void function(DESTROY_SCRIPT_ARGUMENTS); inline static bool _REGISTER_UPDATE_SCRIPT_##function = []() {harmony::DestroyScript::addScript(#function, function); return true; }();

#define REGISTER_ENTER_SCRIPT(function) void function(ENTER_SCRIPT_ARGUMENTS); inline static bool _REGISTER_UPDATE_SCRIPT_##function = []() {harmony::EnterScript::addScript(#function, function); return true; }();
#define REGISTER_EXIT_SCRIPT(function) void function(EXIT_SCRIPT_ARGUMENTS); inline static bool _REGISTER_UPDATE_SCRIPT_##function = []() {harmomy::ExitScript::addScript(#function, function); return true; }();

namespace harmony {

    template<typename ...ARGS>
    class Script {
    public:
        Script(const std::string& name, std::function<void(ARGS...)> function)
            : m_function(function) {
            m_scripts[name] = std::make_shared<Script>(*this);
        }

		void execute(ARGS... args) {
			m_function(args...);
		}

        static void addScript(const std::string& name, std::function<void(ARGS...)> function) {
            if (m_scripts.find(name) == m_scripts.end()) {
                m_scripts[name] = std::make_shared<Script>(name, function);
            }
            else {
                throw std::runtime_error("Script with name '" + name + "' already exists.");
            }
        }

        static std::shared_ptr<Script> getScript(const std::string& name) {
            auto it = m_scripts.find(name);
            if (it != m_scripts.end()) {
                return it->second;
            }
            throw std::runtime_error("Script with name '" + name + "' not found.");
        }

        static void removeScript(const std::string& name) {
            if (m_scripts.erase(name) == 0) {
                throw std::runtime_error("Failed to remove script with name '" + name + "'. Script not found.");
            }
        }

        static void executeScript(const std::string& name, ARGS... args) {
            auto script = getScript(name);
            if (script) {
                script->m_function(args...);
            }
            else {
                throw std::runtime_error("Cannot execute script '" + name + "'. Script not found.");
            }
        }

    public:
        std::function<void(ARGS...)> m_function;

        static inline std::unordered_map<std::string, std::shared_ptr<Script>> m_scripts;
    };

    using UpdateScript = Script<UPDATE_SCRIPT_ARGUMENTS_TYPES>;
    using DrawScript = Script<DRAW_SCRIPT_ARGUMENTS_TYPES>;

    using CreateScript = Script<CREATE_SCRIPT_ARGUMENTS_TYPES>;
    using DestroyScript = Script<DESTROY_SCRIPT_ARGUMENTS_TYPES>;

    using EnterScript = Script<ENTER_SCRIPT_ARGUMENTS_TYPES>;
    using ExitScript = Script<EXIT_SCRIPT_ARGUMENTS_TYPES>;

}
