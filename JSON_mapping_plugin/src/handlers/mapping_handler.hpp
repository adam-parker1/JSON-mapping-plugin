#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>

#include "map_types/base_mapping.hpp"
#include <nlohmann/json.hpp>

using IDSName_t = std::string;
using MachineName_t = std::string;
using MappingName_t = std::string;

using IDSMapRegister_t = std::unordered_map<MappingName_t, std::unique_ptr<Mapping>>;
using IDSMapRegisterStore_t = std::unordered_map<IDSName_t, IDSMapRegister_t>;
using IDSAttrRegisterStore_t = std::unordered_map<IDSName_t, nlohmann::json>;

struct MachineMapping {
    IDSMapRegisterStore_t mappings;
    IDSAttrRegisterStore_t attributes;
};

using MachineRegisterStore_t = std::unordered_map<MachineName_t, MachineMapping>;
using MappingPair = std::pair<nlohmann::json&, IDSMapRegister_t&>;

class MappingHandler {

  public:
    MappingHandler() : m_init(false), m_dd_version("3.39.0"){};
    explicit MappingHandler(std::string dd_version) : m_init(false), m_dd_version(std::move(dd_version)){};

    int reset() {
        m_machine_register.clear();
        m_mapping_config.clear();
        m_init = false;
        return 0;
    };
    int init() {
        if (m_init || !m_machine_register.empty()) {
            return 0;
        }

        m_init = true;
        return 0;
    };
    int set_map_dir(const std::string& mapping_dir);
    std::optional<MappingPair> read_mappings(const MachineName_t& machine, const std::string& request_ids);

  private:
    std::string mapping_path(const MachineName_t& machine, const IDSName_t& ids_name, const std::string& file_name);
    int init_mappings(const MachineName_t& machine, const IDSName_t& ids_name, const nlohmann::json& data);
    int load_machine(const MachineName_t& machine);
    nlohmann::json load_toplevel(const MachineName_t& machine);
    int load_globals(const MachineName_t& machine, const IDSName_t& ids_name);
    int load_mappings(const MachineName_t& machine, const IDSName_t& ids_name);

    static int init_value_mapping(IDSMapRegister_t& map_reg, const std::string& key, nlohmann::json value);
    static int init_plugin_mapping(IDSMapRegister_t& map_reg, const std::string& key, nlohmann::json value,
                                   nlohmann::json ids_attributes);
    static int init_dim_mapping(IDSMapRegister_t& map_reg, const std::string& key, nlohmann::json value);
    static int init_slice_mapping(IDSMapRegister_t& map_reg, const std::string& key, nlohmann::json value);
    static int init_expr_mapping(IDSMapRegister_t& map_reg, const std::string& key, nlohmann::json value);
    static int init_custom_mapping(IDSMapRegister_t& map_reg, const std::string& key, nlohmann::json value);

    MachineRegisterStore_t m_machine_register;
    bool m_init;

    std::string m_dd_version;
    std::string m_mapping_dir;
    nlohmann::json m_mapping_config;
};
