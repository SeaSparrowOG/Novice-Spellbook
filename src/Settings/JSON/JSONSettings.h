#pragma once

#include <json/json.h>

namespace Settings
{
	namespace JSON
	{
		inline static constexpr const char* PROJECTILE_MANAGER_FIELD = "ProjectileManager";

		bool Read();

		class StoredValue
		{
		public:
			virtual bool LoadData(const Json::Value& a_field) = 0;
			virtual ~StoredValue() = default;

		private:
			using _GetFormEditorID = const char* (*)(std::uint32_t);
			std::string                     GetEditorID(const RE::TESForm* a_form);
			inline std::vector<std::string> Split(const std::string& a_str, const std::string& a_delimiter);
			inline bool                     IsOnlyHex(std::string_view a_str, bool a_requirePrefix = true);
			inline std::string              ToLower(std::string_view a_str);

			template <class T>
			T to_num(const std::string& a_str, bool a_hex = false)
			{
				const int base = a_hex ? 16 : 10;

				if constexpr (std::is_same_v<T, double>) {
					return static_cast<T>(std::stod(a_str, nullptr));
				}
				else if constexpr (std::is_same_v<T, float>) {
					return static_cast<T>(std::stof(a_str, nullptr));
				}
				else if constexpr (std::is_same_v<T, std::int64_t>) {
					return static_cast<T>(std::stol(a_str, nullptr, base));
				}
				else if constexpr (std::is_same_v<T, std::uint64_t>) {
					return static_cast<T>(std::stoull(a_str, nullptr, base));
				}
				else if constexpr (std::is_signed_v<T>) {
					return static_cast<T>(std::stoi(a_str, nullptr, base));
				}
				else {
					return static_cast<T>(std::stoul(a_str, nullptr, base));
				}
			}

			template <typename T>
			T* GetFormFromString(const std::string& a_str)
			{
				T* response = nullptr;
				auto* dataHandler = RE::TESDataHandler::GetSingleton();
				if (!dataHandler) {
					return nullptr;
				}

				if (const auto splitID = Split(a_str, "|"); splitID.size() == 2) {
					const auto& modName = splitID[0];
					if (!dataHandler->LookupModByName(modName)) {
						return response;
					}
					if (!IsOnlyHex(splitID[1])) {
						return response;
					}

					try {
						const auto  formID = to_num<RE::FormID>(splitID[1], true);
						auto* intermediate = dataHandler->LookupForm(formID, modName);
						if (intermediate) {
							return skyrim_cast<T*>(intermediate);
						}
					}
					catch (std::exception& e) {
						logger::error("Caught exception: {}", e.what());
						return response;
					}
				}
				auto* intermediate = RE::TESForm::LookupByEditorID(a_str);
				return intermediate ? skyrim_cast<T*>(intermediate) : nullptr;
			}
		};

		class Holder : public REX::Singleton<Holder>
		{
		public:
			bool Read();

			template <typename T>
			bool RequestSettings(const std::string& a_key,
				std::function<std::unique_ptr<T>()> a_factory,
				std::vector<std::unique_ptr<T>>& a_result) 
			{
				auto it = m_storedSettings.find(a_key);
				if (it == m_storedSettings.end()) {
					return true;
				}

				for (const auto& jsonBlock : it->second) {
					auto obj = a_factory();
					if (!obj->LoadData(jsonBlock)) {
						return false;
					}
					a_result.push_back(std::move(obj));
				}

				return true;
			}

		private:
			bool ReadConfig(const Json::Value& a_entry);

			std::unordered_map<std::string, std::vector<Json::Value>> m_storedSettings{};

			inline static constexpr std::uint8_t PARSER_VERSION = 1;
			inline static constexpr const char* MINIMUM_VERSION_FIELD = "MinimumVersion";

			inline static constexpr std::uint8_t ALLOWED_FIELDS_COUNT = 1;
			inline static constexpr std::array<const char*, ALLOWED_FIELDS_COUNT> EXPECTED_FIELDS = {
				PROJECTILE_MANAGER_FIELD
			};

			inline static constexpr std::uint8_t TOP_LEVEL_FIELDS_COUNT = 1;
			inline static constexpr std::array<const char*, TOP_LEVEL_FIELDS_COUNT> TOP_LEVEL_FIELDS = {
				MINIMUM_VERSION_FIELD
			};
		};
	}
}