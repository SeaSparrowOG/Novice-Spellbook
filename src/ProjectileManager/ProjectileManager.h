#pragma once

#include "Settings/JSON/JSONSettings.h"

namespace ProjectileManager
{
	bool LoadSettings();

	class ProjectileManagerSetting : public Settings::JSON::StoredValue
	{
	public:
		bool LoadData(const Json::Value& a_field) override;

		~ProjectileManagerSetting() = default;
	};

	struct ProjectileModifications
	{
		bool  m_modify{ false };
		float m_modifySpeed{ 1.0f };
		float m_modifyGravity{ 1.0f };
	};

	class ProjectileManager : 
		public REX::Singleton<ProjectileManager>
	{
	public:
		bool LoadSettings();

		/// <summary>
		/// Dead Zones are references slow down projectiles that approach with a_range by up to a_strength.
		/// Strength is calculated as 10^(std::clamp(distance/range, 0.1, 1.0)-1.0)*a_strength.
		/// Essentially, the closer you are to the center, the stronger the pull, and the closer you get, the faster
		/// the rate at which the pull increases... increases.
		/// </summary>
		/// <param name="a_ref">The reference to make into Dead Zone. Dead Zones are serialized, and only cleaned when the reference is missing during a save.</param>
		/// <param name="a_strength">The maximum force of the dead zone. 0.01f slows down projectiles by up to 99%, 10.0f speeds them up by 1000%.</param>
		/// <param name="a_range">The range at which the dead zone begins affecting projectiles.</param>
		/// <returns>True if the zone is added, false if a mistake is made.</returns>
		bool AddDeadZone(RE::TESObjectREFR* a_ref, float a_strength, float a_range);

		/// <summary>
		/// Unlike Dead Zones, kill zones instantly "kill" projectiles that edit their range, making them drop out of the air.
		/// </summary>
		/// <param name="a_ref">The ref to make into a Kill Zone.</param>
		/// <param name="a_range">The range at which projectiles are killed.</param>
		/// <returns>True if the zone is added, false if a mistake is made.</returns>
		bool AddKillZone(RE::TESObjectREFR* a_ref, float a_range);

		bool ClearZone(RE::TESObjectREFR* a_ref);
		bool ClearAllZones();

		/// <summary>
		/// Finds the nearest Dead Zone (or Kill Zone) near a projectile and returns needed modifications to the projectile.
		/// </summary>
		/// <param name="a_projectile">The projectile to modify.</param>
		/// <returns>A struct containing a speed and a gravity modification. The m_modify member is a flag to see if changes are needed.</returns>
		ProjectileModifications ModifyProjectile(RE::Projectile* a_projectile);

	private:
		struct Zone
		{
			RE::FormID m_trackedRef{ 0 };
			bool       m_kill{ false };
			float      m_strength{ 0.0f };
			float      m_radius{ 0.0f };
		};

		void CreateNewZone(RE::FormID a_id, bool a_kill, float a_strength, float a_range);

		std::vector<std::unique_ptr<ProjectileManagerSetting>> m_settings{};
		std::vector<Zone> m_zones{};
	};
}