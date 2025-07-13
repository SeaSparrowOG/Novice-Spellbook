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

	class ProjectileManager : 
		public REX::Singleton<ProjectileManager>
	{
	public:
		bool LoadSettings();

	private:
		std::vector<std::unique_ptr<ProjectileManagerSetting>> m_settings{};
	};
}