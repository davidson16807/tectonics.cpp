#pragma once

// std libraries
#include <vector>
#include <string>

namespace compound { 
namespace published { 

    template<typename T>
    std::string to_string(const std::vector<std::string>& names, const std::vector<T>& properties)
    {
        auto name_ptr = std::max_element(names.begin(), names.end(),
            [](const auto& a, const auto& b) {
                return a.size() < b.size();
            });
        const max_size = name_ptr->size();
        std::string result = "";
        for (std::size_t i = 0; i < names.size(); ++i)
        {
            result += names[i] + std::string(max_size+1-names[i].size(), ' ') + si::to_string(properties[i]);
            result += "\n";
        }
        return result;
    }

    template<typename T>
    std::string to_string(const std::vector<std::string>& names, const std::vector<T>& properties)
    {
        auto name_ptr = std::max_element(names.begin(), names.end(),
            [](const auto& a, const auto& b) {
                return a.size() < b.size();
            });
        const max_size = name_ptr->size();
        std::string result = "";
        for (std::size_t i = 0; i < names.size(); ++i)
        {
            result += names[i] + std::string(max_size+1-names[i].size(), ' ') + std::to_string(properties[i]);
            result += "\n";
        }
        return result;
    }

    template<typename T>
    std::string to_string(const std::vector<std::string>& names, const std::map<int,T>& properties)
    {
        auto name_ptr = std::max_element(names.begin(), names.end(),
            [](const auto& a, const auto& b) {
                return a.size() < b.size();
            });
        const max_size = name_ptr->size();
        std::string result = "";
        for (std::size_t i = 0; i < names.size(); ++i)
        {
            result += names[i];
            if (properties.count(i) > 0) 
            {
                result += std::string(max_size+1-names[i].size(), ' ') + si::to_string(properties[i]);
            } 
            result += "\n";
        }
        return result;
    }

    template<typename T>
    std::string to_string(const std::vector<std::string>& names, const std::map<int,T>& properties)
    {
        auto name_ptr = std::max_element(names.begin(), names.end(),
            [](const auto& a, const auto& b) {
                return a.size() < b.size();
            });
        const max_size = name_ptr->size();
        std::string result = "";
        for (std::size_t i = 0; i < names.size(); ++i)
        {
            result += names[i];
            if (properties.count(i) > 0) 
            {
                result += std::string(max_size+1-names[i].size(), ' ') + std::to_string(properties[i]);
            } 
            result += "\n";
        }
        return result;
    }

}}