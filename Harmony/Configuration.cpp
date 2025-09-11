#include "pch.h"
#include "Configuration.h"
#include <stdexcept>
#include <fstream>

namespace {
    // Error messages
    const std::string ERROR_FILE_PATH_NOT_SET = "File path is not set.";
    const std::string ERROR_FAILED_TO_OPEN_FILE = "Failed to open file: ";
    const std::string ERROR_FAILED_TO_OPEN_FILE_FOR_WRITING = "Failed to open file for writing: ";

    // JSON formatting
    constexpr int JSON_INDENT_SIZE = 4;
}

namespace Harmony {
    Configuration::Configuration(const nlohmann::json& data, uint64_t uniqueId)
        : Object(uniqueId), data_(data) {
    }

    void Configuration::load() {
        if (!filePath_) {
            throw std::runtime_error(ERROR_FILE_PATH_NOT_SET);
        }

        std::ifstream file(*filePath_);
        if (!file.is_open()) {
            throw std::runtime_error(ERROR_FAILED_TO_OPEN_FILE + *filePath_);
        }

        file >> data_;
        file.close();
    }

    void Configuration::unload() {
        if (!filePath_) {
            throw std::runtime_error(ERROR_FILE_PATH_NOT_SET);
        }

        // Export the data to the file before clearing it
        std::ofstream file(*filePath_);
        if (!file.is_open()) {
            throw std::runtime_error(ERROR_FAILED_TO_OPEN_FILE_FOR_WRITING + *filePath_);
        }

        file << data_.dump(JSON_INDENT_SIZE); // Pretty-print with an indentation of 4 spaces
        file.close();

        // Clear the data
        data_.clear();
    }

    void Configuration::setFilePath(const std::string& filePath) {
        filePath_ = filePath;
    }

    std::string Configuration::getFilePath() const {
        if (filePath_) {
            return *filePath_;
        }
        throw std::runtime_error(ERROR_FILE_PATH_NOT_SET);
    }

    void Configuration::set(const nlohmann::json& data, const std::vector<std::string>& path) {
        nlohmann::json* current = &data_;
        for (const auto& key : path) {
            current = &(*current)[key];
        }
        *current = data;
    }

    std::optional<nlohmann::json> Configuration::get(const std::vector<std::string>& path) const {
        const nlohmann::json* current = &data_;
        for (const auto& key : path) {
            if (current->contains(key)) {
                current = &(*current)[key];
            }
            else {
                return std::nullopt;
            }
        }
        return std::make_optional<nlohmann::json>(*current);
    }
}