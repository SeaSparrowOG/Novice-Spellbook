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

    bool ProjectileManager::AddDeadZone(RE::TESObjectREFR* a_ref, float a_strength, float a_range)
    {
        auto id = a_ref->formID;
        for (auto& zone : m_zones) {
            if (id == zone.m_trackedRef) {
                if (zone.m_kill) {
                    return false;
                }

                zone.m_radius = a_range;
                zone.m_strength = a_strength;
                return true;
            }
        }
        CreateNewZone(id, false, a_strength, a_range);
        return true;
    }

    bool ProjectileManager::AddKillZone(RE::TESObjectREFR* a_ref, 
        float a_range) 
    {
        auto id = a_ref->formID;
        for (auto& zone : m_zones) {
            if (id == zone.m_trackedRef) {
                zone.m_kill = true;
                zone.m_radius = a_range;
                return true;
            }
        }
        CreateNewZone(id, true, 1.0f, a_range);
        return true;
    }

    bool ProjectileManager::ClearZone(RE::TESObjectREFR* a_ref) {
        auto id = a_ref->formID;
        std::erase_if(m_zones, [&](Zone zone) {
            return zone.m_trackedRef == id;
            });
        return true;
    }

    bool ProjectileManager::ClearAllZones() {
        m_zones.clear();
        return true;
    }

    ProjectileModifications ProjectileManager::ModifyProjectile(RE::Projectile* a_projectile) {
        ProjectileModifications modifications = ProjectileModifications();
        if (m_zones.empty()) {
            return modifications;
        }

        float closestDistance = -1.0f;
        Zone* closestZone = nullptr;
        for (auto& zone : m_zones) {
            auto* zoneRef = RE::TESForm::LookupByID<RE::TESObjectREFR>(zone.m_trackedRef);
            if (!zoneRef || !zoneRef->Is3DLoaded()) {
                continue;
            }

            auto pos = a_projectile->GetPosition();
            float refDistance = zoneRef->GetPosition().GetDistance(pos);
            if (refDistance > zone.m_radius) {
                continue;
            }

            if (!closestZone || refDistance < closestDistance) {
                closestZone = &zone;
                closestDistance = refDistance;
            }
        }

        if (closestZone) {
            modifications.m_modify = true;

            if (closestZone->m_kill) {
                return modifications;
            }

            const float distancePct = std::clamp(closestDistance / closestZone->m_radius, 0.01f, 0.99f);
            const float slowdownFactor = powf(10.0, distancePct - 1.0f);
            const float factor = std::clamp(closestZone->m_strength * slowdownFactor, 0.0f, closestZone->m_strength);
            modifications.m_modifyGravity = factor;
            modifications.m_modifySpeed = factor;
            return modifications;
        }
        return modifications;
    }

    void ProjectileManager::CreateNewZone(RE::FormID a_id, 
        bool a_kill, 
        float a_strength, 
        float a_range)
    {
        Zone zone = Zone();
        zone.m_kill = a_kill;
        zone.m_radius = a_range;
        zone.m_strength = a_strength;
        zone.m_trackedRef = a_id;
        m_zones.push_back(std::move(zone));
    }

    bool ProjectileManagerSetting::LoadData(const Json::Value& a_field) {
        return true;
    }
}
