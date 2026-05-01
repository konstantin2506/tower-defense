#include "config_reader.h"

#include <fstream>

using namespace TowerDefence;

std::unique_ptr<IConfigReader> JsonReader::fromFile(const std::string &path) const
{
    std::ifstream file{path};
    if (!file.is_open()) throw std::runtime_error("Failed to open file: " + path);

    return std::make_unique<JsonReader>(json::parse(file), path);
}

std::unique_ptr<JsonReader> JsonReader::fromJson(json&& json_)
{
    return std::make_unique<JsonReader>(std::move(json_));
}


std::map<std::string, std::unique_ptr<IConfigReader>> JsonReader::getItems(const std::string& field) const
{
    std::map<std::string, std::unique_ptr<IConfigReader>> items;
    if (field.empty()) {
        for (auto& [name, config] : cfg_.items()) {
            items.emplace(name, std::make_unique<JsonReader>(config));
        }
        return items;
    }
    for (auto& [name, config] : cfg_.at(field).items()) {
        items.emplace(name, std::make_unique<JsonReader>(config));
    }
    return items;
}

std::map<std::string, std::string> JsonReader::getStrings(const std::string& field) const
{
    std::map<std::string, std::string> result;
    for (auto& [fieldName, value] : cfg_.at(field).items()) {
        result.emplace(fieldName, value);
    }
    return result;
}

std::map<std::string, size_t> JsonReader::getSize_ts(const std::string& field) const
{
    std::map<std::string, std::size_t> result;
    for (auto& [fieldName, value] : cfg_.at(field).items()) {
        result.emplace(fieldName, value);
    }
    return result;
}


std::unique_ptr<IConfigReader> JsonReader::getNested(const std::string& field) const
{
    return std::make_unique<JsonReader>(cfg_.at(field), path_);
}


