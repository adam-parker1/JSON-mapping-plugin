#pragma once

#include "map_types/map_entry.hpp"

class OffsetEntry : public MapEntry {
  public:
    OffsetEntry() = delete;
    OffsetEntry(PluginType plugin, std::string key,
                std::optional<std::string> var, float offset)
        : MapEntry(plugin, std::move(key), std::move(var)), m_offset{offset} {};

    int map(IDAM_PLUGIN_INTERFACE* interface,
            const std::unordered_map<std::string, std::unique_ptr<Mapping>>&
                entries,
            const nlohmann::json& json_globals) const override;

  private:
    int transform(IDAM_PLUGIN_INTERFACE* interface) const;

    template <typename T> int offset(T& temp_var) const;
    template <typename T> int offset_span(gsl::span<T> temp_span) const;

    float m_offset;
};

template <typename T> int OffsetEntry::offset(T& temp_var) const {

    *temp_var += m_offset;
    return 0;
};

template <typename T>
int OffsetEntry::offset_span(gsl::span<T> temp_span) const {

    if (temp_span.empty()) {
        return 1;
    }

    std::for_each(temp_span.begin(), temp_span.end(),
                  [this](T& elem) { elem += m_offset; });

    return 0;
};
