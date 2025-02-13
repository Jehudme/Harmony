#include "pch.h"
#include "Configuration.h"
#include <stdexcept>
#include <cstdint>
#include <initializer_list>
#include <iosfwd>
#include <optional>
#include <string>
#include <fstream>
#include "Object.h"
#include <nlohmann/json_fwd.hpp>

namespace Harmony {

    Configuration::Configuration(const nlohmann::json& data, const uint64_t& uniqueId)
        : Object(uniqueId), m_data(data) {
    }

    void Configuration::load() {
        if (!m_filePath) {
            throw std::runtime_error("File path is not set.");
        }

        std::ifstream file(*m_filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + *m_filePath);
        }

        file >> m_data;
        file.close();
    }

    void Configuration::unload() {
        if (!m_filePath) {
            throw std::runtime_error("File path is not set.");
        }

        // Export the data to the file before clearing it
        std::ofstream file(*m_filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file for writing: " + *m_filePath);
        }

        file << m_data.dump(4); // Pretty-print with an indentation of 4 spaces
        file.close();

        // Clear the data
        m_data.clear();
    }

    void Configuration::setFilePath(const std::string& filePath) {
        m_filePath = filePath;
    }

    std::string Configuration::getFilePath() const {
        if (m_filePath) {
            return *m_filePath;
        } else {
            throw std::runtime_error("File path is not set.");
        }
    }

    void Configuration::set(const nlohmann::json& data, const std::initializer_list<const char*>& path) {
        nlohmann::json* current = &m_data;
        for (const auto& key : path) {
            current = &(*current)[key];
        }
        *current = data;
    }

    std::optional<nlohmann::json> Configuration::get(const std::initializer_list<const char*>& path) const {
        const nlohmann::json* current = &m_data;
        for (const auto& key : path) {
            if (current->contains(key)) {
                current = &(*current)[key];
            } else {
                return std::nullopt;
            }
        }
        return std::make_optional<nlohmann::json>(*current);
    }

}

