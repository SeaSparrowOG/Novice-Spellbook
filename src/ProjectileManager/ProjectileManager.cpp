#include "ProjectileManager.h"

namespace ProjectileManager
{
    bool LoadSettings() {
        logger::info("  >Loading Projectile Manager settings..."sv);
        auto* manager = ProjectileManager::GetSingleton();
        if (!manager) {
            logger::critical("    >Failed to get internal Projectile Manager."sv);
            return false;
        }
        return manager->LoadSettings();
    }
    bool ProjectileManager::LoadSettings() {
        auto* json = Settings::JSON::Holder::GetSingleton();
        if (!json) {
            logger::critical("    >Failed to get internal JSON holder, aborting load..."sv);
            return false;
        }
        
        if (!json->RequestSettings<ProjectileManagerSetting>(
            Settings::JSON::PROJECTILE_MANAGER_FIELD,
            []() {
                return std::make_unique<ProjectileManagerSetting>();
            },
            m_settings
        ))
        {
            logger::critical("    >Failed to request settings from the json manager."sv);
            return false;
        }
        logger::info("    >Finished loading settings."sv);
        return true;
    }

    bool ProjectileManagerSetting::LoadData(const Json::Value& a_field) {
        return true;
    }
}
